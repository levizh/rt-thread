/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-09     CDT          first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rtdef.h>
#include "board_config.h"
#include "rtconfig.h"

#if defined(BSP_USING_WDT_TIM)
#include "drv_wdt.h"

/* macros define */
#if defined(BSP_USING_WDT)
#define SAMPLE_WDT_NAME     "wdt"
#elif defined(BSP_USING_SWDT)
#define SAMPLE_WDT_NAME     "swdt"
#endif
#define WDT_FED_CNT         (10)

/* variable define */
static rt_device_t wdt_dev;
static char wdt_name[RT_NAME_MAX];

/**
 * @brief function to feed watchdog at mcu free time
 */
static void idle_hook(void)
{
    rt_uint32_t wdt_timeout;
    static rt_uint32_t wdt_feed_cnt = 0;

    /* when wdt fed over 10 times, let wdt timeout and reset system */
    if (WDT_FED_CNT <= wdt_feed_cnt)
    {
        rt_thread_idle_delhook(idle_hook);
        /* stop watch dog */
        if(RT_EOK != rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_STOP, RT_NULL))
        {
            rt_kprintf("ctrl: stop %s failed, command is under supporting, waiting reboot\n", wdt_name);
        }
        else
        {
            rt_kprintf("%s stopped\n", wdt_name);
        }
        return;
    }
    /* Using blocking delay to avoid feed dog */
    DDL_DelayMS(1000);
    /* get watchdog left time before feeding */
    if(RT_EOK != rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_GET_TIMELEFT, &wdt_timeout))
    {
        rt_kprintf("get %s left time failed\n", wdt_name);
        return;
    }
    else
    {
        rt_kprintf("%s left time is %ds \n", wdt_name, wdt_timeout);
    }
    /* Feed watch dog */
    if(RT_EOK != rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, RT_NULL))
    {
        rt_kprintf("feed %s failed\n", wdt_name);
        return;
    }
    else
    {
        ++wdt_feed_cnt;
        rt_kprintf("[%2u] %s fed\n", wdt_feed_cnt, wdt_name);
    }
}

/**
 * @brief function to receive command
 *
 * @param argc input parameter number
 * @param argv array to store the input strings
 */
static int wdt_sample(int argc, char *argv[])
{
    rt_uint32_t wdt_timeout = 6U;

    if (argc == 2)
    {
        rt_strncpy(wdt_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(wdt_name, SAMPLE_WDT_NAME, RT_NAME_MAX);
    }

    wdt_dev = rt_device_find(wdt_name);
    if(!wdt_dev)
    {
        rt_kprintf("find %s failed\n", wdt_name);
        return RT_ERROR;
    }

    if(RT_EOK != rt_device_init(wdt_dev))
    {
        rt_kprintf("initial %s failed\n", wdt_name);
        return RT_ERROR;
    }

    rt_kprintf("%s test begin...\n", SAMPLE_WDT_NAME);

    if(RT_EOK != rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &wdt_timeout))
    {
        rt_kprintf("ctrl: set timeout %s failed\n", wdt_name);
        return RT_ERROR;
    }

    if(RT_EOK != rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL))
    {
        rt_kprintf("ctrl: start %s failed\n", wdt_name);
        return RT_ERROR;
    }
    /* Check if feeding option is effective */
    if(RT_EOK != rt_device_control(wdt_dev, RT_DEVICE_CTRL_WDT_GET_TIMEOUT, &wdt_timeout))
    {
        rt_kprintf("ctrl: get timeout %s failed\n", wdt_name);
        return RT_ERROR;
    }
    else
    {
        rt_kprintf("%s left time is %ds\n", wdt_name, wdt_timeout);
    }

    rt_thread_idle_sethook(idle_hook);

    return RT_EOK;
}

/* export to function msh command list */
MSH_CMD_EXPORT(wdt_sample, wdt/swdt device sample);

#endif /* BSP_USING_WDT */
/*
 EOF
*/
