/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#include <rtthread.h>
#include <rthw.h>
#include "drv_i2c.h"
#include "board_config.h"

#ifdef RT_USING_I2C

#if defined(BSP_USING_I2C1) || defined(BSP_USING_I2C2) || defined(BSP_USING_I2C3) || \
    defined(BSP_USING_I2C4) || defined(BSP_USING_I2C5) || defined(BSP_USING_I2C6)

#ifndef HC32_I2C_DEBUG
    #define I2C_PRINT_DBG(fmt, args...)
    #define I2C_PRINT_ERR(fmt, args...) rt_kprintf(fmt, ##args);
#else
    #define I2C_PRINT_DBG(fmt, args...) rt_kprintf(fmt, ##args);
    #define I2C_PRINT_ERR(fmt, args...) rt_kprintf(fmt, ##args);
#endif

#define I2C_TIMEOUT                 ((uint32_t)0x10000)
#define FCG_I2C_CLK                 FCG_Fcg1PeriphClockCmd

extern rt_err_t rt_hw_board_i2c_init(CM_I2C_TypeDef *I2Cx);

static struct hc32_i2c_config i2c_config[] =
{
#ifdef BSP_USING_I2C1
    I2C1_CONFIG,
#endif
#ifdef BSP_USING_I2C2
    I2C2_CONFIG,
#endif
#ifdef BSP_USING_I2C3
    I2C3_CONFIG,
#endif
#ifdef BSP_USING_I2C4
    I2C4_CONFIG,
#endif
#ifdef BSP_USING_I2C5
    I2C5_CONFIG,
#endif
#ifdef BSP_USING_I2C6
    I2C6_CONFIG,
#endif
};

static struct hc32_i2c i2c_obj[sizeof(i2c_config) / sizeof(i2c_config[0])] = {0};

static rt_err_t hc32_i2c_configure(struct hc32_i2c *i2c_obj)
{
    int ret = RT_ERROR;
    stc_i2c_init_t i2c_init;
    float32_t f32Error = 0.0F;
    RT_ASSERT(RT_NULL != i2c_obj);

    /* Enable I2C clock */
    FCG_I2C_CLK(i2c_obj->config->clock, ENABLE);
    if (RT_EOK != rt_hw_board_i2c_init(i2c_obj->config->Instance))
    {
        return -RT_ERROR;
    }
    I2C_DeInit(i2c_obj->config->Instance);

    i2c_init.u32ClockDiv = I2C_CLK_DIV128;
    i2c_init.u32SclTime = 0UL;
    i2c_init.u32Baudrate = i2c_obj->config->baudrate;

    for (rt_uint32_t i = 0U; i < 8U; i++)
    {
        ret = I2C_Init(i2c_obj->config->Instance, &i2c_init, &f32Error);
        if (RT_EOK != ret)
        {
            i2c_init.u32ClockDiv--;
        }
        else
        {
            break;
        }
    }

    if (RT_EOK == ret)
    {
        I2C_BusWaitCmd(i2c_obj->config->Instance, ENABLE);
        I2C_Cmd(i2c_obj->config->Instance, ENABLE);
    }
    return ret;
}

static void hc32_hw_i2c_reset(struct hc32_i2c *i2c_obj)
{
    I2C_SWResetCmd(i2c_obj->config->Instance, ENABLE);
    I2C_SWResetCmd(i2c_obj->config->Instance, DISABLE);
}

static int hc32_hw_i2c_start(struct hc32_i2c *i2c_obj)
{
    if (LL_OK != I2C_Start(i2c_obj->config->Instance, i2c_obj->config->timeout))
    {
        return RT_ERROR;
    }

    return RT_EOK;
}

static int hc32_hw_i2c_restart(struct hc32_i2c *i2c_obj)
{
    if (LL_OK != I2C_Restart(i2c_obj->config->Instance, i2c_obj->config->timeout))
    {
        return RT_ERROR;
    }

    return RT_EOK;
}

static int hc32_hw_i2c_send_addr(struct hc32_i2c *i2c_obj,
                                 struct rt_i2c_msg *msg)
{
    rt_uint8_t dir = (msg->flags & RT_I2C_RD == RT_I2C_RD) ? (I2C_DIR_RX) : (I2C_DIR_TX);
    if (LL_OK != I2C_TransAddr(i2c_obj->config->Instance, msg->addr, dir, i2c_obj->config->timeout))
    {
        return RT_ERROR;
    }
    return RT_EOK;
}

