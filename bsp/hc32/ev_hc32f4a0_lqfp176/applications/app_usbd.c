/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-15     CDT          first version
 */



#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USB_DEVICE_CDC
/*
 * 程序清单：这是一个 usb device 设备使用例程
 * 例程导出了 cdc_sample 命令到控制终端
 * 命令调用格式：cdc_sample
 * 程序功能：首先会打印三次str_write字符串内容，同时虚拟串口可输入发送任意小于255个字符的字符串，
 * 发送内容可在Finsh串口显示。
*/

#define USBD_DEV_NAME   "vcom"     /* 名称 */
rt_uint8_t str_read[255];

static rt_err_t cdc_rx_handle(rt_device_t dev, rt_size_t size)
{
    /* 读取定时器当前值 */
    rt_device_read(dev, 0, str_read, size);
    rt_kprintf("Read message:  %s\n", str_read);
}

static int cdc_sample(void)
{
    rt_uint8_t i;
    rt_err_t ret = RT_EOK;
    rt_device_t cdc_dev = RT_NULL;                           /* usb device设备句柄 */
    rt_uint8_t str_write[] = "This is a usb cdc device test!\r\n";


    /* 查找定时器设备 */
    cdc_dev = rt_device_find(USBD_DEV_NAME);
    if (cdc_dev == RT_NULL)
    {
        rt_kprintf("cdc sample run failed! can't find %s device!\n", USBD_DEV_NAME);
        return RT_ERROR;
    }

    /* 以读写方式打开设备 */
    ret = rt_device_open(cdc_dev, RT_DEVICE_FLAG_INT_RX);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", USBD_DEV_NAME);
        return ret;
    }
    rt_device_set_rx_indicate(cdc_dev, cdc_rx_handle);

    for (i = 1; i < 4; i++)
    {
        rt_kprintf("Start to send test message 3 timers :%d.\n", i);
        if (rt_device_write(cdc_dev, 0, str_write, sizeof(str_write)) != sizeof(str_write)) {
            rt_kprintf("send test message failed\n");
            return RT_ERROR;
        }
        /* 延时1000ms */
        rt_thread_mdelay(1000);
    }

   // for (;;);
    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(cdc_sample, usbd cdc sample);
#endif


#ifdef RT_USB_DEVICE_HID
/*
 * 程序清单：这是一个 usb hid device 设备使用例程
 * 例程导出了 hid_sample 命令到控制终端
 * 命令调用格式：hid_sample
 * 程序功能：首先会打印str_write[0]字符串内容，然后按下按键WKUP，hid设备将发送按键keyn，n是按下次数，可
 * 通过bus hound查看数据。
 * 发送内容可在Finsh串口显示。
*/

#define USBD_DEV_NAME   "hidd"     /* 名称 */
#define KEY_PIN_NUM     0          /* PA0 */

static int hid_sample(void)
{
    rt_err_t ret = RT_EOK;
    rt_device_t hid_dev = RT_NULL;                           /* usb device设备句柄 */
    char str_write[2][5] = {"test", "Key0"};


    /* 查找定时器设备 */
    hid_dev = rt_device_find(USBD_DEV_NAME);
    if (hid_dev == RT_NULL)
    {
        rt_kprintf("hid sample run failed! can't find %s device!\n", USBD_DEV_NAME);
        return RT_ERROR;
    }

    /* 以收中断方式打开设备 */
    ret = rt_device_open(hid_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", USBD_DEV_NAME);
        return ret;
    }

    rt_kprintf("This is a usb hid device test!\r\n");
    rt_device_write(hid_dev, 0, str_write[0], sizeof(str_write[0]));
    rt_pin_mode(KEY_PIN_NUM, PIN_MODE_INPUT);

    for (;;)
    {
        rt_thread_mdelay(200);
        if (PIN_LOW == rt_pin_read(KEY_PIN_NUM))
        {
            rt_device_write(hid_dev, 0, str_write[1], sizeof(str_write[1]));
            str_write[1][3] += 0x01;
        }
    }

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(hid_sample, usbd hid sample);
#endif


#ifdef RT_USB_DEVICE_MSTORAGE

/* 注意：only FS can used with spi flash */
/* spi1 请使能DMA tx 和DMA rx. */
/* msc class disk name 请命名为w25q64 */
#endif


#ifdef RT_USB_DEVICE_WINUSB
/*
 * 程序清单：这是一个 usb WINUSB device 设备使用例程
 * 例程导出了 winusb_sample 命令到控制终端
 * 命令调用格式：winusb_sample
 * 软件：Bus Hound, WinUSB_Test.exe, 串口助手。
 * 程序功能：MSH命令发送winusb_sample，运行测试程序。打开bus hound软件，选择RTT WINUSB。
 * 通过WinUSB_Test.exe可发送bulk数据（100字符以内）到设备，设备收到后会回发给主机，通过bus hound可以查看。
 * 注意：1、WinUSB_Test.exe中的GUID与驱动程序中设定保持一致；
 *      2、win_usb_read（）函数中的UIO_REQUEST_READ_FULL改为UIO_REQUEST_READ_BEST，实现数据即读即取。
 * 
*/
#define WINUSB_DEV_NAME   "winusb"     /* 名称 */
uint8_t str_read[64];

static rt_err_t winusb_rx_handle(rt_device_t dev, rt_size_t size)
{
    uint8_t i;
    /* 读取定时器当前值 */
    rt_kprintf("Rx:");
    for (i = 0; i < size; i++)
    {
        rt_kprintf("%x", str_read[i]);
    }
    rt_kprintf("\r\n");
    rt_device_write(dev, 0, str_read, sizeof(str_read));
    /* prepare read config */
    rt_device_read(dev, 0, str_read, sizeof(str_read));
		return RT_EOK;
}

static int winusb_sample(void)
{
    rt_err_t ret = RT_EOK;
    rt_device_t winusb_dev = RT_NULL;                       /* usb device设备句柄 */

    /* 查找设备 */
    winusb_dev = rt_device_find(WINUSB_DEV_NAME);
    if (winusb_dev == RT_NULL)
    {
        rt_kprintf("winusb sample run failed! can't find %s device!\n", WINUSB_DEV_NAME);
        return RT_ERROR;
    }

    /* 以读写方式打开设备 */
    ret = rt_device_open(winusb_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", WINUSB_DEV_NAME);
        return ret;
    }
    rt_kprintf("Found and open success %s device!\n", WINUSB_DEV_NAME);

    ret = rt_device_set_rx_indicate(winusb_dev, winusb_rx_handle);
		if(ret == RT_EOK)
		{
        /* prepare read config,set once,read once, */
        rt_device_read(winusb_dev, 0, str_read, sizeof(str_read));
		}
		return ret;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(winusb_sample, usbd winusb sample);
#endif
