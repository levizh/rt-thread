/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-06-21     CDT               first version
 */

#include <rtdevice.h>
#include "drv_config.h"

// #define DRV_DEBUG
#define LOG_TAG             "drv.hwtimer"
#include <drv_log.h>

#ifdef BSP_USING_HWTIMER

#include "drv_irq.h"

enum
{
#ifdef BSP_USING_TIM0_1A
    TIM0_1A_INDEX,
#endif
#ifdef BSP_USING_TIM0_1B
    TIM0_1B_INDEX,
#endif
#ifdef BSP_USING_TIM0_2A
    TIM0_2A_INDEX,
#endif
#ifdef BSP_USING_TIM0_2B
    TIM0_2B_INDEX,
#endif
};

struct hc32_hwtimer
{
    rt_hwtimer_t time_device;
    CM_TMR0_TypeDef *tim_handle;
    uint32_t u32Fcg2Periph;
    uint32_t u32Ch;
    uint32_t u32Flag;
    struct
    {
        en_int_src_t enIntSrc;
        IRQn_Type enIRQn;
        uint8_t u8Int_Prio;
        func_ptr_t irq_callback;
    } isr;
    char *name;
};

static struct hc32_hwtimer hc32_hwtimer_obj[] =
{
#ifdef BSP_USING_TIM0_1A
    TIM0_1A_CONFIG,
#endif
#ifdef BSP_USING_TIM0_1B
    TIM0_1B_CONFIG,
#endif
#ifdef BSP_USING_TIM0_2A
    TIM0_2A_CONFIG,
#endif
#ifdef BSP_USING_TIM0_2B
    TIM0_2B_CONFIG,
#endif
};

static void timer_init(struct rt_hwtimer_device *timer, rt_uint32_t state)
{
    stc_tmr0_init_t stcTmr0Init;
    struct hc32_irq_config irq_config;
    struct hc32_hwtimer *tim_device = (struct hc32_hwtimer *)timer;

    RT_ASSERT(timer != RT_NULL);

    /* Interrupt configuration */
    irq_config.irq_num = tim_device->isr.enIRQn;
    irq_config.int_src = tim_device->isr.enIntSrc;
    irq_config.irq_prio = tim_device->isr.u8Int_Prio;

    if (state)  //open
    {
        /* Counter Frequency Fixed at maxfreq */
        timer->freq = timer->info->maxfreq;

        /* Enable timer0 clock */
        FCG_Fcg2PeriphClockCmd(tim_device->u32Fcg2Periph, ENABLE);

        /* TIMER0 configuration */
        (void)TMR0_StructInit(&stcTmr0Init);
        stcTmr0Init.u32ClockSrc     = TMR0_CLK_SRC_INTERN_CLK;
        stcTmr0Init.u32ClockDiv     = TMR0_CLK_DIV32;
        stcTmr0Init.u32Func         = TMR0_FUNC_CMP;
        stcTmr0Init.u16CompareValue = timer->info->maxcnt;
        (void)TMR0_Init(tim_device->tim_handle, tim_device->u32Ch, &stcTmr0Init);

        if (tim_device->u32Ch == TMR0_CH_A)
        {
            TMR0_IntCmd(tim_device->tim_handle, TMR0_INT_CMP_A, ENABLE);
        }
        else
        {
            TMR0_IntCmd(tim_device->tim_handle, TMR0_INT_CMP_B, ENABLE);
        }
        hc32_install_irq_handler(&irq_config, tim_device->isr.irq_callback, RT_TRUE);
    }
    else    //close
    {
        TMR0_DeInit(tim_device->tim_handle);
        hc32_install_irq_handler(&irq_config, tim_device->isr.irq_callback, RT_FALSE);
        FCG_Fcg2PeriphClockCmd(tim_device->u32Fcg2Periph, DISABLE);
    }
}

static rt_err_t timer_start(rt_hwtimer_t *timer, rt_uint32_t t, rt_hwtimer_mode_t opmode)
{
    rt_err_t result = RT_EOK;
    struct hc32_hwtimer *tim_device = RT_NULL;

    RT_ASSERT(timer != RT_NULL);

    tim_device = (struct hc32_hwtimer *)timer;

    /* stop timer */
    TMR0_Stop(tim_device->tim_handle, tim_device->u32Ch);
    /* reset timer cnt */
    TMR0_SetCountValue(tim_device->tim_handle, tim_device->u32Ch, 0U);
    /* set timer arr */
    TMR0_SetCompareValue(tim_device->tim_handle, tim_device->u32Ch, t - 1U);
    /* start timer */
    TMR0_Start(tim_device->tim_handle, tim_device->u32Ch);

    return result;
}

static void timer_stop(rt_hwtimer_t *timer)
{
    struct hc32_hwtimer *tim_device = RT_NULL;

    RT_ASSERT(timer != RT_NULL);

    tim_device = (struct hc32_hwtimer *)timer;

    /* stop timer */
    TMR0_Stop(tim_device->tim_handle, tim_device->u32Ch);
    /* reset timer cnt */
    TMR0_SetCountValue(tim_device->tim_handle, tim_device->u32Ch, 0U);
}

