/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-08     CDT          first version
 */

#include "board.h"

#ifdef BSP_USING_WDT

// #define DRV_DEBUG
#define LOG_TAG             "drv_wdt"
#include <drv_log.h>

#define CLK_DIV             (8192U)

enum {
    WDT_INIT_ING,
    WDT_INIT_OVER,
    WDT_IS_ENABLE
};

struct hc32_wdt_obj
{
    rt_watchdog_t watchdog;
    stc_wdt_init_t stcwdg;
    rt_uint32_t pclk3;
    rt_uint8_t sta;
};
static struct hc32_wdt_obj hc32_wdt;
static struct rt_watchdog_ops ops;

static rt_uint32_t wdt_get_timeout_s(void)
{
    rt_uint32_t CountPeriod = 0U;

    switch (hc32_wdt.stcwdg.u32CountPeriod)
    {
    case WDT_CNT_PERIOD256:
        CountPeriod = 256U;
        break;
    case WDT_CNT_PERIOD4096:
        CountPeriod = 4096U;
        break;
    case WDT_CNT_PERIOD16384:
        CountPeriod = 16384U;
        break;
    case WDT_CNT_PERIOD65536:
        CountPeriod = 65536U;
        break;
    default:
        break;
    }

    /* timeout(s) = PERI * DIV / PCLK3 */
    return ((rt_uint32_t)(CountPeriod * CLK_DIV / (float)hc32_wdt.pclk3));
}

static rt_uint32_t wdt_get_timeleft_s(void)
{
    /* wdt is down counter */
    return ((rt_uint32_t)(WDT_GetCountValue() * CLK_DIV / (float)hc32_wdt.pclk3));
}

static rt_err_t wdt_init(rt_watchdog_t *wdt)
{
    hc32_wdt.pclk3 = CLK_GetBusClockFreq(CLK_BUS_PCLK3);
    if (!hc32_wdt.pclk3) {
        LOG_E("pclk3 getbusclockfreq failed.");
        return -RT_ERROR;
    }
    hc32_wdt.stcwdg.u32CountPeriod   = WDT_CNT_PERIOD16384;
    hc32_wdt.stcwdg.u32ClockDiv      = WDT_CLK_DIV8192;
    hc32_wdt.stcwdg.u32RefreshRange  = WDT_RANGE_0TO100PCT;
    hc32_wdt.stcwdg.u32LPMCount      = WDT_LPM_CNT_CONTINUE;
    hc32_wdt.stcwdg.u32ExceptionType = WDT_EXP_TYPE_RST;
    hc32_wdt.sta = WDT_INIT_ING;
    /* WDT_CR register only support write once,so can't call WDT_Init of ther */
    return RT_EOK;
}

static rt_err_t wdt_control(rt_watchdog_t *wdt, int cmd, void *arg)
{
    switch (cmd)
    {
    /* feed the watchdog */
    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
        /* Prevention of unexpected start-up when feed dog */
        if (hc32_wdt.sta == WDT_IS_ENABLE) {
            WDT_FeedDog();
        }
        break;
    /* set watchdog timeout */
    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
        if ((*((rt_uint32_t *)arg)) > WDT_CNT_PERIOD65536) {
            LOG_E("wdg set timeout parameter too large, please less than %d", WDT_CNT_PERIOD65536);
            return -RT_EINVAL;
        }
        hc32_wdt.stcwdg.u32CountPeriod = (*((rt_uint32_t *)arg));
        if (WDT_Init(&hc32_wdt.stcwdg) != LL_OK) {
            LOG_E("wdg set timeout failed.");
            return -RT_ERROR;
        }
        hc32_wdt.sta = WDT_INIT_OVER;
        LOG_D("wdg set timeout successful. timeout = %d s", wdt_get_timeout_s());
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMEOUT:
        (*((rt_uint32_t *)arg)) = wdt_get_timeout_s();
        break;
    case RT_DEVICE_CTRL_WDT_START:
        if (hc32_wdt.sta == WDT_INIT_ING) {
            LOG_E("please set the timeout values.");
            return -RT_ERROR;
        }
        /* First reload counter to start WDT */
        WDT_FeedDog();
        hc32_wdt.sta = WDT_IS_ENABLE;
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMELEFT:
        (*((rt_uint32_t *)arg)) = wdt_get_timeleft_s();
        break;
    default:
        LOG_W("This command is not supported.");
        return -RT_ERROR;
    }
    return RT_EOK;
}

int rt_wdt_init(void)
{
    ops.init = &wdt_init;
    ops.control = &wdt_control;
    hc32_wdt.watchdog.ops = &ops;

    /* register watchdog device */
    if (rt_hw_watchdog_register(&hc32_wdt.watchdog, "wdt", RT_DEVICE_FLAG_DEACTIVATE, RT_NULL) != RT_EOK) {
        LOG_E("wdt device register failed.");
        return -RT_ERROR;
    }
    LOG_D("wdt device register success.");
    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_wdt_init);

#endif
