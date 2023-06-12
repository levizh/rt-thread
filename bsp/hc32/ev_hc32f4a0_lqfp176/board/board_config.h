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

/************************ SDRAM port **********************/
#if defined(BSP_USING_SDRAM)
    #define SDRAM_CKE_PORT                  (GPIO_PORT_C)   /* PC03 - EXMC_ALE */
    #define SDRAM_CKE_PIN                   (GPIO_PIN_03)

    #define SDRAM_CLK_PORT                  (GPIO_PORT_G)   /* PD03 - EXMC_CLK */
    #define SDRAM_CLK_PIN                   (GPIO_PIN_08)

    #define SDRAM_DQM0_PORT                 (GPIO_PORT_E)   /* PE00 - EXMC_CE4 */
    #define SDRAM_DQM0_PIN                  (GPIO_PIN_00)
    #define SDRAM_DQM1_PORT                 (GPIO_PORT_E)   /* PE01 - EXMC_CE5 */
    #define SDRAM_DQM1_PIN                  (GPIO_PIN_01)

    #define SDRAM_BA0_PORT                  (GPIO_PORT_D)   /* PD11 - EXMC_ADD16 */
    #define SDRAM_BA0_PIN                   (GPIO_PIN_11)
    #define SDRAM_BA1_PORT                  (GPIO_PORT_D)   /* PD12 - EXMC_ADD17 */
    #define SDRAM_BA1_PIN                   (GPIO_PIN_12)

    #define SDRAM_CS_PORT                   (GPIO_PORT_G)   /* PG09 - EXMC_CE1 */
    #define SDRAM_CS_PIN                    (GPIO_PIN_09)

    #define SDRAM_RAS_PORT                  (GPIO_PORT_F)   /* PF11 - EXMC_OE */
    #define SDRAM_RAS_PIN                   (GPIO_PIN_11)

    #define SDRAM_CAS_PORT                  (GPIO_PORT_G)   /* PG15 - EXMC_BAA */
    #define SDRAM_CAS_PIN                   (GPIO_PIN_15)

    #define SDRAM_WE_PORT                   (GPIO_PORT_C)   /* PC00 - EXMC_WE */
    #define SDRAM_WE_PIN                    (GPIO_PIN_00)

    #define SDRAM_ADD0_PORT                 (GPIO_PORT_F)   /* PF00 - EXMC_ADD0 */
    #define SDRAM_ADD0_PIN                  (GPIO_PIN_00)
    #define SDRAM_ADD1_PORT                 (GPIO_PORT_F)   /* PF01 - EXMC_ADD1 */
    #define SDRAM_ADD1_PIN                  (GPIO_PIN_01)
    #define SDRAM_ADD2_PORT                 (GPIO_PORT_F)   /* PF02 - EXMC_ADD2 */
    #define SDRAM_ADD2_PIN                  (GPIO_PIN_02)
    #define SDRAM_ADD3_PORT                 (GPIO_PORT_F)   /* PF03 - EXMC_ADD3 */
    #define SDRAM_ADD3_PIN                  (GPIO_PIN_03)
    #define SDRAM_ADD4_PORT                 (GPIO_PORT_F)   /* PF04 - EXMC_ADD4 */
    #define SDRAM_ADD4_PIN                  (GPIO_PIN_04)
    #define SDRAM_ADD5_PORT                 (GPIO_PORT_F)   /* PF05 - EXMC_ADD5 */
    #define SDRAM_ADD5_PIN                  (GPIO_PIN_05)
    #define SDRAM_ADD6_PORT                 (GPIO_PORT_F)   /* PF12 - EXMC_ADD6 */
    #define SDRAM_ADD6_PIN                  (GPIO_PIN_12)
    #define SDRAM_ADD7_PORT                 (GPIO_PORT_F)   /* PF13 - EXMC_ADD7 */
    #define SDRAM_ADD7_PIN                  (GPIO_PIN_13)
    #define SDRAM_ADD8_PORT                 (GPIO_PORT_F)   /* PF14 - EXMC_ADD8 */
    #define SDRAM_ADD8_PIN                  (GPIO_PIN_14)
    #define SDRAM_ADD9_PORT                 (GPIO_PORT_F)   /* PF15 - EXMC_ADD9 */
    #define SDRAM_ADD9_PIN                  (GPIO_PIN_15)
    #define SDRAM_ADD10_PORT                (GPIO_PORT_G)   /* PG00 - EXMC_ADD10 */
    #define SDRAM_ADD10_PIN                 (GPIO_PIN_00)
    #define SDRAM_ADD11_PORT                (GPIO_PORT_G)   /* PG01 - EXMC_ADD11 */
    #define SDRAM_ADD11_PIN                 (GPIO_PIN_01)

    #define SDRAM_DATA0_PORT                (GPIO_PORT_D)   /* PD14 - EXMC_DATA0 */
    #define SDRAM_DATA0_PIN                 (GPIO_PIN_14)
    #define SDRAM_DATA1_PORT                (GPIO_PORT_D)   /* PD15 - EXMC_DATA1 */
    #define SDRAM_DATA1_PIN                 (GPIO_PIN_15)
    #define SDRAM_DATA2_PORT                (GPIO_PORT_D)   /* PD00 - EXMC_DATA2 */
    #define SDRAM_DATA2_PIN                 (GPIO_PIN_00)
    #define SDRAM_DATA3_PORT                (GPIO_PORT_D)   /* PD01 - EXMC_DATA3 */
    #define SDRAM_DATA3_PIN                 (GPIO_PIN_01)
    #define SDRAM_DATA4_PORT                (GPIO_PORT_E)   /* PE07 - EXMC_DATA4 */
    #define SDRAM_DATA4_PIN                 (GPIO_PIN_07)
    #define SDRAM_DATA5_PORT                (GPIO_PORT_E)   /* PE08 - EXMC_DATA5 */
    #define SDRAM_DATA5_PIN                 (GPIO_PIN_08)
    #define SDRAM_DATA6_PORT                (GPIO_PORT_E)   /* PE09 - EXMC_DATA6 */
    #define SDRAM_DATA6_PIN                 (GPIO_PIN_09)
    #define SDRAM_DATA7_PORT                (GPIO_PORT_E)   /* PE10 - EXMC_DATA7 */
    #define SDRAM_DATA7_PIN                 (GPIO_PIN_10)
    #define SDRAM_DATA8_PORT                (GPIO_PORT_E)   /* PE11 - EXMC_DATA8 */
    #define SDRAM_DATA8_PIN                 (GPIO_PIN_11)
    #define SDRAM_DATA9_PORT                (GPIO_PORT_E)   /* PE12 - EXMC_DATA9 */
    #define SDRAM_DATA9_PIN                 (GPIO_PIN_12)
    #define SDRAM_DATA10_PORT               (GPIO_PORT_E)   /* PE13 - EXMC_DATA10 */
    #define SDRAM_DATA10_PIN                (GPIO_PIN_13)
    #define SDRAM_DATA11_PORT               (GPIO_PORT_E)   /* PE14 - EXMC_DATA11 */
    #define SDRAM_DATA11_PIN                (GPIO_PIN_14)
    #define SDRAM_DATA12_PORT               (GPIO_PORT_E)   /* PE15 - EXMC_DATA12 */
    #define SDRAM_DATA12_PIN                (GPIO_PIN_15)
    #define SDRAM_DATA13_PORT               (GPIO_PORT_D)   /* PD08 - EXMC_DATA13 */
    #define SDRAM_DATA13_PIN                (GPIO_PIN_08)
    #define SDRAM_DATA14_PORT               (GPIO_PORT_D)   /* PD09 - EXMC_DATA14 */
    #define SDRAM_DATA14_PIN                (GPIO_PIN_09)
    #define SDRAM_DATA15_PORT               (GPIO_PORT_D)   /* PD10 - EXMC_DATA15 */
    #define SDRAM_DATA15_PIN                (GPIO_PIN_10)
