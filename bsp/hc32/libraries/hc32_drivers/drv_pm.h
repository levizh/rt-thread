/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-xx-xx     CDT          first version
 */


#ifndef __DRV_PM_H__
#define __DRV_PM_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <drv_config.h>

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef void (* sleep_mode_init_func)(void);
typedef void (* run_mode_init_func)(uint8_t run_mode);

struct pm_run_mode_config
{
    run_mode_init_func sys_clk_cfg;
};

struct pm_sleep_mode_deep_config
{
    sleep_mode_init_func init_func;
    stc_pwc_stop_mode_config_t cfg;
    uint8_t wait_for_wakeup_type;
    uint32_t wakeup_int_src;
};

struct pm_sleep_mode_shutdown_config
{
    sleep_mode_init_func init_func;
    stc_pwc_pd_mode_config_t cfg;
};
/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define PM_CHECK_EFM()                  (EFM_GetStatus(EFM_FLAG_RDY) == RESET)
#if defined(HC32F4A0)
#define PM_CHECK_XTAL()                 (CM_CMU->XTALSTDCR & CLK_XTALSTD_ON != CLK_XTALSTD_ON)
#define PM_CHECK_DMA()                                              \
(                                       (DMA_GetTransStatus(CM_DMA1, DMA_STAT_TRANS_DMA) == RESET) && \
                                        (DMA_GetTransStatus(CM_DMA2, DMA_STAT_TRANS_DMA) == RESET))
#define PM_CHECK_SWDT()                                             \
(                                       ((CM_ICG->ICG0 & ICG_SWDT_RST_START) != ICG_SWDT_RST_START) || \
                                        ((CM_ICG->ICG0 & ICG_SWDT_LPM_CNT_STOP) == ICG_SWDT_LPM_CNT_STOP))
#define PM_CHECK_PVD()                  (CM_PWC->PVDCR1 & (PWC_PVDCR1_PVD1IRS | PWC_PVDCR1_PVD2IRS) != 0)
#endif
#define PM_SLEEP_SHUTDOWN_CHECK()                                   \
(                                       PM_CHECK_EFM() &&           \
                                        PM_CHECK_XTAL() &&          \
                                        PM_CHECK_SWDT() &&          \
                                        PM_CHECK_PVD())
#define PM_SLEEP_DEEP_CHECK()                                       \
(                                       PM_CHECK_EFM() &&           \
                                        PM_CHECK_XTAL() &&          \
                                        PM_CHECK_DMA())
/*
* please make sure the state of the peripherals meet the requirements of entering the specified sleep mode,
* otherwise system may not entering the right sleep mode or something unexpected may happen.
* PM_SLEEP_CHECK is a demo of requirements and may not be comprehensive,
* please refer user manual to known all the requirements in detail.
*/
#define PM_SLEEP_CHECK(mode)                                        \
(                                       (mode ==  PM_SLEEP_MODE_SHUTDOWN && PM_SLEEP_SHUTDOWN_CHECK()) || \
                                        (mode ==  PM_SLEEP_MODE_DEEP && PM_SLEEP_DEEP_CHECK())|| \
                                        (mode !=  PM_SLEEP_MODE_SHUTDOWN)|| \
                                        (mode !=  PM_SLEEP_MODE_DEEP))

#define PM_SLEEP_WAIT_TYPE_WFI          (PWC_STOP_WFI)
#define PM_SLEEP_WAIT_TYPE_WFE_INT      (PWC_STOP_WFE_INT)
#define PM_SLEEP_WAIT_TYPE_WFE_EVT      (PWC_STOP_WFE_EVT)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
static const struct pm_run_mode_config st_run_mode_cfg = PM_RUN_MODE_CFG;
/*
* sleep_mode_init_func is called before entering sleep mode,
* it can be used to implement operations such as making the peripherals
* meet the requirements shown in PM_SLEEP_CHECK,and some other init operations
*/
static const struct pm_sleep_mode_deep_config st_sleep_deep_cfg = PM_SLEEP_DEEP_CFG;
static const struct pm_sleep_mode_shutdown_config st_sleep_shutdown_cfg = PM_SLEEP_SHUTDOWN_CFG;

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* __DRV_PM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
