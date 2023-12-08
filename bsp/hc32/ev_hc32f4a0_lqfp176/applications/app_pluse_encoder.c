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
 * 程序清单： Pulse encoder 设备使用例程, 请在图形化配置界面打开pulse encoder device，
 * 并使能tmr6_1和tmra_1.
 * 例程导出了 pulse_encoder_sample 命令到控制终端, 通过串口可查看当前的count数值和转速。
 * 命令调用格式：pulse_encoder_sample    参数选择：select < tmr6 | tmra >
 * 编码器的分辨率是1000. tmr6_1的硬件IO：A,B相分别接PB9,PB8。tmra_1:PA8,PA9
 * 程序功能：
*/
#include <rtthread.h>
#include <rtdevice.h>
#include "board_config.h"

#ifdef BSP_USING_PULSE_ENCODER
#define TMR_TICK    2000                  /* 定时2s */
rt_device_t pulse_encoder_dev;            /* 设备句柄 */
rt_int32_t count, rpm;
rt_uint8_t i = 0;

/* 定时器超时函数 */
static void timeout(void *parameter)
{
    rt_device_read(pulse_encoder_dev, 0, &count, 1);
    rpm = count / (TMR_TICK / 1000) * 6 / 100;

    rt_kprintf("count %d: %d\n", i, count);
    rt_kprintf("rpm: %d\n", rpm);

    if (RT_EOK != rt_device_control(pulse_encoder_dev, PULSE_ENCODER_CMD_CLEAR_COUNT, &count))
    {
        rt_kprintf("Can't clear counts!\n");
        return;
    }
    i++;
}

static int pulse_encoder_sample(int argc, char **argv)
{
    rt_err_t ret = RT_EOK;
    char tmr6_device[] = "pulse_61";
    char tmra_device[] = "pulse_a1";
    char *pulse_encoder_name = tmr6_device;
    static rt_timer_t timer;

    /* 参数无输入或者输入错误按照默认值处理 */
    if (argc == 2)
    {
        if (0 == rt_strcmp(argv[1], "tmr6"))
        {
            pulse_encoder_name = tmr6_device;
        } else if (0 == rt_strcmp(argv[1], "tmra"))
        {
            pulse_encoder_name = tmra_device;
        }
    }

    rt_kprintf("%s select!\n", pulse_encoder_name);
    /* 查找设备 */
    pulse_encoder_dev = rt_device_find(pulse_encoder_name);
    if (pulse_encoder_dev == RT_NULL)
    {
        rt_kprintf("pulse_encoder sample run failed! can't find %s device!\n", pulse_encoder_name);
        return RT_ERROR;
    }

    if (RT_EOK != rt_device_control(pulse_encoder_dev, PULSE_ENCODER_CMD_CLEAR_COUNT, &count))
    {
        rt_kprintf("Can't clear counts!\n");
        return RT_ERROR;
    }

    ret = rt_device_open(pulse_encoder_dev, RT_DEVICE_OFLAG_RDONLY);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", pulse_encoder_name);
        return ret;
    }

    /* 创建1s的周期性定时器 */
    timer = rt_timer_create("timer", timeout,
                            RT_NULL, TMR_TICK,
                            RT_TIMER_FLAG_PERIODIC);

    /* 启动定时器 */
    if (timer != RT_NULL) rt_timer_start(timer);

    while (i < 30);
		i = 0;

    rt_timer_stop(timer);
    if (RT_EOK != rt_device_control(pulse_encoder_dev, PULSE_ENCODER_CMD_DISABLE, &count))
    {
        rt_kprintf("Can't disable device!\n");
        return RT_ERROR;
    }
    rt_device_close(pulse_encoder_dev);
    return RT_EOK;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(pulse_encoder_sample, pulse encoder sample: select < tmr6 | tmra >);
#endif