#endif

/************************ RTC *****************************/
#if defined(BSP_USING_RTC)
#if defined(BSP_RTC_USING_XTAL32)
    #define XTAL32_PORT                     (GPIO_PORT_C)
    #define XTAL32_IN_PIN                   (GPIO_PIN_15)
    #define XTAL32_OUT_PIN                  (GPIO_PIN_14)
#endif
#endif

#if defined(RT_USING_PWM)
/*********** PWM_TIMA configure *********/
#if defined(BSP_USING_PWM_TIMA_1)
#if defined(BSP_USING_PWM_TIMA_1_CH1)
    #define PWM_TIMA_1_CH1_PORT             (GPIO_PORT_A)
    #define PWM_TIMA_1_CH1_PIN              (GPIO_PIN_08)
    #define PWM_TIMA_1_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_1_CH2)
    #define PWM_TIMA_1_CH2_PORT             (GPIO_PORT_A)
    #define PWM_TIMA_1_CH2_PIN              (GPIO_PIN_09)
    #define PWM_TIMA_1_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_1_CH3)
    #define PWM_TIMA_1_CH3_PORT             (GPIO_PORT_A)
    #define PWM_TIMA_1_CH3_PIN              (GPIO_PIN_10)
    #define PWM_TIMA_1_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_1_CH4)
    #define PWM_TIMA_1_CH4_PORT             (GPIO_PORT_A)
    #define PWM_TIMA_1_CH4_PIN              (GPIO_PIN_11)
    #define PWM_TIMA_1_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_2)
#if defined(BSP_USING_PWM_TIMA_2_CH1)
    #define PWM_TIMA_2_CH1_PORT             (GPIO_PORT_A)
    #define PWM_TIMA_2_CH1_PIN              (GPIO_PIN_00)
    #define PWM_TIMA_2_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_2_CH2)
    #define PWM_TIMA_2_CH2_PORT             (GPIO_PORT_A)
    #define PWM_TIMA_2_CH2_PIN              (GPIO_PIN_01)
    #define PWM_TIMA_2_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_2_CH3)
    #define PWM_TIMA_2_CH3_PORT             (GPIO_PORT_A)
    #define PWM_TIMA_2_CH3_PIN              (GPIO_PIN_02)
    #define PWM_TIMA_2_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_2_CH4)
    #define PWM_TIMA_2_CH4_PORT             (GPIO_PORT_A)
    #define PWM_TIMA_2_CH4_PIN              (GPIO_PIN_03)
    #define PWM_TIMA_2_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_3)
#if defined(BSP_USING_PWM_TIMA_3_CH1)
    #define PWM_TIMA_3_CH1_PORT             (GPIO_PORT_E)
    #define PWM_TIMA_3_CH1_PIN              (GPIO_PIN_11)
    #define PWM_TIMA_3_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_3_CH2)
    #define PWM_TIMA_3_CH2_PORT             (GPIO_PORT_E)
    #define PWM_TIMA_3_CH2_PIN              (GPIO_PIN_12)
    #define PWM_TIMA_3_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_3_CH3)
    #define PWM_TIMA_3_CH3_PORT             (GPIO_PORT_E)
    #define PWM_TIMA_3_CH3_PIN              (GPIO_PIN_13)
    #define PWM_TIMA_3_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_3_CH4)
    #define PWM_TIMA_3_CH4_PORT             (GPIO_PORT_E)
    #define PWM_TIMA_3_CH4_PIN              (GPIO_PIN_14)
    #define PWM_TIMA_3_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_4)
#if defined(BSP_USING_PWM_TIMA_4_CH1)
    #define PWM_TIMA_4_CH1_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_4_CH1_PIN              (GPIO_PIN_11)
    #define PWM_TIMA_4_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_4_CH2)
    #define PWM_TIMA_4_CH2_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_4_CH2_PIN              (GPIO_PIN_12)
    #define PWM_TIMA_4_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_4_CH3)
    #define PWM_TIMA_4_CH3_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_4_CH3_PIN              (GPIO_PIN_13)
    #define PWM_TIMA_4_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_4_CH4)
    #define PWM_TIMA_4_CH4_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_4_CH4_PIN              (GPIO_PIN_14)
    #define PWM_TIMA_4_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_5)
