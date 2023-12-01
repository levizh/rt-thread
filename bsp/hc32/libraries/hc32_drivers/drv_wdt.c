/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-08     CDT          first version
 * 2023-12-01     CDT          added swdt support
 */

#include "board.h"

#ifdef BSP_USING_WDT_TIM

// #define DRV_DEBUG
#define LOG_TAG             "drv_wdt"
#include <drv_log.h>

enum {
    WDT_INIT_ING,
    WDT_INIT_OVER,
    WDT_IS_ENABLE
};
static struct rt_watchdog_ops ops;

#ifdef BSP_USING_WDT
struct hc32_wdt_obj
{
    rt_watchdog_t watchdog;
    stc_wdt_init_t stcwdg;
    rt_uint32_t pclk3;
    rt_uint32_t clk_div;
    rt_uint8_t sta;
};
static struct hc32_wdt_obj hc32_wdt;

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
    return ((rt_uint32_t)(CountPeriod * hc32_wdt.clk_div / (float)hc32_wdt.pclk3));
}

static rt_uint32_t wdt_get_timeleft_s(void)
{
    /* wdt is down counter */
    return ((rt_uint32_t)(WDT_GetCountValue() * hc32_wdt.clk_div / (float)hc32_wdt.pclk3));
}

static rt_uint32_t wdt_get_div(rt_uint32_t clk_div)
{
    rt_uint32_t div = 0;
    switch (clk_div)
    {
    case 4U:
        div = WDT_CLK_DIV4;
        break;
    case 64U:
        div = WDT_CLK_DIV64;
        break;
    case 128U:
        div = WDT_CLK_DIV128;
        break;
    case 256U:
        div = WDT_CLK_DIV256;
        break;
    case 512U:
        div = WDT_CLK_DIV512;
        break;
    case 1024U:
        div = WDT_CLK_DIV1024;
        break;
    case 2048U:
        div = WDT_CLK_DIV2048;
        break;
    case 8192U:
        div = WDT_CLK_DIV8192;
        break;

    default:
        break;
    }

    return div;
}

