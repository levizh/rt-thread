/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-06-21     CDT               first version
 */

#ifndef __TIM_CONFIG_H__
#define __TIM_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BSP_USING_TIM0_1A
#ifndef TIM0_1A_CONFIG
#define TIM0_1A_CONFIG                            \
    {                                             \
       .tim_handle         = CM_TMR0_1,           \
       .u32Fcg2Periph       = FCG2_PERIPH_TMR0_1, \
       .u32Ch = TMR0_CH_A,                        \
       .u32Flag = TMR0_FLAG_CMP_A,                \
       .isr = {                                   \
            .enIntSrc = INT_SRC_TMR0_1_CMP_A,     \
            .enIRQn = BSP_USING_TIM0_1A_IRQ_NUM,          \
            .u8Int_Prio = BSP_USING_TIM0_1A_IRQ_PRIO,     \
       },                                         \
       .name                     = "tim0_1A"      \
    }
#endif /* TIM0_1A_CONFIG */
#endif /* BSP_USING_TIM0_1A */
#ifdef BSP_USING_TIM0_1B
#ifndef TIM0_1B_CONFIG
#define TIM0_1B_CONFIG                            \
    {                                             \
       .tim_handle         = CM_TMR0_1,           \
       .u32Fcg2Periph       = FCG2_PERIPH_TMR0_1, \
       .u32Ch = TMR0_CH_B,                        \
       .u32Flag = TMR0_FLAG_CMP_B,                \
       .isr = {                                   \
            .enIntSrc = INT_SRC_TMR0_1_CMP_B,     \
            .enIRQn = BSP_USING_TIM0_1B_IRQ_NUM,          \
            .u8Int_Prio = BSP_USING_TIM0_1B_IRQ_PRIO,     \
       },                                         \
       .name                     = "tim0_1B"      \
    }
#endif /* TIM0_1B_CONFIG */
#endif /* BSP_USING_TIM0_1B */
#ifdef BSP_USING_TIM0_2A
#ifndef TIM0_2A_CONFIG
#define TIM0_2A_CONFIG                            \
    {                                             \
       .tim_handle         = CM_TMR0_2,           \
       .u32Fcg2Periph       = FCG2_PERIPH_TMR0_2, \
       .u32Ch = TMR0_CH_A,                        \
       .u32Flag = TMR0_FLAG_CMP_A,                \
       .isr = {                                   \
            .enIntSrc = INT_SRC_TMR0_2_CMP_A,     \
            .enIRQn = BSP_USING_TIM0_2A_IRQ_NUM,          \
            .u8Int_Prio = BSP_USING_TIM0_2A_IRQ_PRIO,     \
       },                                         \
       .name                     = "tim0_2A"      \
    }
#endif /* TIM0_2A_CONFIG */
#endif /* BSP_USING_TIM0_2A */
#ifdef BSP_USING_TIM0_2B
#ifndef TIM0_2B_CONFIG
#define TIM0_2B_CONFIG                            \
    {                                             \
       .tim_handle         = CM_TMR0_2,           \
       .u32Fcg2Periph       = FCG2_PERIPH_TMR0_2, \
       .u32Ch = TMR0_CH_B,                        \
       .u32Flag = TMR0_FLAG_CMP_B,                \
       .isr = {                                   \
            .enIntSrc = INT_SRC_TMR0_2_CMP_B,     \
            .enIRQn = BSP_USING_TIM0_2B_IRQ_NUM,          \
            .u8Int_Prio = BSP_USING_TIM0_2B_IRQ_PRIO,     \
       },                                         \
       .name                     = "tim0_2B"      \
    }
#endif /* TIM0_2B_CONFIG */
#endif /* BSP_USING_TIM0_2B */

#endif /* __TIM_CONFIG_H__ */
