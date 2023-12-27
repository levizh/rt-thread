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
#include "board_config.h"

/* unlock/lock peripheral */
#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
#define EXAMPLE_PERIPH_WP               (LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM)

/** System Base Configuration
*/
void SystemBase_Config(void)
{
#if defined(BSP_USING_ON_CHIP_FLASH_ICODE_CACHE)
    EFM_ICacheCmd(ENABLE);
#endif
#if defined(BSP_USING_ON_CHIP_FLASH_DCODE_CACHE)
    EFM_DCacheCmd(ENABLE);
#endif
#if defined(BSP_USING_ON_CHIP_FLASH_ICODE_PREFETCH)
    EFM_PrefetchCmd(ENABLE);
#endif
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
    stc_clock_xtal_init_t stcXtalInit;
    stc_clock_pll_init_t stcPLLHInit;
#if defined(BSP_RTC_USING_XTAL32) || defined(RT_USING_PM)
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

    GPIO_AnalogCmd(XTAL_PORT, XTAL_IN_PIN | XTAL_OUT_PIN, ENABLE);
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

#if defined(BSP_RTC_USING_XTAL32) || defined(RT_USING_PM)
    /* Xtal32 config */
    GPIO_AnalogCmd(XTAL32_PORT, XTAL32_IN_PIN | XTAL32_OUT_PIN, ENABLE);
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
#if defined(BSP_USING_CAN1)
    CLK_SetCANClockSrc(CLK_CAN1, CLK_CANCLK_SYSCLK_DIV6);
#endif
#if defined(BSP_USING_CAN2)
    CLK_SetCANClockSrc(CLK_CAN2, CLK_CANCLK_SYSCLK_DIV6);
#endif

#if defined(RT_USING_ADC)
    CLK_SetPeriClockSrc(CLK_PERIPHCLK_PCLK);
#endif
}

/** Peripheral Registers Unlock
*/
void PeripheralRegister_Unlock(void)
{
    LL_PERIPH_WE(EXAMPLE_PERIPH_WE);
}

//todo, add into DDL
#define INTEN_REG_MOD           (32U)
/**
 * @brief  Get interrupt source interrupt status
 * @param  [in] enIntSrc: Interrupt source, can be any value @ref en_int_src_t
 * @retval An @ref en_functional_state_t enumeration type value.
 */
en_functional_state_t INTC_GetIntSrcStatus(en_int_src_t enIntSrc)
{
    __IO uint32_t *INTENx;

    /* Parameter validity checking */
    DDL_ASSERT(enIntSrc <= INT_SRC_MAX);

    INTENx = (__IO uint32_t *)((uint32_t)&CM_INTC->INTEN0 + 4UL * ((uint32_t)enIntSrc / INTEN_REG_MOD));

    return ((*INTENx & (1UL << ((uint32_t)enIntSrc & 0x1FU))) != 0UL) ? ENABLE : DISABLE;
}

/**
 * @defgroup USART_LIN_Function_Mask USART LIN Function Mask
 * @{
 */
#define USART_LIN_FUNC_OFFSET           (16U)

#define USART_LIN_FUNC_MASK             (USART_LIN              |              \
                                         USART_LIN_ERR          |              \
                                         USART_LIN_WKUP         |              \
                                         USART_LIN_INT_WKUP     |              \
                                         USART_LIN_INT_BREAK    |              \
                                         USART_LIN_INT_ERR)

/**
 * @brief  Get USART Transmit/Receive Function.
 * @param  [in] USARTx                  Pointer to USART instance register base
 *         This parameter can be one of the following values:
 *           @arg CM_USARTx:            USART unit instance register base
 * @param  [in] u32Func                 USART function type
 *         This parameter can be any composed value of the macros group @ref USART_Function.
  * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t USART_GetFunc(CM_USART_TypeDef *USARTx, uint32_t u32Func)
{
    uint32_t u32BaseFunc;
    uint32_t u32LinFunc;
    en_flag_status_t enRet;

//    DDL_ASSERT(IS_USART_UNIT(USARTx));
//    DDL_ASSERT(IS_USART_FUNC(u32Func));
//    DDL_ASSERT(IS_USART_LIN_FUNC(USARTx, u32Func));
//    DDL_ASSERT(IS_USART_TIMEOUT_FUNC(USARTx, u32Func));

    u32BaseFunc = (u32Func & 0xFFFFUL);
    if (u32BaseFunc > 0UL) {    
        enRet = (0UL == (READ_REG32_BIT(USARTx->CR1, u32BaseFunc)) ? RESET : SET);
    }
    u32LinFunc = ((u32Func & USART_LIN_FUNC_MASK) >> USART_LIN_FUNC_OFFSET);
    if (u32LinFunc > 0UL) {
        enRet |= (0UL == (READ_REG32_BIT(USARTx->CR2, u32LinFunc)) ? RESET : SET);
    }
    return enRet;
}

/*@}*/
