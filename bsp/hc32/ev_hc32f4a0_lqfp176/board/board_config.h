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


#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include <rtconfig.h>
#include "hc32_ll.h"
#include "drv_config.h"


/************************ USART port **********************/
#if defined(BSP_USING_UART1)
    #define USART1_RX_PORT                  (GPIO_PORT_H)
    #define USART1_RX_PIN                   (GPIO_PIN_13)

    #define USART1_TX_PORT                  (GPIO_PORT_H)
    #define USART1_TX_PIN                   (GPIO_PIN_15)
#endif

#if defined(BSP_USING_UART6)
    #define USART6_RX_PORT                  (GPIO_PORT_H)
    #define USART6_RX_PIN                   (GPIO_PIN_06)

    #define USART6_TX_PORT                  (GPIO_PORT_E)
    #define USART6_TX_PIN                   (GPIO_PIN_06)
#endif

/***********  ADC configure *********/
#if defined(BSP_USING_ADC1)
    #define ADC1_CH_PORT                    (GPIO_PORT_C)
    #define ADC1_CH_PIN                     (GPIO_PIN_00)
#endif

#if defined(BSP_USING_ADC2)
    #define ADC2_CH_PORT                    (GPIO_PORT_C)
    #define ADC2_CH_PIN                     (GPIO_PIN_01)
#endif

#if defined(BSP_USING_ADC3)
    #define ADC3_CH_PORT                    (GPIO_PORT_C)
    #define ADC3_CH_PIN                     (GPIO_PIN_02)
#endif

/***********  DAC configure *********/
#if defined(BSP_USING_DAC1)
    #define DAC1_CH1_PORT                   (GPIO_PORT_A)
    #define DAC1_CH1_PIN                    (GPIO_PIN_04)
    #define DAC1_CH2_PORT                   (GPIO_PORT_A)
    #define DAC1_CH2_PIN                    (GPIO_PIN_05)
#endif

#if defined(BSP_USING_DAC2)
    #define DAC2_CH1_PORT                   (GPIO_PORT_C)
    #define DAC2_CH1_PIN                    (GPIO_PIN_04)
#endif

/***********  CAN configure *********/
#if defined(BSP_USING_CAN1)
    #define CAN1_TX_PORT                    (GPIO_PORT_D)
    #define CAN1_TX_PIN                     (GPIO_PIN_05)
    #define CAN1_TX_PIN_FUNC                (GPIO_FUNC_60)

    #define CAN1_RX_PORT                    (GPIO_PORT_D)
    #define CAN1_RX_PIN                     (GPIO_PIN_04)
    #define CAN1_RX_PIN_FUNC                (GPIO_FUNC_61)
#endif

#if defined(BSP_USING_CAN2)
    #define CAN2_TX_PORT                    (GPIO_PORT_D)
    #define CAN2_TX_PIN                     (GPIO_PIN_07)
    #define CAN2_TX_PIN_FUNC                (GPIO_FUNC_62)

    #define CAN2_RX_PORT                    (GPIO_PORT_D)
    #define CAN2_RX_PIN                     (GPIO_PIN_06)
    #define CAN2_RX_PIN_FUNC                (GPIO_FUNC_63)
#endif

/************************ SDIOC port **********************/
#if defined(BSP_USING_SDIO1)
    #define SDIOC1_CK_PORT                  (GPIO_PORT_C)
    #define SDIOC1_CK_PIN                   (GPIO_PIN_12)

    #define SDIOC1_CMD_PORT                 (GPIO_PORT_D)
    #define SDIOC1_CMD_PIN                  (GPIO_PIN_02)

    #define SDIOC1_D0_PORT                  (GPIO_PORT_B)
    #define SDIOC1_D0_PIN                   (GPIO_PIN_07)

    #define SDIOC1_D1_PORT                  (GPIO_PORT_A)
    #define SDIOC1_D1_PIN                   (GPIO_PIN_08)

    #define SDIOC1_D2_PORT                  (GPIO_PORT_C)
    #define SDIOC1_D2_PIN                   (GPIO_PIN_10)

    #define SDIOC1_D3_PORT                  (GPIO_PORT_B)
    #define SDIOC1_D3_PIN                   (GPIO_PIN_05)
