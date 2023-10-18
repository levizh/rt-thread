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

/**
 * The below functions will initialize HC32 board.
 */

#if defined RT_USING_SERIAL
rt_err_t rt_hw_board_uart_init(CM_USART_TypeDef *USARTx)
{
    rt_err_t result = RT_EOK;

    switch ((rt_uint32_t)USARTx)
    {
#if defined(BSP_USING_UART2)
    case (rt_uint32_t)CM_USART2:
        /* Configure USART RX/TX pin. */
        GPIO_SetFunc(USART2_RX_PORT, USART2_RX_PIN, USART2_RX_FUNC);
        GPIO_SetFunc(USART2_TX_PORT, USART2_TX_PIN, USART2_TX_FUNC);
        break;
#endif
#if defined(BSP_USING_UART4)
    case (rt_uint32_t)CM_USART4:
        /* Configure USART RX/TX pin. */
        GPIO_SetFunc(USART4_RX_PORT, USART4_RX_PIN, USART4_RX_FUNC);
        GPIO_SetFunc(USART4_TX_PORT, USART4_TX_PIN, USART4_TX_FUNC);
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined(RT_USING_I2C)
rt_err_t rt_hw_board_i2c_init(CM_I2C_TypeDef *I2Cx)
{
    rt_err_t result = RT_EOK;
    stc_gpio_init_t stcGpioInit;
    (void)GPIO_StructInit(&stcGpioInit);

    switch ((rt_uint32_t)I2Cx)
    {
#if defined(BSP_USING_I2C3)
    case (rt_uint32_t)CM_I2C3:
        /* Configure I2C3 SDA/SCL pin. */
        GPIO_SetFunc(I2C3_SDA_PORT, I2C3_SDA_PIN, I2C3_SDA_FUNC);
        GPIO_SetFunc(I2C3_SCL_PORT, I2C3_SCL_PIN, I2C3_SCL_FUNC);
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
    GPIO_ResetPins(CAN1_STB_PORT, CAN1_STB_PIN);
    GPIO_OutputCmd(CAN1_STB_PORT, CAN1_STB_PIN, ENABLE);
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
#if defined(BSP_USING_SPI3)
    stc_gpio_init_t stcGpioInit;
#endif

    switch ((rt_uint32_t)CM_SPIx)
    {
#if defined(BSP_USING_SPI3)
    case (rt_uint32_t)CM_SPI3:
        GPIO_StructInit(&stcGpioInit);
        stcGpioInit.u16PinState = PIN_STAT_SET;
        stcGpioInit.u16PinDir   = PIN_DIR_OUT;
        GPIO_Init(SPI3_WP_PORT, SPI3_WP_PIN, &stcGpioInit);
        GPIO_Init(SPI3_HOLD_PORT, SPI3_HOLD_PIN, &stcGpioInit);
        GPIO_SetFunc(SPI3_SCK_PORT,  SPI3_SCK_PIN,  SPI3_SCK_FUNC);
        GPIO_SetFunc(SPI3_MOSI_PORT, SPI3_MOSI_PIN, SPI3_MOSI_FUNC);
        GPIO_SetFunc(SPI3_MISO_PORT, SPI3_MISO_PIN, SPI3_MISO_FUNC);
        break;
#endif
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif

#if defined (RT_USING_RTC) || defined(RT_USING_PM)
rt_err_t rt_hw_xtal32_board_init(void)
{
#if defined(BSP_RTC_USING_XTAL32) || defined(RT_USING_PM)
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

        GPIO_SetFunc(SDIOC1_CK_PORT,  SDIOC1_CK_PIN,  SDIOC1_CK_FUNC);
        GPIO_SetFunc(SDIOC1_CMD_PORT, SDIOC1_CMD_PIN, SDIOC1_CMD_FUNC);
        GPIO_SetFunc(SDIOC1_D0_PORT,  SDIOC1_D0_PIN,  SDIOC1_D0_FUNC);
        GPIO_SetFunc(SDIOC1_D1_PORT,  SDIOC1_D1_PIN,  SDIOC1_D1_FUNC);
        GPIO_SetFunc(SDIOC1_D2_PORT,  SDIOC1_D2_PIN,  SDIOC1_D2_FUNC);
        GPIO_SetFunc(SDIOC1_D3_PORT,  SDIOC1_D3_PIN,  SDIOC1_D3_FUNC);
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
    default:
        result = -RT_ERROR;
        break;
    }

    return result;
}
#endif
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

void rt_hw_board_pm_sysclk_cfg(uint8_t run_mode)
{
    switch (run_mode)
    {
    case PM_RUN_MODE_HIGH_SPEED:
    case PM_RUN_MODE_NORMAL_SPEED:
        SystemClock_Config();
        break;

    case PM_RUN_MODE_LOW_SPEED:
        CLK_SetSysClockSrc(CLK_SYSCLK_SRC_XTAL);

    default:
        break;
    }
}
#endif

#if defined(BSP_USING_USBD) || defined(BSP_USING_USBH)
rt_err_t rt_hw_usb_board_init(void)
{
    stc_gpio_init_t stcGpioCfg;
    (void)GPIO_StructInit(&stcGpioCfg);
#if defined(BSP_USING_USBFS)
    stcGpioCfg.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(USBF_DM_PORT, USBF_DM_PIN, &stcGpioCfg);
    (void)GPIO_Init(USBF_DP_PORT, USBF_DP_PIN, &stcGpioCfg);
#if defined(BSP_USING_USBD_VBUS_SENSING)
    GPIO_SetFunc(USBF_VBUS_PORT, USBF_VBUS_PIN, USBF_VBUS_FUNC); /* VBUS */
#endif
#if defined(BSP_USING_USBH)
    GPIO_SetFunc(USBF_DRVVBUS_PORT, USBF_DRVVBUS_PIN, USBF_DRVVBUS_FUNC); /* DRV VBUS */
#endif
#endif
    return RT_EOK;
}
#endif

#if defined(BSP_USING_QSPI)
rt_err_t rt_hw_qspi_board_init(void)
{
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;
#ifndef BSP_QSPI_USING_SOFT_CS
    (void)GPIO_Init(QSPI_FLASH_CS_PORT,  QSPI_FLASH_CS_PIN,  &stcGpioInit);
    GPIO_SetFunc(QSPI_FLASH_CS_PORT,  QSPI_FLASH_CS_PIN,  QSPI_FLASH_CS_FUNC);
#endif
    (void)GPIO_Init(QSPI_FLASH_SCK_PORT, QSPI_FLASH_SCK_PIN, &stcGpioInit);
    (void)GPIO_Init(QSPI_FLASH_IO0_PORT, QSPI_FLASH_IO0_PIN, &stcGpioInit);
    (void)GPIO_Init(QSPI_FLASH_IO1_PORT, QSPI_FLASH_IO1_PIN, &stcGpioInit);
    (void)GPIO_Init(QSPI_FLASH_IO2_PORT, QSPI_FLASH_IO2_PIN, &stcGpioInit);
    (void)GPIO_Init(QSPI_FLASH_IO3_PORT, QSPI_FLASH_IO3_PIN, &stcGpioInit);
    GPIO_SetFunc(QSPI_FLASH_SCK_PORT, QSPI_FLASH_SCK_PIN, QSPI_FLASH_SCK_FUNC);
    GPIO_SetFunc(QSPI_FLASH_IO0_PORT, QSPI_FLASH_IO0_PIN, QSPI_FLASH_IO0_FUNC);
    GPIO_SetFunc(QSPI_FLASH_IO1_PORT, QSPI_FLASH_IO1_PIN, QSPI_FLASH_IO1_FUNC);
    GPIO_SetFunc(QSPI_FLASH_IO2_PORT, QSPI_FLASH_IO2_PIN, QSPI_FLASH_IO2_FUNC);
    GPIO_SetFunc(QSPI_FLASH_IO3_PORT, QSPI_FLASH_IO3_PIN, QSPI_FLASH_IO3_FUNC);

    return RT_EOK;
}
#endif

#if defined(BSP_USING_TIMERA_PULSE_ENCODER)
rt_err_t rt_hw_board_pulse_encoder_tmra_init(void)
{
#if defined(BSP_USING_PULSE_ENCODER_TIMA_1)
    GPIO_SetFunc(PULSE_ENCODER_TIMA_1_A_PORT, PULSE_ENCODER_TIMA_1_A_PIN, PULSE_ENCODER_TIMA_1_A_PIN_FUNC);
    GPIO_SetFunc(PULSE_ENCODER_TIMA_1_B_PORT, PULSE_ENCODER_TIMA_1_B_PIN, PULSE_ENCODER_TIMA_1_B_PIN_FUNC);
#endif
    return RT_EOK;
}
#endif

#if defined(BSP_USING_TIMER6_PULSE_ENCODER)
rt_err_t rt_hw_board_pulse_encoder_tmr6_init(void)
{
#if defined(BSP_USING_PULSE_ENCODER_TIM6_1)
    GPIO_SetFunc(PULSE_ENCODER_TIM6_1_A_PORT, PULSE_ENCODER_TIM6_1_A_PIN, PULSE_ENCODER_TIM6_1_A_PIN_FUNC);
    GPIO_SetFunc(PULSE_ENCODER_TIM6_1_B_PORT, PULSE_ENCODER_TIM6_1_B_PIN, PULSE_ENCODER_TIM6_1_B_PIN_FUNC);
#endif
    return RT_EOK;
}
#endif
