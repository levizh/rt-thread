/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-XX-XX     CDT          first version
 */

#include <board.h>
#include <drv_pm.h>
#include <drv_common.h>
#include <drv_wktm.h>
#include <drv_config.h>

#if defined(RT_USING_PM)

#if defined(BSP_USING_PM)

#define LOG_TAG             "drv_pm"
#include <drv_log.h>

typedef void (* run_switch_func_type)(void);
typedef void (* sleep_enter_func_type)(void);
static void __sleep_enter_light(void);
static void __sleep_enter_stop(void);
static void __sleep_enter_shutdown(void);
static void __run_switch_high_to_low(void);
static void __run_switch_low_to_high(void);

static run_switch_func_type run_switch_func[PM_RUN_MODE_MAX][PM_RUN_MODE_MAX] =
{
    {RT_NULL, RT_NULL, RT_NULL, __run_switch_high_to_low},
    {RT_NULL, RT_NULL, RT_NULL, __run_switch_high_to_low},
    {RT_NULL, RT_NULL, RT_NULL, RT_NULL},
    {__run_switch_low_to_high, __run_switch_low_to_high, RT_NULL, RT_NULL},
};

static sleep_enter_func_type sleep_enter_func[PM_SLEEP_MODE_MAX] =
{
    RT_NULL,
    RT_NULL,
    __sleep_enter_light,
    __sleep_enter_stop,
    RT_NULL,
    __sleep_enter_shutdown,
};

static void uart_console_reconfig(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    rt_device_control(rt_console_get_device(), RT_DEVICE_CTRL_CONFIG, &config);
}

static void __sleep_enter_light(void)
{
    PWC_SLEEP_Enter();
}

static void __sleep_enter_stop(void)
{
    if (st_sleep_deep_cfg.init_func != RT_NULL)
    {
        st_sleep_deep_cfg.init_func();
    }
    RT_ASSERT(PM_SLEEP_CHECK(PM_SLEEP_MODE_DEEP));

    INTC_WakeupSrcCmd(st_sleep_deep_cfg.wakeup_int_src, ENABLE);
    (void)PWC_STOP_Config(&st_sleep_deep_cfg.cfg);
    PWC_STOP_Enter(st_sleep_deep_cfg.wait_for_wakeup_type);
}

static void __sleep_enter_shutdown(void)
{
    if (st_sleep_shutdown_cfg.init_func != RT_NULL)
    {
        st_sleep_shutdown_cfg.init_func();
    }
    RT_ASSERT(PM_SLEEP_CHECK(PM_SLEEP_MODE_SHUTDOWN));

    (void)PWC_PD_Config(&st_sleep_shutdown_cfg.cfg);
    PWC_PD_ClearWakeupStatus(PWC_PD_WKUP_FLAG_ALL);
    PWC_PD_Enter();
}

/**
 * @param pm pointer to power manage structure
 */
static void sleep(struct rt_pm *pm, uint8_t mode)
{
    RT_ASSERT(mode < PM_SLEEP_MODE_MAX);

    if (sleep_enter_func[mode] != NULL)
    {
        sleep_enter_func[mode]();
    }
}

static void __run_switch_high_to_low(void)
{
    st_run_mode_cfg.sys_clk_cfg(PM_RUN_MODE_LOW_SPEED);
    SysTick_Configuration();

    PWC_HighSpeedToLowSpeed();
}

static void __run_switch_low_to_high(void)
{
    PWC_LowSpeedToHighSpeed();

    st_run_mode_cfg.sys_clk_cfg(PM_RUN_MODE_HIGH_SPEED);
    SysTick_Configuration();
}

static void run(struct rt_pm *pm, uint8_t mode)
{
    static uint8_t last_mode = PM_RUN_MODE_NORMAL_SPEED;

    if (mode == last_mode)
        return;

    if (run_switch_func[last_mode][mode] != RT_NULL)
    {
        run_switch_func[last_mode][mode]();
    }

    /* 4. temporary update peripheral clk here
    * TODO: uart_console_reconfig implement in rt_device_pm_ops.frequency_change
    * (add rt_pm_device_register in drv_usart ) */
    uart_console_reconfig();

    last_mode = mode;
}

/**
 * This function caculate the PM tick from OS tick
 *
 * @param tick OS tick
 *
 * @return the PM tick
 */
static rt_tick_t __wakeup_timer_tick_from_os_tick(rt_tick_t tick)
{
    rt_uint32_t freq = hc32_wktm_get_countfreq();

    return (freq * tick / RT_TICK_PER_SECOND);
}

/**
 * This function caculate the OS tick from PM tick
 *
 * @param tick PM tick
 *
 * @return the OS tick
 */
static rt_tick_t __wakeup_timer_tick_to_os_tick(rt_uint32_t tick)
{
    static rt_uint32_t os_tick_remain = 0;
    rt_uint32_t ret, freq;

    freq = hc32_wktm_get_countfreq();
    ret = (tick * RT_TICK_PER_SECOND + os_tick_remain) / freq;

    os_tick_remain += (tick * RT_TICK_PER_SECOND);
    os_tick_remain %= freq;

    return ret;
}

/**
 * This function start the timer of pm
 *
 * @param pm Pointer to power manage structure
 * @param timeout How many OS Ticks that MCU can sleep
 */
static void __wakeup_timer_start(struct rt_pm *pm, rt_uint32_t timeout)
{
    RT_ASSERT(pm != RT_NULL);

    /* Convert OS Tick to pmtimer timeout value */
    timeout = __wakeup_timer_tick_from_os_tick(timeout);

    /* Enter __WAKEUP_TIMER_MODE */
    hc32_wktm_start(timeout);
}

/**
 * This function stop the timer of pm
 *
 * @param pm Pointer to power manage structure
 */
static void __wakeup_timer_stop(struct rt_pm *pm)
{
    RT_ASSERT(pm != RT_NULL);

    /* Reset pmtimer status */
    hc32_wktm_stop();
}

/**
 * This function calculate how many OS Ticks that MCU have suspended
 *
 * @param pm Pointer to power manage structure
 *
 * @return OS Ticks
 */
static rt_tick_t __wakeup_timer_get_tick(struct rt_pm *pm)
{
    rt_uint32_t timer_tick;

    RT_ASSERT(pm != RT_NULL);

    timer_tick = hc32_wktm_get_current_tick();

    return __wakeup_timer_tick_to_os_tick(timer_tick);
}

/**
 * This function initialize the power manager
 */
int drv_pm_hw_init(void)
{
    static const struct rt_pm_ops _ops =
    {
        sleep,
        run,
        __wakeup_timer_start,
        __wakeup_timer_stop,
        __wakeup_timer_get_tick
    };
    RT_ASSERT((st_sleep_deep_cfg.wait_for_wakeup_type == PM_SLEEP_WAIT_TYPE_WFI) || \
              (st_sleep_deep_cfg.wait_for_wakeup_type == PM_SLEEP_WAIT_TYPE_WFE)
             );

    rt_uint8_t timer_mask;
    timer_mask = 1UL << PM_SLEEP_MODE_DEEP;
    timer_mask |= 1UL << PM_SLEEP_MODE_SHUTDOWN;

    /* initialize system pm module */
    rt_system_pm_init(&_ops, timer_mask, RT_NULL);
    return 0;
}

INIT_DEVICE_EXPORT(drv_pm_hw_init);

#endif

#endif  /* RT_USING_PM */
