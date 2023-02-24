/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2022-06-27     lianghongquan     first version
 * 2023-02-22     CDT               add hc32f4a0 support
 */

#include <board.h>

#if defined(BSP_USING_PWM_TMRA)

#include "drv_config.h"
#include <drivers/rt_drv_pwm.h>
#include <pwm_tmra_config.h>

//#define DRV_DEBUG
#define LOG_TAG             "drv_pwm"
#include <drv_log.h>

#if defined(HC32F460)
#define CHANNEL_NUM_MAX     8U
#elif defined(HC32F4A0)
#define CHANNEL_NUM_MAX     4U
#endif

enum
{
#ifdef BSP_USING_PWM_TMRA_1
    PWM_TMRA_1_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_2
    PWM_TMRA_2_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_3
    PWM_TMRA_3_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_4
    PWM_TMRA_4_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_5
    PWM_TMRA_5_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_6
    PWM_TMRA_6_INDEX,
#endif
#if defined(HC32F4A0)
#ifdef BSP_USING_PWM_TMRA_7
    PWM_TMRA_7_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_8
    PWM_TMRA_8_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_9
    PWM_TMRA_9_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_10
    PWM_TMRA_10_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_11
    PWM_TMRA_11_INDEX,
#endif
#ifdef BSP_USING_PWM_TMRA_12
    PWM_TMRA_12_INDEX,
#endif
#endif
};

struct hc32_pwm_tmra
{
    struct rt_device_pwm pwm_device;
    CM_TMRA_TypeDef *instance;
    stc_tmra_init_t stcTmraInit;
    stc_tmra_pwm_init_t stcPwmInit;
    rt_uint32_t channel;
    char *name;
};

static struct hc32_pwm_tmra g_pwm_dev_array[] =
{
#ifdef BSP_USING_PWM_TMRA_1
    PWM_TMRA_1_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_2
    PWM_TMRA_2_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_3
    PWM_TMRA_3_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_4
    PWM_TMRA_4_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_5
    PWM_TMRA_5_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_6
    PWM_TMRA_6_CONFIG,
#endif
#if defined(HC32F4A0)
#ifdef BSP_USING_PWM_TMRA_7
    PWM_TMRA_7_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_8
    PWM_TMRA_8_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_9
    PWM_TMRA_9_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_10
    PWM_TMRA_10_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_11
    PWM_TMRA_11_CONFIG,
#endif
#ifdef BSP_USING_PWM_TMRA_12
    PWM_TMRA_12_CONFIG,
#endif
#endif
};

static rt_uint32_t get_tmra_clk_notdiv(CM_TMRA_TypeDef *TMRAx)
{
    rt_uint32_t u32clkFreq;
    switch ((rt_uint32_t)TMRAx)
    {
    case (rt_uint32_t)CM_TMRA_1:
    case (rt_uint32_t)CM_TMRA_2:
    case (rt_uint32_t)CM_TMRA_3:
    case (rt_uint32_t)CM_TMRA_4:
        u32clkFreq = CLK_GetBusClockFreq(CLK_BUS_PCLK0);
        break;
#if defined(HC32F4A0)
    case (rt_uint32_t)CM_TMRA_5:
    case (rt_uint32_t)CM_TMRA_6:
    case (rt_uint32_t)CM_TMRA_7:
    case (rt_uint32_t)CM_TMRA_8:
    case (rt_uint32_t)CM_TMRA_9:
    case (rt_uint32_t)CM_TMRA_10:
    case (rt_uint32_t)CM_TMRA_11:
    case (rt_uint32_t)CM_TMRA_12:
        u32clkFreq = CLK_GetBusClockFreq(CLK_BUS_PCLK1);
        break;
#endif
    default:
        break;
    }
    return (u32clkFreq ? u32clkFreq : HCLK_VALUE);
}

