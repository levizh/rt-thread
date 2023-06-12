/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-06-09     CDT               first version
 */

#include "board.h"
#include "drv_config.h"

#ifdef RT_USING_PULSE_ENCODER

//#define DRV_DEBUG
#define LOG_TAG             "drv.pulse_encoder"
#include <drv_log.h>

#if defined(BSP_USING_TIMERA_PULSE_ENCODER)

#if !defined(BSP_USING_PULSE_ENCODER_TIMA_1) && !defined(BSP_USING_PULSE_ENCODER_TIMA_2) && !defined(BSP_USING_PULSE_ENCODER_TIMA_3) && \
    !defined(BSP_USING_PULSE_ENCODER_TIMA_4) && !defined(BSP_USING_PULSE_ENCODER_TIMA_5) && !defined(BSP_USING_PULSE_ENCODER_TIMA_6) && \
    !defined(BSP_USING_PULSE_ENCODER_TIMA_7) && !defined(BSP_USING_PULSE_ENCODER_TIMA_8) && !defined(BSP_USING_PULSE_ENCODER_TIMA_9) && \
    !defined(BSP_USING_PULSE_ENCODER_TIMA_10) && !defined(BSP_USING_PULSE_ENCODER_TIMA_11) && !defined(BSP_USING_PULSE_ENCODER_TIMA_12)
    #error "Please define at least one BSP_USING_PULSE_ENCODERx"
    /* this driver can be disabled at menuconfig -> RT-Thread Components -> Device Drivers */
#endif

enum
{
#ifdef BSP_USING_PULSE_ENCODER_TIMA_1
    PULSE_ENCODER_TIMA_1_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_2
    PULSE_ENCODER_TIMA_2_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_3
    PULSE_ENCODER_TIMA_3_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_4
    PULSE_ENCODER_TIMA_4_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_5
    PULSE_ENCODER_TIMA_5_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_6
    PULSE_ENCODER_TIMA_6_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_7
    PULSE_ENCODER_TIMA_7_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_8
    PULSE_ENCODER_TIMA_8_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_9
    PULSE_ENCODER_TIMA_9_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_10
    PULSE_ENCODER_TIMA_10_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_11
    PULSE_ENCODER_TIMA_11_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_12
    PULSE_ENCODER_TIMA_12_INDEX,
#endif
};

struct hc32_pulse_encoder_tima_device
{
    struct rt_pulse_encoder_device pulse_encoder;
    CM_TMRA_TypeDef *tim_handler;
    uint32_t u32Fcg2Periph;
    struct {
        uint16_t u16CountUpCond;
        uint16_t u16CountDownCond;
    } hw_count;
    struct {
        en_int_src_t enIntSrc_OVF;
        en_int_src_t enIntSrc_UDF;
        IRQn_Type enIRQn;
        uint8_t u8Int_Prio;
    } isr;
    rt_uint32_t u32PeriodValue;
    rt_int32_t over_under_flowcount;
    char *name;
};

static struct hc32_pulse_encoder_tima_device hc32_pulse_encoder_tima_obj[] =
{
#ifdef BSP_USING_PULSE_ENCODER_TIMA_1
    PULSE_ENCODER_TIMA_1_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_2
    PULSE_ENCODER_TIMA_2_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_3
    PULSE_ENCODER_TIMA_3_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_4
    PULSE_ENCODER_TIMA_4_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_5
    PULSE_ENCODER_TIMA_5_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_6
    PULSE_ENCODER_TIMA_6_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_7
    PULSE_ENCODER_TIMA_7_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_8
    PULSE_ENCODER_TIMA_8_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_9
    PULSE_ENCODER_TIMA_9_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_10
    PULSE_ENCODER_TIMA_10_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_11
    PULSE_ENCODER_TIMA_11_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_12
    PULSE_ENCODER_TIMA_12_CONFIG,
#endif
};

