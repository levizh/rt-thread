/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#include <rtdevice.h>
#include "board_config.h"
#include "tca9539.h"

/**
 * The below functions will initialize HC32 board.
 */

#if defined RT_USING_SERIAL
rt_err_t rt_hw_board_uart_init(CM_USART_TypeDef *USARTx)
{
    rt_err_t result = RT_EOK;

    switch ((rt_uint32_t)USARTx)
    {
#if defined(BSP_USING_UART1)
    case (rt_uint32_t)CM_USART1:
        /* Configure USART RX/TX pin. */
        GPIO_SetFunc(USART1_RX_PORT, USART1_RX_PIN, GPIO_FUNC_33);
        GPIO_SetFunc(USART1_TX_PORT, USART1_TX_PIN, GPIO_FUNC_32);
        break;
#endif
#if defined(BSP_USING_UART6)
    case (rt_uint32_t)CM_USART6:
        /* Configure USART RX/TX pin. */
        GPIO_SetFunc(USART6_RX_PORT, USART6_RX_PIN, GPIO_FUNC_37);
        GPIO_SetFunc(USART6_TX_PORT, USART6_TX_PIN, GPIO_FUNC_36);
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined(RT_USING_ADC)
rt_err_t rt_hw_board_adc_init(CM_ADC_TypeDef *ADCx)
{
    rt_err_t result = RT_EOK;
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    switch ((rt_uint32_t)ADCx)
    {
#if defined(BSP_USING_ADC1)
    case (rt_uint32_t)CM_ADC1:
        (void)GPIO_Init(ADC1_CH_PORT, ADC1_CH_PIN, &stcGpioInit);
        break;
#endif
#if defined(BSP_USING_ADC2)
    case (rt_uint32_t)CM_ADC2:
        (void)GPIO_Init(ADC2_CH_PORT, ADC2_CH_PIN, &stcGpioInit);
        break;
#endif
#if defined(BSP_USING_ADC3)
    case (rt_uint32_t)CM_ADC3:
        (void)GPIO_Init(ADC3_CH_PORT, ADC3_CH_PIN, &stcGpioInit);
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined(RT_USING_DAC)
rt_err_t rt_hw_board_dac_init(CM_DAC_TypeDef *DACx)
{
    rt_err_t result = RT_EOK;
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    switch ((rt_uint32_t)DACx)
    {
#if defined(BSP_USING_DAC1)
    case (rt_uint32_t)CM_DAC1:
        (void)GPIO_Init(DAC1_CH1_PORT, DAC1_CH1_PIN, &stcGpioInit);
        (void)GPIO_Init(DAC1_CH2_PORT, DAC1_CH2_PIN, &stcGpioInit);
        break;
#endif
#if defined(BSP_USING_DAC2)
    case (rt_uint32_t)CM_DAC2:
        (void)GPIO_Init(DAC2_CH1_PORT, DAC2_CH1_PIN, &stcGpioInit);
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined(RT_USING_CAN)
void CanPhyEnable(void)
{
    TCA9539_WritePin(CAN_STB_PORT, CAN_STB_PIN, TCA9539_PIN_RESET);
    TCA9539_ConfigPin(CAN_STB_PORT, CAN_STB_PIN, TCA9539_DIR_OUT);
}
rt_err_t rt_hw_board_can_init(CM_CAN_TypeDef *CANx)
{
    rt_err_t result = RT_EOK;

    switch ((rt_uint32_t)CANx)
    {
#if defined(BSP_USING_CAN1)
    case (rt_uint32_t)CM_CAN1:
        GPIO_SetFunc(CAN1_TX_PORT, CAN1_TX_PIN, CAN1_TX_PIN_FUNC);
        GPIO_SetFunc(CAN1_RX_PORT, CAN1_RX_PIN, CAN1_RX_PIN_FUNC);
        break;
#endif
#if defined(BSP_USING_CAN2)
    case (rt_uint32_t)CM_CAN2:
        GPIO_SetFunc(CAN2_TX_PORT, CAN2_TX_PIN, CAN2_TX_PIN_FUNC);
        GPIO_SetFunc(CAN2_RX_PORT, CAN2_RX_PIN, CAN2_RX_PIN_FUNC);
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif


#if defined (RT_USING_SPI)
rt_err_t rt_hw_spi_board_init(CM_SPI_TypeDef *CM_SPIx)
{
    rt_err_t result = RT_EOK;

    switch ((rt_uint32_t)CM_SPIx)
    {
#if defined(BSP_USING_SPI1)
    case (rt_uint32_t)CM_SPI1:
        GPIO_SetFunc(GPIO_PORT_C, GPIO_PIN_06, GPIO_FUNC_40);   /* SCK  */
        GPIO_SetFunc(GPIO_PORT_B, GPIO_PIN_13, GPIO_FUNC_41);   /* MOSI */
        GPIO_SetFunc(GPIO_PORT_B, GPIO_PIN_12, GPIO_FUNC_42);   /* MISO */
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined(BSP_USING_ETH)
/* PHY hardware reset time */
#define PHY_HW_RST_DELAY                (0x40U)

rt_err_t rt_hw_eth_phy_reset(CM_ETH_TypeDef *CM_ETHx)
{
    TCA9539_ConfigPin(TCA9539_IO_PORT1, EIO_ETH_RST, TCA9539_DIR_OUT);
    TCA9539_WritePin(TCA9539_IO_PORT1,  EIO_ETH_RST, TCA9539_PIN_RESET);
    rt_thread_mdelay(PHY_HW_RST_DELAY);
    TCA9539_WritePin(TCA9539_IO_PORT1,  EIO_ETH_RST, TCA9539_PIN_SET);
    rt_thread_mdelay(PHY_HW_RST_DELAY);
    return RT_EOK;
}

rt_err_t rt_hw_eth_board_init(CM_ETH_TypeDef *CM_ETHx)
{
#if defined(ETH_USING_INTERFACE_RMII)
    /*
        ETH_SMI_MDIO ----------------> PA2
        ETH_SMI_MDC -----------------> PC1
        ETH_RMII_TX_EN --------------> PG11
        ETH_RMII_TXD0 ---------------> PG13
        ETH_RMII_TXD1 ---------------> PG14
        ETH_RMII_REF_CLK ------------> PA1
        ETH_RMII_CRS_DV -------------> PA7
        ETH_RMII_RXD0 ---------------> PC4
        ETH_RMII_RXD1 ---------------> PC5
        ETH_RMII_RX_ER --------------> PI10
    */
    /* Configure PA1, PA2 and PA7 */
    GPIO_SetFunc(GPIO_PORT_A, (GPIO_PIN_01 | GPIO_PIN_02 | GPIO_PIN_07), GPIO_FUNC_11);
    /* Configure PC1, PC4 and PC5 */
    GPIO_SetFunc(GPIO_PORT_C, (GPIO_PIN_01 | GPIO_PIN_04 | GPIO_PIN_05), GPIO_FUNC_11);
    /* Configure PG11, PG13 and PG14 */
    GPIO_SetFunc(GPIO_PORT_G, (GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14), GPIO_FUNC_11);
    /* Configure PI10 */
    GPIO_SetFunc(GPIO_PORT_I, GPIO_PIN_10, GPIO_FUNC_11);
#else
    /*
        ETH_SMI_MDIO ----------------> PA2
        ETH_SMI_MDC -----------------> PC1
        ETH_MII_TX_CLK --------------> PB6
        ETH_MII_TX_EN ---------------> PG11
        ETH_MII_TXD0 ----------------> PG13
        ETH_MII_TXD1 ----------------> PG14
        ETH_MII_TXD2 ----------------> PB9
        ETH_MII_TXD3 ----------------> PB8
        ETH_MII_RX_CLK --------------> PA1
        ETH_MII_RX_DV ---------------> PA7
        ETH_MII_RXD0 ----------------> PC4
        ETH_MII_RXD1 ----------------> PC5
        ETH_MII_RXD2 ----------------> PB0
        ETH_MII_RXD3 ----------------> PB1
        ETH_MII_RX_ER ---------------> PI10
        ETH_MII_CRS -----------------> PH2
        ETH_MII_COL -----------------> PH3
    */
    /* Configure PA1, PA2 and PA7 */
    GPIO_SetFunc(GPIO_PORT_A, (GPIO_PIN_01 | GPIO_PIN_02 | GPIO_PIN_07), GPIO_FUNC_11);
    /* Configure PB0, PB1, PB6, PB8 and PB9 */
    GPIO_SetFunc(GPIO_PORT_B, (GPIO_PIN_00 | GPIO_PIN_01 | GPIO_PIN_06 | GPIO_PIN_08 | GPIO_PIN_09), GPIO_FUNC_11);
    /* Configure PC1, PC4 and PC5 */
    GPIO_SetFunc(GPIO_PORT_C, (GPIO_PIN_01 | GPIO_PIN_04 | GPIO_PIN_05), GPIO_FUNC_11);
    /* Configure PG11, PG13 and PG14 */
    GPIO_SetFunc(GPIO_PORT_G, (GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14), GPIO_FUNC_11);
    /* Configure PH2, PH3 */
    GPIO_SetFunc(GPIO_PORT_H, (GPIO_PIN_02 | GPIO_PIN_03), GPIO_FUNC_11);
    /* Configure PI10 */
    GPIO_SetFunc(GPIO_PORT_I, GPIO_PIN_10, GPIO_FUNC_11);
#endif
    return RT_EOK;
}
#endif

#if defined (RT_USING_RTC)
rt_err_t rt_hw_xtal32_board_init(void)
{
#if defined(BSP_RTC_USING_XTAL32)
    GPIO_AnalogCmd(XTAL32_PORT, XTAL32_IN_PIN | XTAL32_OUT_PIN, ENABLE);
#endif
    return RT_EOK;
}
#endif

#if defined (RT_USING_SDIO)
rt_err_t rt_hw_board_sdio_init(CM_SDIOC_TypeDef *SDIOCx)
{
    rt_err_t result = RT_EOK;
    stc_gpio_init_t stcGpioInit;

    switch ((rt_uint32_t)SDIOCx)
    {
#if defined(BSP_USING_SDIO1)
    case (rt_uint32_t)CM_SDIOC1:
        /************************* Set pin drive capacity *************************/
        (void)GPIO_StructInit(&stcGpioInit);
        stcGpioInit.u16PinDrv = PIN_HIGH_DRV;
        (void)GPIO_Init(SDIOC1_CK_PORT,  SDIOC1_CK_PIN,  &stcGpioInit);
        (void)GPIO_Init(SDIOC1_CMD_PORT, SDIOC1_CMD_PIN, &stcGpioInit);
        (void)GPIO_Init(SDIOC1_D0_PORT,  SDIOC1_D0_PIN,  &stcGpioInit);
        (void)GPIO_Init(SDIOC1_D1_PORT,  SDIOC1_D1_PIN,  &stcGpioInit);
        (void)GPIO_Init(SDIOC1_D2_PORT,  SDIOC1_D2_PIN,  &stcGpioInit);
        (void)GPIO_Init(SDIOC1_D3_PORT,  SDIOC1_D3_PIN,  &stcGpioInit);

        GPIO_SetFunc(SDIOC1_CK_PORT,  SDIOC1_CK_PIN,  GPIO_FUNC_9);
        GPIO_SetFunc(SDIOC1_CMD_PORT, SDIOC1_CMD_PIN, GPIO_FUNC_9);
        GPIO_SetFunc(SDIOC1_D0_PORT,  SDIOC1_D0_PIN,  GPIO_FUNC_9);
        GPIO_SetFunc(SDIOC1_D1_PORT,  SDIOC1_D1_PIN,  GPIO_FUNC_9);
        GPIO_SetFunc(SDIOC1_D2_PORT,  SDIOC1_D2_PIN,  GPIO_FUNC_9);
        GPIO_SetFunc(SDIOC1_D3_PORT,  SDIOC1_D3_PIN,  GPIO_FUNC_9);
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined(RT_USING_PWM)
#if defined(BSP_USING_PWM_TIMA)
rt_err_t rt_hw_board_pwm_tmra_init(CM_TMRA_TypeDef *TMRAx)
{
    rt_err_t result = RT_EOK;
    switch ((rt_uint32_t)TMRAx)
    {
#if defined(BSP_USING_PWM_TIMA_1)
    case (rt_uint32_t)CM_TMRA_1:
    #ifdef BSP_USING_PWM_TIMA_1_CH1
        GPIO_SetFunc(PWM_TIMA_1_CH1_PORT, PWM_TIMA_1_CH1_PIN, PWM_TIMA_1_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH2
        GPIO_SetFunc(PWM_TIMA_1_CH2_PORT, PWM_TIMA_1_CH2_PIN, PWM_TIMA_1_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH3
        GPIO_SetFunc(PWM_TIMA_1_CH3_PORT, PWM_TIMA_1_CH3_PIN, PWM_TIMA_1_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_1_CH4
        GPIO_SetFunc(PWM_TIMA_1_CH4_PORT, PWM_TIMA_1_CH4_PIN, PWM_TIMA_1_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_2)
    case (rt_uint32_t)CM_TMRA_2:
    #ifdef BSP_USING_PWM_TIMA_2_CH1
        GPIO_SetFunc(PWM_TIMA_2_CH1_PORT, PWM_TIMA_2_CH1_PIN, PWM_TIMA_2_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH2
        GPIO_SetFunc(PWM_TIMA_2_CH2_PORT, PWM_TIMA_2_CH2_PIN, PWM_TIMA_2_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH3
        GPIO_SetFunc(PWM_TIMA_2_CH3_PORT, PWM_TIMA_2_CH3_PIN, PWM_TIMA_2_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_2_CH4
        GPIO_SetFunc(PWM_TIMA_2_CH4_PORT, PWM_TIMA_2_CH4_PIN, PWM_TIMA_2_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_3)
    case (rt_uint32_t)CM_TMRA_3:
    #ifdef BSP_USING_PWM_TIMA_3_CH1
        GPIO_SetFunc(PWM_TIMA_3_CH1_PORT, PWM_TIMA_3_CH1_PIN, PWM_TIMA_3_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH2
        GPIO_SetFunc(PWM_TIMA_3_CH2_PORT, PWM_TIMA_3_CH2_PIN, PWM_TIMA_3_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH3
        GPIO_SetFunc(PWM_TIMA_3_CH3_PORT, PWM_TIMA_3_CH3_PIN, PWM_TIMA_3_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_3_CH4
        GPIO_SetFunc(PWM_TIMA_3_CH4_PORT, PWM_TIMA_3_CH4_PIN, PWM_TIMA_3_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_4)
    case (rt_uint32_t)CM_TMRA_4:
    #ifdef BSP_USING_PWM_TIMA_4_CH1
        GPIO_SetFunc(PWM_TIMA_4_CH1_PORT, PWM_TIMA_4_CH1_PIN, PWM_TIMA_4_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH2
        GPIO_SetFunc(PWM_TIMA_4_CH2_PORT, PWM_TIMA_4_CH2_PIN, PWM_TIMA_4_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH3
        GPIO_SetFunc(PWM_TIMA_4_CH3_PORT, PWM_TIMA_4_CH3_PIN, PWM_TIMA_4_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_4_CH4
        GPIO_SetFunc(PWM_TIMA_4_CH4_PORT, PWM_TIMA_4_CH4_PIN, PWM_TIMA_4_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_5)
    case (rt_uint32_t)CM_TMRA_5:
    #ifdef BSP_USING_PWM_TIMA_5_CH1
        GPIO_SetFunc(PWM_TIMA_5_CH1_PORT, PWM_TIMA_5_CH1_PIN, PWM_TIMA_5_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH2
        GPIO_SetFunc(PWM_TIMA_5_CH2_PORT, PWM_TIMA_5_CH2_PIN, PWM_TIMA_5_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH3
        GPIO_SetFunc(PWM_TIMA_5_CH3_PORT, PWM_TIMA_5_CH3_PIN, PWM_TIMA_5_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_5_CH4
        GPIO_SetFunc(PWM_TIMA_5_CH4_PORT, PWM_TIMA_5_CH4_PIN, PWM_TIMA_5_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_6)
    case (rt_uint32_t)CM_TMRA_6:
    #ifdef BSP_USING_PWM_TIMA_6_CH1
        GPIO_SetFunc(PWM_TIMA_6_CH1_PORT, PWM_TIMA_6_CH1_PIN, PWM_TIMA_6_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH2
        GPIO_SetFunc(PWM_TIMA_6_CH2_PORT, PWM_TIMA_6_CH2_PIN, PWM_TIMA_6_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH3
        GPIO_SetFunc(PWM_TIMA_6_CH3_PORT, PWM_TIMA_6_CH3_PIN, PWM_TIMA_6_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_6_CH4
        GPIO_SetFunc(PWM_TIMA_6_CH4_PORT, PWM_TIMA_6_CH4_PIN, PWM_TIMA_6_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_7)
    case (rt_uint32_t)CM_TMRA_7:
    #ifdef BSP_USING_PWM_TIMA_7_CH1
        GPIO_SetFunc(PWM_TIMA_7_CH1_PORT, PWM_TIMA_7_CH1_PIN, PWM_TIMA_7_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_7_CH2
        GPIO_SetFunc(PWM_TIMA_7_CH2_PORT, PWM_TIMA_7_CH2_PIN, PWM_TIMA_7_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_7_CH3
        GPIO_SetFunc(PWM_TIMA_7_CH3_PORT, PWM_TIMA_7_CH3_PIN, PWM_TIMA_7_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_7_CH4
        GPIO_SetFunc(PWM_TIMA_7_CH4_PORT, PWM_TIMA_7_CH4_PIN, PWM_TIMA_7_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_8)
    case (rt_uint32_t)CM_TMRA_8:
    #ifdef BSP_USING_PWM_TIMA_8_CH1
        GPIO_SetFunc(PWM_TIMA_8_CH1_PORT, PWM_TIMA_8_CH1_PIN, PWM_TIMA_8_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_8_CH2
        GPIO_SetFunc(PWM_TIMA_8_CH2_PORT, PWM_TIMA_8_CH2_PIN, PWM_TIMA_8_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_8_CH3
        GPIO_SetFunc(PWM_TIMA_8_CH3_PORT, PWM_TIMA_8_CH3_PIN, PWM_TIMA_8_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_8_CH4
        GPIO_SetFunc(PWM_TIMA_8_CH4_PORT, PWM_TIMA_8_CH4_PIN, PWM_TIMA_8_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_9)
    case (rt_uint32_t)CM_TMRA_9:
    #ifdef BSP_USING_PWM_TIMA_9_CH1
        GPIO_SetFunc(PWM_TIMA_9_CH1_PORT, PWM_TIMA_9_CH1_PIN, PWM_TIMA_9_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_9_CH2
        GPIO_SetFunc(PWM_TIMA_9_CH2_PORT, PWM_TIMA_9_CH2_PIN, PWM_TIMA_9_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_9_CH3
        GPIO_SetFunc(PWM_TIMA_9_CH3_PORT, PWM_TIMA_9_CH3_PIN, PWM_TIMA_9_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_9_CH4
        GPIO_SetFunc(PWM_TIMA_9_CH4_PORT, PWM_TIMA_9_CH4_PIN, PWM_TIMA_9_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_10)
    case (rt_uint32_t)CM_TMRA_10:
    #ifdef BSP_USING_PWM_TIMA_10_CH1
        GPIO_SetFunc(PWM_TIMA_10_CH1_PORT, PWM_TIMA_10_CH1_PIN, PWM_TIMA_10_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_10_CH2
        GPIO_SetFunc(PWM_TIMA_10_CH2_PORT, PWM_TIMA_10_CH2_PIN, PWM_TIMA_10_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_10_CH3
        GPIO_SetFunc(PWM_TIMA_10_CH3_PORT, PWM_TIMA_10_CH3_PIN, PWM_TIMA_10_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_10_CH4
        GPIO_SetFunc(PWM_TIMA_10_CH4_PORT, PWM_TIMA_10_CH4_PIN, PWM_TIMA_10_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_11)
    case (rt_uint32_t)CM_TMRA_11:
    #ifdef BSP_USING_PWM_TIMA_11_CH1
        GPIO_SetFunc(PWM_TIMA_11_CH1_PORT, PWM_TIMA_11_CH1_PIN, PWM_TIMA_11_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_11_CH2
        GPIO_SetFunc(PWM_TIMA_11_CH2_PORT, PWM_TIMA_11_CH2_PIN, PWM_TIMA_11_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_11_CH3
        GPIO_SetFunc(PWM_TIMA_11_CH3_PORT, PWM_TIMA_11_CH3_PIN, PWM_TIMA_11_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_11_CH4
        GPIO_SetFunc(PWM_TIMA_11_CH4_PORT, PWM_TIMA_11_CH4_PIN, PWM_TIMA_11_CH4_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIMA_12)
    case (rt_uint32_t)CM_TMRA_12:
    #ifdef BSP_USING_PWM_TIMA_12_CH1
        GPIO_SetFunc(PWM_TIMA_12_CH1_PORT, PWM_TIMA_12_CH1_PIN, PWM_TIMA_12_CH1_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_12_CH2
        GPIO_SetFunc(PWM_TIMA_12_CH2_PORT, PWM_TIMA_12_CH2_PIN, PWM_TIMA_12_CH2_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_12_CH3
        GPIO_SetFunc(PWM_TIMA_12_CH3_PORT, PWM_TIMA_12_CH3_PIN, PWM_TIMA_12_CH3_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIMA_12_CH4
        GPIO_SetFunc(PWM_TIMA_12_CH4_PORT, PWM_TIMA_12_CH4_PIN, PWM_TIMA_12_CH4_PIN_FUNC);
    #endif
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined(BSP_USING_PWM_TIM4)
rt_err_t rt_hw_board_pwm_tmr4_init(CM_TMR4_TypeDef *TMR4x)
{
    rt_err_t result = RT_EOK;
    switch ((rt_uint32_t)TMR4x)
    {
#if defined(BSP_USING_PWM_TIM4_1)
    case (rt_uint32_t)CM_TMR4_1:
    #ifdef BSP_USING_PWM_TIM4_1_OUH
        GPIO_SetFunc(PWM_TIM4_1_OUH_PORT, PWM_TIM4_1_OUH_PIN, PWM_TIM4_1_OUH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OUL
        GPIO_SetFunc(PWM_TIM4_1_OUL_PORT, PWM_TIM4_1_OUL_PIN, PWM_TIM4_1_OUL_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OVH
        GPIO_SetFunc(PWM_TIM4_1_OVH_PORT, PWM_TIM4_1_OVH_PIN, PWM_TIM4_1_OVH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OVL
        GPIO_SetFunc(PWM_TIM4_1_OVL_PORT, PWM_TIM4_1_OVL_PIN, PWM_TIM4_1_OVL_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OWH
        GPIO_SetFunc(PWM_TIM4_1_OWH_PORT, PWM_TIM4_1_OWH_PIN, PWM_TIM4_1_OWH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_1_OWL
        GPIO_SetFunc(PWM_TIM4_1_OWL_PORT, PWM_TIM4_1_OWL_PIN, PWM_TIM4_1_OWL_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM4_2)
    case (rt_uint32_t)CM_TMR4_2:
    #ifdef BSP_USING_PWM_TIM4_2_OUH
        GPIO_SetFunc(PWM_TIM4_2_OUH_PORT, PWM_TIM4_2_OUH_PIN, PWM_TIM4_2_OUH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OUL
        GPIO_SetFunc(PWM_TIM4_2_OUL_PORT, PWM_TIM4_2_OUL_PIN, PWM_TIM4_2_OUL_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OVH
        GPIO_SetFunc(PWM_TIM4_2_OVH_PORT, PWM_TIM4_2_OVH_PIN, PWM_TIM4_2_OVH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OVL
        GPIO_SetFunc(PWM_TIM4_2_OVL_PORT, PWM_TIM4_2_OVL_PIN, PWM_TIM4_2_OVL_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OWH
        GPIO_SetFunc(PWM_TIM4_2_OWH_PORT, PWM_TIM4_2_OWH_PIN, PWM_TIM4_2_OWH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_2_OWL
        GPIO_SetFunc(PWM_TIM4_2_OWL_PORT, PWM_TIM4_2_OWL_PIN, PWM_TIM4_2_OWL_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM4_3)
    case (rt_uint32_t)CM_TMR4_3:
    #ifdef BSP_USING_PWM_TIM4_3_OUH
        GPIO_SetFunc(PWM_TIM4_3_OUH_PORT, PWM_TIM4_3_OUH_PIN, PWM_TIM4_3_OUH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OUL
        GPIO_SetFunc(PWM_TIM4_3_OUL_PORT, PWM_TIM4_3_OUL_PIN, PWM_TIM4_3_OUL_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OVH
        GPIO_SetFunc(PWM_TIM4_3_OVH_PORT, PWM_TIM4_3_OVH_PIN, PWM_TIM4_3_OVH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OVL
        GPIO_SetFunc(PWM_TIM4_3_OVL_PORT, PWM_TIM4_3_OVL_PIN, PWM_TIM4_3_OVL_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OWH
        GPIO_SetFunc(PWM_TIM4_3_OWH_PORT, PWM_TIM4_3_OWH_PIN, PWM_TIM4_3_OWH_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM4_3_OWL
        GPIO_SetFunc(PWM_TIM4_3_OWL_PORT, PWM_TIM4_3_OWL_PIN, PWM_TIM4_3_OWL_PIN_FUNC);
    #endif
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined(BSP_USING_PWM_TIM6)
rt_err_t rt_hw_board_pwm_tmr6_init(CM_TMR6_TypeDef *TMR6x)
{
    rt_err_t result = RT_EOK;
    switch ((rt_uint32_t)TMR6x)
    {
#if defined(BSP_USING_PWM_TIM6_1)
    case (rt_uint32_t)CM_TMR6_1:
    #ifdef BSP_USING_PWM_TIM6_1_A
        GPIO_SetFunc(PWM_TIM6_1_A_PORT, PWM_TIM6_1_A_PIN, PWM_TIM6_1_A_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM6_1_B
        GPIO_SetFunc(PWM_TIM6_1_B_PORT, PWM_TIM6_1_B_PIN, PWM_TIM6_1_B_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM6_2)
    case (rt_uint32_t)CM_TMR6_2:
    #ifdef BSP_USING_PWM_TIM6_2_A
        GPIO_SetFunc(PWM_TIM6_2_A_PORT, PWM_TIM6_2_A_PIN, PWM_TIM6_2_A_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM6_2_B
        GPIO_SetFunc(PWM_TIM6_2_B_PORT, PWM_TIM6_2_B_PIN, PWM_TIM6_2_B_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM6_3)
    case (rt_uint32_t)CM_TMR6_3:
    #ifdef BSP_USING_PWM_TIM6_3_A
        GPIO_SetFunc(PWM_TIM6_3_A_PORT, PWM_TIM6_3_A_PIN, PWM_TIM6_3_A_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM6_3_B
        GPIO_SetFunc(PWM_TIM6_3_B_PORT, PWM_TIM6_3_B_PIN, PWM_TIM6_3_B_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM6_4)
    case (rt_uint32_t)CM_TMR6_4:
    #ifdef BSP_USING_PWM_TIM6_4_A
        GPIO_SetFunc(PWM_TIM6_4_A_PORT, PWM_TIM6_4_A_PIN, PWM_TIM6_4_A_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM6_4_B
        GPIO_SetFunc(PWM_TIM6_4_B_PORT, PWM_TIM6_4_B_PIN, PWM_TIM6_4_B_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM6_5)
    case (rt_uint32_t)CM_TMR6_5:
    #ifdef BSP_USING_PWM_TIM6_5_A
        GPIO_SetFunc(PWM_TIM6_5_A_PORT, PWM_TIM6_5_A_PIN, PWM_TIM6_5_A_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM6_5_B
        GPIO_SetFunc(PWM_TIM6_5_B_PORT, PWM_TIM6_5_B_PIN, PWM_TIM6_5_B_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM6_6)
    case (rt_uint32_t)CM_TMR6_6:
    #ifdef BSP_USING_PWM_TIM6_6_A
        GPIO_SetFunc(PWM_TIM6_6_A_PORT, PWM_TIM6_6_A_PIN, PWM_TIM6_6_A_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM6_6_B
        GPIO_SetFunc(PWM_TIM6_6_B_PORT, PWM_TIM6_6_B_PIN, PWM_TIM6_6_B_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM6_7)
    case (rt_uint32_t)CM_TMR6_7:
    #ifdef BSP_USING_PWM_TIM6_7_A
        GPIO_SetFunc(PWM_TIM6_7_A_PORT, PWM_TIM6_7_A_PIN, PWM_TIM6_7_A_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM6_7_B
        GPIO_SetFunc(PWM_TIM6_7_B_PORT, PWM_TIM6_7_B_PIN, PWM_TIM6_7_B_PIN_FUNC);
    #endif
        break;
#endif
#if defined(BSP_USING_PWM_TIM6_8)
    case (rt_uint32_t)CM_TMR6_8:
    #ifdef BSP_USING_PWM_TIM6_8_A
        GPIO_SetFunc(PWM_TIM6_8_A_PORT, PWM_TIM6_8_A_PIN, PWM_TIM6_8_A_PIN_FUNC);
    #endif
    #ifdef BSP_USING_PWM_TIM6_8_B
        GPIO_SetFunc(PWM_TIM6_8_B_PORT, PWM_TIM6_8_B_PIN, PWM_TIM6_8_B_PIN_FUNC);
    #endif
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }
#endif

#if defined (BSP_USING_SDRAM)
rt_err_t rt_hw_board_sdram_init(void)
{
    rt_err_t result = RT_EOK;
    stc_gpio_init_t stcGpioInit;

    /************************* Set pin drive capacity *************************/
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;

    /* DMC_CKE */
    (void)GPIO_Init(SDRAM_CKE_PORT, SDRAM_CKE_PIN, &stcGpioInit);

    /* DMC_CLK */
    (void)GPIO_Init(SDRAM_CLK_PORT, SDRAM_CLK_PIN, &stcGpioInit);

    /* DMC_LDQM && DMC_UDQM */
    (void)GPIO_Init(SDRAM_DQM0_PORT, SDRAM_DQM0_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DQM1_PORT, SDRAM_DQM1_PIN, &stcGpioInit);

    /* DMC_BA[0:1] */
    (void)GPIO_Init(SDRAM_BA0_PORT, SDRAM_BA0_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_BA1_PORT, SDRAM_BA1_PIN, &stcGpioInit);

    /* DMC_CAS && DMC_RAS */
    (void)GPIO_Init(SDRAM_CAS_PORT, SDRAM_CAS_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_RAS_PORT, SDRAM_RAS_PIN, &stcGpioInit);

    /* DMC_WE */
    (void)GPIO_Init(SDRAM_WE_PORT, SDRAM_WE_PIN, &stcGpioInit);

    /* DMC_DATA[0:15] */
    (void)GPIO_Init(SDRAM_DATA0_PORT, SDRAM_DATA0_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA1_PORT, SDRAM_DATA1_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA2_PORT, SDRAM_DATA2_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA3_PORT, SDRAM_DATA3_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA4_PORT, SDRAM_DATA4_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA5_PORT, SDRAM_DATA5_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA6_PORT, SDRAM_DATA6_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA7_PORT, SDRAM_DATA7_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA8_PORT, SDRAM_DATA8_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA9_PORT, SDRAM_DATA9_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA10_PORT, SDRAM_DATA10_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA11_PORT, SDRAM_DATA11_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA12_PORT, SDRAM_DATA12_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA13_PORT, SDRAM_DATA13_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA14_PORT, SDRAM_DATA14_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_DATA15_PORT, SDRAM_DATA15_PIN, &stcGpioInit);

    /* DMC_ADD[0:11]*/
    (void)GPIO_Init(SDRAM_ADD0_PORT, SDRAM_ADD0_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD1_PORT, SDRAM_ADD1_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD2_PORT, SDRAM_ADD2_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD3_PORT, SDRAM_ADD3_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD4_PORT, SDRAM_ADD4_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD5_PORT, SDRAM_ADD5_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD6_PORT, SDRAM_ADD6_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD7_PORT, SDRAM_ADD7_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD8_PORT, SDRAM_ADD8_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD9_PORT, SDRAM_ADD9_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD10_PORT, SDRAM_ADD10_PIN, &stcGpioInit);
    (void)GPIO_Init(SDRAM_ADD11_PORT, SDRAM_ADD11_PIN, &stcGpioInit);

    /************************** Set EXMC pin function *************************/
    /* DMC_CKE */
    GPIO_SetFunc(SDRAM_CKE_PORT, SDRAM_CKE_PIN, GPIO_FUNC_12);

    /* DMC_CLK */
    GPIO_SetFunc(SDRAM_CLK_PORT, SDRAM_CLK_PIN, GPIO_FUNC_12);

    /* DMC_LDQM && DMC_UDQM */
    GPIO_SetFunc(SDRAM_DQM0_PORT, SDRAM_DQM0_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DQM1_PORT, SDRAM_DQM1_PIN, GPIO_FUNC_12);

    /* DMC_BA[0:1] */
    GPIO_SetFunc(SDRAM_BA0_PORT, SDRAM_BA0_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_BA1_PORT, SDRAM_BA1_PIN, GPIO_FUNC_12);

    /* DMC_CS */
    GPIO_SetFunc(SDRAM_CS_PORT, SDRAM_CS_PIN, GPIO_FUNC_12);

    /* DMC_CAS && DMC_RAS */
    GPIO_SetFunc(SDRAM_CAS_PORT, SDRAM_CAS_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_RAS_PORT, SDRAM_RAS_PIN, GPIO_FUNC_12);

    /* DMC_WE */
    GPIO_SetFunc(SDRAM_WE_PORT, SDRAM_WE_PIN, GPIO_FUNC_12);

    /* DMC_DATA[0:15] */
    GPIO_SetFunc(SDRAM_DATA0_PORT, SDRAM_DATA0_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA1_PORT, SDRAM_DATA1_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA2_PORT, SDRAM_DATA2_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA3_PORT, SDRAM_DATA3_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA4_PORT, SDRAM_DATA4_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA5_PORT, SDRAM_DATA5_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA6_PORT, SDRAM_DATA6_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA7_PORT, SDRAM_DATA7_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA8_PORT, SDRAM_DATA8_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA9_PORT, SDRAM_DATA9_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA10_PORT, SDRAM_DATA10_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA11_PORT, SDRAM_DATA11_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA12_PORT, SDRAM_DATA12_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA13_PORT, SDRAM_DATA13_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA14_PORT, SDRAM_DATA14_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_DATA15_PORT, SDRAM_DATA15_PIN, GPIO_FUNC_12);

    /* DMC_ADD[0:11]*/
    GPIO_SetFunc(SDRAM_ADD0_PORT, SDRAM_ADD0_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD1_PORT, SDRAM_ADD1_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD2_PORT, SDRAM_ADD2_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD3_PORT, SDRAM_ADD3_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD4_PORT, SDRAM_ADD4_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD5_PORT, SDRAM_ADD5_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD6_PORT, SDRAM_ADD6_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD7_PORT, SDRAM_ADD7_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD8_PORT, SDRAM_ADD8_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD9_PORT, SDRAM_ADD9_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD10_PORT, SDRAM_ADD10_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(SDRAM_ADD11_PORT, SDRAM_ADD11_PIN, GPIO_FUNC_12);

    return result;
}
#endif

#ifdef RT_USING_PM
#define EFM_ERASE_TIME_MAX_IN_MILLISECOND                   (20)
#define PLL_SRC                                             ((CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLSRC) >> CMU_PLLHCFGR_PLLSRC_POS)

static void _pm_sleep_common_init(rt_bool_t b_disable_unused_clk)
{
    CLK_Xtal32Cmd(ENABLE);

    rt_tick_t tick_start = rt_tick_get_millisecond();
    rt_err_t rt_stat = RT_EOK;
    //wait flash idle
    while (SET != EFM_GetStatus(EFM_FLAG_RDY))
    {
        if (rt_tick_get_millisecond() - tick_start > EFM_ERASE_TIME_MAX_IN_MILLISECOND)
        {
            rt_stat = RT_ERROR;
            break;
        }
    }
    RT_ASSERT(rt_stat == RT_EOK);

    if (b_disable_unused_clk)
    {
        uint32_t cur_clk_src = READ_REG8_BIT(CM_CMU->CKSWR, CMU_CKSWR_CKSW);

        switch (cur_clk_src)
        {
        case CLK_SYSCLK_SRC_HRC:
            CLK_PLLCmd(DISABLE);
            CLK_MrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            CLK_XtalCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_PLL, DISABLE);
            break;
        case CLK_SYSCLK_SRC_MRC:
            CLK_PLLCmd(DISABLE);
            CLK_HrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            CLK_XtalCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_PLL | PWC_LDO_HRC, DISABLE);

            break;
        case CLK_SYSCLK_SRC_XTAL:
            CLK_PLLCmd(DISABLE);
            CLK_HrcCmd(DISABLE);
            CLK_MrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_PLL | PWC_LDO_HRC, DISABLE);

            break;
        case CLK_SYSCLK_SRC_XTAL32:
            CLK_PLLCmd(DISABLE);
            CLK_HrcCmd(DISABLE);
            CLK_MrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            CLK_XtalCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_PLL | PWC_LDO_HRC, DISABLE);

            break;
        case CLK_SYSCLK_SRC_PLL:
            if (CLK_PLL_SRC_XTAL == PLL_SRC)
            {
                CLK_HrcCmd(DISABLE);
            }
            else
            {
                CLK_XtalCmd(DISABLE);
            }
            CLK_MrcCmd(DISABLE);
            CLK_LrcCmd(DISABLE);
            PWC_LDO_Cmd(PWC_LDO_HRC, DISABLE);

            break;
        default:
            break;
        }
    }

}

void rt_hw_board_pm_sleep_deep_init(void)
{
#if (PM_SLEEP_DEEP_CFG_CLK   == PWC_STOP_CLK_KEEP)
    _pm_sleep_common_init(RT_TRUE);
#else
    _pm_sleep_common_init(RT_FALSE);
    CLK_PLLCmd(DISABLE);
    CLK_HrcCmd(DISABLE);
    CLK_LrcCmd(DISABLE);
    CLK_XtalCmd(DISABLE);
    PWC_LDO_Cmd(PWC_LDO_PLL | PWC_LDO_HRC, DISABLE);
#endif
}

void rt_hw_board_pm_sleep_shutdown_init(void)
{
    _pm_sleep_common_init(RT_TRUE);
}

static void __xtal_init(void)
{
    stc_clock_xtal_init_t stcXtalInit;

    /* XTAL config */
    (void)CLK_XtalStructInit(&stcXtalInit);
    /* Config Xtal and Enable Xtal */
    stcXtalInit.u8State = CLK_XTAL_ON;
    stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
    stcXtalInit.u8Drv = CLK_XTAL_DRV_ULOW;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;

    GPIO_AnalogCmd(GPIO_PORT_H, GPIO_PIN_01 | GPIO_PIN_00, ENABLE);
    (void)CLK_XtalInit(&stcXtalInit);
}

void rt_hw_board_pm_sysclk_cfg(uint8_t run_mode)
{
    switch (run_mode)
    {
    case PM_RUN_MODE_HIGH_SPEED:
    case PM_RUN_MODE_NORMAL_SPEED:
        CLK_XtalCmd(DISABLE);
        SystemClock_Config();
        break;

    case PM_RUN_MODE_LOW_SPEED:
        __xtal_init();
        CLK_SetSysClockSrc(CLK_SYSCLK_SRC_XTAL);

    default:
        break;
    }
}
#endif

#if defined(BSP_USING_USBD)
rt_err_t rt_hw_usb_board_init(void)
{
    stc_gpio_init_t stcGpioCfg;
    (void)GPIO_StructInit(&stcGpioCfg);
#if defined(BSP_USING_USBFS)
    stcGpioCfg.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(USBF_DM_PORT, USBF_DM_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBF_DP_PORT, USBF_DP_PIN, &stcGpioCfg);
#if defined(BSP_USING_USBD_VBUS_SENSING)
    GPIO_SetFunc(USBF_VBUS_PORT, USBF_VBUS_PIN, GPIO_FUNC_10); /* VBUS */
#endif
#elif defined(BSP_USING_USBHS)
#if defined(BSP_USING_USBHS_PHY_EMBED)
    /* USBHS work in embedded PHY */
    stcGpioCfg.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(USBH_DM_PORT, USBH_DM_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_DP_PORT, USBH_DP_PIN, &stcGpioCfg);
#if defined(BSP_USING_USBD_VBUS_SENSING)
    GPIO_SetFunc(USBH_VBUS_PORT, USBH_VBUS_PIN, GPIO_FUNC_12);
#endif
#else
   /* Reset 3300 */
    TCA9539_WritePin(TCA9539_IO_PORT1, USB_3300_RESET_PIN, TCA9539_PIN_SET);
    TCA9539_ConfigPin(TCA9539_IO_PORT1, USB_3300_RESET_PIN, TCA9539_DIR_OUT);

    (void)GPIO_StructInit(&stcGpioCfg);
    /* High drive capability */
    stcGpioCfg.u16PinDrv = PIN_HIGH_DRV;
    (void)GPIO_Init(USBH_ULPI_D0_PORT, USBH_ULPI_D0_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_ULPI_D1_PORT, USBH_ULPI_D1_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_ULPI_D2_PORT, USBH_ULPI_D2_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_ULPI_D3_PORT, USBH_ULPI_D3_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_ULPI_D4_PORT, USBH_ULPI_D4_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_ULPI_D5_PORT, USBH_ULPI_D5_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_ULPI_D6_PORT, USBH_ULPI_D6_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_ULPI_D7_PORT, USBH_ULPI_D7_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBH_ULPI_STP_PORT, USBH_ULPI_STP_PIN, &stcGpioCfg);

    GPIO_SetFunc(USBH_ULPI_CLK_PORT, USBH_ULPI_CLK_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_DIR_PORT, USBH_ULPI_DIR_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_NXT_PORT, USBH_ULPI_NXT_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_STP_PORT, USBH_ULPI_STP_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_D0_PORT, USBH_ULPI_D0_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_D1_PORT, USBH_ULPI_D1_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_D2_PORT, USBH_ULPI_D2_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_D3_PORT, USBH_ULPI_D3_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_D4_PORT, USBH_ULPI_D4_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_D5_PORT, USBH_ULPI_D5_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_D6_PORT, USBH_ULPI_D6_PIN, GPIO_FUNC_10);
    GPIO_SetFunc(USBH_ULPI_D7_PORT, USBH_ULPI_D7_PIN, GPIO_FUNC_10);

    TCA9539_WritePin(TCA9539_IO_PORT1, USB_3300_RESET_PIN, TCA9539_PIN_RESET);
#endif
    
#endif
    return RT_EOK;
}
#endif
