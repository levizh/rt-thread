/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#ifndef __I2C_CONFIG_H__
#define __I2C_CONFIG_H__

#include <rtthread.h>
#include "irq_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(BSP_USING_I2C1)
#ifndef I2C1_CONFIG
#define I2C1_CONFIG                                             \
    {                                                           \
        .name     = "i2c1",                                     \
        .Instance = CM_I2C1,                                    \
        .clock    = FCG1_PERIPH_I2C1,                           \
        .baudrate = 100000UL,                                   \
        .timeout  = 10000UL,                                    \
    }
#endif /* I2C1_CONFIG */
#endif

#if defined(BSP_USING_I2C2)
#ifndef I2C2_CONFIG
#define I2C2_CONFIG                                             \
    {                                                           \
        .name     = "i2c2",                                     \
        .Instance = CM_I2C2,                                    \
        .clock    = FCG1_PERIPH_I2C2,                           \
        .baudrate = 100000UL,                                   \
        .timeout  = 10000UL,                                    \
    }
#endif /* I2C2_CONFIG */
#endif

#if defined(BSP_USING_I2C3)
#ifndef I2C3_CONFIG
#define I2C3_CONFIG                                             \
    {                                                           \
        .name     = "i2c3",                                     \
        .Instance = CM_I2C3,                                    \
        .clock    = FCG1_PERIPH_I2C3,                           \
        .baudrate = 100000UL,                                   \
        .timeout  = 10000UL,                                    \
    }
#endif /* I2C3_CONFIG */
#endif

#if defined(BSP_USING_I2C4)
#ifndef I2C4_CONFIG
#define I2C4_CONFIG                                             \
    {                                                           \
        .name     = "i2c4",                                     \
        .Instance = CM_I2C4,                                    \
        .clock    = FCG1_PERIPH_I2C4,                           \
        .baudrate = 100000UL,                                   \
        .timeout  = 10000UL,                                    \
    }
#endif /* I2C4_CONFIG */
#endif

#if defined(BSP_USING_I2C5)
#ifndef I2C5_CONFIG
#define I2C5_CONFIG                                             \
    {                                                           \
        .name     = "i2c5",                                     \
        .Instance = CM_I2C5,                                    \
        .clock    = FCG1_PERIPH_I2C5,                           \
        .baudrate = 100000UL,                                   \
        .timeout  = 10000UL,                                    \
    }
#endif /* I2C5_CONFIG */
#endif

#if defined(BSP_USING_I2C6)
#ifndef I2C6_CONFIG
#define I2C6_CONFIG                                             \
    {                                                           \
        .name     = "i2c6",                                     \
        .Instance = CM_I2C6,                                    \
        .clock    = FCG1_PERIPH_I2C6,                           \
        .baudrate = 100000UL,                                   \
        .timeout  = 10000UL,                                    \
    }
#endif /* I2C6_CONFIG */
#endif

#ifdef __cplusplus
}
#endif

#endif