#endif

#if defined(BSP_USING_RTC)
#if defined(BSP_RTC_USING_XTAL32)
    #define XTAL32_PORT                     (GPIO_PORT_C)
    #define XTAL32_IN_PIN                   (GPIO_PIN_15)
    #define XTAL32_OUT_PIN                  (GPIO_PIN_14)
#endif
#endif

/***********  PWM_TMRA configure *********/
#if defined(RT_USING_PWM)

#if defined(BSP_USING_PWM_TMRA_1)
#if defined(BSP_USING_PWM_TMRA_1_CH1)
    #define PWM_TMRA_1_CH1_PORT             (GPIO_PORT_A)
    #define PWM_TMRA_1_CH1_PIN              (GPIO_PIN_08)
    #define PWM_TMRA_1_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_1_CH2)
    #define PWM_TMRA_1_CH2_PORT             (GPIO_PORT_A)
    #define PWM_TMRA_1_CH2_PIN              (GPIO_PIN_09)
    #define PWM_TMRA_1_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_1_CH3)
    #define PWM_TMRA_1_CH3_PORT             (GPIO_PORT_A)
    #define PWM_TMRA_1_CH3_PIN              (GPIO_PIN_10)
    #define PWM_TMRA_1_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_1_CH4)
    #define PWM_TMRA_1_CH4_PORT             (GPIO_PORT_A)
    #define PWM_TMRA_1_CH4_PIN              (GPIO_PIN_11)
    #define PWM_TMRA_1_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_2)
#if defined(BSP_USING_PWM_TMRA_2_CH1)
    #define PWM_TMRA_2_CH1_PORT             (GPIO_PORT_A)
    #define PWM_TMRA_2_CH1_PIN              (GPIO_PIN_00)
    #define PWM_TMRA_2_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_2_CH2)
    #define PWM_TMRA_2_CH2_PORT             (GPIO_PORT_A)
    #define PWM_TMRA_2_CH2_PIN              (GPIO_PIN_01)
    #define PWM_TMRA_2_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_2_CH3)
    #define PWM_TMRA_2_CH3_PORT             (GPIO_PORT_A)
    #define PWM_TMRA_2_CH3_PIN              (GPIO_PIN_02)
    #define PWM_TMRA_2_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_2_CH4)
    #define PWM_TMRA_2_CH4_PORT             (GPIO_PORT_A)
    #define PWM_TMRA_2_CH4_PIN              (GPIO_PIN_03)
    #define PWM_TMRA_2_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_3)
#if defined(BSP_USING_PWM_TMRA_3_CH1)
    #define PWM_TMRA_3_CH1_PORT             (GPIO_PORT_E)
    #define PWM_TMRA_3_CH1_PIN              (GPIO_PIN_11)
    #define PWM_TMRA_3_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_3_CH2)
    #define PWM_TMRA_3_CH2_PORT             (GPIO_PORT_E)
    #define PWM_TMRA_3_CH2_PIN              (GPIO_PIN_12)
    #define PWM_TMRA_3_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_3_CH3)
    #define PWM_TMRA_3_CH3_PORT             (GPIO_PORT_E)
    #define PWM_TMRA_3_CH3_PIN              (GPIO_PIN_13)
    #define PWM_TMRA_3_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_3_CH4)
    #define PWM_TMRA_3_CH4_PORT             (GPIO_PORT_E)
    #define PWM_TMRA_3_CH4_PIN              (GPIO_PIN_14)
    #define PWM_TMRA_3_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_4)
