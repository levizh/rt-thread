/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-10     CDT          first version
 */


/*
 * 程序清单： FAL使用例程
 * 例程导出了 fal_sample 命令到控制终端
 * 命令调用格式：fal_sample
 * 程序功能：
*/
#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_FAL

#include "board_config.h"
#include <fal.h>

#define Partition_Name   "app"
#define Buff_Size        1024
#define TEST_OFF         0x50000
#define TEST_CNT         32

static int fal_sample(int argc, char **argv)
{
    const struct fal_partition *param;
    int ret;
    uint8_t write_buffer[Buff_Size] = {0};
    uint8_t read_buffer[Buff_Size] = {0};
    uint32_t Address;
    uint8_t errFlag = 0;

    fal_init(); //抽象层初始化
    /* Set write buffer, clear read buffer */
    for (uint16_t index = 0; index < Buff_Size; index++)
    {
        write_buffer[index] = index;
        read_buffer[index] = 0;
    }

    param = fal_partition_find(Partition_Name);

    if (param == RT_NULL)
    {
        rt_kprintf("not find partition app!\r\n");
        return -1;
    }
    for (int j = 0; j < TEST_CNT; j++)
    {
        errFlag = 0;
        Address = TEST_OFF + j * Buff_Size;
        rt_kprintf("........test %d address %x........\r\n", j + 1, Address);
        /* erase process */
        ret = fal_partition_erase(param, Address, Buff_Size);
        if (ret >= 0)
        {
            rt_kprintf("Erase succeeded!\r\n");
        }
        else
        {
            rt_kprintf("Erase failed!\r\n");
            return ret;
        }

        /* write process */
        ret = fal_partition_write(param, Address, write_buffer, sizeof(write_buffer));
        if (ret >= 0)
        {
            rt_kprintf("Write succeeded!\r\n");
        }
        else
        {
            rt_kprintf("Write failed!\r\n");
            return ret;
        }
        /* read out and compare */
        ret = fal_partition_read(param, Address, read_buffer, Buff_Size);
        if (ret >= 0)
        {
            rt_kprintf("Read succeeded!\r\n");
        }
        else
        {
            rt_kprintf("Read failed!\r\n");
            return ret;
        }

        for (int i = 0; i < Buff_Size; i++)
        {
            if (read_buffer[i] != write_buffer[i])
            {
                rt_kprintf("Data verification failed:\r\n");
                rt_kprintf("NUM: %d  Write: %x    Read: %x \r\n", i, write_buffer[i], read_buffer[i]);
                errFlag = 1;
                ret = -1;
            }
        }
        if (0 == errFlag)
        {
            rt_kprintf("Data verification OK!\r\n");
        }
    }

    return ret;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(fal_sample, fal sample);
#endif