#if defined(BSP_USING_PWM_TIMA_5_CH1)
    #define PWM_TIMA_5_CH1_PORT             (GPIO_PORT_H)
    #define PWM_TIMA_5_CH1_PIN              (GPIO_PIN_10)
    #define PWM_TIMA_5_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_5_CH2)
    #define PWM_TIMA_5_CH2_PORT             (GPIO_PORT_H)
    #define PWM_TIMA_5_CH2_PIN              (GPIO_PIN_11)
    #define PWM_TIMA_5_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_5_CH3)
    #define PWM_TIMA_5_CH3_PORT             (GPIO_PORT_H)
    #define PWM_TIMA_5_CH3_PIN              (GPIO_PIN_12)
    #define PWM_TIMA_5_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_5_CH4)
    #define PWM_TIMA_5_CH4_PORT             (GPIO_PORT_H)
    #define PWM_TIMA_5_CH4_PIN              (GPIO_PIN_01)
    #define PWM_TIMA_5_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_6)
#if defined(BSP_USING_PWM_TIMA_6_CH1)
    #define PWM_TIMA_6_CH1_PORT             (GPIO_PORT_D)
    #define PWM_TIMA_6_CH1_PIN              (GPIO_PIN_08)
    #define PWM_TIMA_6_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_6_CH2)
    #define PWM_TIMA_6_CH2_PORT             (GPIO_PORT_D)
    #define PWM_TIMA_6_CH2_PIN              (GPIO_PIN_09)
    #define PWM_TIMA_6_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_6_CH3)
    #define PWM_TIMA_6_CH3_PORT             (GPIO_PORT_D)
    #define PWM_TIMA_6_CH3_PIN              (GPIO_PIN_10)
    #define PWM_TIMA_6_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_6_CH4)
    #define PWM_TIMA_6_CH4_PORT             (GPIO_PORT_D)
    #define PWM_TIMA_6_CH4_PIN              (GPIO_PIN_11)
    #define PWM_TIMA_6_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_7)
#if defined(BSP_USING_PWM_TIMA_7_CH1)
    #define PWM_TIMA_7_CH1_PORT             (GPIO_PORT_B)
    #define PWM_TIMA_7_CH1_PIN              (GPIO_PIN_13)
    #define PWM_TIMA_7_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_7_CH2)
    #define PWM_TIMA_7_CH2_PORT             (GPIO_PORT_B)
    #define PWM_TIMA_7_CH2_PIN              (GPIO_PIN_14)
    #define PWM_TIMA_7_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_7_CH3)
    #define PWM_TIMA_7_CH3_PORT             (GPIO_PORT_B)
    #define PWM_TIMA_7_CH3_PIN              (GPIO_PIN_15)
    #define PWM_TIMA_7_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_7_CH4)
    #define PWM_TIMA_7_CH4_PORT             (GPIO_PORT_B)
    #define PWM_TIMA_7_CH4_PIN              (GPIO_PIN_12)
    #define PWM_TIMA_7_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_8)
#if defined(BSP_USING_PWM_TIMA_8_CH1)
    #define PWM_TIMA_8_CH1_PORT             (GPIO_PORT_G)
    #define PWM_TIMA_8_CH1_PIN              (GPIO_PIN_11)
    #define PWM_TIMA_8_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_8_CH2)
    #define PWM_TIMA_8_CH2_PORT             (GPIO_PORT_G)
    #define PWM_TIMA_8_CH2_PIN              (GPIO_PIN_12)
    #define PWM_TIMA_8_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_8_CH3)
    #define PWM_TIMA_8_CH3_PORT             (GPIO_PORT_G)
    #define PWM_TIMA_8_CH3_PIN              (GPIO_PIN_13)
    #define PWM_TIMA_8_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_8_CH4)
    #define PWM_TIMA_8_CH4_PORT             (GPIO_PORT_G)
    #define PWM_TIMA_8_CH4_PIN              (GPIO_PIN_14)
    #define PWM_TIMA_8_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_9)
#if defined(BSP_USING_PWM_TIMA_9_CH1)
    #define PWM_TIMA_9_CH1_PORT             (GPIO_PORT_E)
    #define PWM_TIMA_9_CH1_PIN              (GPIO_PIN_02)
    #define PWM_TIMA_9_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_9_CH2)
    #define PWM_TIMA_9_CH2_PORT             (GPIO_PORT_E)
    #define PWM_TIMA_9_CH2_PIN              (GPIO_PIN_03)
    #define PWM_TIMA_9_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_9_CH3)
    #define PWM_TIMA_9_CH3_PORT             (GPIO_PORT_E)
    #define PWM_TIMA_9_CH3_PIN              (GPIO_PIN_04)
    #define PWM_TIMA_9_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_9_CH4)
    #define PWM_TIMA_9_CH4_PORT             (GPIO_PORT_E)
    #define PWM_TIMA_9_CH4_PIN              (GPIO_PIN_05)
    #define PWM_TIMA_9_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_10)
#if defined(BSP_USING_PWM_TIMA_10_CH1)
    #define PWM_TIMA_10_CH1_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_10_CH1_PIN              (GPIO_PIN_06)
    #define PWM_TIMA_10_CH1_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_10_CH2)
    #define PWM_TIMA_10_CH2_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_10_CH2_PIN              (GPIO_PIN_07)
    #define PWM_TIMA_10_CH2_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_10_CH3)
    #define PWM_TIMA_10_CH3_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_10_CH3_PIN              (GPIO_PIN_08)
    #define PWM_TIMA_10_CH3_PIN_FUNC         (GPIO_FUNC_4)
#endif
#if defined(BSP_USING_PWM_TIMA_10_CH4)
    #define PWM_TIMA_10_CH4_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_10_CH4_PIN              (GPIO_PIN_09)
    #define PWM_TIMA_10_CH4_PIN_FUNC         (GPIO_FUNC_4)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_11)
