/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2022-06-27     lianghongquan     first version
<<<<<<< HEAD
 * 2023-02-22     CDT               add hc32f4a0,hc32f4a2 support
=======
 * 2023-02-22     CDT               add hc32f4a0 support
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
 */

#include <board.h>

#include "drv_config.h"
#include <drivers/rt_drv_pwm.h>
#include <pwm_tmr_config.h>

#if defined(BSP_USING_PWM)

//#define DRV_DEBUG
#define LOG_TAG             "drv_pwm"
#include <drv_log.h>

#if defined(BSP_USING_PWM_TIMA)

#if defined(HC32F460)
<<<<<<< HEAD
#define TMRA_CHANNEL_NUM_MAX     8U
#elif defined(HC32F4A0) || defined(HC32F4A2)
#define TMRA_CHANNEL_NUM_MAX     4U
=======
    #define TMRA_CHANNEL_NUM_MAX     8U
#elif defined(HC32F4A0)
    #define TMRA_CHANNEL_NUM_MAX     4U
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
#endif

enum
{
#ifdef BSP_USING_PWM_TIMA_1
    PWM_TIMA_1_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_2
    PWM_TIMA_2_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_3
    PWM_TIMA_3_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_4
    PWM_TIMA_4_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_5
    PWM_TIMA_5_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_6
    PWM_TIMA_6_INDEX,
#endif
<<<<<<< HEAD
#if defined(HC32F4A0) || defined(HC32F4A2)
=======
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
#ifdef BSP_USING_PWM_TIMA_7
    PWM_TIMA_7_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_8
    PWM_TIMA_8_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_9
    PWM_TIMA_9_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_10
    PWM_TIMA_10_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_11
    PWM_TIMA_11_INDEX,
#endif
#ifdef BSP_USING_PWM_TIMA_12
    PWM_TIMA_12_INDEX,
#endif
<<<<<<< HEAD
#endif
=======
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    PWM_TIMA_UNIT_NUM,
};

struct hc32_pwm_tmra
{
    struct rt_device_pwm pwm_device;
    char *name;
    CM_TMRA_TypeDef *instance;
    rt_uint32_t channel;
    uint32_t CompareValue[TMRA_CHANNEL_NUM_MAX];
<<<<<<< HEAD
=======
    rt_bool_t complementary[TMRA_CHANNEL_NUM_MAX];
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    stc_tmra_init_t stcTmraInit;
    stc_tmra_pwm_init_t stcPwmInit;
};

static struct hc32_pwm_tmra g_pwm_tmra_array[] =
{
#ifdef BSP_USING_PWM_TIMA_1
    PWM_TIMA_1_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_2
    PWM_TIMA_2_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_3
    PWM_TIMA_3_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_4
    PWM_TIMA_4_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_5
    PWM_TIMA_5_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_6
    PWM_TIMA_6_CONFIG,
#endif
<<<<<<< HEAD
#if defined(HC32F4A0) || defined(HC32F4A2)
=======
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
#ifdef BSP_USING_PWM_TIMA_7
    PWM_TIMA_7_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_8
    PWM_TIMA_8_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_9
    PWM_TIMA_9_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_10
    PWM_TIMA_10_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_11
    PWM_TIMA_11_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIMA_12
    PWM_TIMA_12_CONFIG,
#endif
<<<<<<< HEAD
#endif
=======
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
};

static rt_uint32_t tmra_get_clk_notdiv(CM_TMRA_TypeDef *TMRAx)
{
    rt_uint32_t u32clkFreq;
<<<<<<< HEAD
    switch ((rt_uint32_t)TMRAx)
    {
    case (rt_uint32_t)CM_TMRA_1:
    case (rt_uint32_t)CM_TMRA_2:
    case (rt_uint32_t)CM_TMRA_3:
    case (rt_uint32_t)CM_TMRA_4:
        u32clkFreq = CLK_GetBusClockFreq(CLK_BUS_PCLK0);
        break;
#if defined(HC32F4A0) || defined(HC32F4A2)
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
=======
    rt_uint32_t u32BusName;

#if defined(HC32F4A0)
    if ((rt_uint32_t)TMRAx >= (rt_uint32_t)CM_TMRA_1)
    {
        u32BusName = CLK_BUS_PCLK0;
    }
    else
    {
        u32BusName = CLK_BUS_PCLK1;
    }
#elif defined(HC32F460)
    u32BusName = CLK_BUS_PCLK1;
#endif
    u32clkFreq = CLK_GetBusClockFreq(u32BusName);

>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    return (u32clkFreq ? u32clkFreq : HCLK_VALUE);
}

static rt_uint32_t tmra_get_clk_bydiv(CM_TMRA_TypeDef *TMRAx)
{
    rt_uint32_t u32clkFreq;
    uint16_t u16Div;

    u32clkFreq = tmra_get_clk_notdiv(TMRAx);
<<<<<<< HEAD
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
=======
    u16Div = (READ_REG16(TMRAx->BCSTRL) & TMRA_BCSTRL_CKDIV);
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
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    }
    return u32clkFreq;
}

<<<<<<< HEAD
static void tmra_pwmp_enable(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
    rt_uint8_t i;
    for (i=0; i<PWM_TIMA_UNIT_NUM; i++)
    {
        if (g_pwm_tmra_array[i].instance == TMRAx)
        {
            stc_tmra_pwm_init_t *stcPwmInit = &g_pwm_tmra_array[i].stcPwmInit;
            stcPwmInit->u32CompareValue = TMRA_GetCompareValue(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX));
            TMRA_PWM_Init(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX), stcPwmInit);
=======
static void tmra_duyt100or0_output(CM_TMRA_TypeDef *TMRAx, uint32_t channel, uint32_t CompareValue)
{
    rt_uint8_t i;
    rt_uint8_t ch = channel % TMRA_CHANNEL_NUM_MAX;
    for (i = 0; i < PWM_TIMA_UNIT_NUM; i++)
    {
        if (g_pwm_tmra_array[i].instance == TMRAx)
        {
            /* Realize 100% duty output */
            if (TMRA_GetPeriodValue(TMRAx) == CompareValue)
            {
                if (RT_TRUE == g_pwm_tmra_array[i].complementary[ch])
                {
                    TMRA_PWM_SetForcePolarity(TMRAx, ch, TMRA_PWM_FORCE_LOW);
                }
                else
                {
                    TMRA_PWM_SetForcePolarity(TMRAx, ch, TMRA_PWM_FORCE_HIGH);
                }
            }
            else if (0 == CompareValue)        /* Realize 0% duty output */
            {
                if (RT_TRUE == g_pwm_tmra_array[i].complementary[ch])
                {
                    TMRA_PWM_SetForcePolarity(TMRAx, ch, TMRA_PWM_FORCE_HIGH);
                }
                else
                {
                    TMRA_PWM_SetForcePolarity(TMRAx, ch, TMRA_PWM_FORCE_LOW);
                }
            }
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
            break;
        }
    }
}

<<<<<<< HEAD
static void tmra_pwmn_enable(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
=======
static void tmra_pwmp_enable(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
    rt_uint8_t i;
    stc_tmra_pwm_init_t *stcPwmInit;
    for (i = 0; i < PWM_TIMA_UNIT_NUM; i++)
    {
        if (g_pwm_tmra_array[i].instance == TMRAx)
        {
            stcPwmInit = &g_pwm_tmra_array[i].stcPwmInit;
            stcPwmInit->u32CompareValue = TMRA_GetCompareValue(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX));
            TMRA_PWM_Init(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX), stcPwmInit);
            g_pwm_tmra_array[i].complementary[configuration->channel % TMRA_CHANNEL_NUM_MAX] = RT_FALSE;
            break;
        }
    }
    if (0 == stcPwmInit->u32CompareValue || stcPwmInit->u32CompareValue == TMRA_GetPeriodValue(TMRAx))
    {
        tmra_duyt100or0_output(TMRAx, configuration->channel, stcPwmInit->u32CompareValue);
    }
    else
    {
        TMRA_PWM_SetForcePolarity(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX), TMRA_PWM_FORCE_INVD);
    }
}

static void tmra_pwmn_enable(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
    rt_uint8_t i;
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    stc_tmra_pwm_init_t stcPwmNInit;
    stcPwmNInit.u32CompareValue = TMRA_GetCompareValue(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX));
    stcPwmNInit.u16StartPolarity = TMRA_PWM_HIGH;
    stcPwmNInit.u16StopPolarity = TMRA_PWM_HIGH;
    stcPwmNInit.u16CompareMatchPolarity = TMRA_PWM_LOW;
    stcPwmNInit.u16PeriodMatchPolarity = TMRA_PWM_HIGH;
    TMRA_PWM_Init(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX), &stcPwmNInit);
<<<<<<< HEAD
=======
    /* Flags whether the channel complements output */
    for (i = 0; i < PWM_TIMA_UNIT_NUM; i++)
    {
        if (g_pwm_tmra_array[i].instance == TMRAx)
        {
            g_pwm_tmra_array[i].complementary[configuration->channel % TMRA_CHANNEL_NUM_MAX] = RT_TRUE;
        }
    }
    if (0 == stcPwmNInit.u32CompareValue || stcPwmNInit.u32CompareValue == TMRA_GetPeriodValue(TMRAx))
    {
        tmra_duyt100or0_output(TMRAx, configuration->channel, stcPwmNInit.u32CompareValue);
    }
    else
    {
        TMRA_PWM_SetForcePolarity(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX), TMRA_PWM_FORCE_INVD);
    }
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
}

static rt_err_t tmra_pwm_enable(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration, rt_bool_t enable)
{
    /* channel0 will set complementary but belong to pwm_ch1 */
    if (configuration->complementary && configuration->channel != 0)
    {
        tmra_pwmn_enable(TMRAx, configuration);
<<<<<<< HEAD
    } else {
=======
    }
    else
    {
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        tmra_pwmp_enable(TMRAx, configuration);
    }

    if (enable)
    {
        TMRA_PWM_OutputCmd(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX), ENABLE);
<<<<<<< HEAD
    } else {
=======
    }
    else
    {
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        TMRA_PWM_OutputCmd(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX), DISABLE);
    }

    return RT_EOK;
}