#ifdef BSP_USING_PULSE_ENCODER_TIMA_1
void TMRA_1_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_1_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_1_INDEX].over_under_flowcount++;
}
void TMRA_1_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_1_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_1_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_2
void TMRA_2_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_2_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_2_INDEX].over_under_flowcount++;
}
void TMRA_2_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_2_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_2_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_3
void TMRA_3_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_3_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_3_INDEX].over_under_flowcount++;
}
void TMRA_3_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_3_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_3_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_4
void TMRA_4_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_4_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_4_INDEX].over_under_flowcount++;
}
void TMRA_4_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_4_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_4_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_5
void TMRA_5_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_5_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_5_INDEX].over_under_flowcount++;
}
void TMRA_5_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_5_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_5_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_6
void TMRA_6_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_6_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_6_INDEX].over_under_flowcount++;
}
void TMRA_6_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_6_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_6_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_7
void TMRA_7_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_7_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_7_INDEX].over_under_flowcount++;
}
void TMRA_7_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_7_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_7_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_8
void TMRA_8_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_8_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_8_INDEX].over_under_flowcount++;
}
void TMRA_8_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_8_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_8_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_9
void TMRA_9_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_9_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_9_INDEX].over_under_flowcount++;
}
void TMRA_9_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_9_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_9_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_10
void TMRA_10_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_10_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_10_INDEX].over_under_flowcount++;
}
void TMRA_10_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_10_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_10_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_11
void TMRA_11_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_11_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_11_INDEX].over_under_flowcount++;
}
void TMRA_11_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_11_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_11_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIMA_12
void TMRA_12_Ovf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_12_INDEX].tim_handler, TMRA_FLAG_OVF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_12_INDEX].over_under_flowcount++;
}
void TMRA_12_Udf_IrqHandler(void)
{
    TMRA_ClearStatus(hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_12_INDEX].tim_handler, TMRA_FLAG_UDF);
    hc32_pulse_encoder_tima_obj[PULSE_ENCODER_TIMA_12_INDEX].over_under_flowcount--;
}
#endif

rt_err_t pulse_encoder_tima_init(struct rt_pulse_encoder_device *pulse_encoder)
{
    stc_tmra_init_t stcTmraInit;
    struct hc32_pulse_encoder_tima_device *hc32_device;
    hc32_device = (struct hc32_pulse_encoder_tima_device*)pulse_encoder;

    /* Enable TimerA peripheral clock. */
    FCG_Fcg2PeriphClockCmd(hc32_device->u32Fcg2Periph, ENABLE);

    (void)TMRA_StructInit(&stcTmraInit);
    /* Initializes position-count unit. */
    stcTmraInit.u8CountSrc = TMRA_CNT_SRC_HW;
    stcTmraInit.hw_count.u16CountUpCond   = hc32_device->hw_count.u16CountUpCond;
    stcTmraInit.hw_count.u16CountDownCond = hc32_device->hw_count.u16CountDownCond;
    stcTmraInit.u32PeriodValue = hc32_device->u32PeriodValue;
    (void)TMRA_Init(hc32_device->tim_handler, &stcTmraInit);

    /* Enable the specified interrupts of TimerA. */
    TMRA_IntCmd(hc32_device->tim_handler, TMRA_INT_OVF | TMRA_INT_UDF, ENABLE);

    /* OVF interrupt configuration */
    (void)INTC_ShareIrqCmd(hc32_device->isr.enIntSrc_OVF, ENABLE);
    /* UDF interrupt configuration */
    (void)INTC_ShareIrqCmd(hc32_device->isr.enIntSrc_UDF, ENABLE);

    NVIC_ClearPendingIRQ(hc32_device->isr.enIRQn);
    NVIC_SetPriority(hc32_device->isr.enIRQn, hc32_device->isr.u8Int_Prio);
    NVIC_EnableIRQ(hc32_device->isr.enIRQn);

    LOG_D("pulse_encoder_tima_init");

    return RT_EOK;
}

rt_err_t pulse_encoder_tima_clear_count(struct rt_pulse_encoder_device *pulse_encoder)
{
    struct hc32_pulse_encoder_tima_device *hc32_device;
    hc32_device = (struct hc32_pulse_encoder_tima_device*)pulse_encoder;
    hc32_device->over_under_flowcount = 0;
    TMRA_SetCountValue(hc32_device->tim_handler, 0);
    return RT_EOK;
}

rt_int32_t pulse_encoder_tima_get_count(struct rt_pulse_encoder_device *pulse_encoder)
{
    struct hc32_pulse_encoder_tima_device *hc32_device;
    hc32_device = (struct hc32_pulse_encoder_tima_device*)pulse_encoder;
    return (rt_int32_t)((rt_int16_t)TMRA_GetCountValue(hc32_device->tim_handler) + (hc32_device->over_under_flowcount * (hc32_device->u32PeriodValue + 1)));
}