#if defined(BSP_USING_PWM_TIMA_11_CH1)
    #define PWM_TIMA_11_CH1_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_11_CH1_PIN              (GPIO_PIN_00)
    #define PWM_TIMA_11_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_11_CH2)
    #define PWM_TIMA_11_CH2_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_11_CH2_PIN              (GPIO_PIN_01)
    #define PWM_TIMA_11_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_11_CH3)
    #define PWM_TIMA_11_CH3_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_11_CH3_PIN              (GPIO_PIN_02)
    #define PWM_TIMA_11_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_11_CH4)
    #define PWM_TIMA_11_CH4_PORT             (GPIO_PORT_F)
    #define PWM_TIMA_11_CH4_PIN              (GPIO_PIN_03)
    #define PWM_TIMA_11_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

#if defined(BSP_USING_PWM_TIMA_12)
#if defined(BSP_USING_PWM_TIMA_12_CH1)
    #define PWM_TIMA_12_CH1_PORT             (GPIO_PORT_D)
    #define PWM_TIMA_12_CH1_PIN              (GPIO_PIN_01)
    #define PWM_TIMA_12_CH1_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_12_CH2)
    #define PWM_TIMA_12_CH2_PORT             (GPIO_PORT_D)
    #define PWM_TIMA_12_CH2_PIN              (GPIO_PIN_02)
    #define PWM_TIMA_12_CH2_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_12_CH3)
    #define PWM_TIMA_12_CH3_PORT             (GPIO_PORT_D)
    #define PWM_TIMA_12_CH3_PIN              (GPIO_PIN_03)
    #define PWM_TIMA_12_CH3_PIN_FUNC         (GPIO_FUNC_5)
#endif
#if defined(BSP_USING_PWM_TIMA_12_CH4)
    #define PWM_TIMA_12_CH4_PORT             (GPIO_PORT_D)
    #define PWM_TIMA_12_CH4_PIN              (GPIO_PIN_04)
    #define PWM_TIMA_12_CH4_PIN_FUNC         (GPIO_FUNC_5)
#endif
#endif

/*********** PWM_TIM4 configure *********/
#if defined(BSP_USING_PWM_TIM4_1)
#if defined(BSP_USING_PWM_TIM4_1_OUH)
    #define PWM_TIM4_1_OUH_PORT             (GPIO_PORT_E)
    #define PWM_TIM4_1_OUH_PIN              (GPIO_PIN_09)
    #define PWM_TIM4_1_OUH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_1_OUL)
    #define PWM_TIM4_1_OUL_PORT             (GPIO_PORT_E)
    #define PWM_TIM4_1_OUL_PIN              (GPIO_PIN_08)
    #define PWM_TIM4_1_OUL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_1_OVH)
    #define PWM_TIM4_1_OVH_PORT             (GPIO_PORT_E)
    #define PWM_TIM4_1_OVH_PIN              (GPIO_PIN_11)
    #define PWM_TIM4_1_OVH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_1_OVL)
    #define PWM_TIM4_1_OVL_PORT             (GPIO_PORT_E)
    #define PWM_TIM4_1_OVL_PIN              (GPIO_PIN_10)
    #define PWM_TIM4_1_OVL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_1_OWH)
    #define PWM_TIM4_1_OWH_PORT             (GPIO_PORT_E)
    #define PWM_TIM4_1_OWH_PIN              (GPIO_PIN_13)
    #define PWM_TIM4_1_OWH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_1_OWL)
    #define PWM_TIM4_1_OWL_PORT             (GPIO_PORT_E)
    #define PWM_TIM4_1_OWL_PIN              (GPIO_PIN_12)
    #define PWM_TIM4_1_OWL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#endif
#if defined(BSP_USING_PWM_TIM4_2)
#if defined(BSP_USING_PWM_TIM4_2_OUH)
    #define PWM_TIM4_2_OUH_PORT             (GPIO_PORT_A)
    #define PWM_TIM4_2_OUH_PIN              (GPIO_PIN_00)
    #define PWM_TIM4_2_OUH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_2_OUL)
    #define PWM_TIM4_2_OUL_PORT             (GPIO_PORT_A)
    #define PWM_TIM4_2_OUL_PIN              (GPIO_PIN_01)
    #define PWM_TIM4_2_OUL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_2_OVH)
    #define PWM_TIM4_2_OVH_PORT             (GPIO_PORT_A)
    #define PWM_TIM4_2_OVH_PIN              (GPIO_PIN_02)
    #define PWM_TIM4_2_OVH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_2_OVL)
    #define PWM_TIM4_2_OVL_PORT             (GPIO_PORT_A)
    #define PWM_TIM4_2_OVL_PIN              (GPIO_PIN_03)
    #define PWM_TIM4_2_OVL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_2_OWH)
    #define PWM_TIM4_2_OWH_PORT             (GPIO_PORT_A)
    #define PWM_TIM4_2_OWH_PIN              (GPIO_PIN_04)
    #define PWM_TIM4_2_OWH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_2_OWL)
    #define PWM_TIM4_2_OWL_PORT             (GPIO_PORT_A)
    #define PWM_TIM4_2_OWL_PIN              (GPIO_PIN_05)
    #define PWM_TIM4_2_OWL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#endif
#if defined(BSP_USING_PWM_TIM4_3)
#if defined(BSP_USING_PWM_TIM4_3_OUH)
    #define PWM_TIM4_3_OUH_PORT             (GPIO_PORT_C)
    #define PWM_TIM4_3_OUH_PIN              (GPIO_PIN_10)
    #define PWM_TIM4_3_OUH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_3_OUL)
    #define PWM_TIM4_3_OUL_PORT             (GPIO_PORT_D)
    #define PWM_TIM4_3_OUL_PIN              (GPIO_PIN_00)
    #define PWM_TIM4_3_OUL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_3_OVH)
    #define PWM_TIM4_3_OVH_PORT             (GPIO_PORT_C)
    #define PWM_TIM4_3_OVH_PIN              (GPIO_PIN_11)
    #define PWM_TIM4_3_OVH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_3_OVL)
    #define PWM_TIM4_3_OVL_PORT             (GPIO_PORT_D)
    #define PWM_TIM4_3_OVL_PIN              (GPIO_PIN_01)
    #define PWM_TIM4_3_OVL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_3_OWH)
    #define PWM_TIM4_3_OWH_PORT             (GPIO_PORT_C)
    #define PWM_TIM4_3_OWH_PIN              (GPIO_PIN_12)
    #define PWM_TIM4_3_OWH_PIN_FUNC         (GPIO_FUNC_2)
