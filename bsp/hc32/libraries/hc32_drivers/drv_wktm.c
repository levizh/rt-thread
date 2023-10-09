/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-09     CDT          first version
 */

#include <board.h>
#include <drv_wktm.h>

#if defined(RT_USING_PM)

#if defined(BSP_USING_PM)

// #define DRV_DEBUG
#define LOG_TAG             "drv_wktm"
#include <drv_log.h>

#define CMPVAL_MAX          0xFFFU

#if defined(HC32F460)
#define WKTM_IRQn           (INT130_IRQn)
#elif defined(HC32F4A0)
#define WKTM_IRQn           (INT131_IRQn)
#endif

static rt_uint32_t cmpval = CMPVAL_MAX;

/**
 * @brief  Wakeup timer IRQ handler
 * @param  None
 * @retval None
 */
void PWC_WakeupTimer_IrqHandler(void)
{
    if (SET == PWC_WKT_GetStatus())
    {
        LOG_D("Wake-up timer overflow.");
        PWC_WKT_ClearStatus();
    }
}

/**
 * This function get current count value of WKTM
 * @param  None
 * @return the count vlaue
 */
rt_uint32_t hc32_wktm_get_current_tick(void)
{
    /* TODO */
    return (CMPVAL_MAX);
}

/**
 * This function get the max value that WKTM can count
 * @param  None
 * @return the max count
 */
rt_uint32_t hc32_wktm_get_tick_max(void)
{
    return (CMPVAL_MAX);
}

/**
 * This function start WKTM with reload value
 * @param reload The value that Comparison value of the Counter
 * @return RT_EOK
 */
rt_err_t hc32_wktm_start(rt_uint32_t reload)
{
    if (reload > CMPVAL_MAX || !reload)
    {
        reload = CMPVAL_MAX;
    }
    cmpval = reload;
    PWC_WKT_SetCompareValue(cmpval);
    PWC_WKT_Cmd(ENABLE);

    return RT_EOK;
}

/**
 * @brief  This function stop WKTM
 * @param  None
 * @retval None
 */
void hc32_wktm_stop(void)
{
    PWC_WKT_Cmd(DISABLE);
}

/**
 * This function get the count clock of WKTM
 * @param  None
 * @return the count clock frequency in Hz
 */
rt_uint32_t hc32_wktm_get_countfreq(void)
{
    rt_uint32_t freq = 0;
    freq = 64U / (float)cmpval;
    return freq;
}

/**
 * @brief  This function initialize the wktm
 * @param  None
 * @retval type code
 */
int hc32_hw_wktm_init(void)
{
    rt_err_t ret = RT_EOK;

    /* Wake up by WKTM */
    PWC_PD_WakeupCmd(PWC_PD_WKUP_WKTM, ENABLE);
    /* Disable WKTM inadvance */
    PWC_WKT_Cmd(DISABLE);
    /* WKTM init */
    PWC_WKT_Config(PWC_WKT_CLK_SRC_64HZ, CMPVAL_MAX);
    /* Wakeup timer NVIC config, not mandatory for this sample */
    (void)INTC_ShareIrqCmd(INT_SRC_WKTM_PRD, ENABLE);
    NVIC_ClearPendingIRQ(WKTM_IRQn);
    NVIC_SetPriority(WKTM_IRQn, DDL_IRQ_PRIO_DEFAULT);
    NVIC_EnableIRQ(WKTM_IRQn);

    return ret;
}

INIT_DEVICE_EXPORT(hc32_hw_wktm_init);

#endif

#endif  /* RT_USING_PM */