static rt_err_t timer_ctrl(rt_hwtimer_t *timer, rt_uint32_t cmd, void *arg)
{
    rt_err_t result = -RT_ERROR;
    uint32_t freq = *(uint32_t *)arg;

    RT_ASSERT(timer != RT_NULL);
    RT_ASSERT(arg != RT_NULL);

    switch (cmd)
    {
    case HWTIMER_CTRL_FREQ_SET:
    {
        if (freq != timer->freq)
        {
            LOG_W("Not Support To Set The Counter Frequency! Default is %d Hz", timer->freq);
            result = -RT_EINVAL;
        }
        else
        {
            result = RT_EOK;
        }
    }
    break;
    default:
    {
        result = -RT_EINVAL;
    }
    break;
    }

    return result;
}

static rt_uint32_t timer_counter_get(rt_hwtimer_t *timer)
{
    struct hc32_hwtimer *tim_device = RT_NULL;
    rt_uint32_t Counter;

    RT_ASSERT(timer != RT_NULL);

    tim_device = (struct hc32_hwtimer *)timer;

    Counter = TMR0_GetCountValue(tim_device->tim_handle, tim_device->u32Ch);

    return Counter;
}

#ifdef BSP_USING_TIM0_1A
static void TMR0_1A_callback(void)
{
    TMR0_ClearStatus(hc32_hwtimer_obj[TIM0_1A_INDEX].tim_handle, hc32_hwtimer_obj[TIM0_1A_INDEX].u32Flag);
    rt_device_hwtimer_isr(&hc32_hwtimer_obj[TIM0_1A_INDEX].time_device);
}
#endif
#ifdef BSP_USING_TIM0_1B
static void TMR0_1B_callback(void)
{
    TMR0_ClearStatus(hc32_hwtimer_obj[TIM0_1B_INDEX].tim_handle, hc32_hwtimer_obj[TIM0_1B_INDEX].u32Flag);
    rt_device_hwtimer_isr(&hc32_hwtimer_obj[TIM0_1B_INDEX].time_device);
}
#endif
#ifdef BSP_USING_TIM0_2A
static void TMR0_2A_callback(void)
{
    TMR0_ClearStatus(hc32_hwtimer_obj[TIM0_2A_INDEX].tim_handle, hc32_hwtimer_obj[TIM0_2A_INDEX].u32Flag);
    rt_device_hwtimer_isr(&hc32_hwtimer_obj[TIM0_2A_INDEX].time_device);
}
#endif
#ifdef BSP_USING_TIM0_2B
static void TMR0_2B_callback(void)
{
    TMR0_ClearStatus(hc32_hwtimer_obj[TIM0_2B_INDEX].tim_handle, hc32_hwtimer_obj[TIM0_2B_INDEX].u32Flag);
    rt_device_hwtimer_isr(&hc32_hwtimer_obj[TIM0_2B_INDEX].time_device);
}
#endif

static struct rt_hwtimer_info _info;
void tmr0_get_info_callback(void)
{
    /* Div = 32 */
    _info.maxcnt  = CLK_GetBusClockFreq(CLK_BUS_PCLK1) / 32U / 1000U; //Period = 1ms
    _info.maxfreq = CLK_GetBusClockFreq(CLK_BUS_PCLK1) / 32U;
    _info.minfreq = CLK_GetBusClockFreq(CLK_BUS_PCLK1) / 32U / _info.maxcnt;
    _info.cntmode = HWTIMER_CNTMODE_UP;

#ifdef BSP_USING_TIM0_1A
    hc32_hwtimer_obj[TIM0_1A_INDEX].isr.irq_callback = TMR0_1A_callback;
#endif
#ifdef BSP_USING_TIM0_1B
    hc32_hwtimer_obj[TIM0_1B_INDEX].isr.irq_callback = TMR0_1B_callback;
#endif
#ifdef BSP_USING_TIM0_2A
    hc32_hwtimer_obj[TIM0_2A_INDEX].isr.irq_callback = TMR0_2A_callback;
#endif
#ifdef BSP_USING_TIM0_2B
    hc32_hwtimer_obj[TIM0_2B_INDEX].isr.irq_callback = TMR0_2B_callback;
#endif
}

static const struct rt_hwtimer_ops _ops =
{
    .init = timer_init,
    .start = timer_start,
    .stop = timer_stop,
    .count_get = timer_counter_get,
    .control = timer_ctrl,
};

static int hc32_hwtimer_init(void)
{
    int i;
    int result = RT_EOK;

    tmr0_get_info_callback();
    for (i = 0; i < sizeof(hc32_hwtimer_obj) / sizeof(hc32_hwtimer_obj[0]); i++)
    {
        hc32_hwtimer_obj[i].time_device.info = &_info;
        hc32_hwtimer_obj[i].time_device.ops  = &_ops;
        if (rt_device_hwtimer_register(&hc32_hwtimer_obj[i].time_device,
                                       hc32_hwtimer_obj[i].name, &hc32_hwtimer_obj[i].tim_handle) == RT_EOK)
        {
            LOG_D("%s register success", hc32_hwtimer_obj[i].name);
        }
        else
        {
            LOG_E("%s register failed", hc32_hwtimer_obj[i].name);
            result = -RT_ERROR;
        }
    }

    return result;
}
INIT_BOARD_EXPORT(hc32_hwtimer_init);

#endif /* BSP_USING_HWTIMER */
