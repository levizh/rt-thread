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
*   3）重复上述按键操作，MCU循环进入休眠模式(idle、deep、standby、shutdown)和退出对应的休眠模式。
*   每次进入休眠模式前，MCU打印 "sleep:" + 休眠模式名称
*   每次退出休眠模式后，MCU打印 "wake from sleep:" + 休眠模式名称
*
* 代码使用方法
*   在 main 函数 while 循环之前调用pm_sample_init()
*   在 main 函数 while 循环之内调用pm_sample_func()
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define EFM_ERASE_TIME_MAX_IN_MILLISECOND                   (20)
#define PLL_SRC                                             ((CM_CMU->PLLCFGR & CMU_PLLCFGR_PLLSRC) >> CMU_PLLCFGR_PLLSRC_POS)

#define KEYCNT_BACKUP_ADDR                                  (uint32_t *)(0x200F0010)
#define KEYCNT_CMD_SLEEP_NONE                               (0)
#define KEYCNT_CMD_SLEEP_IDLE                               (1)
#define KEYCNT_CMD_SLEEP_DEEP                               (3)
#define KEYCNT_CMD_SLEEP_STANDBY                            (5)
#define KEYCNT_CMD_SLEEP_SHUTDOWN                           (7)

#define IRQN_KEY10  INT001_IRQn
#define IRQN_WKTM   INT013_IRQn


volatile uint32_t g_keycnt_cmd;
volatile rt_bool_t g_wkup_flag = RT_FALSE;

static void _init_sleep(rt_bool_t b_disable_unused_clk)
{
    CLK_Xtal32Cmd(ENABLE);

    rt_tick_t tick_start = rt_tick_get_millisecond();
    rt_err_t rt_stat = RT_EOK;
    //wait flash idle
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

void KEY10_IrqHandler(void)
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
    // rt_kprintf("g_keycnt_cmd =%d, ", g_keycnt_cmd);
    // rt_kprintf("recv sleep request\n");
    NVIC_DisableIRQ(IRQN_KEY10);
    INTC_EventCmd(INTC_EVT1, ENABLE);
}

void WKTM_IrqHandler(void)
{
    if (SET == EXTINT_GetExtIntStatus(EXTINT_CH00))
    {
        EXTINT_ClearExtIntStatus(EXTINT_CH00);
        __DSB();
        __ISB();
        rt_pm_release(PM_SLEEP_MODE_IDLE);
        rt_pm_request(PM_SLEEP_MODE_NONE);
    }
}