static rt_err_t wdt_init(rt_watchdog_t *wdt)
{
    hc32_wdt.pclk3 = CLK_GetBusClockFreq(CLK_BUS_PCLK3);
    if (!hc32_wdt.pclk3) {
        LOG_E("pclk3 getbusclockfreq failed.");
        return -RT_ERROR;
    }
    hc32_wdt.stcwdg.u32CountPeriod   = WDT_CNT_PERIOD16384;
    hc32_wdt.stcwdg.u32ClockDiv      = wdt_get_div(BSP_WDT_DIV);
    if (!hc32_wdt.stcwdg.u32ClockDiv) {
        LOG_E("clock div %d is Invalid value.", BSP_WDT_DIV);
        return -RT_ERROR;
    }
    hc32_wdt.clk_div = BSP_WDT_DIV;
    hc32_wdt.stcwdg.u32RefreshRange  = (BSP_WDT_REFRESH_RANGE << WDT_CR_WDPT_POS);
#ifdef BSP_WDT_CONTINUE_COUNT
    hc32_wdt.stcwdg.u32LPMCount      = WDT_LPM_CNT_CONTINUE;
#else
    hc32_wdt.stcwdg.u32LPMCount      = WDT_LPM_CNT_STOP;
#endif
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

#else   /* BSP_USING_WDT */

struct hc32_swdt_obj
{
    rt_watchdog_t watchdog;
    stc_swdt_init_t stcwdg;
    rt_uint32_t swdtclk;
    rt_uint32_t clk_div;
    rt_uint8_t sta;
};
static struct hc32_swdt_obj hc32_swdt;

static rt_uint32_t swdt_get_timeout_s(void)
{
    rt_uint32_t CountPeriod = 0U;

    switch (hc32_swdt.stcwdg.u32CountPeriod)
    {
    case SWDT_CNT_PERIOD256:
        CountPeriod = 256U;
        break;
    case SWDT_CNT_PERIOD4096:
        CountPeriod = 4096U;
        break;
    case SWDT_CNT_PERIOD16384:
        CountPeriod = 16384U;
        break;
    case SWDT_CNT_PERIOD65536:
        CountPeriod = 65536U;
        break;
    default:
        break;
    }

    /* timeout(s) = PERI * DIV / SWDTCLK */
    return ((rt_uint32_t)(CountPeriod * hc32_swdt.clk_div / (float)hc32_swdt.swdtclk));
}

static rt_uint32_t swdt_get_timeleft_s(void)
{
    /* swdt is down counter */
    return ((rt_uint32_t)(SWDT_GetCountValue() * hc32_swdt.clk_div / (float)hc32_swdt.swdtclk));
}

static rt_uint32_t swdt_get_div(rt_uint32_t clk_div)
{
    rt_uint32_t div = 0;
    switch (clk_div)
    {
    case 1U:
        div = SWDT_CLK_DIV1;
        break;
    case 16U:
        div = SWDT_CLK_DIV16;
        break;
    case 32U:
        div = SWDT_CLK_DIV32;
        break;
    case 64U:
        div = SWDT_CLK_DIV64;
        break;
    case 128U:
        div = SWDT_CLK_DIV128;
        break;
    case 256U:
        div = SWDT_CLK_DIV256;
        break;
    case 2048U:
        div = SWDT_CLK_DIV2048;
        break;

    default:
        break;
    }

    return div;
}

static rt_err_t swdt_init(rt_watchdog_t *swdt)
{
    hc32_swdt.swdtclk = 10000U;
    hc32_swdt.stcwdg.u32CountPeriod   = SWDT_CNT_PERIOD16384;
    hc32_swdt.stcwdg.u32ClockDiv      = swdt_get_div(BSP_SWDT_DIV);
    if (!hc32_swdt.stcwdg.u32ClockDiv) {
        LOG_E("clock div %d is Invalid value.", BSP_SWDT_DIV);
        return -RT_ERROR;
    }
    hc32_swdt.clk_div = BSP_SWDT_DIV;
    hc32_swdt.stcwdg.u32RefreshRange  = (BSP_WDT_REFRESH_RANGE << SWDT_CR_WDPT_POS);
#ifdef BSP_swdt_CONTINUE_COUNT
    hc32_swdt.stcwdg.u32LPMCount      = SWDT_LPM_CNT_CONTINUE;
#else
    hc32_swdt.stcwdg.u32LPMCount      = SWDT_LPM_CNT_STOP;
#endif
    hc32_swdt.stcwdg.u32ExceptionType = SWDT_EXP_TYPE_RST;
    hc32_swdt.sta = WDT_INIT_ING;
    /* SWDT_CR register only support write once,so can't call swdt_Init of ther */

    return RT_EOK;
}

static rt_err_t swdt_control(rt_watchdog_t *swdt, int cmd, void *arg)
{
    switch (cmd)
    {
    /* feed the watchdog */
    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
        /* Prevention of unexpected start-up when feed dog */
        if (hc32_swdt.sta == WDT_IS_ENABLE) {
            SWDT_FeedDog();
        }
        break;
    /* set watchdog timeout */
    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
        if ((*((rt_uint32_t *)arg)) > SWDT_CNT_PERIOD65536) {
            LOG_E("swdg set timeout parameter too large, please less than %d", SWDT_CNT_PERIOD65536);
            return -RT_EINVAL;
        }
        hc32_swdt.stcwdg.u32CountPeriod = (*((rt_uint32_t *)arg));
        if (SWDT_Init(&hc32_swdt.stcwdg) != LL_OK) {
            LOG_E("swdg set timeout failed.");
            return -RT_ERROR;
        }
        hc32_swdt.sta = WDT_INIT_OVER;
        LOG_D("swdg set timeout successful. timeout = %d s", swdt_get_timeout_s());
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMEOUT:
        (*((rt_uint32_t *)arg)) = swdt_get_timeout_s();
        break;
    case RT_DEVICE_CTRL_WDT_START:
        if (hc32_swdt.sta == WDT_INIT_ING) {
            LOG_E("please set the timeout values.");
            return -RT_ERROR;
        }
        /* First reload counter to start swdt */
        SWDT_FeedDog();
        hc32_swdt.sta = WDT_IS_ENABLE;
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMELEFT:
        (*((rt_uint32_t *)arg)) = swdt_get_timeleft_s();
        break;
    default:
        LOG_W("This command is not supported.");
        return -RT_ERROR;
    }
    return RT_EOK;
}

int rt_swdt_init(void)
{
    ops.init = &swdt_init;
    ops.control = &swdt_control;
    hc32_swdt.watchdog.ops = &ops;

    /* register watchdog device */
    if (rt_hw_watchdog_register(&hc32_swdt.watchdog, "swdt", RT_DEVICE_FLAG_DEACTIVATE, RT_NULL) != RT_EOK) {
        LOG_E("swdt device register failed.");
        return -RT_ERROR;
    }
    LOG_D("swdt device register success.");
    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_swdt_init);

#endif  /* BSP_USING_SWDT */

#endif