rt_err_t pulse_encoder_tima_control(struct rt_pulse_encoder_device *pulse_encoder, rt_uint32_t cmd, void *args)
{
    rt_err_t result;
    struct hc32_pulse_encoder_tima_device *hc32_device;
    hc32_device = (struct hc32_pulse_encoder_tima_device*)pulse_encoder;

    result = RT_EOK;

    switch (cmd)
    {
    case PULSE_ENCODER_CMD_ENABLE:
        TMRA_Start(hc32_device->tim_handler);
        LOG_D("TMRA_Start");
        break;
    case PULSE_ENCODER_CMD_DISABLE:
        TMRA_Stop(hc32_device->tim_handler);
        LOG_D("TMRA_Stop");
        break;
    default:
        result = -RT_ENOSYS;
        break;
    }

    return result;
}

static const struct rt_pulse_encoder_ops tima_ops =
{
    .init = pulse_encoder_tima_init,
    .get_count = pulse_encoder_tima_get_count,
    .clear_count = pulse_encoder_tima_clear_count,
    .control = pulse_encoder_tima_control,
};

#endif /* BSP_USING_TIMERA_PULSE_ENCODER */

#if defined(BSP_USING_TIMER6_PULSE_ENCODER)

#if !defined(BSP_USING_PULSE_ENCODER_TIM6_1) && !defined(BSP_USING_PULSE_ENCODER_TIM6_2) && !defined(BSP_USING_PULSE_ENCODER_TIM6_3) && \
    !defined(BSP_USING_PULSE_ENCODER_TIM6_4) && !defined(BSP_USING_PULSE_ENCODER_TIM6_5) && !defined(BSP_USING_PULSE_ENCODER_TIM6_6) && \
    !defined(BSP_USING_PULSE_ENCODER_TIM6_7) && !defined(BSP_USING_PULSE_ENCODER_TIM6_8)
    #error "Please define at least one BSP_USING_PULSE_ENCODERx"
    /* this driver can be disabled at menuconfig -> RT-Thread Components -> Device Drivers */
#endif

enum
{
#ifdef BSP_USING_PULSE_ENCODER_TIM6_1
    PULSE_ENCODER_TIM6_1_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_2
    PULSE_ENCODER_TIM6_2_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_3
    PULSE_ENCODER_TIM6_3_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_4
    PULSE_ENCODER_TIM6_4_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_5
    PULSE_ENCODER_TIM6_5_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_6
    PULSE_ENCODER_TIM6_6_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_7
    PULSE_ENCODER_TIM6_7_INDEX,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_8
    PULSE_ENCODER_TIM6_8_INDEX,
#endif
};

struct hc32_pulse_encoder_tim6_device
{
    struct rt_pulse_encoder_device pulse_encoder;
    CM_TMR6_TypeDef *tim_handler;
    uint32_t u32Fcg2Periph;
    struct {
        uint32_t u32CountUpCond;
        uint32_t u32CountDownCond;
    } hw_count;
    struct {
        en_int_src_t enIntSrc_OVF;
        en_int_src_t enIntSrc_UDF;
        IRQn_Type enIRQn;
        uint8_t u8Int_Prio;
    } isr;
    rt_uint32_t u32PeriodValue;
    rt_int32_t over_under_flowcount;
    char *name;
};

static struct hc32_pulse_encoder_tim6_device hc32_pulse_encoder_tim6_obj[] =
{
#ifdef BSP_USING_PULSE_ENCODER_TIM6_1
    PULSE_ENCODER_TIM6_1_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_2
    PULSE_ENCODER_TIM6_2_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_3
    PULSE_ENCODER_TIM6_3_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_4
    PULSE_ENCODER_TIM6_4_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_5
    PULSE_ENCODER_TIM6_5_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_6
    PULSE_ENCODER_TIM6_6_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_7
    PULSE_ENCODER_TIM6_7_CONFIG,
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_8
    PULSE_ENCODER_TIM6_8_CONFIG,
#endif
};

