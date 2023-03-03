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
extern void rt_hw_board_pm_sleep_deep_init(void);
extern void rt_hw_board_pm_sleep_shutdown_init(void);

#ifndef PM_RUN_MODE_CFG
#define PM_RUN_MODE_CFG                                                        \
    {                                                                          \
       .sys_clk_cfg = rt_hw_board_pm_sysclk_cfg                                \
    }
#endif /* PM_RUN_MODE_CFG */

#ifndef PM_SLEEP_MODE_CFG
#define PM_SLEEP_MODE_CFG                                                      \
{                                                                              \
    .init_func =                                                               \
     {                                                                         \
       RT_NULL,                                                                \
       RT_NULL,                                                                \
       RT_NULL,                                                                \
       rt_hw_board_pm_sleep_deep_init,                                         \
       RT_NULL,                                                                \
       rt_hw_board_pm_sleep_shutdown_init,                                     \
    } ,                                                                        \
    .deep_mode_wait_type = PM_SLEEP_WAIT_TYPE_WFI,                             \
}
#endif

#ifndef PM_SLEEP_DEEP_CFG
#define PM_SLEEP_DEEP_CFG_CLK           PWC_STOP_CLK_KEEP
#define PM_SLEEP_DEEP_CFG                                                      \
{                                                                              \
    .init_func = rt_hw_board_pm_sleep_deep_init,                               \
    .cfg =                                                                     \
    {                                                                          \
        PM_SLEEP_DEEP_CFG_CLK,                                                 \
        PWC_STOP_DRV_LOW,                                                      \
        PWC_STOP_EXBUS_HIZ,                                                    \
        PWC_STOP_FLASH_WAIT_ON,                                                \
    },                                                                         \
    .wait_for_wakeup_type = PM_SLEEP_WAIT_TYPE_WFI,                            \
    .wakeup_int_src = (INTC_STOP_WKUP_EXTINT_CH0 | INTC_STOP_WKUP_WKTM),       \
}
#endif /*PM_SLEEP_DEEP_CFG*/

#ifndef PM_SLEEP_SHUTDOWN_CFG
#define PM_SLEEP_SHUTDOWN_CFG                                                  \
{                                                                              \
    .init_func = rt_hw_board_pm_sleep_shutdown_init,                           \
    .cfg =                                                                     \
    {                                                                          \
        PWC_PD_MD3,                                                            \
        PWC_PD_IO_KEEP1,                                                       \
        PWC_PD_VCAP_0P047UF,                                                   \
    },                                                                         \
}
#endif /*PM_SLEEP_SHUTDOWN_CFG*/

#endif /* BSP_USING_PM */

#ifdef __cplusplus
}
#endif

#endif /* __PM_CONFIG_H__ */