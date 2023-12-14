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

#ifdef BSP_USING_TIMA_1
#ifndef TIMA_1_CONFIG
#define TIMA_1_CONFIG                                       \
    {                                                       \
       .tim_handle      = CM_TMRA_1,                        \
       .clock_source    = CLK_BUS_PCLK1,                    \
       .clock           = FCG2_PERIPH_TMRA_1,               \
       .flag            = TMRA_FLAG_OVF,                    \
       .isr             =                                   \
       {                                                    \
            .enIntSrc   = INT_SRC_TMRA_1_OVF,               \
            .enIRQn     = BSP_USING_TIMA_1_IRQ_NUM,         \
            .u8Int_Prio = BSP_USING_TIMA_1_IRQ_PRIO,        \
       },                                                   \
       .name            = "timA_1"                          \
    }
#endif /* TIMA_1_CONFIG */
#endif /* BSP_USING_TIMA_1 */

#ifdef BSP_USING_TIMA_2
#ifndef TIMA_2_CONFIG
#define TIMA_2_CONFIG                                       \
    {                                                       \
       .tim_handle      = CM_TMRA_2,                        \
       .clock_source    = CLK_BUS_PCLK1,                    \
       .clock           = FCG2_PERIPH_TMRA_2,               \
       .flag            = TMRA_FLAG_OVF,                    \
       .isr             =                                   \
       {                                                    \
            .enIntSrc   = INT_SRC_TMRA_2_OVF,               \
            .enIRQn     = BSP_USING_TIMA_2_IRQ_NUM,         \
            .u8Int_Prio = BSP_USING_TIMA_2_IRQ_PRIO,        \
       },                                                   \
       .name            = "timA_2"                          \
    }
#endif /* TIMA_2_CONFIG */
#endif /* BSP_USING_TIMA_2 */

#ifdef BSP_USING_TIMA_3
#ifndef TIMA_3_CONFIG
#define TIMA_3_CONFIG                                       \
    {                                                       \
       .tim_handle      = CM_TMRA_3,                        \
       .clock_source    = CLK_BUS_PCLK1,                    \
       .clock           = FCG2_PERIPH_TMRA_3,               \
       .flag            = TMRA_FLAG_OVF,                    \
       .isr             =                                   \
       {                                                    \
            .enIntSrc   = INT_SRC_TMRA_3_OVF,               \
            .enIRQn     = BSP_USING_TIMA_3_IRQ_NUM,         \
            .u8Int_Prio = BSP_USING_TIMA_3_IRQ_PRIO,        \
       },                                                   \
       .name            = "timA_3"                          \
    }
#endif /* TIMA_3_CONFIG */
#endif /* BSP_USING_TIMA_3 */

#ifdef BSP_USING_TIMA_4
#ifndef TIMA_4_CONFIG
#define TIMA_4_CONFIG                                       \
    {                                                       \
       .tim_handle      = CM_TMRA_4,                        \
       .clock_source    = CLK_BUS_PCLK1,                    \
       .clock           = FCG2_PERIPH_TMRA_4,               \
       .flag            = TMRA_FLAG_OVF,                    \
       .isr             =                                   \
       {                                                    \
            .enIntSrc   = INT_SRC_TMRA_4_OVF,               \
            .enIRQn     = BSP_USING_TIMA_4_IRQ_NUM,         \
            .u8Int_Prio = BSP_USING_TIMA_4_IRQ_PRIO,        \
       },                                                   \
       .name            = "timA_4"                          \
    }
#endif /* TIMA_4_CONFIG */
#endif /* BSP_USING_TIMA_4 */

#ifdef BSP_USING_TIMA_5
#ifndef TIMA_5_CONFIG
#define TIMA_5_CONFIG                                       \
    {                                                       \
       .tim_handle      = CM_TMRA_5,                        \
       .clock_source    = CLK_BUS_PCLK1,                    \
       .clock           = FCG2_PERIPH_TMRA_5,               \
       .flag            = TMRA_FLAG_OVF,                    \
       .isr             =                                   \
       {                                                    \
            .enIntSrc   = INT_SRC_TMRA_5_OVF,               \
            .enIRQn     = BSP_USING_TIMA_5_IRQ_NUM,         \
            .u8Int_Prio = BSP_USING_TIMA_5_IRQ_PRIO,        \
       },                                                   \
       .name            = "timA_5"                          \
    }
#endif /* TIMA_5_CONFIG */
#endif /* BSP_USING_TIMA_5 */

#ifdef BSP_USING_TIMA_6
#ifndef TIMA_6_CONFIG
#define TIMA_6_CONFIG                                       \
    {                                                       \
       .tim_handle      = CM_TMRA_6,                        \
       .clock_source    = CLK_BUS_PCLK1,                    \
       .clock           = FCG2_PERIPH_TMRA_6,               \
       .flag            = TMRA_FLAG_OVF,                    \
       .isr             =                                   \
       {                                                    \
            .enIntSrc   = INT_SRC_TMRA_6_OVF,               \
            .enIRQn     = BSP_USING_TIMA_6_IRQ_NUM,         \
            .u8Int_Prio = BSP_USING_TIMA_6_IRQ_PRIO,        \
       },                                                   \
       .name            = "timA_6"                          \
    }
#endif /* TIMA_6_CONFIG */
#endif /* BSP_USING_TIMA_6 */

#endif /* __TIM_CONFIG_H__ */