static rt_uint32_t get_tmra_clk_bydiv(CM_TMRA_TypeDef *TMRAx)
{
    rt_uint32_t u32clkFreq;
    uint16_t u16Div;

    u32clkFreq = get_tmra_clk_notdiv(TMRAx);
    u16Div = (READ_REG16(TMRAx->BCSTR) & TMRA_BCSTR_CKDIV);
    switch (u16Div)
    {
        case (TMRA_CLK_DIV1):
            break;
        case (TMRA_CLK_DIV2):
            u32clkFreq /= 2;
            break;
        case (TMRA_CLK_DIV4):
            u32clkFreq /= 4;
            break;
        case (TMRA_CLK_DIV8):
            u32clkFreq /= 8;
            break;
        case (TMRA_CLK_DIV16):
            u32clkFreq /= 16;
            break;
        case (TMRA_CLK_DIV32):
            u32clkFreq /= 32;
            break;
        case (TMRA_CLK_DIV64):
            u32clkFreq /= 64;
            break;
        case (TMRA_CLK_DIV128):
            u32clkFreq /= 128;
            break;
        case (TMRA_CLK_DIV256):
            u32clkFreq /= 256;
            break;
        case (TMRA_CLK_DIV512):
            u32clkFreq /= 512;
            break;
        case (TMRA_CLK_DIV1024):
            u32clkFreq /= 1024;
            break;
        default:
            break;
    }
    return u32clkFreq;
}

static rt_err_t drv_pwm_control(struct rt_device_pwm *device, int cmd, void *arg);
static struct rt_pwm_ops drv_ops =
{
    drv_pwm_control
};

static void tmra_pwm_enable(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
    rt_uint8_t i;
    for (i=0; i<CHANNEL_NUM_MAX; i++)
    {
        if (g_pwm_dev_array[i].instance == TMRAx)
        {
            stc_tmra_pwm_init_t *stcPwmInit = &g_pwm_dev_array[i].stcPwmInit;
            stcPwmInit->u32CompareValue = TMRA_GetCompareValue(TMRAx, configuration->channel);
            TMRA_PWM_Init(TMRAx, configuration->channel, stcPwmInit);
            break;
        }
    }
}

static void tmra_pwmn_enable(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
    stc_tmra_pwm_init_t stcPwmNInit;
    stcPwmNInit.u32CompareValue = TMRA_GetCompareValue(TMRAx, configuration->channel);
    stcPwmNInit.u16StartPolarity = TMRA_PWM_HIGH;
    stcPwmNInit.u16StopPolarity = TMRA_PWM_HIGH;
    stcPwmNInit.u16CompareMatchPolarity = TMRA_PWM_LOW;
    stcPwmNInit.u16PeriodMatchPolarity = TMRA_PWM_HIGH;
    TMRA_PWM_Init(TMRAx, configuration->channel, &stcPwmNInit);
}

static rt_err_t drv_pwm_enable(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration, rt_bool_t enable)
{
    /* channel0 will set complementary but belong to pwm_ch1 */
    if (configuration->complementary && configuration->channel != 0)
    {
        tmra_pwmn_enable(TMRAx, configuration);
    } else {
        tmra_pwm_enable(TMRAx, configuration);
    }

    if (enable)
    {
        TMRA_PWM_OutputCmd(TMRAx, configuration->channel, ENABLE);
        TMRA_Start(TMRAx);
    }
    else
    {
        TMRA_PWM_OutputCmd(TMRAx, configuration->channel, DISABLE);
        TMRA_Stop(TMRAx);
    }

    return RT_EOK;
}

static rt_err_t drv_pwm_get(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    u32clkFreq = get_tmra_clk_bydiv(TMRAx);
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
    configuration->period = u64clk_ns * TMRA_GetPeriodValue(TMRAx);
    configuration->pulse = u64clk_ns * TMRA_GetCompareValue(TMRAx, configuration->channel);
    return RT_EOK;
}