#ifdef BSP_USING_PULSE_ENCODER_TIM6_1
void TMR6_1_GOvf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_1_INDEX].tim_handler, TMR6_FLAG_OVF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_1_INDEX].over_under_flowcount++;
}
void TMR6_1_GUdf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_1_INDEX].tim_handler, TMR6_FLAG_UDF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_1_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_2
void TMR6_2_GOvf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_2_INDEX].tim_handler, TMR6_FLAG_OVF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_2_INDEX].over_under_flowcount++;
}
void TMR6_2_GUdf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_2_INDEX].tim_handler, TMR6_FLAG_UDF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_2_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_3
void TMR6_3_GOvf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_3_INDEX].tim_handler, TMR6_FLAG_OVF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_3_INDEX].over_under_flowcount++;
}
void TMR6_3_GUdf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_3_INDEX].tim_handler, TMR6_FLAG_UDF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_3_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_4
void TMR6_4_GOvf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_4_INDEX].tim_handler, TMR6_FLAG_OVF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_4_INDEX].over_under_flowcount++;
}
void TMR6_4_GUdf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_4_INDEX].tim_handler, TMR6_FLAG_UDF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_4_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_5
void TMR6_5_GOvf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_5_INDEX].tim_handler, TMR6_FLAG_OVF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_5_INDEX].over_under_flowcount++;
}
void TMR6_5_GUdf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_5_INDEX].tim_handler, TMR6_FLAG_UDF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_5_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_6
void TMR6_6_GOvf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_6_INDEX].tim_handler, TMR6_FLAG_OVF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_6_INDEX].over_under_flowcount++;
}
void TMR6_6_GUdf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_6_INDEX].tim_handler, TMR6_FLAG_UDF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_6_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_7
void TMR6_7_GOvf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_7_INDEX].tim_handler, TMR6_FLAG_OVF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_7_INDEX].over_under_flowcount++;
}
void TMR6_7_GUdf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_7_INDEX].tim_handler, TMR6_FLAG_UDF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_7_INDEX].over_under_flowcount--;
}
#endif
#ifdef BSP_USING_PULSE_ENCODER_TIM6_8
void TMR6_8_GOvf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_8_INDEX].tim_handler, TMR6_FLAG_OVF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_8_INDEX].over_under_flowcount++;
}
void TMR6_8_GUdf_IrqHandler(void)
{
    TMR6_ClearStatus(hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_8_INDEX].tim_handler, TMR6_FLAG_UDF);
    hc32_pulse_encoder_tim6_obj[PULSE_ENCODER_TIM6_8_INDEX].over_under_flowcount--;
}
#endif

rt_err_t pulse_encoder_tim6_init(struct rt_pulse_encoder_device *pulse_encoder)
{
    stc_tmr6_init_t stcTmr6Init;
    struct hc32_pulse_encoder_tim6_device *hc32_device;
    hc32_device = (struct hc32_pulse_encoder_tim6_device*)pulse_encoder;

    /* Enable Timer6 peripheral clock. */
    FCG_Fcg2PeriphClockCmd(hc32_device->u32Fcg2Periph, ENABLE);

    (void)TMR6_StructInit(&stcTmr6Init);
    /* Initializes position-count unit. */
    stcTmr6Init.u8CountSrc = TMR6_CNT_SRC_HW;
    stcTmr6Init.hw_count.u32CountUpCond   = hc32_device->hw_count.u32CountUpCond;
    stcTmr6Init.hw_count.u32CountDownCond = hc32_device->hw_count.u32CountDownCond;
    stcTmr6Init.u32PeriodValue = hc32_device->u32PeriodValue;
    (void)TMR6_Init(hc32_device->tim_handler, &stcTmr6Init);

    /* Enable the specified interrupts of Timer6. */
    TMR6_IntCmd(hc32_device->tim_handler, TMR6_INT_OVF | TMR6_INT_UDF, ENABLE);

    /* OVF interrupt configuration */
    (void)INTC_ShareIrqCmd(hc32_device->isr.enIntSrc_OVF, ENABLE);
    /* UDF interrupt configuration */
    (void)INTC_ShareIrqCmd(hc32_device->isr.enIntSrc_UDF, ENABLE);

    NVIC_ClearPendingIRQ(hc32_device->isr.enIRQn);
    NVIC_SetPriority(hc32_device->isr.enIRQn, hc32_device->isr.u8Int_Prio);
    NVIC_EnableIRQ(hc32_device->isr.enIRQn);

    LOG_D("pulse_encoder_tim6_init");

    return RT_EOK;
}