static int hc32_hw_i2c_stop(struct hc32_i2c *i2c_obj)
{
    if (LL_OK != I2C_Stop(i2c_obj->config->Instance, i2c_obj->config->timeout))
    {
        return RT_ERROR;
    }
    return RT_EOK;
}

static int hc32_i2c_write(struct hc32_i2c *i2c_obj,
                          struct rt_i2c_msg *msg)
{
    int ret;

    ret = I2C_TransData(i2c_obj->config->Instance, msg->buf, msg->len, i2c_obj->config->timeout);

    return ret;
}

static int hc32_i2c_read(struct hc32_i2c *i2c_obj,
                         struct rt_i2c_msg *msg)
{
    int ret;

    if (msg->len == 1UL)
    {
        I2C_AckConfig(i2c_obj->config->Instance, I2C_NACK);
    }
    ret = I2C_ReceiveData(i2c_obj->config->Instance, msg->buf, msg->len, i2c_obj->config->timeout);

    return ret;
}

static rt_size_t hc32_i2c_master_xfer(struct rt_i2c_bus_device *bus,
                                      struct rt_i2c_msg msgs[],
                                      rt_uint32_t num)
{
    rt_int32_t i, ret;
    rt_uint16_t ignore_nack;
    struct rt_i2c_msg *msg = msgs;
    struct hc32_i2c *i2c_obj;

    RT_ASSERT((msgs != RT_NULL) && (bus != RT_NULL));

    i2c_obj = rt_container_of(bus, struct hc32_i2c, bus);

    if (num == 0)
    {
        return 0;
    }

    hc32_hw_i2c_reset(i2c_obj);
    for (i = 0; i < num; i++)
    {
        msg = &msgs[i];
        ignore_nack = msg->flags & RT_I2C_IGNORE_NACK;
        if (!(msg->flags & RT_I2C_NO_START))
        {
            if (i)
            {
                hc32_hw_i2c_restart(i2c_obj);
            }
            else
            {
                hc32_hw_i2c_start(i2c_obj);
            }
            /* addr R or W */
            ret = hc32_hw_i2c_send_addr(i2c_obj, msg);
            if ((ret != RT_EOK) && !ignore_nack)
            {
                I2C_PRINT_DBG("receive NACK from device addr 0x%02x msg %d", msgs[i].addr, i);
                goto out;
            }
        }
        if (msg->flags & RT_I2C_RD)
        {
            ret = hc32_i2c_read(i2c_obj, msg);
            if (ret != RT_EOK)
            {
                I2C_PRINT_ERR("[%s:%d]I2C Read error!\n", __func__, __LINE__);
                goto out;
            }
        }
        else
        {
            ret = hc32_i2c_write(i2c_obj, msg);
            if (ret != RT_EOK)
            {
                I2C_PRINT_ERR("[%s:%d]I2C Write error!\n", __func__, __LINE__);
                goto out;
            }
        }
    }
    ret = i;

out:
    if (!(msg->flags & RT_I2C_NO_STOP))
    {
        hc32_hw_i2c_stop(i2c_obj);
        I2C_PRINT_DBG("send stop condition\n");
    }
    return ret;
}

static const struct rt_i2c_bus_device_ops hc32_i2c_ops =
{
    .master_xfer = hc32_i2c_master_xfer,
    RT_NULL,
    RT_NULL
};

int hc32_hw_i2c_init(void)
{
    int ret = RT_ERROR;
    rt_size_t obj_num = sizeof(i2c_obj) / sizeof(struct hc32_i2c);
    I2C_PRINT_DBG("%s start\n", __func__);

    for (int i = 0; i < obj_num; i++)
    {
        i2c_obj[i].bus.ops = &hc32_i2c_ops;
        i2c_obj[i].config = &i2c_config[i];
        i2c_obj[i].bus.timeout = i2c_config[i].timeout;
        hc32_i2c_configure(&i2c_obj[i]);
        ret = rt_i2c_bus_device_register(&i2c_obj[i].bus, i2c_obj[i].config->name);
    }
    I2C_PRINT_DBG("%s end\n", __func__);

    return ret;
}

INIT_BOARD_EXPORT(hc32_hw_i2c_init);

#endif

#endif /* RT_USING_I2C */