#endif
#if defined(BSP_USING_PWM_TIM4_3_OWL)
    #define PWM_TIM4_3_OWL_PORT             (GPIO_PORT_D)
    #define PWM_TIM4_3_OWL_PIN              (GPIO_PIN_02)
    #define PWM_TIM4_3_OWL_PIN_FUNC         (GPIO_FUNC_2)
#endif
#endif

/*********** PWM_TIM6 configure *********/
#if defined(BSP_USING_PWM_TIM6_1)
#if defined(BSP_USING_PWM_TIM6_1_A)
    #define PWM_TIM6_1_A_PORT               (GPIO_PORT_F)
    #define PWM_TIM6_1_A_PIN                (GPIO_PIN_13)
    #define PWM_TIM6_1_A_PIN_FUNC           (GPIO_FUNC_3)
#endif
#if defined(BSP_USING_PWM_TIM6_1_B)
    #define PWM_TIM6_1_B_PORT               (GPIO_PORT_F)
    #define PWM_TIM6_1_B_PIN                (GPIO_PIN_14)
    #define PWM_TIM6_1_B_PIN_FUNC           (GPIO_FUNC_3)
#endif
#endif
#if defined(BSP_USING_PWM_TIM6_2)
#if defined(BSP_USING_PWM_TIM6_2_A)
    #define PWM_TIM6_2_A_PORT               (GPIO_PORT_E)
    #define PWM_TIM6_2_A_PIN                (GPIO_PIN_11)
    #define PWM_TIM6_2_A_PIN_FUNC           (GPIO_FUNC_3)
#endif
#if defined(BSP_USING_PWM_TIM6_2_B)
    #define PWM_TIM6_2_B_PORT               (GPIO_PORT_E)
    #define PWM_TIM6_2_B_PIN                (GPIO_PIN_10)
    #define PWM_TIM6_2_B_PIN_FUNC           (GPIO_FUNC_3)
#endif
#endif
#if defined(BSP_USING_PWM_TIM6_3)
#if defined(BSP_USING_PWM_TIM6_3_A)
    #define PWM_TIM6_3_A_PORT               (GPIO_PORT_E)
    #define PWM_TIM6_3_A_PIN                (GPIO_PIN_13)
    #define PWM_TIM6_3_A_PIN_FUNC           (GPIO_FUNC_3)
#endif
#if defined(BSP_USING_PWM_TIM6_3_B)
    #define PWM_TIM6_3_B_PORT               (GPIO_PORT_E)
    #define PWM_TIM6_3_B_PIN                (GPIO_PIN_12)
    #define PWM_TIM6_3_B_PIN_FUNC           (GPIO_FUNC_3)
#endif
#endif
#if defined(BSP_USING_PWM_TIM6_4)
#if defined(BSP_USING_PWM_TIM6_4_A)
    #define PWM_TIM6_4_A_PORT               (GPIO_PORT_A)
    #define PWM_TIM6_4_A_PIN                (GPIO_PIN_11)
    #define PWM_TIM6_4_A_PIN_FUNC           (GPIO_FUNC_3)
#endif
#if defined(BSP_USING_PWM_TIM6_4_B)
    #define PWM_TIM6_4_B_PORT               (GPIO_PORT_G)
    #define PWM_TIM6_4_B_PIN                (GPIO_PIN_14)
    #define PWM_TIM6_4_B_PIN_FUNC           (GPIO_FUNC_3)
#endif
#endif
#if defined(BSP_USING_PWM_TIM6_5)
#if defined(BSP_USING_PWM_TIM6_5_A)
    #define PWM_TIM6_5_A_PORT               (GPIO_PORT_C)
    #define PWM_TIM6_5_A_PIN                (GPIO_PIN_04)
    #define PWM_TIM6_5_A_PIN_FUNC           (GPIO_FUNC_3)
#endif
#if defined(BSP_USING_PWM_TIM6_5_B)
    #define PWM_TIM6_5_B_PORT               (GPIO_PORT_C)
    #define PWM_TIM6_5_B_PIN                (GPIO_PIN_05)
    #define PWM_TIM6_5_B_PIN_FUNC           (GPIO_FUNC_3)
#endif
#endif
#if defined(BSP_USING_PWM_TIM6_6)
#if defined(BSP_USING_PWM_TIM6_6_A)
    #define PWM_TIM6_6_A_PORT               (GPIO_PORT_A)
    #define PWM_TIM6_6_A_PIN                (GPIO_PIN_02)
    #define PWM_TIM6_6_A_PIN_FUNC           (GPIO_FUNC_3)
#endif
#if defined(BSP_USING_PWM_TIM6_6_B)
    #define PWM_TIM6_6_B_PORT               (GPIO_PORT_A)
    #define PWM_TIM6_6_B_PIN                (GPIO_PIN_03)
    #define PWM_TIM6_6_B_PIN_FUNC           (GPIO_FUNC_3)
#endif
#endif
#if defined(BSP_USING_PWM_TIM6_7)
#if defined(BSP_USING_PWM_TIM6_7_A)
    #define PWM_TIM6_7_A_PORT               (GPIO_PORT_A)
    #define PWM_TIM6_7_A_PIN                (GPIO_PIN_04)
    #define PWM_TIM6_7_A_PIN_FUNC           (GPIO_FUNC_3)
#endif
#if defined(BSP_USING_PWM_TIM6_7_B)
    #define PWM_TIM6_7_B_PORT               (GPIO_PORT_A)
    #define PWM_TIM6_7_B_PIN                (GPIO_PIN_05)
    #define PWM_TIM6_7_B_PIN_FUNC           (GPIO_FUNC_3)
#endif
#endif
#if defined(BSP_USING_PWM_TIM6_8)
#if defined(BSP_USING_PWM_TIM6_8_A)
    #define PWM_TIM6_8_A_PORT               (GPIO_PORT_C)
    #define PWM_TIM6_8_A_PIN                (GPIO_PIN_08)
    #define PWM_TIM6_8_A_PIN_FUNC           (GPIO_FUNC_3)