void _key10_int_init(void)
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
    (void)GPIO_Init(GPIO_PORT_B, GPIO_PIN_01, &stcGpioInit);

    /* Extint config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;
    (void)EXTINT_Init(EXTINT_CH01, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = INT_SRC_PORT_EIRQ1;
    stcIrqSignConfig.enIRQn   = IRQN_KEY10;
    stcIrqSignConfig.pfnCallback = KEY10_IrqHandler;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}

void _wktm_int_init(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;

    stcIrqSignConfig.enIntSrc = INT_SRC_WKTM_PRD;
    stcIrqSignConfig.enIRQn   = IRQN_WKTM;
    stcIrqSignConfig.pfnCallback = WKTM_IrqHandler;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
    NVIC_EnableIRQ(IRQN_WKTM);
}

void _wkup_cfg_sleep_deep()
{
    INTC_WakeupSrcCmd(INTC_STOP_WKUP_EXTINT_CH1, ENABLE);
}

void _wkup_cfg_sleep_standby(void)
{
    PWC_PD_SetWakeupTriggerEdge(PWC_PD_WKUP_TRIG_WKUP1, PWC_PD_WKUP_TRIG_FALLING);
    PWC_PD_WakeupCmd(PWC_PD_WKUP_WKUP01, ENABLE);

    PWC_PD_ClearWakeupStatus(PWC_PD_WKUP_FLAG_ALL);
}
void _wkup_cfg_sleep_shutdown(void)
{
    PWC_PD_SetWakeupTriggerEdge(PWC_PD_WKUP_TRIG_WKUP1, PWC_PD_WKUP_TRIG_FALLING);
    PWC_PD_WakeupCmd(PWC_PD_WKUP_WKUP01, ENABLE);

    NVIC_EnableIRQ(IRQN_WKTM);
    PWC_PD_WakeupCmd(PWC_PD_WKUP_WKTM, ENABLE);

    PWC_PD_ClearWakeupStatus(PWC_PD_WKUP_FLAG_ALL);
}

void _sleep_enter_event_idle(void)
{
    NVIC_DisableIRQ(IRQN_WKTM);
    rt_kprintf("sleep: idle\n");
}

void _sleep_enter_event_deep(void)
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
}

void _sleep_enter_event_standby(void)
{
    _wkup_cfg_sleep_standby();
    *KEYCNT_BACKUP_ADDR = g_keycnt_cmd;
    rt_kprintf("sleep: standby\n");
    DDL_DelayMS(50);
}

void _sleep_enter_event_shutdown(void)
{
    NVIC_EnableIRQ(IRQN_WKTM);
    _wkup_cfg_sleep_shutdown();
    *KEYCNT_BACKUP_ADDR = g_keycnt_cmd;
    rt_kprintf("sleep: shutdown\n");
    DDL_DelayMS(50);
}

void _sleep_exit_event_idle(void)
{
    rt_pm_release(PM_SLEEP_MODE_IDLE);
    rt_pm_request(PM_SLEEP_MODE_NONE);
    rt_kprintf("wakeup from sleep: idle\n");
}

void _sleep_exit_event_deep(void)
{
    PWC_STOP_ClockRecover();
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

void  _notify_func(uint8_t event, uint8_t mode, void *data)
{
    if (event == RT_PM_ENTER_SLEEP)
    {
        SysTick->CTRL  = 0;
        if (sleep_enter_func[mode] == RT_NULL)
        {
            return;
        }
        sleep_enter_func[mode]();
    }
    else
    {
        SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                         SysTick_CTRL_TICKINT_Msk   |
                         SysTick_CTRL_ENABLE_Msk;
        if (sleep_exit_func[mode] == RT_NULL)
        {
            return;
        }
        g_keycnt_cmd++;
        g_wkup_flag = RT_TRUE;
        // rt_kprintf("g_keycnt_cmd =%d, ", g_keycnt_cmd);
        sleep_exit_func[mode]();
        NVIC_EnableIRQ(IRQN_KEY10);
    }
}

void pm_sample_func(void)
{
    rt_uint8_t sleep_mode = PM_SLEEP_MODE_NONE;
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
        return;

        break;
    }
    rt_pm_request(sleep_mode);
    rt_pm_release(PM_SLEEP_MODE_NONE);
}

void _keycnt_cmd_init_after_power_on(void)
{
    en_flag_status_t wkup_from_wktm = PWC_PD_GetWakeupStatus(PWC_PD_WKUP_FLAG_WKTM);
    en_flag_status_t wkup_from_ptwk = PWC_PD_GetWakeupStatus(PWC_PD_WKUP_FLAG_WKUP0);

    g_keycnt_cmd = *KEYCNT_BACKUP_ADDR;
    if (g_keycnt_cmd == KEYCNT_CMD_SLEEP_STANDBY)
    {
        if (wkup_from_ptwk || wkup_from_wktm)
        {
            g_keycnt_cmd++;
            // rt_kprintf("g_keycnt_cmd =%d\n", g_keycnt_cmd);
            rt_kprintf("wakeup from sleep: standby\n\n");
        }
        else
        {
            g_keycnt_cmd = KEYCNT_CMD_SLEEP_NONE;
        }
    }
    else if (g_keycnt_cmd >= KEYCNT_CMD_SLEEP_SHUTDOWN)
    {
        if ((wkup_from_ptwk || wkup_from_wktm))
        {
            // rt_kprintf("g_keycnt_cmd =%d \n", KEYCNT_CMD_SLEEP_NONE);
            rt_kprintf("wakeup from sleep: shutdown\n\n");
        }
        g_keycnt_cmd = KEYCNT_CMD_SLEEP_NONE;
    }

    // rt_kprintf("KEYCNT_BACKUP_ADDR addr =0x%p,value = %d\n", KEYCNT_BACKUP_ADDR, *KEYCNT_BACKUP_ADDR);
    // rt_kprintf("wkup_from_wktm = %d\n", wkup_from_wktm);
    // rt_kprintf("wkup_from_ptwk = %d\n", wkup_from_ptwk);
}

void pm_sample_init(void)
{
    // rt_kprintf("pm_sample_init\n\n");

    _keycnt_cmd_init_after_power_on();
    _key10_int_init();
    _wktm_int_init();
    FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_SRAMRET, ENABLE);
    SET_REG32_BIT(SCB->SCR, SCB_SCR_SEVONPEND_Msk);

    rt_pm_notify_set(_notify_func, NULL);
}
