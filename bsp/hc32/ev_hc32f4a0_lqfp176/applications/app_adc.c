/*
 * 程序清单： ADC 设备使用例程
 * 例程导出了 adc_sample 命令到控制终端
 * 命令调用格式：adc_sample
 * 程序功能：通过 ADC 设备采样电压值并转换为数值。
 *           示例代码参考电压为3.3V,转换位数为12位。
*/

#include <rtthread.h>
#include <rtdevice.h>
#include "board_config.h"
#include "drv_adc.h"

#define REFER_VOLTAGE       330         /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */

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
        } else if (0 == rt_strcmp(argv[1], "adc3")) {
            rt_strcpy(adc_device, "adc3");
            adc_max_channel = 20;
        }
    }

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(adc_device);
    if (adc_dev == RT_NULL) {
        rt_kprintf("adc sample run failed! can't find %s device!\n", adc_device);
        return RT_ERROR;
    }

    /* 遍历所有通道 */
    for (adc_channel = 0; adc_channel < adc_max_channel; adc_channel++) {
        /* 使能设备 */
        rt_adc_enable(adc_dev, adc_channel);

        /* 读取采样值 */
        value = rt_adc_read(adc_dev, adc_channel);
        rt_kprintf("Channel: %d, value is :%d   %x\n", adc_channel, value, value);

        /* 转换为对应电压值 */
        vol = value * REFER_VOLTAGE / CONVERT_BITS;
        rt_kprintf("Simulate voltage is :%d.%02d \n", vol / 100, vol % 100);

        vol =  rt_adc_voltage(adc_dev, adc_channel);
        rt_kprintf("Read voltage is :%d \n", vol);
        rt_kprintf("*********************\n\n");
    }
    rt_kprintf("*******The %s all channel have be tested**********\n", adc_device);
    return RT_EOK;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(adc_vol_sample, adc convert sample: select < adc1 | adc2 | adc3 >);