#endif
#if defined(BSP_USING_PWM_TIM6_8_B)
    #define PWM_TIM6_8_B_PORT               (GPIO_PORT_C)
    #define PWM_TIM6_8_B_PIN                (GPIO_PIN_09)
    #define PWM_TIM6_8_B_PIN_FUNC           (GPIO_FUNC_3)
#endif
#endif

#endif


#if defined(BSP_USING_USBD) || defined(BSP_USING_USBH)
#if defined(BSP_USING_USBFS)
    /* USBFS Core*/
    #define USBF_DP_PORT                    (GPIO_PORT_A)
    #define USBF_DP_PIN                     (GPIO_PIN_12)
    #define USBF_DM_PORT                    (GPIO_PORT_A)
    #define USBF_DM_PIN                     (GPIO_PIN_11)
    #define USBF_VBUS_PORT                  (GPIO_PORT_A)
    #define USBF_VBUS_PIN                   (GPIO_PIN_09)
    #define USBF_DRVVBUS_PORT               (GPIO_PORT_C)
    #define USBF_DRVVBUS_PIN                (GPIO_PIN_09)
#elif defined(BSP_USING_USBHS)
/* USBHS Core*/
#if defined(BSP_USING_USBHS_PHY_EMBED)
    #define USBH_DP_PORT                    (GPIO_PORT_B)
    #define USBH_DP_PIN                     (GPIO_PIN_15)
    #define USBH_DM_PORT                    (GPIO_PORT_B)
    #define USBH_DM_PIN                     (GPIO_PIN_14)
    #define USBH_VBUS_PORT                  (GPIO_PORT_B)
    #define USBH_VBUS_PIN                   (GPIO_PIN_13)
    #define USBF_DRVVBUS_PORT               (GPIO_PORT_B)
    #define USBF_DRVVBUS_PIN                (GPIO_PIN_11)
#else
    /* USBHS Core, external PHY */
    #define USBH_ULPI_CLK_PORT              (GPIO_PORT_E)
    #define USBH_ULPI_CLK_PIN               (GPIO_PIN_12)
    #define USBH_ULPI_DIR_PORT              (GPIO_PORT_C)
    #define USBH_ULPI_DIR_PIN               (GPIO_PIN_02)
    #define USBH_ULPI_NXT_PORT              (GPIO_PORT_C)
    #define USBH_ULPI_NXT_PIN               (GPIO_PIN_03)
    #define USBH_ULPI_STP_PORT              (GPIO_PORT_C)
    #define USBH_ULPI_STP_PIN               (GPIO_PIN_00)
    #define USBH_ULPI_D0_PORT               (GPIO_PORT_E)
    #define USBH_ULPI_D0_PIN                (GPIO_PIN_13)
    #define USBH_ULPI_D1_PORT               (GPIO_PORT_E)
    #define USBH_ULPI_D1_PIN                (GPIO_PIN_14)
    #define USBH_ULPI_D2_PORT               (GPIO_PORT_E)
    #define USBH_ULPI_D2_PIN                (GPIO_PIN_15)
    #define USBH_ULPI_D3_PORT               (GPIO_PORT_B)
    #define USBH_ULPI_D3_PIN                (GPIO_PIN_10)
    #define USBH_ULPI_D4_PORT               (GPIO_PORT_B)
    #define USBH_ULPI_D4_PIN                (GPIO_PIN_11)
    #define USBH_ULPI_D5_PORT               (GPIO_PORT_B)
    #define USBH_ULPI_D5_PIN                (GPIO_PIN_12)
    #define USBH_ULPI_D6_PORT               (GPIO_PORT_B)
    #define USBH_ULPI_D6_PIN                (GPIO_PIN_13)
    #define USBH_ULPI_D7_PORT               (GPIO_PORT_E)
    #define USBH_ULPI_D7_PIN                (GPIO_PIN_11)
    /* 3300 reset */                      
    #define USB_3300_RESET_PORT             (EIO_PORT1)
    #define USB_3300_RESET_PIN              (EIO_USB3300_RST)
#endif
#endif
#endif