#if defined(BSP_USING_PWM_TMRA_4_CH1)
    #define PWM_TMRA_4_CH1_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_4_CH1_PIN              (GPIO_PIN_11)
    #define PWM_TMRA_4_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_4_CH2)
    #define PWM_TMRA_4_CH2_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_4_CH2_PIN              (GPIO_PIN_12)
    #define PWM_TMRA_4_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_4_CH3)
    #define PWM_TMRA_4_CH3_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_4_CH3_PIN              (GPIO_PIN_13)
    #define PWM_TMRA_4_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_4_CH4)
    #define PWM_TMRA_4_CH4_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_4_CH4_PIN              (GPIO_PIN_14)
    #define PWM_TMRA_4_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_5)
#if defined(BSP_USING_PWM_TMRA_5_CH1)
    #define PWM_TMRA_5_CH1_PORT             (GPIO_PORT_H)
    #define PWM_TMRA_5_CH1_PIN              (GPIO_PIN_10)
    #define PWM_TMRA_5_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_5_CH2)
    #define PWM_TMRA_5_CH2_PORT             (GPIO_PORT_H)
    #define PWM_TMRA_5_CH2_PIN              (GPIO_PIN_11)
    #define PWM_TMRA_5_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_5_CH3)
    #define PWM_TMRA_5_CH3_PORT             (GPIO_PORT_H)
    #define PWM_TMRA_5_CH3_PIN              (GPIO_PIN_12)
    #define PWM_TMRA_5_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_5_CH4)
    #define PWM_TMRA_5_CH4_PORT             (GPIO_PORT_H)
    #define PWM_TMRA_5_CH4_PIN              (GPIO_PIN_01)
    #define PWM_TMRA_5_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_6)
#if defined(BSP_USING_PWM_TMRA_6_CH1)
    #define PWM_TMRA_6_CH1_PORT             (GPIO_PORT_D)
    #define PWM_TMRA_6_CH1_PIN              (GPIO_PIN_08)
    #define PWM_TMRA_6_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_6_CH2)
    #define PWM_TMRA_6_CH2_PORT             (GPIO_PORT_D)
    #define PWM_TMRA_6_CH2_PIN              (GPIO_PIN_09)
    #define PWM_TMRA_6_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_6_CH3)
    #define PWM_TMRA_6_CH3_PORT             (GPIO_PORT_D)
    #define PWM_TMRA_6_CH3_PIN              (GPIO_PIN_10)
    #define PWM_TMRA_6_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_6_CH4)
    #define PWM_TMRA_6_CH4_PORT             (GPIO_PORT_D)
    #define PWM_TMRA_6_CH4_PIN              (GPIO_PIN_11)
    #define PWM_TMRA_6_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_7)
#if defined(BSP_USING_PWM_TMRA_7_CH1)
    #define PWM_TMRA_7_CH1_PORT             (GPIO_PORT_B)
    #define PWM_TMRA_7_CH1_PIN              (GPIO_PIN_13)
    #define PWM_TMRA_7_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_7_CH2)
    #define PWM_TMRA_7_CH2_PORT             (GPIO_PORT_B)
    #define PWM_TMRA_7_CH2_PIN              (GPIO_PIN_14)
    #define PWM_TMRA_7_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_7_CH3)
    #define PWM_TMRA_7_CH3_PORT             (GPIO_PORT_B)
    #define PWM_TMRA_7_CH3_PIN              (GPIO_PIN_15)
    #define PWM_TMRA_7_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_7_CH4)
    #define PWM_TMRA_7_CH4_PORT             (GPIO_PORT_B)
    #define PWM_TMRA_7_CH4_PIN              (GPIO_PIN_12)
    #define PWM_TMRA_7_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_8)
#if defined(BSP_USING_PWM_TMRA_8_CH1)
    #define PWM_TMRA_8_CH1_PORT             (GPIO_PORT_G)
    #define PWM_TMRA_8_CH1_PIN              (GPIO_PIN_11)
    #define PWM_TMRA_8_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_8_CH2)
    #define PWM_TMRA_8_CH2_PORT             (GPIO_PORT_G)
    #define PWM_TMRA_8_CH2_PIN              (GPIO_PIN_12)
    #define PWM_TMRA_8_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_8_CH3)
    #define PWM_TMRA_8_CH3_PORT             (GPIO_PORT_G)
    #define PWM_TMRA_8_CH3_PIN              (GPIO_PIN_13)
    #define PWM_TMRA_8_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_8_CH4)
    #define PWM_TMRA_8_CH4_PORT             (GPIO_PORT_G)
    #define PWM_TMRA_8_CH4_PIN              (GPIO_PIN_14)
    #define PWM_TMRA_8_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_9)