static rt_err_t tmra_pwm_get(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    u32clkFreq = tmra_get_clk_bydiv(TMRAx);
<<<<<<< HEAD
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
=======
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    configuration->period = u64clk_ns * TMRA_GetPeriodValue(TMRAx);
    configuration->pulse = u64clk_ns * TMRA_GetCompareValue(TMRAx, (configuration->channel % TMRA_CHANNEL_NUM_MAX));
    return RT_EOK;
}

static rt_uint64_t tmra_auto_set_div(CM_TMRA_TypeDef *TMRAx, uint32_t period)
{
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    rt_uint64_t u64val;
    u32clkFreq = tmra_get_clk_bydiv(TMRAx);
<<<<<<< HEAD
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
    u64val = (rt_uint64_t)period / u64clk_ns;
    /* clk not match, need change div */
    if ((period <= u64clk_ns) || (u64val > 0xFFFF))
    {
        uint32_t div_bit;
        u32clkFreq = tmra_get_clk_notdiv(TMRAx);
        u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
        u64val = (rt_uint64_t)period / u64clk_ns;
        for (div_bit = 0; div_bit <= 10; div_bit++)
        {
            if (u64val < 0xFFFF)
=======
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
    u64val = (rt_uint64_t)period / u64clk_ns;
    /* clk not match, need change div */
    if ((period <= u64clk_ns) || (u64val > 0xFFFFU))
    {
        uint32_t div_bit;
        u32clkFreq = tmra_get_clk_notdiv(TMRAx);
        u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
        u64val = (rt_uint64_t)period / u64clk_ns;
        for (div_bit = 0; div_bit <= 10; div_bit++)
        {
            if (u64val < 0xFFFFU)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
            {
                break;
            }
            u64val /= 2;
        }
        if (div_bit > 10)
        {
<<<<<<< HEAD
            return RT_ERROR;
        }
        TMRA_SetClockDiv(TMRAx, div_bit << TMRA_BCSTR_CKDIV_POS);
        u32clkFreq = tmra_get_clk_bydiv(TMRAx);
        u64clk_ns = (rt_uint64_t)(1000000000ul / u32clkFreq);
=======
            return 0;
        }
        TMRA_SetClockDiv(TMRAx, div_bit << TMRA_BCSTRL_CKDIV_POS);
        u32clkFreq = tmra_get_clk_bydiv(TMRAx);
        u64clk_ns = (rt_uint64_t)(1000000000UL / u32clkFreq);
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    }
    return u64clk_ns;
}

static rt_err_t tmra_pwm_set_period(CM_TMRA_TypeDef *TMRAx, uint32_t channel, uint32_t period)
{
<<<<<<< HEAD
    rt_uint8_t i,j;
    rt_uint64_t u64clk_ns = tmra_auto_set_div(TMRAx, period);
    TMRA_SetPeriodValue(TMRAx, period / u64clk_ns);
    /* setting PeriodValue maybe change the div,so we need to recalculate the CompareValue */
    for (i=0; i<PWM_TIMA_UNIT_NUM; i++)
    {
        if (g_pwm_tmra_array[i].instance == TMRAx)
        {
            for (j=0; j<TMRA_CHANNEL_NUM_MAX; j++)
=======
    rt_uint8_t i, j;
    rt_uint64_t u64clk_ns = tmra_auto_set_div(TMRAx, period);
    if (!u64clk_ns)
    {
        return -RT_ERROR;
    }
    TMRA_SetPeriodValue(TMRAx, period / u64clk_ns);
    /* setting PeriodValue maybe change the div,so we need to recalculate the CompareValue */
    for (i = 0; i < PWM_TIMA_UNIT_NUM; i++)
    {
        if (g_pwm_tmra_array[i].instance == TMRAx)
        {
            for (j = 0; j < TMRA_CHANNEL_NUM_MAX; j++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
            {
                TMRA_SetCompareValue(TMRAx, j, g_pwm_tmra_array[i].CompareValue[j] / u64clk_ns);
            }
            g_pwm_tmra_array[i].stcTmraInit.u32PeriodValue = period;
            break;
        }
    }
    return RT_EOK;
}

static rt_err_t tmra_pwm_set_pulse(CM_TMRA_TypeDef *TMRAx, uint32_t channel, uint32_t pulse)
{
    rt_uint8_t i;
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
<<<<<<< HEAD
    rt_uint8_t ch = channel % TMRA_CHANNEL_NUM_MAX;
    u32clkFreq = tmra_get_clk_bydiv(TMRAx);
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
    TMRA_SetCompareValue(TMRAx, ch, pulse / u64clk_ns);
    for (i=0; i<PWM_TIMA_UNIT_NUM; i++)
=======
    rt_uint32_t CompareValue;
    rt_uint8_t ch = channel % TMRA_CHANNEL_NUM_MAX;
    u32clkFreq = tmra_get_clk_bydiv(TMRAx);
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
    TMRA_SetCompareValue(TMRAx, ch, pulse / u64clk_ns);
    CompareValue = TMRA_GetCompareValue(TMRAx, ch);
    for (i = 0; i < PWM_TIMA_UNIT_NUM; i++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    {
        if (g_pwm_tmra_array[i].instance == TMRAx)
        {
            g_pwm_tmra_array[i].CompareValue[ch] = pulse;
<<<<<<< HEAD
            /* Realize 100% duty output */
            if (g_pwm_tmra_array[i].stcTmraInit.u32PeriodValue == pulse) {
                TMRA_PWM_SetForcePolarity(TMRAx, ch, TMRA_PWM_FORCE_HIGH);
            } else {
                TMRA_PWM_SetForcePolarity(TMRAx, ch, TMRA_PWM_FORCE_INVD);
            }
            break;
        }
    }
=======
            break;
        }
    }
    if (0 == CompareValue || CompareValue == TMRA_GetPeriodValue(TMRAx))
    {
        tmra_duyt100or0_output(TMRAx, channel, CompareValue);
    }
    else
    {
        TMRA_PWM_SetForcePolarity(TMRAx, ch, TMRA_PWM_FORCE_INVD);
    }
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    return RT_EOK;
}

static rt_err_t tmra_pwm_set(CM_TMRA_TypeDef *TMRAx, struct rt_pwm_configuration *configuration)
{
    tmra_pwm_set_period(TMRAx, configuration->channel, configuration->period);
    tmra_pwm_set_pulse(TMRAx, configuration->channel, configuration->pulse);

    return RT_EOK;
}

static rt_err_t pwm_tmra_init(struct hc32_pwm_tmra *device)
{
    CM_TMRA_TypeDef *TMRAx;
    uint32_t i;
    RT_ASSERT(device != RT_NULL);

    TMRAx = device->instance;
    TMRA_Init(TMRAx, &device->stcTmraInit);
<<<<<<< HEAD
    for (i=0; i<TMRA_CHANNEL_NUM_MAX; i++)
=======
    for (i = 0; i < TMRA_CHANNEL_NUM_MAX; i++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    {
        if ((device->channel >> i) & 0x01)
        {
            TMRA_PWM_Init(TMRAx, i, &device->stcPwmInit);
        }
    }
    TMRA_Start(TMRAx);

    return RT_EOK;
}

static void pwm_tmra_get_channel(void)
{
#ifdef BSP_USING_PWM_TIMA_1
<<<<<<< HEAD
    #ifdef BSP_USING_PWM_TIMA_1_CH1
        g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH2
        g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH3
        g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH4
        g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TIMA_1_CH5
        g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH6
        g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH7
        g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH8
        g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_2
    #ifdef BSP_USING_PWM_TIMA_2_CH1
        g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH2
        g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH3
        g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH4
        g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TIMA_2_CH5
        g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH6
        g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH7
        g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH8
        g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_3
    #ifdef BSP_USING_PWM_TIMA_3_CH1
        g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH2
        g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH3
        g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH4
        g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TIMA_3_CH5
        g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH6
        g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH7
        g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH8
        g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_4
    #ifdef BSP_USING_PWM_TIMA_4_CH1
        g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH2
        g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH3
        g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH4
        g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TIMA_4_CH5
        g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH6
        g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH7
        g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH8
        g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_5
    #ifdef BSP_USING_PWM_TIMA_5_CH1
        g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH2
        g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH3
        g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH4
        g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TIMA_5_CH5
        g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH6
        g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH7
        g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH8
        g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_6
    #ifdef BSP_USING_PWM_TIMA_6_CH1
        g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH2
        g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH3
        g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH4
        g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= 1 << 3;
    #endif
#if defined(HC32F460)
    #ifdef BSP_USING_PWM_TIMA_6_CH5
        g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH6
        g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= 1 << 5;
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH7
        g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= 1 << 6;
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH8
        g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= 1 << 7;
    #endif
#endif
#endif
#if defined(HC32F4A0) || defined(HC32F4A2)
#ifdef BSP_USING_PWM_TIMA_7
    #ifdef BSP_USING_PWM_TIMA_7_CH1
        g_pwm_tmra_array[PWM_TIMA_7_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_7_CH2
        g_pwm_tmra_array[PWM_TIMA_7_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_7_CH3
        g_pwm_tmra_array[PWM_TIMA_7_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_7_CH4
        g_pwm_tmra_array[PWM_TIMA_7_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TIMA_8
    #ifdef BSP_USING_PWM_TIMA_8_CH1
        g_pwm_tmra_array[PWM_TIMA_8_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_8_CH2
        g_pwm_tmra_array[PWM_TIMA_8_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_8_CH3
        g_pwm_tmra_array[PWM_TIMA_8_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_8_CH4
        g_pwm_tmra_array[PWM_TIMA_8_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TIMA_9
    #ifdef BSP_USING_PWM_TIMA_9_CH1
        g_pwm_tmra_array[PWM_TIMA_9_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_9_CH2
        g_pwm_tmra_array[PWM_TIMA_9_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_9_CH3
        g_pwm_tmra_array[PWM_TIMA_9_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_9_CH4
        g_pwm_tmra_array[PWM_TIMA_9_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TIMA_10
    #ifdef BSP_USING_PWM_TIMA_10_CH1
        g_pwm_tmra_array[PWM_TIMA_10_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_10_CH2
        g_pwm_tmra_array[PWM_TIMA_10_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_10_CH3
        g_pwm_tmra_array[PWM_TIMA_10_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_10_CH4
        g_pwm_tmra_array[PWM_TIMA_10_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TIMA_11
    #ifdef BSP_USING_PWM_TIMA_11_CH1
        g_pwm_tmra_array[PWM_TIMA_11_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_11_CH2
        g_pwm_tmra_array[PWM_TIMA_11_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_11_CH3
        g_pwm_tmra_array[PWM_TIMA_11_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_11_CH4
        g_pwm_tmra_array[PWM_TIMA_11_INDEX].channel |= 1 << 3;
    #endif
#endif
#ifdef BSP_USING_PWM_TIMA_12
    #ifdef BSP_USING_PWM_TIMA_12_CH1
        g_pwm_tmra_array[PWM_TIMA_12_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIMA_12_CH2
        g_pwm_tmra_array[PWM_TIMA_12_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIMA_12_CH3
        g_pwm_tmra_array[PWM_TIMA_12_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIMA_12_CH4
        g_pwm_tmra_array[PWM_TIMA_12_INDEX].channel |= 1 << 3;
    #endif
=======
#ifdef BSP_USING_PWM_TIMA_1_CH1
    g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_1_CH2
    g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_1_CH3
    g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_1_CH4
    g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIMA_1_CH5
    g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIMA_1_CH6
    g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= (1 << 5);
#endif
#ifdef BSP_USING_PWM_TIMA_1_CH7
    g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= (1 << 6);
#endif
#ifdef BSP_USING_PWM_TIMA_1_CH8
    g_pwm_tmra_array[PWM_TIMA_1_INDEX].channel |= (1 << 7);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_2
#ifdef BSP_USING_PWM_TIMA_2_CH1
    g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_2_CH2
    g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_2_CH3
    g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_2_CH4
    g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIMA_2_CH5
    g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIMA_2_CH6
    g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= (1 << 5);
#endif
#ifdef BSP_USING_PWM_TIMA_2_CH7
    g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= (1 << 6);
#endif
#ifdef BSP_USING_PWM_TIMA_2_CH8
    g_pwm_tmra_array[PWM_TIMA_2_INDEX].channel |= (1 << 7);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_3
#ifdef BSP_USING_PWM_TIMA_3_CH1
    g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_3_CH2
    g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_3_CH3
    g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_3_CH4
    g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIMA_3_CH5
    g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIMA_3_CH6
    g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= (1 << 5);
#endif
#ifdef BSP_USING_PWM_TIMA_3_CH7
    g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= (1 << 6);
#endif
#ifdef BSP_USING_PWM_TIMA_3_CH8
    g_pwm_tmra_array[PWM_TIMA_3_INDEX].channel |= (1 << 7);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_4
#ifdef BSP_USING_PWM_TIMA_4_CH1
    g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_4_CH2
    g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_4_CH3
    g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_4_CH4
    g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIMA_4_CH5
    g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIMA_4_CH6
    g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= (1 << 5);
#endif
#ifdef BSP_USING_PWM_TIMA_4_CH7
    g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= (1 << 6);
#endif
#ifdef BSP_USING_PWM_TIMA_4_CH8
    g_pwm_tmra_array[PWM_TIMA_4_INDEX].channel |= (1 << 7);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_5
#ifdef BSP_USING_PWM_TIMA_5_CH1
    g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_5_CH2
    g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_5_CH3
    g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_5_CH4
    g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIMA_5_CH5
    g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIMA_5_CH6
    g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= (1 << 5);
#endif
#ifdef BSP_USING_PWM_TIMA_5_CH7
    g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= (1 << 6);
#endif
#ifdef BSP_USING_PWM_TIMA_5_CH8
    g_pwm_tmra_array[PWM_TIMA_5_INDEX].channel |= (1 << 7);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_6
#ifdef BSP_USING_PWM_TIMA_6_CH1
    g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_6_CH2
    g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_6_CH3
    g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_6_CH4
    g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIMA_6_CH5
    g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIMA_6_CH6
    g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= (1 << 5);
#endif
#ifdef BSP_USING_PWM_TIMA_6_CH7
    g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= (1 << 6);
#endif
#ifdef BSP_USING_PWM_TIMA_6_CH8
    g_pwm_tmra_array[PWM_TIMA_6_INDEX].channel |= (1 << 7);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_7
#ifdef BSP_USING_PWM_TIMA_7_CH1
    g_pwm_tmra_array[PWM_TIMA_7_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_7_CH2
    g_pwm_tmra_array[PWM_TIMA_7_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_7_CH3
    g_pwm_tmra_array[PWM_TIMA_7_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_7_CH4
    g_pwm_tmra_array[PWM_TIMA_7_INDEX].channel |= (1 << 3);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_8
#ifdef BSP_USING_PWM_TIMA_8_CH1
    g_pwm_tmra_array[PWM_TIMA_8_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_8_CH2
    g_pwm_tmra_array[PWM_TIMA_8_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_8_CH3
    g_pwm_tmra_array[PWM_TIMA_8_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_8_CH4
    g_pwm_tmra_array[PWM_TIMA_8_INDEX].channel |= (1 << 3);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_9
#ifdef BSP_USING_PWM_TIMA_9_CH1
    g_pwm_tmra_array[PWM_TIMA_9_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_9_CH2
    g_pwm_tmra_array[PWM_TIMA_9_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_9_CH3
    g_pwm_tmra_array[PWM_TIMA_9_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_9_CH4
    g_pwm_tmra_array[PWM_TIMA_9_INDEX].channel |= (1 << 3);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_10
#ifdef BSP_USING_PWM_TIMA_10_CH1
    g_pwm_tmra_array[PWM_TIMA_10_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_10_CH2
    g_pwm_tmra_array[PWM_TIMA_10_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_10_CH3
    g_pwm_tmra_array[PWM_TIMA_10_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_10_CH4
    g_pwm_tmra_array[PWM_TIMA_10_INDEX].channel |= (1 << 3);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_11
#ifdef BSP_USING_PWM_TIMA_11_CH1
    g_pwm_tmra_array[PWM_TIMA_11_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_11_CH2
    g_pwm_tmra_array[PWM_TIMA_11_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_11_CH3
    g_pwm_tmra_array[PWM_TIMA_11_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_11_CH4
    g_pwm_tmra_array[PWM_TIMA_11_INDEX].channel |= (1 << 3);
#endif
#endif
#ifdef BSP_USING_PWM_TIMA_12
#ifdef BSP_USING_PWM_TIMA_12_CH1
    g_pwm_tmra_array[PWM_TIMA_12_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIMA_12_CH2
    g_pwm_tmra_array[PWM_TIMA_12_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIMA_12_CH3
    g_pwm_tmra_array[PWM_TIMA_12_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIMA_12_CH4
    g_pwm_tmra_array[PWM_TIMA_12_INDEX].channel |= (1 << 3);
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
#endif
#endif
}

static void enable_tmra_unit_clk(void)
{
#ifdef BSP_USING_PWM_TIMA_1
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_1, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_2
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_2, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_3
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_3, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_4
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_4, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_5
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_5, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_6
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_6, ENABLE);
#endif
<<<<<<< HEAD
#if defined(HC32F4A0) || defined(HC32F4A2)
=======
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
#ifdef BSP_USING_PWM_TIMA_7
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_7, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_8
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_8, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_9
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_9, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_10
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_10, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_11
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_11, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIMA_12
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_12, ENABLE);
#endif
<<<<<<< HEAD
#endif
=======
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
}

static rt_err_t tmra_pwm_control(struct rt_device_pwm *device, int cmd, void *arg)
{
    struct rt_pwm_configuration *configuration = (struct rt_pwm_configuration *)arg;
    struct hc32_pwm_tmra *pwm;
    pwm = rt_container_of(device, struct hc32_pwm_tmra, pwm_device);
    CM_TMRA_TypeDef *TMRAx = pwm->instance;

    switch (cmd)
    {
    case PWM_CMD_ENABLE:
        return tmra_pwm_enable(TMRAx, configuration, RT_TRUE);
    case PWM_CMD_DISABLE:
        return tmra_pwm_enable(TMRAx, configuration, RT_FALSE);
    case PWM_CMD_SET:
        return tmra_pwm_set(TMRAx, configuration);
    case PWM_CMD_GET:
        return tmra_pwm_get(TMRAx, configuration);
    case PWM_CMD_SET_PERIOD:
        return tmra_pwm_set_period(TMRAx, configuration->channel, configuration->period);
    case PWM_CMD_SET_PULSE:
        return tmra_pwm_set_pulse(TMRAx, configuration->channel, configuration->pulse);
    default:
        return RT_EINVAL;
    }
}

static struct rt_pwm_ops tmra_ops =
{
    tmra_pwm_control
};

#endif  /* BSP_USING_PWM_TIMA */

#if defined(BSP_USING_PWM_TIM4)

#define TMR4_CHANNEL_NUM_MAX     6U

enum
{
#ifdef BSP_USING_PWM_TIM4_1
    PWM_TIM4_1_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM4_2
    PWM_TIM4_2_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM4_3
    PWM_TIM4_3_INDEX,
#endif
    PWM_TIM4_UNIT_NUM,
};

struct hc32_pwm_tmr4
{
    struct rt_device_pwm pwm_device;
    char *name;
    CM_TMR4_TypeDef *instance;
    rt_uint32_t channel;
    rt_uint32_t PeriodValue;
    rt_uint32_t CompareValue[TMR4_CHANNEL_NUM_MAX];
    stc_tmr4_init_t stcTmr4Init;
    stc_tmr4_oc_init_t stcTmr4OcInit;
    stc_tmr4_pwm_init_t stcTmr4PwmInit;
};

static struct hc32_pwm_tmr4 g_pwm_tmr4_array[] =
{
#ifdef BSP_USING_PWM_TIM4_1
    PWM_TIM4_1_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM4_2
    PWM_TIM4_2_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM4_3
    PWM_TIM4_3_CONFIG,
#endif
};

static rt_uint32_t tmr4_get_clk_notdiv(CM_TMR4_TypeDef *TMR4x)
{
    rt_uint32_t u32clkFreq;
    switch ((rt_uint32_t)TMR4x)
    {
    case (rt_uint32_t)CM_TMR4_1:
    case (rt_uint32_t)CM_TMR4_2:
    case (rt_uint32_t)CM_TMR4_3:
        u32clkFreq = CLK_GetBusClockFreq(CLK_BUS_PCLK0);
        break;
    default:
        break;
    }
    return (u32clkFreq ? u32clkFreq : HCLK_VALUE);
}

static rt_uint32_t tmr4_get_clk_bydiv(CM_TMR4_TypeDef *TMR4x)
{
    rt_uint32_t u32clkFreq;
    uint16_t u16Div;

    u32clkFreq = tmr4_get_clk_notdiv(TMR4x);
    u16Div = (READ_REG16(TMR4x->CCSR) & TMR4_CCSR_CKDIV);
    switch (u16Div)
    {
<<<<<<< HEAD
        case (TMR4_CLK_DIV1):
            break;
        case (TMR4_CLK_DIV2):
            u32clkFreq /= 2;
            break;
        case (TMR4_CLK_DIV4):
            u32clkFreq /= 4;
            break;
        case (TMR4_CLK_DIV8):
            u32clkFreq /= 8;
            break;
        case (TMR4_CLK_DIV16):
            u32clkFreq /= 16;
            break;
        case (TMR4_CLK_DIV32):
            u32clkFreq /= 32;
            break;
        case (TMR4_CLK_DIV64):
            u32clkFreq /= 64;
            break;
        case (TMR4_CLK_DIV128):
            u32clkFreq /= 128;
            break;
        case (TMR4_CLK_DIV256):
            u32clkFreq /= 256;
            break;
        case (TMR4_CLK_DIV512):
            u32clkFreq /= 512;
            break;
        case (TMR4_CLK_DIV1024):
            u32clkFreq /= 1024;
            break;
        default:
            break;
=======
    case (TMR4_CLK_DIV1):
        break;
    case (TMR4_CLK_DIV2):
        u32clkFreq /= 2;
        break;
    case (TMR4_CLK_DIV4):
        u32clkFreq /= 4;
        break;
    case (TMR4_CLK_DIV8):
        u32clkFreq /= 8;
        break;
    case (TMR4_CLK_DIV16):
        u32clkFreq /= 16;
        break;
    case (TMR4_CLK_DIV32):
        u32clkFreq /= 32;
        break;
    case (TMR4_CLK_DIV64):
        u32clkFreq /= 64;
        break;
    case (TMR4_CLK_DIV128):
        u32clkFreq /= 128;
        break;
    case (TMR4_CLK_DIV256):
        u32clkFreq /= 256;
        break;
    case (TMR4_CLK_DIV512):
        u32clkFreq /= 512;
        break;
    case (TMR4_CLK_DIV1024):
        u32clkFreq /= 1024;
        break;
    default:
        break;
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    }
    return u32clkFreq;
}

static void tmr4_pwmp_enable(CM_TMR4_TypeDef *TMR4x, struct rt_pwm_configuration *configuration)
{
    TMR4_PWM_SetPolarity(TMR4x, (configuration->channel % TMR4_CHANNEL_NUM_MAX) / 2, TMR4_PWM_OXH_HOLD_OXL_HOLD);
}

static void tmr4_pwmn_enable(CM_TMR4_TypeDef *TMR4x, struct rt_pwm_configuration *configuration)
{
<<<<<<< HEAD
    if (configuration->channel % 2) {
        TMR4_PWM_SetPolarity(TMR4x, (configuration->channel % TMR4_CHANNEL_NUM_MAX) / 2, TMR4_PWM_OXH_HOLD_OXL_INVT);
    } else {
=======
    if (configuration->channel % 2)
    {
        TMR4_PWM_SetPolarity(TMR4x, (configuration->channel % TMR4_CHANNEL_NUM_MAX) / 2, TMR4_PWM_OXH_HOLD_OXL_INVT);
    }
    else
    {
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        TMR4_PWM_SetPolarity(TMR4x, (configuration->channel % TMR4_CHANNEL_NUM_MAX) / 2, TMR4_PWM_OXH_INVT_OXL_HOLD);
    }
}

static rt_err_t tmr4_pwm_enable(CM_TMR4_TypeDef *TMR4x, struct rt_pwm_configuration *configuration, rt_bool_t enable)
{
    /* channel0 will set complementary but belong to pwm_ch1 */
    if (configuration->complementary && configuration->channel != 0)
    {
        tmr4_pwmn_enable(TMR4x, configuration);
<<<<<<< HEAD
    } else {
=======
    }
    else
    {
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        tmr4_pwmp_enable(TMR4x, configuration);
    }

    if (enable)
    {
        TMR4_OC_Cmd(TMR4x, (configuration->channel % TMR4_CHANNEL_NUM_MAX), ENABLE);
<<<<<<< HEAD
    } else {
=======
    }
    else
    {
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        TMR4_OC_Cmd(TMR4x, (configuration->channel % TMR4_CHANNEL_NUM_MAX), DISABLE);
    }

    return RT_EOK;
}

static rt_err_t tmr4_pwm_get(CM_TMR4_TypeDef *TMR4x, struct rt_pwm_configuration *configuration)
{
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    u32clkFreq = tmr4_get_clk_bydiv(TMR4x);
<<<<<<< HEAD
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
=======
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    configuration->period = u64clk_ns * TMR4_GetPeriodValue(TMR4x);
    configuration->pulse = u64clk_ns * TMR4_OC_GetCompareValue(TMR4x, configuration->channel);
    return RT_EOK;
}

static rt_uint64_t tmr4_auto_set_div(CM_TMR4_TypeDef *TMR4x, uint32_t period)
{
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    rt_uint64_t u64val;
    u32clkFreq = tmr4_get_clk_bydiv(TMR4x);
<<<<<<< HEAD
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
    u64val = (rt_uint64_t)period / u64clk_ns;
    /* clk not match, need change div */
    if ((period <= u64clk_ns) || (u64val > 0xFFFF))
    {
        uint32_t div_bit;
        u32clkFreq = tmr4_get_clk_notdiv(TMR4x);
        u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
        u64val = (rt_uint64_t)period / u64clk_ns;
        for (div_bit = 0; div_bit <= 10; div_bit++)
        {
            if (u64val < 0xFFFF)
=======
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
    u64val = (rt_uint64_t)period / u64clk_ns;
    /* clk not match, need change div */
    if ((period <= u64clk_ns) || (u64val > 0xFFFFU))
    {
        uint32_t div_bit;
        u32clkFreq = tmr4_get_clk_notdiv(TMR4x);
        u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
        u64val = (rt_uint64_t)period / u64clk_ns;
        for (div_bit = 0; div_bit <= 10; div_bit++)
        {
            if (u64val < 0xFFFFU)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
            {
                break;
            }
            u64val /= 2;
        }
        if (div_bit > 10)
        {
<<<<<<< HEAD
            return RT_ERROR;
        }
        TMR4_SetClockDiv(TMR4x, div_bit << TMR4_CCSR_CKDIV_POS);
        u32clkFreq = tmr4_get_clk_bydiv(TMR4x);
        u64clk_ns = (rt_uint64_t)(1000000000ul / u32clkFreq);
=======
            return 0;
        }
        TMR4_SetClockDiv(TMR4x, div_bit << TMR4_CCSR_CKDIV_POS);
        u32clkFreq = tmr4_get_clk_bydiv(TMR4x);
        u64clk_ns = (rt_uint64_t)(1000000000UL / u32clkFreq);
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    }
    return u64clk_ns;
}

static rt_err_t tmr4_pwm_set_period(CM_TMR4_TypeDef *TMR4x, uint32_t channel, uint32_t period)
{
<<<<<<< HEAD
    rt_uint8_t i,j;
    rt_uint64_t u64clk_ns = tmr4_auto_set_div(TMR4x, period);
    TMR4_SetPeriodValue(TMR4x, period / u64clk_ns);
    /* setting PeriodValue maybe change the div,so we need to recalculate the CompareValue */
    for (i=0; i<PWM_TIM4_UNIT_NUM; i++)
    {
        if (g_pwm_tmr4_array[i].instance == TMR4x)
        {
            for (j=0; j<TMR4_CHANNEL_NUM_MAX; j++)
=======
    rt_uint8_t i, j;
    rt_uint64_t u64clk_ns = tmr4_auto_set_div(TMR4x, period);
    if (!u64clk_ns)
    {
        return -RT_ERROR;
    }
    TMR4_SetPeriodValue(TMR4x, period / u64clk_ns);
    /* setting PeriodValue maybe change the div,so we need to recalculate the CompareValue */
    for (i = 0; i < PWM_TIM4_UNIT_NUM; i++)
    {
        if (g_pwm_tmr4_array[i].instance == TMR4x)
        {
            for (j = 0; j < TMR4_CHANNEL_NUM_MAX; j++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
            {
                TMR4_OC_SetCompareValue(TMR4x, j, g_pwm_tmr4_array[i].CompareValue[j] / u64clk_ns);
            }
            g_pwm_tmr4_array[i].PeriodValue = period;
            break;
        }
    }
    return RT_EOK;
}

<<<<<<< HEAD
static void tmr4_pwm_set_cmpmode(CM_TMR4_TypeDef *TMR4x, uint32_t channel, uint32_t pulse)
=======
static void tmr4_pwm_set_cmpmode(CM_TMR4_TypeDef *TMR4x, uint32_t channel)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
{
    un_tmr4_oc_ocmrh_t unTmr4OcOcmrh;
    un_tmr4_oc_ocmrl_t unTmr4OcOcmrl;
    uint32_t ch = channel % TMR4_CHANNEL_NUM_MAX;
<<<<<<< HEAD
    if (pulse) {
        if(ch % 2) {
            /* TMR4 OC low channel: compare mode OCMR[31:0] 0x000 124F = b 0101 1010 0100 0000   0101 1010 0100 1111 */
            unTmr4OcOcmrl.OCMRx = 0x5A405A4F;
            TMR4_OC_SetLowChCompareMode(TMR4x, ch, unTmr4OcOcmrl);
        } else {
            /* TMR4 OC high channel: compare mode OCMR[15:0] = 0x5A4F = b 0101 1010 0100 1111 */
            unTmr4OcOcmrh.OCMRx = 0x5A4F;
            TMR4_OC_SetHighChCompareMode(TMR4x, ch, unTmr4OcOcmrh);
        }
    } else {
        /* Realize 0% duty output */
        if(ch % 2) {
            /* TMR4 OC low channel: compare mode OCMR[31:0] 0x000 124F = b 1010 1010 1000 0000   1010 1010 1000 1111 */
            unTmr4OcOcmrl.OCMRx = 0xAA80AA8F;
            TMR4_OC_SetLowChCompareMode(TMR4x, ch, unTmr4OcOcmrl);
        } else {
            /* TMR4 OC high channel: compare mode OCMR[15:0] = 0x5A4F = b 1010 1010 1000 1111 */
            unTmr4OcOcmrh.OCMRx = 0xAA8F;
            TMR4_OC_SetHighChCompareMode(TMR4x, ch, unTmr4OcOcmrh);
        }
=======
    if (ch % 2)
    {
        /* TMR4 OC low channel: compare mode OCMR[31:0] 0x2370 237F = b 0100 1010 0110 0000   0100 1010 0110 1111 */
        unTmr4OcOcmrl.OCMRx = 0x4A604A6FU;
        TMR4_OC_SetLowChCompareMode(TMR4x, ch, unTmr4OcOcmrl);
    }
    else
    {
        /* TMR4 OC high channel: compare mode OCMR[15:0] = 0x4A6F = b 0100 1010 0110 1111 */
        unTmr4OcOcmrh.OCMRx = 0x4A6F;
        TMR4_OC_SetHighChCompareMode(TMR4x, ch, unTmr4OcOcmrh);
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    }
}

static rt_err_t tmr4_pwm_set_pulse(CM_TMR4_TypeDef *TMR4x, uint32_t channel, uint32_t pulse)
{
    rt_uint8_t i;
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    uint32_t ch = channel % TMR4_CHANNEL_NUM_MAX;
    u32clkFreq = tmr4_get_clk_bydiv(TMR4x);
<<<<<<< HEAD
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
    TMR4_OC_SetCompareValue(TMR4x, ch, pulse / u64clk_ns);
    for (i=0; i<PWM_TIM4_UNIT_NUM; i++)
=======
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
    TMR4_OC_SetCompareValue(TMR4x, ch, pulse / u64clk_ns);
    for (i = 0; i < PWM_TIM4_UNIT_NUM; i++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    {
        if (g_pwm_tmr4_array[i].instance == TMR4x)
        {
            g_pwm_tmr4_array[i].CompareValue[ch] = pulse;
<<<<<<< HEAD
            tmr4_pwm_set_cmpmode(TMR4x, ch, pulse);
=======
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
            break;
        }
    }
    return RT_EOK;
}

static rt_err_t tmr4_pwm_set(CM_TMR4_TypeDef *TMR4x, struct rt_pwm_configuration *configuration)
{
    tmr4_pwm_set_period(TMR4x, configuration->channel, configuration->period);
    tmr4_pwm_set_pulse(TMR4x, configuration->channel, configuration->pulse);

    return RT_EOK;
}

static void enable_tmr4_unit_clk(void)
{
#ifdef BSP_USING_PWM_TIM4_1
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR4_1, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM4_2
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR4_2, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM4_3
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR4_3, ENABLE);
#endif
}

static rt_err_t pwm_tmr4_init(struct hc32_pwm_tmr4 *device)
{
    CM_TMR4_TypeDef *TMR4x;
    uint32_t i;
    RT_ASSERT(device != RT_NULL);

    TMR4x = device->instance;
    TMR4_Init(TMR4x, &device->stcTmr4Init);
<<<<<<< HEAD
    for (i=0; i<TMR4_CHANNEL_NUM_MAX; i++)
=======
    for (i = 0; i < TMR4_CHANNEL_NUM_MAX; i++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    {
        if ((device->channel >> i) & 0x01)
        {
            TMR4_OC_Init(TMR4x, i, &device->stcTmr4OcInit);
            TMR4_PWM_Init(TMR4x, i, &device->stcTmr4PwmInit);
            TMR4_PWM_SetPortOutputMode(TMR4x, i, TMR4_PWM_PIN_OUTPUT_NORMAL);
<<<<<<< HEAD
        }
        tmr4_pwm_set_cmpmode(TMR4x, i, device->CompareValue[i]);
=======
            tmr4_pwm_set_cmpmode(TMR4x, i);
        }
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    }
    TMR4_PWM_MainOutputCmd(TMR4x, ENABLE);
    TMR4_Start(TMR4x);

    return RT_EOK;
}

static void pwm_tmr4_get_channel(void)
{
#ifdef BSP_USING_PWM_TIM4_1
<<<<<<< HEAD
    #ifdef BSP_USING_PWM_TIM4_1_OUH
        g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OUL
        g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OVH
        g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OVL
        g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= 1 << 3;
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OWH
        g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OWH
        g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= 1 << 5;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM4_2
    #ifdef BSP_USING_PWM_TIM4_2_OUH
        g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OUL
        g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OVH
        g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OVL
        g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= 1 << 3;
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OWH
        g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OWH
        g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= 1 << 5;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM4_3
    #ifdef BSP_USING_PWM_TIM4_3_OUH
        g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OUL
        g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= 1 << 1;
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OVH
        g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= 1 << 2;
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OVL
        g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= 1 << 3;
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OWH
        g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= 1 << 4;
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OWH
        g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= 1 << 5;
    #endif
=======
#ifdef BSP_USING_PWM_TIM4_1_OUH
    g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM4_1_OUL
    g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIM4_1_OVH
    g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIM4_1_OVL
    g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIM4_1_OWH
    g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIM4_1_OWH
    g_pwm_tmr4_array[PWM_TIM4_1_INDEX].channel |= (1 << 5);
#endif
#endif
#ifdef BSP_USING_PWM_TIM4_2
#ifdef BSP_USING_PWM_TIM4_2_OUH
    g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM4_2_OUL
    g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIM4_2_OVH
    g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIM4_2_OVL
    g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIM4_2_OWH
    g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIM4_2_OWH
    g_pwm_tmr4_array[PWM_TIM4_2_INDEX].channel |= (1 << 5);
#endif
#endif
#ifdef BSP_USING_PWM_TIM4_3
#ifdef BSP_USING_PWM_TIM4_3_OUH
    g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM4_3_OUL
    g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= (1 << 1);
#endif
#ifdef BSP_USING_PWM_TIM4_3_OVH
    g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= (1 << 2);
#endif
#ifdef BSP_USING_PWM_TIM4_3_OVL
    g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= (1 << 3);
#endif
#ifdef BSP_USING_PWM_TIM4_3_OWH
    g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= (1 << 4);
#endif
#ifdef BSP_USING_PWM_TIM4_3_OWH
    g_pwm_tmr4_array[PWM_TIM4_3_INDEX].channel |= (1 << 5);
#endif
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
#endif
}

static rt_err_t tmr4_pwm_control(struct rt_device_pwm *device, int cmd, void *arg)
{
    struct rt_pwm_configuration *configuration = (struct rt_pwm_configuration *)arg;
    struct hc32_pwm_tmr4 *pwm;
    pwm = rt_container_of(device, struct hc32_pwm_tmr4, pwm_device);
    CM_TMR4_TypeDef *TMR4x = pwm->instance;

    switch (cmd)
    {
    case PWM_CMD_ENABLE:
        return tmr4_pwm_enable(TMR4x, configuration, RT_TRUE);
    case PWM_CMD_DISABLE:
        return tmr4_pwm_enable(TMR4x, configuration, RT_FALSE);
    case PWM_CMD_SET:
        return tmr4_pwm_set(TMR4x, configuration);
    case PWM_CMD_GET:
        return tmr4_pwm_get(TMR4x, configuration);
    case PWM_CMD_SET_PERIOD:
        return tmr4_pwm_set_period(TMR4x, configuration->channel, configuration->period);
    case PWM_CMD_SET_PULSE:
        return tmr4_pwm_set_pulse(TMR4x, configuration->channel, configuration->pulse);
<<<<<<< HEAD
    // case PWM_CMD_SET_DEAD_TIME:
    //     return tmr4_dead_time_set(TMR4x, configuration);
=======
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    default:
        return RT_EINVAL;
    }
}

static struct rt_pwm_ops tmr4_ops =
{
    tmr4_pwm_control
};

#endif  /* BSP_USING_PWM_TIM4 */

#if defined(BSP_USING_PWM_TIM6)

#define TMR6_CHANNEL_NUM_MAX     2U

enum
{
#ifdef BSP_USING_PWM_TIM6_1
    PWM_TIM6_1_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM6_2
    PWM_TIM6_2_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM6_3
    PWM_TIM6_3_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM6_4
    PWM_TIM6_4_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM6_5
    PWM_TIM6_5_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM6_6
    PWM_TIM6_6_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM6_7
    PWM_TIM6_7_INDEX,
#endif
#ifdef BSP_USING_PWM_TIM6_8
    PWM_TIM6_8_INDEX,
#endif
    PWM_TIM6_UNIT_NUM,
};

struct hc32_pwm_tmr6
{
    struct rt_device_pwm pwm_device;
    char *name;
    CM_TMR6_TypeDef *instance;
    rt_uint32_t channel;
    stc_tmr6_init_t stcTmr6Init;
    stc_tmr6_pwm_init_t stcPwmInit[TMR6_CHANNEL_NUM_MAX];
<<<<<<< HEAD
=======
    rt_bool_t complementary[TMRA_CHANNEL_NUM_MAX];
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
};

static struct hc32_pwm_tmr6 g_pwm_tmr6_array[] =
{
#ifdef BSP_USING_PWM_TIM6_1
    PWM_TIM6_1_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM6_2
    PWM_TIM6_2_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM6_3
    PWM_TIM6_3_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM6_4
    PWM_TIM6_4_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM6_5
    PWM_TIM6_5_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM6_6
    PWM_TIM6_6_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM6_7
    PWM_TIM6_7_CONFIG,
#endif
#ifdef BSP_USING_PWM_TIM6_8
    PWM_TIM6_8_CONFIG,
#endif
};

static rt_uint32_t tmr6_get_clk_notdiv(CM_TMR6_TypeDef *TMR6x)
{
    rt_uint32_t u32clkFreq;
    switch ((rt_uint32_t)TMR6x)
    {
    case (rt_uint32_t)CM_TMR6_1:
    case (rt_uint32_t)CM_TMR6_2:
    case (rt_uint32_t)CM_TMR6_3:
    case (rt_uint32_t)CM_TMR6_4:
    case (rt_uint32_t)CM_TMR6_5:
    case (rt_uint32_t)CM_TMR6_6:
    case (rt_uint32_t)CM_TMR6_7:
    case (rt_uint32_t)CM_TMR6_8:
        u32clkFreq = CLK_GetBusClockFreq(CLK_BUS_PCLK0);
        break;
    default:
        break;
    }
    return (u32clkFreq ? u32clkFreq : HCLK_VALUE);
}

static rt_uint32_t tmr6_get_clk_bydiv(CM_TMR6_TypeDef *TMR6x)
{
    rt_uint32_t u32clkFreq;
    uint16_t u16Div;

    u32clkFreq = tmr6_get_clk_notdiv(TMR6x);
    u16Div = (READ_REG16(TMR6x->GCONR) & TMR6_GCONR_CKDIV);
    switch (u16Div)
    {
<<<<<<< HEAD
        case (TMR6_CLK_DIV1):
            break;
        case (TMR6_CLK_DIV2):
            u32clkFreq /= 2;
            break;
        case (TMR6_CLK_DIV4):
            u32clkFreq /= 4;
            break;
        case (TMR6_CLK_DIV8):
            u32clkFreq /= 8;
            break;
        case (TMR6_CLK_DIV16):
            u32clkFreq /= 16;
            break;
        case (TMR6_CLK_DIV32):
            u32clkFreq /= 32;
            break;
        case (TMR6_CLK_DIV64):
            u32clkFreq /= 64;
            break;
        case (TMR6_CLK_DIV128):
            u32clkFreq /= 128;
            break;
        case (TMR6_CLK_DIV256):
            u32clkFreq /= 256;
            break;
        case (TMR6_CLK_DIV512):
            u32clkFreq /= 512;
            break;
        case (TMR6_CLK_DIV1024):
            u32clkFreq /= 1024;
            break;
        default:
            break;
=======
    case (TMR6_CLK_DIV1):
        break;
    case (TMR6_CLK_DIV2):
        u32clkFreq /= 2;
        break;
    case (TMR6_CLK_DIV4):
        u32clkFreq /= 4;
        break;
    case (TMR6_CLK_DIV8):
        u32clkFreq /= 8;
        break;
    case (TMR6_CLK_DIV16):
        u32clkFreq /= 16;
        break;
    case (TMR6_CLK_DIV32):
        u32clkFreq /= 32;
        break;
    case (TMR6_CLK_DIV64):
        u32clkFreq /= 64;
        break;
    case (TMR6_CLK_DIV128):
        u32clkFreq /= 128;
        break;
    case (TMR6_CLK_DIV256):
        u32clkFreq /= 256;
        break;
    case (TMR6_CLK_DIV512):
        u32clkFreq /= 512;
        break;
    case (TMR6_CLK_DIV1024):
        u32clkFreq /= 1024;
        break;
    default:
        break;
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    }
    return u32clkFreq;
}

<<<<<<< HEAD
=======
static void tmr6_duyt100or0_output(CM_TMR6_TypeDef *TMR6x, uint32_t channel, uint32_t CompareValue)
{
    rt_uint8_t i;
    rt_uint8_t ch = channel % TMR6_CHANNEL_NUM_MAX;
    for (i = 0; i < PWM_TIM6_UNIT_NUM; i++)
    {
        if (g_pwm_tmr6_array[i].instance == TMR6x)
        {
            /* Realize 100% duty output */
            if (TMR6_GetPeriodValue(TMR6x, TMR6_PERIOD_REG_A) == CompareValue)
            {
                if (RT_TRUE == g_pwm_tmr6_array[i].complementary[ch])
                {
                    TMR6_PWM_SetForcePolarity(TMR6x, ch, TMR6_PWM_FORCE_LOW);
                }
                else
                {
                    TMR6_PWM_SetForcePolarity(TMR6x, ch, TMR6_PWM_FORCE_HIGH);
                }
            }
            else if (0 == CompareValue)        /* Realize 0% duty output */
            {
                if (RT_TRUE == g_pwm_tmr6_array[i].complementary[ch])
                {
                    TMR6_PWM_SetForcePolarity(TMR6x, ch, TMR6_PWM_FORCE_HIGH);
                }
                else
                {
                    TMR6_PWM_SetForcePolarity(TMR6x, ch, TMR6_PWM_FORCE_LOW);
                }
            }
            break;
        }
    }
}

>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
static void tmr6_pwmp_enable(CM_TMR6_TypeDef *TMR6x, struct rt_pwm_configuration *configuration)
{
    rt_uint8_t i;
    rt_uint8_t ch = configuration->channel % TMR6_CHANNEL_NUM_MAX;
    stc_tmr6_pwm_init_t stcPwmInit;
<<<<<<< HEAD
    for (i=0; i<PWM_TIM6_UNIT_NUM; i++)
=======
    for (i = 0; i < PWM_TIM6_UNIT_NUM; i++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    {
        if (g_pwm_tmr6_array[i].instance == TMR6x)
        {
            rt_memcpy(&stcPwmInit, &g_pwm_tmr6_array[i].stcPwmInit[ch], sizeof(stc_tmr6_pwm_init_t));
            stcPwmInit.u32CompareValue = TMR6_GetCompareValue(TMR6x, ch);
            TMR6_PWM_Init(TMR6x, ch, &stcPwmInit);
<<<<<<< HEAD
            break;
        }
    }
=======
            g_pwm_tmr6_array[i].complementary[ch] = RT_FALSE;
            break;
        }
    }
    if (0 == stcPwmInit.u32CompareValue || stcPwmInit.u32CompareValue == TMR6_GetPeriodValue(TMR6x, TMR6_PERIOD_REG_A))
    {
        tmr6_duyt100or0_output(TMR6x, configuration->channel, stcPwmInit.u32CompareValue);
    }
    else
    {
        TMR6_PWM_SetForcePolarity(TMR6x, ch, TMR6_PWM_FORCE_INVD);
    }
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
}

static void tmr6_pwmn_enable(CM_TMR6_TypeDef *TMR6x, struct rt_pwm_configuration *configuration)
{
<<<<<<< HEAD
=======
    rt_uint8_t i;
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    rt_uint8_t ch = configuration->channel % TMR6_CHANNEL_NUM_MAX;
    stc_tmr6_pwm_init_t stcPwmInit;
    stcPwmInit.u32CompareValue = TMR6_GetCompareValue(TMR6x, ch);
    stcPwmInit.u32StartPolarity           = TMR6_PWM_HIGH;
    stcPwmInit.u32StopPolarity            = TMR6_PWM_HIGH;
    if (ch)
    {
        stcPwmInit.u32CountUpMatchAPolarity   = TMR6_PWM_HOLD;
        stcPwmInit.u32CountDownMatchAPolarity = TMR6_PWM_HOLD;
        stcPwmInit.u32CountUpMatchBPolarity   = TMR6_PWM_LOW;
        stcPwmInit.u32CountDownMatchBPolarity = TMR6_PWM_LOW;
<<<<<<< HEAD
    } else {
=======
    }
    else
    {
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        stcPwmInit.u32CountUpMatchAPolarity   = TMR6_PWM_LOW;
        stcPwmInit.u32CountDownMatchAPolarity = TMR6_PWM_LOW;
        stcPwmInit.u32CountUpMatchBPolarity   = TMR6_PWM_HOLD;
        stcPwmInit.u32CountDownMatchBPolarity = TMR6_PWM_HOLD;
    }
    stcPwmInit.u32UdfPolarity             = TMR6_PWM_HIGH;
    stcPwmInit.u32OvfPolarity             = TMR6_PWM_HIGH;
    TMR6_PWM_Init(TMR6x, ch, &stcPwmInit);
<<<<<<< HEAD
=======
    /* Flags whether the channel complements output */
    for (i = 0; i < PWM_TIM6_UNIT_NUM; i++)
    {
        if (g_pwm_tmr6_array[i].instance == TMR6x)
        {
            g_pwm_tmr6_array[i].complementary[ch] = RT_TRUE;
        }
    }
    if (0 == stcPwmInit.u32CompareValue || stcPwmInit.u32CompareValue == TMR6_GetPeriodValue(TMR6x, TMR6_PERIOD_REG_A))
    {
        tmr6_duyt100or0_output(TMR6x, configuration->channel, stcPwmInit.u32CompareValue);
    }
    else
    {
        TMR6_PWM_SetForcePolarity(TMR6x, ch, TMR6_PWM_FORCE_INVD);
    }
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
}

static rt_err_t tmr6_pwm_enable(CM_TMR6_TypeDef *TMR6x, struct rt_pwm_configuration *configuration, rt_bool_t enable)
{
    /* channel0 will set complementary but belong to pwm_ch1 */
    if (configuration->complementary && configuration->channel != 0)
    {
        tmr6_pwmn_enable(TMR6x, configuration);
<<<<<<< HEAD
    } else {
=======
    }
    else
    {
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        tmr6_pwmp_enable(TMR6x, configuration);
    }

    if (enable)
    {
        TMR6_PWM_OutputCmd(TMR6x, (configuration->channel % TMR6_CHANNEL_NUM_MAX), ENABLE);
<<<<<<< HEAD
    } else {
=======
    }
    else
    {
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        TMR6_PWM_OutputCmd(TMR6x, (configuration->channel % TMR6_CHANNEL_NUM_MAX), DISABLE);
    }

    return RT_EOK;
}

static rt_err_t tmr6_pwm_get(CM_TMR6_TypeDef *TMR6x, struct rt_pwm_configuration *configuration)
{
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    u32clkFreq = tmr6_get_clk_bydiv(TMR6x);
<<<<<<< HEAD
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
=======
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    configuration->period = u64clk_ns * TMR6_GetPeriodValue(TMR6x, TMR6_PERIOD_REG_A);
    configuration->pulse = u64clk_ns * TMR6_GetCompareValue(TMR6x, (configuration->channel % TMR6_CHANNEL_NUM_MAX));
    return RT_EOK;
}

static rt_uint64_t tmr6_auto_set_div(CM_TMR6_TypeDef *TMR6x, uint32_t period)
{
    rt_uint32_t u32clkFreq;
    rt_uint64_t u64clk_ns;
    rt_uint64_t u64val;
    u32clkFreq = tmr6_get_clk_bydiv(TMR6x);
<<<<<<< HEAD
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
    u64val = (rt_uint64_t)period / u64clk_ns;
    /* clk not match, need change div */
    if ((period <= u64clk_ns) || (u64val > 0xFFFF))
    {
        uint32_t div_bit;
        u32clkFreq = tmr6_get_clk_notdiv(TMR6x);
        u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
        u64val = (rt_uint64_t)period / u64clk_ns;
        for (div_bit = 0; div_bit <= 10; div_bit++)
        {
            if (u64val < 0xFFFF)
=======
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
    u64val = (rt_uint64_t)period / u64clk_ns;
    /* clk not match, need change div */
    if ((period <= u64clk_ns) || (u64val > 0xFFFFU))
    {
        uint32_t div_bit;
        u32clkFreq = tmr6_get_clk_notdiv(TMR6x);
        u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
        u64val = (rt_uint64_t)period / u64clk_ns;
        for (div_bit = 0; div_bit <= 10; div_bit++)
        {
            if (u64val < 0xFFFFU)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
            {
                break;
            }
            u64val /= 2;
        }
        if (div_bit > 10)
        {
<<<<<<< HEAD
            return RT_ERROR;
        }
        TMR6_SetClockDiv(TMR6x, div_bit << TMR6_GCONR_CKDIV_POS);
        u32clkFreq = tmr6_get_clk_bydiv(TMR6x);
        u64clk_ns = (rt_uint64_t)(1000000000ul / u32clkFreq);
=======
            return 0;
        }
        TMR6_SetClockDiv(TMR6x, div_bit << TMR6_GCONR_CKDIV_POS);
        u32clkFreq = tmr6_get_clk_bydiv(TMR6x);
        u64clk_ns = (rt_uint64_t)(1000000000UL / u32clkFreq);
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    }
    return u64clk_ns;
}

static rt_err_t tmr6_pwm_set_period(CM_TMR6_TypeDef *TMR6x, uint32_t channel, uint32_t period)
{
<<<<<<< HEAD
    rt_uint8_t i,j;
    rt_uint64_t u64clk_ns = tmr6_auto_set_div(TMR6x, period);
    TMR6_SetPeriodValue(TMR6x, TMR6_PERIOD_REG_A, period / u64clk_ns);
    /* setting PeriodValue maybe change the div,so we need to recalculate the CompareValue */
    for (i=0; i<PWM_TIM6_UNIT_NUM; i++)
    {
        if (g_pwm_tmr6_array[i].instance == TMR6x)
        {
            for (j=0; j<TMR6_CHANNEL_NUM_MAX; j++)
=======
    rt_uint8_t i, j;
    rt_uint64_t u64clk_ns = tmr6_auto_set_div(TMR6x, period);
    if (!u64clk_ns)
    {
        return -RT_ERROR;
    }
    TMR6_SetPeriodValue(TMR6x, TMR6_PERIOD_REG_A, period / u64clk_ns);
    /* setting PeriodValue maybe change the div,so we need to recalculate the CompareValue */
    for (i = 0; i < PWM_TIM6_UNIT_NUM; i++)
    {
        if (g_pwm_tmr6_array[i].instance == TMR6x)
        {
            for (j = 0; j < TMR6_CHANNEL_NUM_MAX; j++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
            {
                TMR6_SetCompareValue(TMR6x, j, g_pwm_tmr6_array[i].stcPwmInit[j].u32CompareValue / u64clk_ns);
            }
            g_pwm_tmr6_array[i].stcTmr6Init.u32PeriodValue = period;
            break;
        }
    }
    return RT_EOK;
}

static rt_err_t tmr6_pwm_set_pulse(CM_TMR6_TypeDef *TMR6x, uint32_t channel, uint32_t pulse)
{
    rt_uint8_t i;
    rt_uint32_t u32clkFreq;
<<<<<<< HEAD
    rt_uint64_t u64clk_ns;
    rt_uint8_t ch = channel % TMR6_CHANNEL_NUM_MAX;
    u32clkFreq = tmr6_get_clk_bydiv(TMR6x);
    u64clk_ns = (rt_uint64_t)1000000000ul / u32clkFreq;
    TMR6_SetCompareValue(TMR6x, ch, pulse / u64clk_ns);
    for (i=0; i<PWM_TIM6_UNIT_NUM; i++)
=======
    rt_uint32_t CompareValue;
    rt_uint64_t u64clk_ns;
    rt_uint8_t ch = channel % TMR6_CHANNEL_NUM_MAX;
    u32clkFreq = tmr6_get_clk_bydiv(TMR6x);
    u64clk_ns = (rt_uint64_t)1000000000UL / u32clkFreq;
    TMR6_SetCompareValue(TMR6x, ch, pulse / u64clk_ns);
    CompareValue = TMR6_GetCompareValue(TMR6x, ch);
    for (i = 0; i < PWM_TIM6_UNIT_NUM; i++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    {
        if (g_pwm_tmr6_array[i].instance == TMR6x)
        {
            g_pwm_tmr6_array[i].stcPwmInit[ch].u32CompareValue = pulse;
<<<<<<< HEAD
            /* Realize 100% duty output */
            if (g_pwm_tmr6_array[i].stcTmr6Init.u32PeriodValue == pulse) {
                TMR6_PWM_SetForcePolarity(TMR6x, ch, TMR6_PWM_FORCE_HIGH);
            } else {
                TMR6_PWM_SetForcePolarity(TMR6x, ch, TMR6_PWM_FORCE_INVD);
            }
            break;
        }
    }
=======
            break;
        }
    }
    if (0 == CompareValue || CompareValue == TMR6_GetPeriodValue(TMR6x, TMR6_PERIOD_REG_A))
    {
        tmr6_duyt100or0_output(TMR6x, channel, CompareValue);
    }
    else
    {
        TMR6_PWM_SetForcePolarity(TMR6x, channel, TMR6_PWM_FORCE_INVD);
    }
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    return RT_EOK;
}

static rt_err_t tmr6_pwm_set(CM_TMR6_TypeDef *TMR6x, struct rt_pwm_configuration *configuration)
{
    tmr6_pwm_set_period(TMR6x, configuration->channel, configuration->period);
    tmr6_pwm_set_pulse(TMR6x, configuration->channel, configuration->pulse);

    return RT_EOK;
}

static void enable_tmr6_unit_clk(void)
{
#ifdef BSP_USING_PWM_TIM6_1
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR6_1, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM6_2
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR6_2, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM6_3
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR6_3, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM6_4
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR6_4, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM6_5
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR6_5, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM6_6
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR6_6, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM6_7
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR6_7, ENABLE);
#endif
#ifdef BSP_USING_PWM_TIM6_8
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR6_8, ENABLE);
#endif
}

static rt_err_t pwm_tmr6_init(struct hc32_pwm_tmr6 *device)
{
    CM_TMR6_TypeDef *TMR6x;
    uint32_t i;
    RT_ASSERT(device != RT_NULL);

    TMR6x = device->instance;
    TMR6_Init(TMR6x, &device->stcTmr6Init);
<<<<<<< HEAD
    for (i=0; i<TMR6_CHANNEL_NUM_MAX; i++)
=======
    for (i = 0; i < TMR6_CHANNEL_NUM_MAX; i++)
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
    {
        if ((device->channel >> i) & 0x01)
        {
            TMR6_PWM_Init(TMR6x, i, &device->stcPwmInit[i]);
        }
    }
    TMR6_Start(TMR6x);

    return RT_EOK;
}

static void pwm_tmr6_get_channel(void)
{
#ifdef BSP_USING_PWM_TIM6_1
<<<<<<< HEAD
    #ifdef BSP_USING_PWM_TIM6_1_A
        g_pwm_tmr6_array[PWM_TIM6_1_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM6_1_B
        g_pwm_tmr6_array[PWM_TIM6_1_INDEX].channel |= 1 << 1;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM6_2
    #ifdef BSP_USING_PWM_TIM6_2_A
        g_pwm_tmr6_array[PWM_TIM6_2_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM6_2_B
        g_pwm_tmr6_array[PWM_TIM6_2_INDEX].channel |= 1 << 1;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM6_3
    #ifdef BSP_USING_PWM_TIM6_3_A
        g_pwm_tmr6_array[PWM_TIM6_3_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM6_3_B
        g_pwm_tmr6_array[PWM_TIM6_3_INDEX].channel |= 1 << 1;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM6_4
    #ifdef BSP_USING_PWM_TIM6_4_A
        g_pwm_tmr6_array[PWM_TIM6_4_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM6_4_B
        g_pwm_tmr6_array[PWM_TIM6_4_INDEX].channel |= 1 << 1;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM6_5
    #ifdef BSP_USING_PWM_TIM6_5_A
        g_pwm_tmr6_array[PWM_TIM6_5_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM6_5_B
        g_pwm_tmr6_array[PWM_TIM6_5_INDEX].channel |= 1 << 1;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM6_6
    #ifdef BSP_USING_PWM_TIM6_6_A
        g_pwm_tmr6_array[PWM_TIM6_6_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM6_6_B
        g_pwm_tmr6_array[PWM_TIM6_6_INDEX].channel |= 1 << 1;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM6_7
    #ifdef BSP_USING_PWM_TIM6_7_A
        g_pwm_tmr6_array[PWM_TIM6_7_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM6_7_B
        g_pwm_tmr6_array[PWM_TIM6_7_INDEX].channel |= 1 << 1;
    #endif
#endif
#ifdef BSP_USING_PWM_TIM6_8
    #ifdef BSP_USING_PWM_TIM6_8_A
        g_pwm_tmr6_array[PWM_TIM6_8_INDEX].channel |= 1 << 0;
    #endif
    #ifdef BSP_USING_PWM_TIM6_8_B
        g_pwm_tmr6_array[PWM_TIM6_8_INDEX].channel |= 1 << 1;
    #endif
=======
#ifdef BSP_USING_PWM_TIM6_1_A
    g_pwm_tmr6_array[PWM_TIM6_1_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM6_1_B
    g_pwm_tmr6_array[PWM_TIM6_1_INDEX].channel |= (1 << 1);
#endif
#endif
#ifdef BSP_USING_PWM_TIM6_2
#ifdef BSP_USING_PWM_TIM6_2_A
    g_pwm_tmr6_array[PWM_TIM6_2_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM6_2_B
    g_pwm_tmr6_array[PWM_TIM6_2_INDEX].channel |= (1 << 1);
#endif
#endif
#ifdef BSP_USING_PWM_TIM6_3
#ifdef BSP_USING_PWM_TIM6_3_A
    g_pwm_tmr6_array[PWM_TIM6_3_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM6_3_B
    g_pwm_tmr6_array[PWM_TIM6_3_INDEX].channel |= (1 << 1);
#endif
#endif
#ifdef BSP_USING_PWM_TIM6_4
#ifdef BSP_USING_PWM_TIM6_4_A
    g_pwm_tmr6_array[PWM_TIM6_4_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM6_4_B
    g_pwm_tmr6_array[PWM_TIM6_4_INDEX].channel |= (1 << 1);
#endif
#endif
#ifdef BSP_USING_PWM_TIM6_5
#ifdef BSP_USING_PWM_TIM6_5_A
    g_pwm_tmr6_array[PWM_TIM6_5_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM6_5_B
    g_pwm_tmr6_array[PWM_TIM6_5_INDEX].channel |= (1 << 1);
#endif
#endif
#ifdef BSP_USING_PWM_TIM6_6
#ifdef BSP_USING_PWM_TIM6_6_A
    g_pwm_tmr6_array[PWM_TIM6_6_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM6_6_B
    g_pwm_tmr6_array[PWM_TIM6_6_INDEX].channel |= (1 << 1);
#endif
#endif
#ifdef BSP_USING_PWM_TIM6_7
#ifdef BSP_USING_PWM_TIM6_7_A
    g_pwm_tmr6_array[PWM_TIM6_7_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM6_7_B
    g_pwm_tmr6_array[PWM_TIM6_7_INDEX].channel |= (1 << 1);
#endif
#endif
#ifdef BSP_USING_PWM_TIM6_8
#ifdef BSP_USING_PWM_TIM6_8_A
    g_pwm_tmr6_array[PWM_TIM6_8_INDEX].channel |= (1 << 0);
#endif
#ifdef BSP_USING_PWM_TIM6_8_B
    g_pwm_tmr6_array[PWM_TIM6_8_INDEX].channel |= (1 << 1);
#endif
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
#endif
}

static rt_err_t tmr6_pwm_control(struct rt_device_pwm *device, int cmd, void *arg)
{
    struct rt_pwm_configuration *configuration = (struct rt_pwm_configuration *)arg;
    struct hc32_pwm_tmr6 *pwm;
    pwm = rt_container_of(device, struct hc32_pwm_tmr6, pwm_device);
    CM_TMR6_TypeDef *TMR6x = pwm->instance;

    switch (cmd)
    {
    case PWM_CMD_ENABLE:
        return tmr6_pwm_enable(TMR6x, configuration, RT_TRUE);
    case PWM_CMD_DISABLE:
        return tmr6_pwm_enable(TMR6x, configuration, RT_FALSE);
    case PWM_CMD_SET:
        return tmr6_pwm_set(TMR6x, configuration);
    case PWM_CMD_GET:
        return tmr6_pwm_get(TMR6x, configuration);
    case PWM_CMD_SET_PERIOD:
        return tmr6_pwm_set_period(TMR6x, configuration->channel, configuration->period);
    case PWM_CMD_SET_PULSE:
        return tmr6_pwm_set_pulse(TMR6x, configuration->channel, configuration->pulse);
    default:
        return RT_EINVAL;
    }
}

static struct rt_pwm_ops tmr6_ops =
{
    tmr6_pwm_control
};

#endif  /* BSP_USING_PWM_TIM6 */

static int rt_hw_pwm_tmr_init(void)
{
    int i = 0;
    rt_err_t result = RT_EOK;

#if defined(BSP_USING_PWM_TIMA)
    pwm_tmra_get_channel();
    enable_tmra_unit_clk();
    for (i = 0; i < sizeof(g_pwm_tmra_array) / sizeof(g_pwm_tmra_array[0]); i++)
    {
        /* pwm init */
        pwm_tmra_init(&g_pwm_tmra_array[i]);
        /* gpio init */
<<<<<<< HEAD
        extern rt_err_t rt_hw_board_pwm_tmra_init(CM_TMRA_TypeDef *TMRAx);
=======
        extern rt_err_t rt_hw_board_pwm_tmra_init(CM_TMRA_TypeDef * TMRAx);
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        rt_hw_board_pwm_tmra_init(g_pwm_tmra_array[i].instance);
        /* register pwm device */
        result = rt_device_pwm_register(&g_pwm_tmra_array[i].pwm_device, g_pwm_tmra_array[i].name, &tmra_ops, &g_pwm_tmra_array[i].instance);
        RT_ASSERT(result == RT_EOK);
    }
#endif

#if defined(BSP_USING_PWM_TIM4)
    pwm_tmr4_get_channel();
    enable_tmr4_unit_clk();
    for (i = 0; i < sizeof(g_pwm_tmr4_array) / sizeof(g_pwm_tmr4_array[0]); i++)
    {
        /* pwm init */
        pwm_tmr4_init(&g_pwm_tmr4_array[i]);
        /* gpio init */
<<<<<<< HEAD
        extern rt_err_t rt_hw_board_pwm_tmr4_init(CM_TMR4_TypeDef *TMR4x);
=======
        extern rt_err_t rt_hw_board_pwm_tmr4_init(CM_TMR4_TypeDef * TMR4x);
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        rt_hw_board_pwm_tmr4_init(g_pwm_tmr4_array[i].instance);
        /* register pwm device */
        result = rt_device_pwm_register(&g_pwm_tmr4_array[i].pwm_device, g_pwm_tmr4_array[i].name, &tmr4_ops, &g_pwm_tmr4_array[i].instance);
        RT_ASSERT(result == RT_EOK);
    }
#endif

#if defined(BSP_USING_PWM_TIM6)
    pwm_tmr6_get_channel();
    enable_tmr6_unit_clk();
    for (i = 0; i < sizeof(g_pwm_tmr6_array) / sizeof(g_pwm_tmr6_array[0]); i++)
    {
        /* pwm init */
        pwm_tmr6_init(&g_pwm_tmr6_array[i]);
        /* gpio init */
<<<<<<< HEAD
        extern rt_err_t rt_hw_board_pwm_tmr6_init(CM_TMR6_TypeDef *TMR6x);
=======
        extern rt_err_t rt_hw_board_pwm_tmr6_init(CM_TMR6_TypeDef * TMR6x);
>>>>>>> 98b36a8b145734f8302270eab9e44c21c19e4a28
        rt_hw_board_pwm_tmr6_init(g_pwm_tmr6_array[i].instance);
        /* register pwm device */
        result = rt_device_pwm_register(&g_pwm_tmr6_array[i].pwm_device, g_pwm_tmr6_array[i].name, &tmr6_ops, &g_pwm_tmr6_array[i].instance);
        RT_ASSERT(result == RT_EOK);
    }
#endif

    return result;
}
INIT_DEVICE_EXPORT(rt_hw_pwm_tmr_init);

#endif  /* BSP_USING_PWM */