/*********** TIMA_PULSE_ENCODER configure *********/
#if defined(RT_USING_PULSE_ENCODER)
#if defined(BSP_USING_TIMERA_PULSE_ENCODER)
#if defined(BSP_USING_PULSE_ENCODER_TIMA_1)
    #define PULSE_ENCODER_TIMA_1_A_PORT      (GPIO_PORT_A)
    #define PULSE_ENCODER_TIMA_1_A_PIN       (GPIO_PIN_08)
    #define PULSE_ENCODER_TIMA_1_A_PIN_FUNC  (GPIO_FUNC_4)
    #define PULSE_ENCODER_TIMA_1_B_PORT      (GPIO_PORT_A)
    #define PULSE_ENCODER_TIMA_1_B_PIN       (GPIO_PIN_09)
    #define PULSE_ENCODER_TIMA_1_B_PIN_FUNC  (GPIO_FUNC_4)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_1 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_2)
    #define PULSE_ENCODER_TIMA_2_A_PORT      (GPIO_PORT_A)
    #define PULSE_ENCODER_TIMA_2_A_PIN       (GPIO_PIN_00)
    #define PULSE_ENCODER_TIMA_2_A_PIN_FUNC  (GPIO_FUNC_4)
    #define PULSE_ENCODER_TIMA_2_B_PORT      (GPIO_PORT_A)
    #define PULSE_ENCODER_TIMA_2_B_PIN       (GPIO_PIN_01)
    #define PULSE_ENCODER_TIMA_2_B_PIN_FUNC  (GPIO_FUNC_4)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_2 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_3)
    #define PULSE_ENCODER_TIMA_3_A_PORT      (GPIO_PORT_B)
    #define PULSE_ENCODER_TIMA_3_A_PIN       (GPIO_PIN_04)
    #define PULSE_ENCODER_TIMA_3_A_PIN_FUNC  (GPIO_FUNC_4)
    #define PULSE_ENCODER_TIMA_3_B_PORT      (GPIO_PORT_B)
    #define PULSE_ENCODER_TIMA_3_B_PIN       (GPIO_PIN_05)
    #define PULSE_ENCODER_TIMA_3_B_PIN_FUNC  (GPIO_FUNC_4)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_3 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_4)
    #define PULSE_ENCODER_TIMA_4_A_PORT      (GPIO_PORT_B)
    #define PULSE_ENCODER_TIMA_4_A_PIN       (GPIO_PIN_06)
    #define PULSE_ENCODER_TIMA_4_A_PIN_FUNC  (GPIO_FUNC_4)
    #define PULSE_ENCODER_TIMA_4_B_PORT      (GPIO_PORT_B)
    #define PULSE_ENCODER_TIMA_4_B_PIN       (GPIO_PIN_07)
    #define PULSE_ENCODER_TIMA_4_B_PIN_FUNC  (GPIO_FUNC_4)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_4 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_5)
    #define PULSE_ENCODER_TIMA_5_A_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIMA_5_A_PIN       (GPIO_PIN_10)
    #define PULSE_ENCODER_TIMA_5_A_PIN_FUNC  (GPIO_FUNC_5)
    #define PULSE_ENCODER_TIMA_5_B_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIMA_5_B_PIN       (GPIO_PIN_11)
    #define PULSE_ENCODER_TIMA_5_B_PIN_FUNC  (GPIO_FUNC_5)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_5 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_6)
    #define PULSE_ENCODER_TIMA_6_A_PORT      (GPIO_PORT_D)
    #define PULSE_ENCODER_TIMA_6_A_PIN       (GPIO_PIN_04)
    #define PULSE_ENCODER_TIMA_6_A_PIN_FUNC  (GPIO_FUNC_4)
    #define PULSE_ENCODER_TIMA_6_B_PORT      (GPIO_PORT_D)
    #define PULSE_ENCODER_TIMA_6_B_PIN       (GPIO_PIN_05)
    #define PULSE_ENCODER_TIMA_6_B_PIN_FUNC  (GPIO_FUNC_4)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_6 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_7)
    #define PULSE_ENCODER_TIMA_7_A_PORT      (GPIO_PORT_B)
    #define PULSE_ENCODER_TIMA_7_A_PIN       (GPIO_PIN_13)
    #define PULSE_ENCODER_TIMA_7_A_PIN_FUNC  (GPIO_FUNC_4)
    #define PULSE_ENCODER_TIMA_7_B_PORT      (GPIO_PORT_B)
    #define PULSE_ENCODER_TIMA_7_B_PIN       (GPIO_PIN_14)
    #define PULSE_ENCODER_TIMA_7_B_PIN_FUNC  (GPIO_FUNC_4)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_7 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_8)
    #define PULSE_ENCODER_TIMA_8_A_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIMA_8_A_PIN       (GPIO_PIN_00)
    #define PULSE_ENCODER_TIMA_8_A_PIN_FUNC  (GPIO_FUNC_4)
    #define PULSE_ENCODER_TIMA_8_B_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIMA_8_B_PIN       (GPIO_PIN_01)
    #define PULSE_ENCODER_TIMA_8_B_PIN_FUNC  (GPIO_FUNC_4)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_8 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_9)
    #define PULSE_ENCODER_TIMA_9_A_PORT      (GPIO_PORT_G)
    #define PULSE_ENCODER_TIMA_9_A_PIN       (GPIO_PIN_04)
    #define PULSE_ENCODER_TIMA_9_A_PIN_FUNC  (GPIO_FUNC_4)
    #define PULSE_ENCODER_TIMA_9_B_PORT      (GPIO_PORT_G)
    #define PULSE_ENCODER_TIMA_9_B_PIN       (GPIO_PIN_05)
    #define PULSE_ENCODER_TIMA_9_B_PIN_FUNC  (GPIO_FUNC_4)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_9 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_10)
    #define PULSE_ENCODER_TIMA_10_A_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIMA_10_A_PIN       (GPIO_PIN_14)
    #define PULSE_ENCODER_TIMA_10_A_PIN_FUNC  (GPIO_FUNC_04)
    #define PULSE_ENCODER_TIMA_10_B_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIMA_10_B_PIN       (GPIO_PIN_15)
    #define PULSE_ENCODER_TIMA_10_B_PIN_FUNC  (GPIO_FUNC_04)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_10 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_11)
    #define PULSE_ENCODER_TIMA_11_A_PORT      (GPIO_PORT_F)
    #define PULSE_ENCODER_TIMA_11_A_PIN       (GPIO_PIN_00)
    #define PULSE_ENCODER_TIMA_11_A_PIN_FUNC  (GPIO_FUNC_05)
    #define PULSE_ENCODER_TIMA_11_B_PORT      (GPIO_PORT_F)
    #define PULSE_ENCODER_TIMA_11_B_PIN       (GPIO_PIN_01)
    #define PULSE_ENCODER_TIMA_11_B_PIN_FUNC  (GPIO_FUNC_05)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_11 */
#if defined(BSP_USING_PULSE_ENCODER_TIMA_12)
    #define PULSE_ENCODER_TIMA_12_A_PORT      (GPIO_PORT_D)
    #define PULSE_ENCODER_TIMA_12_A_PIN       (GPIO_PIN_01)
    #define PULSE_ENCODER_TIMA_12_A_PIN_FUNC  (GPIO_FUNC_05)
    #define PULSE_ENCODER_TIMA_12_B_PORT      (GPIO_PORT_D)
    #define PULSE_ENCODER_TIMA_12_B_PIN       (GPIO_PIN_02)
    #define PULSE_ENCODER_TIMA_12_B_PIN_FUNC  (GPIO_FUNC_05)
#endif /* BSP_USING_PULSE_ENCODER_TIMA_12 */
#endif /* BSP_USING_TIMERA_PULSE_ENCODER */

