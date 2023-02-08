/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author               Notes
 * 2022-04-28     CDT                  first version
 * 2022-06-10     xiaoxiaolisunny      modify
 */

#include <board.h>
#include <rtdbg.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <sys/time.h>

#ifdef BSP_USING_RTC

//#define DRV_DEBUG
#define LOG_TAG             "drv.rtc"
#include <drv_log.h>

static rt_rtc_dev_t rtc_dev;

static rt_err_t hc32_rtc_get_timeval(struct timeval *tv)
{

    stc_rtc_time_t stcRtcTime = {0};
    stc_rtc_date_t stcRtcDate = {0};
    struct tm tm_new = {0};

    if (LL_OK != RTC_GetTime(RTC_DATA_FMT_DEC, &stcRtcTime))
    {
        return -RT_ERROR;
    }
    if (LL_OK != RTC_GetDate(RTC_DATA_FMT_DEC, &stcRtcDate))
    {
        return -RT_ERROR;
    }

    tm_new.tm_sec  = stcRtcTime.u8Second;
    tm_new.tm_min  = stcRtcTime.u8Minute;
    tm_new.tm_hour = stcRtcTime.u8Hour;
    tm_new.tm_mday = stcRtcDate.u8Day;
    tm_new.tm_mon  = stcRtcDate.u8Month - 1;
    tm_new.tm_year = stcRtcDate.u8Year + 100;

    tv->tv_sec = timegm(&tm_new);

    return RT_EOK;
}

static rt_err_t hc32_rtc_set_time_stamp(time_t time_stamp)
{
    stc_rtc_time_t stcRtcTime = {0};
    stc_rtc_date_t stcRtcDate = {0};
    struct tm tm = {0};

    gmtime_r(&time_stamp, &tm);

    if (tm.tm_year < 100)
    {
        return -RT_ERROR;
    }

    stcRtcTime.u8Second  = tm.tm_sec ;
    stcRtcTime.u8Minute  = tm.tm_min ;
    stcRtcTime.u8Hour    = tm.tm_hour;
    stcRtcDate.u8Day     = tm.tm_mday;
    stcRtcDate.u8Month   = tm.tm_mon + 1;
    stcRtcDate.u8Year    = tm.tm_year - 100;
    stcRtcDate.u8Weekday = tm.tm_wday;

    if (LL_OK != RTC_SetTime(RTC_DATA_FMT_DEC, &stcRtcTime))
    {
        return -RT_ERROR;
    }
    if (LL_OK != RTC_SetDate(RTC_DATA_FMT_DEC, &stcRtcDate))
    {
        return -RT_ERROR;
    }

    LOG_D("set rtc time.");
    return RT_EOK;
}

static rt_err_t hc32_rtc_init(void)
{
    stc_rtc_init_t stcRtcInit;

#ifdef BSP_RTC_USING_XTAL32
    stc_clock_xtal32_init_t stcXtal32Init;
    /* Xtal32 config */
    //GPIO_AnalogCmd(BSP_XTAL32_PORT, BSP_XTAL32_IN_PIN | BSP_XTAL32_OUT_PIN, ENABLE);
    (void)CLK_Xtal32StructInit(&stcXtal32Init);
    stcXtal32Init.u8State  = CLK_XTAL32_ON;
    stcXtal32Init.u8Drv    = CLK_XTAL32_DRV_HIGH;
    stcXtal32Init.u8Filter = CLK_XTAL32_FILTER_RUN_MD;
    (void)CLK_Xtal32Init(&stcXtal32Init);
    /* Waiting for XTAL32 stabilization */
    rt_thread_delay(1000);
#endif

    /* RTC stopped */
    if (DISABLE == RTC_GetCounterState())
    {
        /* Reset RTC counter */
        if (LL_ERR_TIMEOUT == RTC_DeInit())
        {
            return -RT_ERROR;
        }
        else
        {
            /* Configure structure initialization */
            (void)RTC_StructInit(&stcRtcInit);
            /* Configuration RTC structure */
#ifdef BSP_RTC_USING_XTAL32
            stcRtcInit.u8ClockSrc = RTC_CLK_SRC_XTAL32;
#else
            stcRtcInit.u8ClockSrc = RTC_CLK_SRC_LRC;
#endif
            stcRtcInit.u8HourFormat  = RTC_HOUR_FMT_24H;
            (void)RTC_Init(&stcRtcInit);
            /* Startup RTC count */
            RTC_Cmd(ENABLE);
        }
    }
    return RT_EOK;
}

static rt_err_t hc32_rtc_get_secs(time_t *sec)
{
    struct timeval tv;

    hc32_rtc_get_timeval(&tv);
    *(time_t *) sec = tv.tv_sec;
    LOG_D("RTC: get rtc_time %d", *sec);

    return RT_EOK;
}

static rt_err_t hc32_rtc_set_secs(time_t *sec)
{
    rt_err_t result = RT_EOK;

    if (hc32_rtc_set_time_stamp(*sec))
    {
        result = -RT_ERROR;
    }
    LOG_D("RTC: set rtc_time %d", *sec);

    return result;
}

const static struct rt_rtc_ops hc32_rtc_ops =
{
    hc32_rtc_init,
    hc32_rtc_get_secs,
    hc32_rtc_set_secs,
    RT_NULL,
    RT_NULL,
    hc32_rtc_get_timeval,
    RT_NULL
};

int rt_hw_rtc_init(void)
{
    rt_err_t result;

    rtc_dev.ops = &hc32_rtc_ops;
    result = rt_hw_rtc_register(&rtc_dev, "rtc", RT_DEVICE_FLAG_RDWR, RT_NULL);
    if (result != RT_EOK)
    {
        LOG_E("rtc register err code: %d", result);
        return result;
    }
    LOG_D("rtc init success");
    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_rtc_init);

#endif /* BSP_USING_RTC */