static rt_err_t drv_pwm_set(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration, int cmd)
{
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    rt_uint64_t u64val;
    rt_uint64_t pulse_now;

    u32clkFreq = get_tmra_clk_bydiv(TMRAx);
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
    u64val = (rt_uint64_t)configuration->period / u64clk_ns;
    /* We need to read the current pulse value because it is 0 when we set period only */
    if (configuration->pulse == 0)
    {
        pulse_now = (TMRA_GetCompareValue(TMRAx, configuration->channel) * u64clk_ns);
    }
    /* clk not match, need change div */
    if ((configuration->period != 0 && configuration->period <= u64clk_ns) || (u64val > 0xFFFF))
    {
        uint32_t div_bit;
        u32clkFreq = get_tmra_clk_notdiv(TMRAx);
        u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
        u64val = (rt_uint64_t)configuration->period / u64clk_ns;
        for (div_bit = 0; div_bit <= 10; div_bit++)
        {
            if (u64val < 0xFFFF)
            {
                break;
            }
            u64val /= 2;
        }
        if (div_bit > 10)
        {
            return -RT_ERROR;
        }
        TMRA_SetClockDiv(TMRAx, div_bit << TMRA_BCSTR_CKDIV_POS);
        u32clkFreq = get_tmra_clk_bydiv(TMRAx);
        u64clk_ns = (rt_uint64_t)(1000000000ul / u32clkFreq);
    }

    if (cmd == PWM_CMD_SET || cmd == PWM_CMD_SET_PERIOD)
    {
        TMRA_SetPeriodValue(TMRAx, configuration->period / u64clk_ns);
        /* When resetting the period only, you need to recalculate and set the pulse value to prevent clock div changes */
        if (configuration->pulse == 0)
        {
            TMRA_SetCompareValue(TMRAx, configuration->channel, pulse_now / u64clk_ns);
        }
    }
    if (cmd == PWM_CMD_SET || cmd == PWM_CMD_SET_PULSE)
    {
        TMRA_SetCompareValue(TMRAx, configuration->channel, configuration->pulse / u64clk_ns);
    }
    return RT_EOK;
}

static rt_err_t drv_pwm_control(struct rt_device_pwm *device, int cmd, void *arg)
{
    struct rt_pwm_configuration *configuration = (struct rt_pwm_configuration *)arg;
    struct hc32_pwm_tmra *pwm;
    pwm = rt_container_of(device, struct hc32_pwm_tmra, pwm_device);
    CM_TMRA_TypeDef *TMRAx = pwm->instance;

    switch (cmd)
    {
    case PWM_CMD_ENABLE:
        return drv_pwm_enable(TMRAx, configuration, RT_TRUE);
    case PWM_CMD_DISABLE:
        return drv_pwm_enable(TMRAx, configuration, RT_FALSE);
    case PWM_CMD_SET:
        return drv_pwm_set(TMRAx, configuration, PWM_CMD_SET);
    case PWM_CMD_GET:
        return drv_pwm_get(TMRAx, configuration);
    case PWM_CMD_SET_PERIOD:
        return drv_pwm_set(TMRAx, configuration, PWM_CMD_SET_PERIOD);
    case PWM_CMD_SET_PULSE:
        return drv_pwm_set(TMRAx, configuration, PWM_CMD_SET_PULSE);
    default:
        return -RT_EINVAL;
    }
}

static rt_err_t pwm_tmra_init(struct hc32_pwm_tmra *device)
{
    CM_TMRA_TypeDef *TMRAx;
    uint32_t i;
    RT_ASSERT(device != RT_NULL);

    TMRAx = device->instance;
    TMRA_Init(TMRAx, &device->stcTmraInit);
    for (i=0; i<CHANNEL_NUM_MAX; i++)
    {
        if ((device->channel >> i) & 0x01)
        {
            TMRA_PWM_Init(TMRAx, i, &device->stcPwmInit);
        }
    }
    return RT_EOK;
}