#if defined(BSP_USING_PWM_TMRA_9_CH1)
    #define PWM_TMRA_9_CH1_PORT             (GPIO_PORT_E)
    #define PWM_TMRA_9_CH1_PIN              (GPIO_PIN_02)
    #define PWM_TMRA_9_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_9_CH2)
    #define PWM_TMRA_9_CH2_PORT             (GPIO_PORT_E)
    #define PWM_TMRA_9_CH2_PIN              (GPIO_PIN_03)
    #define PWM_TMRA_9_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_9_CH3)
    #define PWM_TMRA_9_CH3_PORT             (GPIO_PORT_E)
    #define PWM_TMRA_9_CH3_PIN              (GPIO_PIN_04)
    #define PWM_TMRA_9_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_9_CH4)
    #define PWM_TMRA_9_CH4_PORT             (GPIO_PORT_E)
    #define PWM_TMRA_9_CH4_PIN              (GPIO_PIN_05)
    #define PWM_TMRA_9_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_10)
#if defined(BSP_USING_PWM_TMRA_10_CH1)
    #define PWM_TMRA_10_CH1_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_10_CH1_PIN              (GPIO_PIN_06)
    #define PWM_TMRA_10_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_10_CH2)
    #define PWM_TMRA_10_CH2_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_10_CH2_PIN              (GPIO_PIN_07)
    #define PWM_TMRA_10_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_10_CH3)
    #define PWM_TMRA_10_CH3_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_10_CH3_PIN              (GPIO_PIN_08)
    #define PWM_TMRA_10_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TMRA_10_CH4)
    #define PWM_TMRA_10_CH4_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_10_CH4_PIN              (GPIO_PIN_09)
    #define PWM_TMRA_10_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_11)
#if defined(BSP_USING_PWM_TMRA_11_CH1)
    #define PWM_TMRA_11_CH1_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_11_CH1_PIN              (GPIO_PIN_00)
    #define PWM_TMRA_11_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_11_CH2)
    #define PWM_TMRA_11_CH2_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_11_CH2_PIN              (GPIO_PIN_01)
    #define PWM_TMRA_11_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_11_CH3)
    #define PWM_TMRA_11_CH3_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_11_CH3_PIN              (GPIO_PIN_02)
    #define PWM_TMRA_11_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_11_CH4)
    #define PWM_TMRA_11_CH4_PORT             (GPIO_PORT_F)
    #define PWM_TMRA_11_CH4_PIN              (GPIO_PIN_03)
    #define PWM_TMRA_11_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#if defined(BSP_USING_PWM_TMRA_12)
#if defined(BSP_USING_PWM_TMRA_12_CH1)
    #define PWM_TMRA_12_CH1_PORT             (GPIO_PORT_D)
    #define PWM_TMRA_12_CH1_PIN              (GPIO_PIN_01)
    #define PWM_TMRA_12_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_12_CH2)
    #define PWM_TMRA_12_CH2_PORT             (GPIO_PORT_D)
    #define PWM_TMRA_12_CH2_PIN              (GPIO_PIN_02)
    #define PWM_TMRA_12_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_12_CH3)
    #define PWM_TMRA_12_CH3_PORT             (GPIO_PORT_D)
    #define PWM_TMRA_12_CH3_PIN              (GPIO_PIN_03)
    #define PWM_TMRA_12_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TMRA_12_CH4)
    #define PWM_TMRA_12_CH4_PORT             (GPIO_PORT_D)
    #define PWM_TMRA_12_CH4_PIN              (GPIO_PIN_04)
    #define PWM_TMRA_12_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#endif


#endif
