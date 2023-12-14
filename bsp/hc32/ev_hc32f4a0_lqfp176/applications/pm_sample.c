/*
* 功能
*   展示RTT休眠模式的进入和退出
*   hc32 drv_pm 支持的RTT休眠模式包括： idle、deep、standby、shutdown
*   每种休眠模式与芯片低功耗模式的对应关系是：
*       RTT                    |   HC32
*       -----------------------|----------
*       PM_SLEEP_MODE_IDLE     | 睡眠模式
*       PM_SLEEP_MODE_DEEP     | 停止模式
*       PM_SLEEP_MODE_STANDBY  | 掉电模式1或2（可配，默认配置是模式1）
*       PM_SLEEP_MODE_SHUTDOWN | 掉电模式3或4（可配，默认配置是模式3）
*
* 操作步骤：
*   1）按下按键K10：  MCU进入休眠模式
*   2）再按下按键K10：MCU退出休眠模式
*   3）重复上述按键操作，MCU循环进入休眠模式(deep、standby、shutdown、idle)和退出对应的休眠模式。
*   每次进入休眠模式前，MCU打印 "sleep:" + 休眠模式名称
*   每次退出休眠模式后，MCU打印 "wake from sleep:" + 休眠模式名称
*
*   如果定义了宏 SUPPORT_WKTM_WAKE，那么deep休眠模式有以下两种方式唤醒：
*   1）按下按键K10唤醒
*   2）经过 WKTM_WAKE_TIMEOUT 定义的毫秒时间后由 WKTM 唤醒
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drivers/lptimer.h>


#if defined(BSP_USING_PM)

#define EFM_ERASE_TIME_MAX_IN_MILLISECOND                   (20)
#define PLL_SRC                                             ((CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLSRC) >> CMU_PLLHCFGR_PLLSRC_POS)

#define KEYCNT_BACKUP_ADDR                                  (uint32_t *)(0x200F0010)
#define KEYCNT_CMD_SLEEP_NONE                               (0)
#define KEYCNT_CMD_SLEEP_IDLE                               (1)
#define KEYCNT_CMD_SLEEP_DEEP                               (3)
#define KEYCNT_CMD_SLEEP_STANDBY                            (5)
#define KEYCNT_CMD_SLEEP_SHUTDOWN                           (7)

#define IRQN_KEY10  INT001_IRQn
#define IRQN_WKTM   INT013_IRQn

#define SUPPORT_WKTM_WAKE
#define WKTM_WAKE_TIMEOUT                                   (60)

// #define PM_DBG
#if defined PM_DBG
    #define pm_dbg  rt_kprintf
#else
    #define pm_dbg
#endif

static volatile uint32_t g_keycnt_cmd;
static volatile rt_bool_t g_wkup_flag = RT_FALSE;
static struct rt_lptimer g_lptimer;

void rt_lptimer_init(rt_lptimer_t  timer,
                     const char *name,
                     void (*timeout)(void *parameter),
                     void       *parameter,
                     rt_tick_t   time,
                     rt_uint8_t  flag);

rt_err_t rt_lptimer_start(rt_lptimer_t timer);
rt_err_t rt_lptimer_stop(rt_lptimer_t timer);


static void _init_sleep(rt_bool_t b_disable_unused_clk)
{
    CLK_Xtal32Cmd(ENABLE);

    rt_tick_t tick_start = rt_tick_get_millisecond();
    rt_err_t rt_stat = RT_EOK;
    // Wait for flash to become idle
    while (SET != EFM_GetStatus(EFM_FLAG_RDY))
    {
        if (rt_tick_get_millisecond() - tick_start > EFM_ERASE_TIME_MAX_IN_MILLISECOND)
        {
            rt_stat = RT_ERROR;
            break;
        }
    }
    RT_ASSERT(rt_stat == RT_EOK);

    if (b_disable_unused_clk)
    {
        uint32_t cur_clk_src = READ_REG8_BIT(CM_CMU->CKSWR, CMU_CKSWR_CKSW);

        switch (cur_clk_src)
        {
        case CLK_SYSCLK_SRC_HRC:
            CLK_PLLCmd(DISABLE);
            CLK_MrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            CLK_XtalCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_PLL, DISABLE);
            break;
        case CLK_SYSCLK_SRC_MRC:
            CLK_PLLCmd(DISABLE);
            CLK_HrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            CLK_XtalCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_PLL | PWC_LDO_HRC, DISABLE);

            break;
        case CLK_SYSCLK_SRC_XTAL:
            CLK_PLLCmd(DISABLE);
            CLK_HrcCmd(DISABLE);
            CLK_MrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_PLL | PWC_LDO_HRC, DISABLE);

            break;
        case CLK_SYSCLK_SRC_XTAL32:
            CLK_PLLCmd(DISABLE);
            CLK_HrcCmd(DISABLE);
            CLK_MrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            CLK_XtalCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_PLL | PWC_LDO_HRC, DISABLE);

            break;
        case CLK_SYSCLK_SRC_PLL:
            if (CLK_PLL_SRC_XTAL == PLL_SRC)
            {
                CLK_HrcCmd(DISABLE);
            }
            else
            {
                CLK_XtalCmd(DISABLE);
            }
            CLK_MrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            // PWC_LDO_Cmd(PWC_LDO_HRC, DISABLE);

            break;
        default:
            break;
        }
    }
}

static void KEY10_IrqHandler(void)
{
    if (SET == EXTINT_GetExtIntStatus(EXTINT_CH00))
    {
        EXTINT_ClearExtIntStatus(EXTINT_CH00);
        __DSB();
        __ISB();
    }

    if (g_wkup_flag)
    {
        g_wkup_flag = RT_FALSE;
        return;
    }

    g_keycnt_cmd++;
    pm_dbg("g_keycnt_cmd =%d, ", g_keycnt_cmd);
    pm_dbg("recv sleep request\n");
    NVIC_DisableIRQ(IRQN_KEY10);
    NVIC_ClearPendingIRQ(IRQN_KEY10);
}

static void WKTM_IrqHandler(void)
{
    pm_dbg("Wake-up timer ovweflow.\r\n");
    if (SET == PWC_WKT_GetStatus())
    {
        PWC_WKT_ClearStatus();
        __DSB();
        __ISB();
    }
    if (SET == EXTINT_GetExtIntStatus(EXTINT_CH00))
    {
        EXTINT_ClearExtIntStatus(EXTINT_CH00);
        __DSB();
        __ISB();
        rt_pm_release(PM_SLEEP_MODE_IDLE);
        rt_pm_request(PM_SLEEP_MODE_NONE);
    }
    if (g_wkup_flag)
    {
        g_wkup_flag = RT_FALSE;
    }
    NVIC_DisableIRQ(IRQN_WKTM);
}

static void _key10_int_init(void)
{
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    // KEYSCAN_Cmd(ENABLE);
    // rt_pin_mode(GET_PIN(A, 0), PIN_MODE_INPUT_PULLUP);
    // rt_pin_irq_enable(GET_PIN(A, 0), RT_TRUE);

    /* configuration structure initialization */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    /* GPIO config */
    (void)GPIO_Init(GPIO_PORT_A, GPIO_PIN_00, &stcGpioInit);

    /* Extint config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;
    (void)EXTINT_Init(EXTINT_CH00, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = INT_SRC_PORT_EIRQ0;
    stcIrqSignConfig.enIRQn   = IRQN_KEY10;
    stcIrqSignConfig.pfnCallback = KEY10_IrqHandler;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);
    INTC_IrqSignOut(INT022_IRQn);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}

static void _wktm_int_init(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;

    stcIrqSignConfig.enIntSrc = INT_SRC_WKTM_PRD;
    stcIrqSignConfig.enIRQn   = IRQN_WKTM;
    stcIrqSignConfig.pfnCallback = WKTM_IrqHandler;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
}

static void __lptimer_timeout_cb(void *parameter)
{
    rt_interrupt_enter();
    rt_kprintf("\n lptimer callback \n");
    rt_interrupt_leave();
}

static void _lptimer_stop(void)
{
    rt_lptimer_stop(&g_lptimer);
}

static void _lptimer_start(void)
{
    rt_lptimer_start(&g_lptimer);
}

static void _wkup_cfg_sleep_deep()
{
    INTC_WakeupSrcCmd(INTC_STOP_WKUP_EXTINT_CH0, ENABLE);
#ifdef SUPPORT_WKTM_WAKE
    INTC_WakeupSrcCmd(INTC_STOP_WKUP_WKTM, ENABLE);
    NVIC_EnableIRQ(IRQN_WKTM);
#endif
}

static void _wkup_cfg_sleep_standby(void)
{
    PWC_PD_SetWakeupTriggerEdge(PWC_PD_WKUP_TRIG_WKUP0, PWC_PD_WKUP_TRIG_FALLING);
    PWC_PD_WakeupCmd(PWC_PD_WKUP_WKUP00, ENABLE);

    PWC_PD_ClearWakeupStatus(PWC_PD_WKUP_FLAG_ALL);
}
static void _wkup_cfg_sleep_shutdown(void)
{
    PWC_PD_SetWakeupTriggerEdge(PWC_PD_WKUP_TRIG_WKUP0, PWC_PD_WKUP_TRIG_FALLING);
    PWC_PD_WakeupCmd(PWC_PD_WKUP_WKUP00, ENABLE);

}

static void _sleep_enter_event_idle(void)
{
    rt_kprintf("sleep: idle\n");
}

static void _sleep_enter_event_deep(void)
{
#if (PM_SLEEP_DEEP_CFG_CLK   == PWC_STOP_CLK_KEEP)
    _init_sleep(RT_TRUE);
#else
    _init_sleep(RT_FALSE);
    CLK_PLLCmd(DISABLE);
    CLK_HrcCmd(DISABLE);
    CLK_LrcCmd(DISABLE);
    CLK_XtalCmd(DISABLE);
    PWC_LDO_Cmd(PWC_LDO_PLL | PWC_LDO_HRC, DISABLE);
#endif
    _wkup_cfg_sleep_deep();
    rt_kprintf("sleep: deep\n");
    DDL_DelayMS(50);
#ifdef SUPPORT_WKTM_WAKE
    _lptimer_start();
#endif
}

static void _sleep_enter_event_standby(void)
{
    _wkup_cfg_sleep_standby();
    PWC_BKR_Write(0, g_keycnt_cmd & 0xFF);
    *KEYCNT_BACKUP_ADDR = g_keycnt_cmd;
    rt_kprintf("sleep: standby\n");
    DDL_DelayMS(50);
}

static void _sleep_enter_event_shutdown(void)
{
    _wkup_cfg_sleep_shutdown();
    *KEYCNT_BACKUP_ADDR = g_keycnt_cmd;
    rt_kprintf("sleep: shutdown\n");
    DDL_DelayMS(50);
}

static void _sleep_exit_event_idle(void)
{
    rt_pm_release(PM_SLEEP_MODE_IDLE);
    rt_pm_request(PM_SLEEP_MODE_NONE);
    rt_kprintf("wakeup from sleep: idle\n");
}

static void _sleep_exit_event_deep(void)
{
#ifdef SUPPORT_WKTM_WAKE
    INTC_WakeupSrcCmd(INTC_STOP_WKUP_WKTM, DISABLE);
#endif
    rt_pm_release(PM_SLEEP_MODE_DEEP);
    rt_pm_request(PM_SLEEP_MODE_NONE);
    rt_kprintf("wakeup from sleep: deep\n");
}

typedef void (*notify)(void);
static notify sleep_enter_func[PM_SLEEP_MODE_MAX] =
{
    RT_NULL,
    _sleep_enter_event_idle,
    RT_NULL,
    _sleep_enter_event_deep,
    _sleep_enter_event_standby,
    _sleep_enter_event_shutdown,
};

static notify sleep_exit_func[PM_SLEEP_MODE_MAX] =
{
    RT_NULL,
    _sleep_exit_event_idle,
    RT_NULL,
    _sleep_exit_event_deep,
    RT_NULL,
    RT_NULL,
};

static void  _notify_func(uint8_t event, uint8_t mode, void *data)
{
#ifdef SUPPORT_WKTM_WAKE
    _lptimer_stop();
#endif
    if (event == RT_PM_ENTER_SLEEP)
    {
        SysTick_Suspend();
        if (sleep_enter_func[mode] == RT_NULL)
        {
            return;
        }
        sleep_enter_func[mode]();
    }
    else
    {
        SysTick_Resume();
        if (sleep_exit_func[mode] != RT_NULL)
        {
            sleep_exit_func[mode]();
        }
        g_keycnt_cmd++;
        g_wkup_flag = RT_TRUE;
        pm_dbg("g_keycnt_cmd =%d, ", g_keycnt_cmd);

        NVIC_EnableIRQ(IRQN_KEY10);
    }
}

static void pm_cmd_handler(void *parameter)
{
    rt_uint8_t sleep_mode = PM_SLEEP_MODE_NONE;

    while (1)
    {
        switch (g_keycnt_cmd)
        {
        case KEYCNT_CMD_SLEEP_IDLE:
            sleep_mode = PM_SLEEP_MODE_IDLE;
            break;
        case KEYCNT_CMD_SLEEP_DEEP:
            sleep_mode = PM_SLEEP_MODE_DEEP;
            break;
        case KEYCNT_CMD_SLEEP_STANDBY:
            sleep_mode = PM_SLEEP_MODE_STANDBY;
            break;
        case KEYCNT_CMD_SLEEP_SHUTDOWN:
            sleep_mode = PM_SLEEP_MODE_SHUTDOWN;
            break;

        default:
            rt_thread_mdelay(50);
            continue;;
            break;
        }
        rt_pm_request(sleep_mode);
        rt_pm_release(PM_SLEEP_MODE_NONE);
        rt_thread_mdelay(500);
    }
}

static void _keycnt_cmd_init_after_power_on(void)
{
    en_flag_status_t wkup_from_wktm = PWC_PD_GetWakeupStatus(PWC_PD_WKUP_FLAG_WKTM);
    en_flag_status_t wkup_from_ptwk = PWC_PD_GetWakeupStatus(PWC_PD_WKUP_FLAG_WKUP0);
    en_flag_status_t bakram_pd = PWC_BKR_GetStatus(PWC_BACKUP_RAM_FLAG_RAMPDF);
    uint8_t bkr0 = PWC_BKR_Read(0);

    if (bakram_pd == RT_TRUE)
    {
        g_keycnt_cmd = KEYCNT_CMD_SLEEP_NONE;
    }
    else
    {
        g_keycnt_cmd = *KEYCNT_BACKUP_ADDR;
        if (g_keycnt_cmd == KEYCNT_CMD_SLEEP_STANDBY)
        {
            if (wkup_from_ptwk || wkup_from_wktm)
            {
                g_keycnt_cmd++;
                pm_dbg("g_keycnt_cmd =%d, ", g_keycnt_cmd);
                rt_kprintf("wakeup from sleep: standby\n\n");
            }
            else
            {
                g_keycnt_cmd = KEYCNT_CMD_SLEEP_NONE;
            }
        }
        else if (g_keycnt_cmd >= KEYCNT_CMD_SLEEP_SHUTDOWN)
        {
            if ((g_keycnt_cmd == KEYCNT_CMD_SLEEP_SHUTDOWN) && \
                    (wkup_from_ptwk || wkup_from_wktm)         \
               )
            {
                pm_dbg("g_keycnt_cmd =%d \n", KEYCNT_CMD_SLEEP_NONE);
                rt_kprintf("wakeup from sleep: shutdown\n\n");
            }
            g_keycnt_cmd = KEYCNT_CMD_SLEEP_NONE;
        }
    }

    pm_dbg("KEYCNT_BACKUP_ADDR addr =0x%p,value = %d\n", KEYCNT_BACKUP_ADDR, *KEYCNT_BACKUP_ADDR);
    pm_dbg("wkup_from_wktm = %d\n", wkup_from_wktm);
    pm_dbg("wkup_from_ptwk = %d\n", wkup_from_ptwk);
    pm_dbg("bakram_pd = %d\n", bakram_pd);
    pm_dbg("bkr0 = %d\n", bkr0);
}

static void _vbat_init(void)
{
    while (PWC_BKR_GetStatus(PWC_BACKUP_RAM_FLAG_RAMVALID) == RESET)
    {
        rt_thread_sleep(10);
    }
    FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_SRAMB, ENABLE);
    pm_dbg("vbat init success\n");
}

int pm_sample_init(void)
{
    pm_dbg("pm_sample_init\n\n");

    _keycnt_cmd_init_after_power_on();
    _vbat_init();
    _key10_int_init();
#ifdef SUPPORT_WKTM_WAKE
    _wktm_int_init();
    rt_lptimer_init(&g_lptimer,
                    "lpm",
                    __lptimer_timeout_cb,
                    RT_NULL,
                    WKTM_WAKE_TIMEOUT,
                    RT_TIMER_FLAG_PERIODIC);
#endif
    rt_pm_notify_set(_notify_func, NULL);

    rt_thread_t  thread = rt_thread_create("pm_cmd_handler", pm_cmd_handler, RT_NULL, 1024, 25, 10);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        rt_kprintf("create pm sample thread failed!\n");
    }
}

INIT_APP_EXPORT(pm_sample_init);

#endif
