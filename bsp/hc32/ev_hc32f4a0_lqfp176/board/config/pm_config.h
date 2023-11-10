/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-xx-xx     CDT          first version
 */

#ifndef __PM_CONFIG_H__
#define __PM_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BSP_USING_PM
extern void rt_hw_board_pm_sysclk_cfg(uint8_t run_mode);

#ifndef PM_RUN_MODE_CFG
#define PM_RUN_MODE_CFG                                                        \
    {                                                                          \
       .sys_clk_cfg = rt_hw_board_pm_sysclk_cfg                                \
    }
#endif /* PM_RUN_MODE_CFG */

#ifndef PM_SLEEP_IDLE_CFG
#define PM_SLEEP_IDLE_CFG                                                      \
{                                                                              \
    .wait_for_type = PM_SLEEP_WAIT_FOR_EVT,                                    \
}
#endif /*PM_SLEEP_IDLE_CFG*/

#ifndef PM_SLEEP_DEEP_CFG
#define PM_SLEEP_DEEP_CFG                                                      \
{                                                                              \
    {                                                                          \
        .u16Clock = PWC_STOP_CLK_KEEP,                                         \
        .u8StopDrv = PWC_STOP_DRV_HIGH,                                        \
        .u16ExBusHold = PWC_STOP_EXBUS_HIZ,                                    \
        .u16FlashWait = PWC_STOP_FLASH_WAIT_ON,                                \
    },                                                                         \
    .wait_for_type = PM_SLEEP_WAIT_FOR_EVT,                                    \
}
#endif /*PM_SLEEP_DEEP_CFG*/

#ifndef PM_SLEEP_STANDBY_CFG
#define PM_SLEEP_STANDBY_CFG                                                   \
{                                                                              \
    {                                                                          \
        .u8Mode = PWC_PD_MD1,                                                  \
        .u8IOState = PWC_PD_IO_KEEP1,                                          \
        .u8VcapCtrl = PWC_PD_VCAP_0P047UF,                                     \
    },                                                                         \
}
#endif /*PM_SLEEP_STANDBY_CFG*/

#ifndef PM_SLEEP_SHUTDOWN_CFG
#define PM_SLEEP_SHUTDOWN_CFG                                                  \
{                                                                              \
    {                                                                          \
        .u8Mode = PWC_PD_MD3,                                                  \
        .u8IOState = PWC_PD_IO_KEEP1,                                          \
        .u8VcapCtrl = PWC_PD_VCAP_0P047UF,                                     \
    },                                                                         \
}
#endif /*PM_SLEEP_SHUTDOWN_CFG*/

#endif /* BSP_USING_PM */

#ifdef __cplusplus
}
#endif

#endif /* __PM_CONFIG_H__ */
