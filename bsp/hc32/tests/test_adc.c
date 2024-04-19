/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022-2024, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-10     CDT          first version
 */


/*
 * 程序清单： ADC 设备使用例程
 * 例程导出了 adc_sample 命令到控制终端
 * 命令调用格式：adc_sample    参数选择：adc1 | adc2 | adc3
 * 程序功能：通过 ADC 设备采样电压值并转换为数值。
 *           示例代码参考电压为3.3V,转换位数为12位。
*/
#include <rtthread.h>
#include <rtdevice.h>
#include "board_config.h"
#include "drv_adc.h"

#define REFER_VOLTAGE       3300        /* 参考电压 3.3V,单位mv */
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */

#ifdef RT_USING_ADC
/* Timer的配置需与文件 “adc_config.h”中的 ADC1_EOCA_DMA_CONFIG 对应 */
/* 这里使用Timer01 A作为ADC1的触发源 */
rt_err_t adc_dma_trig_config(void)
{
    FCG_Fcg2PeriphClockCmd(PWC_FCG2_TMR0_1, ENABLE);
 
    stc_tmr0_init_t stcTmr0Init;
    (void)TMR0_StructInit(&stcTmr0Init);
    (void)TMR0_Init(CM_TMR0_1, TMR0_CH_A, &stcTmr0Init);
    return 0;
}

rt_err_t adc_dma_trig_start(void)
{
    TMR0_SetCountValue(CM_TMR0_1, TMR0_CH_A, 0x0);
    TMR0_SetCompareValue(CM_TMR0_1, TMR0_CH_A, 0x7FFF);
    TMR0_Start(CM_TMR0_1, TMR0_CH_A);
    return 0;
}

rt_err_t adc_dma_trig_stop(void)
{
    TMR0_Stop(CM_TMR0_1, TMR0_CH_A);
    TMR0_ClearStatus(CM_TMR0_1, TMR0_FLAG_CMP_A);
    return 0;
}

struct adc_dev_priv_params adc_priv;
struct adc_dev_dma_priv_ops priv_ops;

static int adc_vol_sample(int argc, char **argv)
{
    rt_adc_device_t adc_dev;            /* ADC 设备句柄 */
    rt_uint32_t value;
    rt_uint32_t vol;
    rt_uint8_t adc_channel;
    char adc_device[] = "adc1";
    rt_uint8_t adc_max_channel = 16;

    /* 参数无输入或者输入错误按照默认值处理 */
    if (argc == 2) {
        if (0 == rt_strcmp(argv[1], "adc2")) {
            rt_strcpy(adc_device, "adc2");
            adc_max_channel = 8;
        } else if (0 == rt_strcmp(argv[1], "adc3")) {
            rt_strcpy(adc_device, "adc3");
            adc_max_channel = 12;
        }
    }

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(adc_device);
    if (adc_dev == RT_NULL) {
        rt_kprintf("adc sample run failed! can't find %s device!\n", adc_device);
        return RT_ERROR;
    }
    adc_priv.flag = ADC_USING_EOCA_DMA_FLAG;
    
    priv_ops.dma_trig_config = &adc_dma_trig_config;
    priv_ops.dma_trig_start = &adc_dma_trig_start;
    priv_ops.dma_trig_stop = &adc_dma_trig_stop;
    adc_priv.ops = &priv_ops;
    
    adc_dev->parent.user_data = &adc_priv;
    /* 遍历所有通道 */
    for (adc_channel = 0; adc_channel < adc_max_channel; adc_channel++) {
        /* 使能设备 */
        rt_adc_enable(adc_dev, adc_channel);

        /* 读取采样值 */
        value = rt_adc_read(adc_dev, adc_channel);
        rt_kprintf("Channel: %d, value is :%d   0x%x\n", adc_channel, value, value);

        /* 转换为对应电压值 */
        vol = value * REFER_VOLTAGE / CONVERT_BITS;
        rt_kprintf("Simulate voltage is :%d mv\n", vol);

        vol =  rt_adc_voltage(adc_dev, adc_channel);
        rt_kprintf("Read voltage is :%d mv\n", vol);
        rt_kprintf("*********************\n\n");
    }
    rt_kprintf("*******The %s all channel have be tested**********\n", adc_device);
    return RT_EOK;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(adc_vol_sample, adc convert sample: select < adc1 | adc2 | adc3 >);
#endif