#if defined(BSP_USING_TIMER6_PULSE_ENCODER)
#if defined(BSP_USING_PULSE_ENCODER_TIM6_1)
    #define PULSE_ENCODER_TIM6_1_A_PORT      (GPIO_PORT_B)
    #define PULSE_ENCODER_TIM6_1_A_PIN       (GPIO_PIN_09)
    #define PULSE_ENCODER_TIM6_1_A_PIN_FUNC  (GPIO_FUNC_3)
    #define PULSE_ENCODER_TIM6_1_B_PORT      (GPIO_PORT_B)
    #define PULSE_ENCODER_TIM6_1_B_PIN       (GPIO_PIN_08)
    #define PULSE_ENCODER_TIM6_1_B_PIN_FUNC  (GPIO_FUNC_3)
#endif /* BSP_USING_PULSE_ENCODER_TIM6_1 */
#if defined(BSP_USING_PULSE_ENCODER_TIM6_2)
    #define PULSE_ENCODER_TIM6_2_A_PORT      (GPIO_PORT_E)
    #define PULSE_ENCODER_TIM6_2_A_PIN       (GPIO_PIN_11)
    #define PULSE_ENCODER_TIM6_2_A_PIN_FUNC  (GPIO_FUNC_3)
    #define PULSE_ENCODER_TIM6_2_B_PORT      (GPIO_PORT_E)
    #define PULSE_ENCODER_TIM6_2_B_PIN       (GPIO_PIN_10)
    #define PULSE_ENCODER_TIM6_2_B_PIN_FUNC  (GPIO_FUNC_3)
#endif /* BSP_USING_PULSE_ENCODER_TIM6_2 */
#if defined(BSP_USING_PULSE_ENCODER_TIM6_3)
    #define PULSE_ENCODER_TIM6_3_A_PORT      (GPIO_PORT_E)
    #define PULSE_ENCODER_TIM6_3_A_PIN       (GPIO_PIN_13)
    #define PULSE_ENCODER_TIM6_3_A_PIN_FUNC  (GPIO_FUNC_3)
    #define PULSE_ENCODER_TIM6_3_B_PORT      (GPIO_PORT_E)
    #define PULSE_ENCODER_TIM6_3_B_PIN       (GPIO_PIN_12)
    #define PULSE_ENCODER_TIM6_3_B_PIN_FUNC  (GPIO_FUNC_3)
#endif /* BSP_USING_PULSE_ENCODER_TIM6_3 */
#if defined(BSP_USING_PULSE_ENCODER_TIM6_4)
    #define PULSE_ENCODER_TIM6_4_A_PORT      (GPIO_PORT_H)
    #define PULSE_ENCODER_TIM6_4_A_PIN       (GPIO_PIN_10)
    #define PULSE_ENCODER_TIM6_4_A_PIN_FUNC  (GPIO_FUNC_3)
    #define PULSE_ENCODER_TIM6_4_B_PORT      (GPIO_PORT_H)
    #define PULSE_ENCODER_TIM6_4_B_PIN       (GPIO_PIN_11)
    #define PULSE_ENCODER_TIM6_4_B_PIN_FUNC  (GPIO_FUNC_3)
#endif /* BSP_USING_PULSE_ENCODER_TIM6_4 */
#if defined(BSP_USING_PULSE_ENCODER_TIM6_5)
    #define PULSE_ENCODER_TIM6_5_A_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIM6_5_A_PIN       (GPIO_PIN_04)
    #define PULSE_ENCODER_TIM6_5_A_PIN_FUNC  (GPIO_FUNC_3)
    #define PULSE_ENCODER_TIM6_5_B_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIM6_5_B_PIN       (GPIO_PIN_05)
    #define PULSE_ENCODER_TIM6_5_B_PIN_FUNC  (GPIO_FUNC_3)
#endif /* BSP_USING_PULSE_ENCODER_TIM6_5 */
#if defined(BSP_USING_PULSE_ENCODER_TIM6_6)
    #define PULSE_ENCODER_TIM6_6_A_PORT      (GPIO_PORT_A)
    #define PULSE_ENCODER_TIM6_6_A_PIN       (GPIO_PIN_02)
    #define PULSE_ENCODER_TIM6_6_A_PIN_FUNC  (GPIO_FUNC_3)
    #define PULSE_ENCODER_TIM6_6_B_PORT      (GPIO_PORT_A)
    #define PULSE_ENCODER_TIM6_6_B_PIN       (GPIO_PIN_03)
    #define PULSE_ENCODER_TIM6_6_B_PIN_FUNC  (GPIO_FUNC_3)
#endif /* BSP_USING_PULSE_ENCODER_TIM6_6 */
#if defined(BSP_USING_PULSE_ENCODER_TIM6_7)
    #define PULSE_ENCODER_TIM6_7_A_PORT      (GPIO_PORT_A)
    #define PULSE_ENCODER_TIM6_7_A_PIN       (GPIO_PIN_04)
    #define PULSE_ENCODER_TIM6_7_A_PIN_FUNC  (GPIO_FUNC_3)
    #define PULSE_ENCODER_TIM6_7_B_PORT      (GPIO_PORT_A)
    #define PULSE_ENCODER_TIM6_7_B_PIN       (GPIO_PIN_05)
    #define PULSE_ENCODER_TIM6_7_B_PIN_FUNC  (GPIO_FUNC_3)
#endif /* BSP_USING_PULSE_ENCODER_TIM6_7 */
#if defined(BSP_USING_PULSE_ENCODER_TIM6_8)
    #define PULSE_ENCODER_TIM6_8_A_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIM6_8_A_PIN       (GPIO_PIN_08)
    #define PULSE_ENCODER_TIM6_8_A_PIN_FUNC  (GPIO_FUNC_3)
    #define PULSE_ENCODER_TIM6_8_B_PORT      (GPIO_PORT_C)
    #define PULSE_ENCODER_TIM6_8_B_PIN       (GPIO_PIN_09)
    #define PULSE_ENCODER_TIM6_8_B_PIN_FUNC  (GPIO_FUNC_3)
#endif /* BSP_USING_PULSE_ENCODER_TIM6_8 */
#endif /* BSP_USING_TIMER6_PULSE_ENCODER */
#endif /* RT_USING_PULSE_ENCODER */

#endif