static void pwm_get_channel(void)
{
#ifdef BSP_USING_PWM_TMRA_1
    #ifdef BSP_USING_PWM_TMRA_1_CH1
        g_pwm_dev_array[PWM_TMRA_1_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_1_CH2
        g_pwm_dev_array[PWM_TMRA_1_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_1_CH3
        g_pwm_dev_array[PWM_TMRA_1_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_1_CH4
        g_pwm_dev_array[PWM_TMRA_1_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TMRA_1_CH5
        g_pwm_dev_array[PWM_TMRA_1_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TMRA_1_CH6
        g_pwm_dev_array[PWM_TMRA_1_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TMRA_1_CH7
        g_pwm_dev_array[PWM_TMRA_1_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TMRA_1_CH8
        g_pwm_dev_array[PWM_TMRA_1_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TMRA_2
    #ifdef BSP_USING_PWM_TMRA_2_CH1
        g_pwm_dev_array[PWM_TMRA_2_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_2_CH2
        g_pwm_dev_array[PWM_TMRA_2_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_2_CH3
        g_pwm_dev_array[PWM_TMRA_2_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_2_CH4
        g_pwm_dev_array[PWM_TMRA_2_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TMRA_2_CH5
        g_pwm_dev_array[PWM_TMRA_2_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TMRA_2_CH6
        g_pwm_dev_array[PWM_TMRA_2_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TMRA_2_CH7
        g_pwm_dev_array[PWM_TMRA_2_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TMRA_2_CH8
        g_pwm_dev_array[PWM_TMRA_2_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TMRA_3
    #ifdef BSP_USING_PWM_TMRA_3_CH1
        g_pwm_dev_array[PWM_TMRA_3_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_3_CH2
        g_pwm_dev_array[PWM_TMRA_3_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_3_CH3
        g_pwm_dev_array[PWM_TMRA_3_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_3_CH4
        g_pwm_dev_array[PWM_TMRA_3_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TMRA_3_CH5
        g_pwm_dev_array[PWM_TMRA_3_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TMRA_3_CH6
        g_pwm_dev_array[PWM_TMRA_3_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TMRA_3_CH7
        g_pwm_dev_array[PWM_TMRA_3_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TMRA_3_CH8
        g_pwm_dev_array[PWM_TMRA_3_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TMRA_4
    #ifdef BSP_USING_PWM_TMRA_4_CH1
        g_pwm_dev_array[PWM_TMRA_4_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_4_CH2
        g_pwm_dev_array[PWM_TMRA_4_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_4_CH3
        g_pwm_dev_array[PWM_TMRA_4_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_4_CH4
        g_pwm_dev_array[PWM_TMRA_4_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TMRA_4_CH5
        g_pwm_dev_array[PWM_TMRA_4_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TMRA_4_CH6
        g_pwm_dev_array[PWM_TMRA_4_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TMRA_4_CH7
        g_pwm_dev_array[PWM_TMRA_4_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TMRA_4_CH8
        g_pwm_dev_array[PWM_TMRA_4_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TMRA_5
    #ifdef BSP_USING_PWM_TMRA_5_CH1
        g_pwm_dev_array[PWM_TMRA_5_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_5_CH2
        g_pwm_dev_array[PWM_TMRA_5_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_5_CH3
        g_pwm_dev_array[PWM_TMRA_5_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_5_CH4
        g_pwm_dev_array[PWM_TMRA_5_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TMRA_5_CH5
        g_pwm_dev_array[PWM_TMRA_5_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TMRA_5_CH6
        g_pwm_dev_array[PWM_TMRA_5_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TMRA_5_CH7
        g_pwm_dev_array[PWM_TMRA_5_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TMRA_5_CH8
        g_pwm_dev_array[PWM_TMRA_5_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TMRA_6
    #ifdef BSP_USING_PWM_TMRA_6_CH1
        g_pwm_dev_array[PWM_TMRA_6_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_6_CH2
        g_pwm_dev_array[PWM_TMRA_6_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_6_CH3
        g_pwm_dev_array[PWM_TMRA_6_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_6_CH4
        g_pwm_dev_array[PWM_TMRA_6_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TMRA_6_CH5
        g_pwm_dev_array[PWM_TMRA_6_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TMRA_6_CH6
        g_pwm_dev_array[PWM_TMRA_6_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TMRA_6_CH7
        g_pwm_dev_array[PWM_TMRA_6_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TMRA_6_CH8
        g_pwm_dev_array[PWM_TMRA_6_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#if defined(HC32F4A0)
#ifdef BSP_USING_PWM_TMRA_7
    #ifdef BSP_USING_PWM_TMRA_7_CH1
        g_pwm_dev_array[PWM_TMRA_7_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_7_CH2
        g_pwm_dev_array[PWM_TMRA_7_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_7_CH3
        g_pwm_dev_array[PWM_TMRA_7_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_7_CH4
        g_pwm_dev_array[PWM_TMRA_7_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TMRA_8
    #ifdef BSP_USING_PWM_TMRA_8_CH1
        g_pwm_dev_array[PWM_TMRA_8_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_8_CH2
        g_pwm_dev_array[PWM_TMRA_8_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_8_CH3
        g_pwm_dev_array[PWM_TMRA_8_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_8_CH4
        g_pwm_dev_array[PWM_TMRA_8_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TMRA_9
    #ifdef BSP_USING_PWM_TMRA_9_CH1
        g_pwm_dev_array[PWM_TMRA_9_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_9_CH2
        g_pwm_dev_array[PWM_TMRA_9_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_9_CH3
        g_pwm_dev_array[PWM_TMRA_9_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_9_CH4
        g_pwm_dev_array[PWM_TMRA_9_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TMRA_10
    #ifdef BSP_USING_PWM_TMRA_10_CH1
        g_pwm_dev_array[PWM_TMRA_10_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_10_CH2
        g_pwm_dev_array[PWM_TMRA_10_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_10_CH3
        g_pwm_dev_array[PWM_TMRA_10_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_10_CH4
        g_pwm_dev_array[PWM_TMRA_10_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TMRA_11
    #ifdef BSP_USING_PWM_TMRA_11_CH1
        g_pwm_dev_array[PWM_TMRA_11_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_11_CH2
        g_pwm_dev_array[PWM_TMRA_11_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_11_CH3
        g_pwm_dev_array[PWM_TMRA_11_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_11_CH4
        g_pwm_dev_array[PWM_TMRA_11_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TMRA_12
    #ifdef BSP_USING_PWM_TMRA_12_CH1
        g_pwm_dev_array[PWM_TMRA_12_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TMRA_12_CH2
        g_pwm_dev_array[PWM_TMRA_12_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TMRA_12_CH3
        g_pwm_dev_array[PWM_TMRA_12_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TMRA_12_CH4
        g_pwm_dev_array[PWM_TMRA_12_INDEX].channel |= 1 << 3;
    #endif
#endif
#endif
}

static void enable_timera_unitclk(void)
{
#ifdef BSP_USING_PWM_TMRA_1
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_1, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_2
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_2, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_3
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_3, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_4
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_4, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_5
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_5, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_6
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_6, ENABLE);
#endif
#if defined(HC32F4A0)
#ifdef BSP_USING_PWM_TMRA_7
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_7, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_8
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_8, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_9
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_9, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_10
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_10, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_11
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_11, ENABLE);
#endif
#ifdef BSP_USING_PWM_TMRA_12
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_12, ENABLE);
#endif
#endif
}

extern rt_err_t rt_hw_board_pwm_tmra_init(CM_TMRA_TypeDef *TMRAx);
static int rt_hw_pwm_tmra_init(void)
{
    int i = 0;
    rt_err_t result = RT_EOK;

    pwm_get_channel();
    enable_timera_unitclk();
    for (i = 0; i < sizeof(g_pwm_dev_array) / sizeof(g_pwm_dev_array[0]); i++)
    {
        /* pwm init */
        pwm_tmra_init(&g_pwm_dev_array[i]);
        /* gpio init */
        rt_hw_board_pwm_tmra_init(g_pwm_dev_array[i].instance);
        /* register UART device */
        result = rt_device_pwm_register(&g_pwm_dev_array[i].pwm_device, g_pwm_dev_array[i].name, &drv_ops, &g_pwm_dev_array[i].instance);
        RT_ASSERT(result == RT_EOK);
    }
    return result;
}
INIT_DEVICE_EXPORT(rt_hw_pwm_tmra_init);
#endif
