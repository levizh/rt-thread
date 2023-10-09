/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-09     CDT          first version
 */

#include "board.h"

/* unlock/lock peripheral */
#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
#define EXAMPLE_PERIPH_WP               (LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM)

#if defined(BSP_RTC_USING_XTAL32)
extern rt_err_t rt_hw_xtal32_board_init(void);
#endif

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
    stc_clock_xtal_init_t stcXtalInit;
    stc_clock_pll_init_t stcPLLHInit;
#if defined(BSP_RTC_USING_XTAL32)
    stc_clock_xtal32_init_t stcXtal32Init;
#endif

    /* PCLK0, HCLK Max 200MHz */
    /* PCLK1, PCLK4, EX BUS Max 100MHz */
    /* PCLK2 Max 60MHz */
    /* PCLK3 Max 50MHz */
    CLK_SetClockDiv(CLK_BUS_CLK_ALL,
                    (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 |
                     CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2 | CLK_EXCLK_DIV2 |
                     CLK_HCLK_DIV1));
    // todo, all chips
    //GPIO_AnalogCmd(BSP_XTAL_PORT, BSP_XTAL_IN_PIN | BSP_XTAL_OUT_PIN, ENABLE);
    
    (void)CLK_XtalStructInit(&stcXtalInit);
    /* Config Xtal and enable Xtal */
    stcXtalInit.u8Mode   = CLK_XTAL_MD_OSC;
    stcXtalInit.u8Drv    = CLK_XTAL_DRV_ULOW;
    stcXtalInit.u8State  = CLK_XTAL_ON;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);

    (void)CLK_PLLStructInit(&stcPLLHInit);
    /* VCO = (8/1)*100 = 800MHz*/
    stcPLLHInit.u8PLLState = CLK_PLL_ON;
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = 1UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLN = 100UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLP = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLQ = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLR = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
    (void)CLK_PLLInit(&stcPLLHInit);

    /* Highspeed SRAM, SRAM0 set to 0 Read/Write wait cycle */
    SRAM_SetWaitCycle((SRAM_SRAMH | SRAM_SRAM0), SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
    /* SRAMB set to 1 Read/Write wait cycle */
    SRAM_SetWaitCycle(SRAM_SRAMB, SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);
    /* 3 cycles for 150 ~ 200MHz */
    (void)EFM_SetWaitCycle(EFM_WAIT_CYCLE3);
    /* 3 cycles for 150 ~ 200MHz */
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT3);
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);

#if defined(BSP_RTC_USING_XTAL32)
    /* Xtal32 config */
    rt_hw_xtal32_board_init();
    (void)CLK_Xtal32StructInit(&stcXtal32Init);
    stcXtal32Init.u8State  = CLK_XTAL32_ON;
    stcXtal32Init.u8Drv    = CLK_XTAL32_DRV_HIGH;
    stcXtal32Init.u8Filter = CLK_XTAL32_FILTER_RUN_MD;
    (void)CLK_Xtal32Init(&stcXtal32Init);
#endif
}

/** Peripheral Clock Configuration
*/
void PeripheralClock_Config(void)
{
#if defined(HC32F448)
#if defined(RT_USING_ADC)
    CLK_SetPeriClockSrc(CLK_PERIPHCLK_PCLK);
#endif
#endif
}

/** Peripheral Registers Unlock
*/
void PeripheralRegister_Unlock(void)
{
    LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
}

/*@}*/