rt_err_t pulse_encoder_tim6_clear_count(struct rt_pulse_encoder_device *pulse_encoder)
{
    struct hc32_pulse_encoder_tim6_device *hc32_device;
    hc32_device = (struct hc32_pulse_encoder_tim6_device*)pulse_encoder;
    hc32_device->over_under_flowcount = 0;
    TMR6_SetCountValue(hc32_device->tim_handler, 0);
    return RT_EOK;
}

rt_int32_t pulse_encoder_tim6_get_count(struct rt_pulse_encoder_device *pulse_encoder)
{
    struct hc32_pulse_encoder_tim6_device *hc32_device;
    hc32_device = (struct hc32_pulse_encoder_tim6_device*)pulse_encoder;
    return (rt_int32_t)((rt_int16_t)TMR6_GetCountValue(hc32_device->tim_handler) + (hc32_device->over_under_flowcount * (hc32_device->u32PeriodValue + 1)));
}

rt_err_t pulse_encoder_tim6_control(struct rt_pulse_encoder_device *pulse_encoder, rt_uint32_t cmd, void *args)
{
    rt_err_t result;
    struct hc32_pulse_encoder_tim6_device *hc32_device;
    hc32_device = (struct hc32_pulse_encoder_tim6_device*)pulse_encoder;

    result = RT_EOK;

    switch (cmd)
    {
    case PULSE_ENCODER_CMD_ENABLE:
        TMR6_Start(hc32_device->tim_handler);
        LOG_D("TMR6_Start");
        break;
    case PULSE_ENCODER_CMD_DISABLE:
        TMR6_Stop(hc32_device->tim_handler);
        LOG_D("TMR6_Stop");
        break;
    default:
        result = -RT_ENOSYS;
        break;
    }

    return result;
}

static const struct rt_pulse_encoder_ops tim6_ops =
{
    .init = pulse_encoder_tim6_init,
    .get_count = pulse_encoder_tim6_get_count,
    .clear_count = pulse_encoder_tim6_clear_count,
    .control = pulse_encoder_tim6_control,
};

#endif /* BSP_USING_TIMER6_PULSE_ENCODER */

int hw_pulse_encoder_init(void)
{
    int i;
    int result;

    result = RT_EOK;

#if defined(BSP_USING_TIMERA_PULSE_ENCODER)
    extern rt_err_t rt_hw_board_pulse_encoder_tmra_init(void);
    result = rt_hw_board_pulse_encoder_tmra_init();
    for (i = 0; i < sizeof(hc32_pulse_encoder_tima_obj) / sizeof(hc32_pulse_encoder_tima_obj[0]); i++)
    {
        hc32_pulse_encoder_tima_obj[i].pulse_encoder.type = AB_PHASE_PULSE_ENCODER;
        hc32_pulse_encoder_tima_obj[i].pulse_encoder.ops = &tima_ops;

        if (rt_device_pulse_encoder_register(&hc32_pulse_encoder_tima_obj[i].pulse_encoder, hc32_pulse_encoder_tima_obj[i].name, RT_NULL) != RT_EOK)
        {
            LOG_E("%s register failed", hc32_pulse_encoder_tima_obj[i].name);
            result = -RT_ERROR;
        }
    }
#endif /* BSP_USING_TIMERA_PULSE_ENCODER */

#if defined(BSP_USING_TIMER6_PULSE_ENCODER)
    extern rt_err_t rt_hw_board_pulse_encoder_tmr6_init(void);
    result = rt_hw_board_pulse_encoder_tmr6_init();
    for (i = 0; i < sizeof(hc32_pulse_encoder_tim6_obj) / sizeof(hc32_pulse_encoder_tim6_obj[0]); i++)
    {
        hc32_pulse_encoder_tim6_obj[i].pulse_encoder.type = AB_PHASE_PULSE_ENCODER;
        hc32_pulse_encoder_tim6_obj[i].pulse_encoder.ops = &tim6_ops;

        if (rt_device_pulse_encoder_register(&hc32_pulse_encoder_tim6_obj[i].pulse_encoder, hc32_pulse_encoder_tim6_obj[i].name, RT_NULL) != RT_EOK)
        {
            LOG_E("%s register failed", hc32_pulse_encoder_tim6_obj[i].name);
            result = -RT_ERROR;
        }
    }
#endif /* BSP_USING_TIMER6_PULSE_ENCODER */

    return result;
}
INIT_BOARD_EXPORT(hw_pulse_encoder_init);

#endif
