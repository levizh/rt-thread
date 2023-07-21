/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#ifndef __DRV_I2C_H__
#define __DRV_I2C_H__

#include <rtdevice.h>
#include "board_config.h"

#ifdef BSP_USING_I2C

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
struct hc32_i2c_config
{
    const char      *name;
    CM_I2C_TypeDef  *Instance;
    rt_uint32_t     clock;
    rt_uint32_t     baudrate;
    rt_uint32_t     timeout;
};

struct hc32_i2c
{
    struct hc32_i2c_config      *config;
    struct rt_i2c_bus_device    bus;
};

// int hc32_hw_i2c_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_USING_I2C */

#endif /* __DRV_I2C_H__ */
