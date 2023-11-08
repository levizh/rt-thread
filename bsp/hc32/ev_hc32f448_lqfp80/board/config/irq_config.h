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

#ifndef __IRQ_CONFIG_H__
#define __IRQ_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_EXTINT0_IRQ_NUM             EXTINT_PORT_EIRQ0_IRQn
#define BSP_EXTINT0_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT1_IRQ_NUM             EXTINT_PORT_EIRQ1_IRQn
#define BSP_EXTINT1_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT2_IRQ_NUM             EXTINT_PORT_EIRQ2_IRQn
#define BSP_EXTINT2_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT3_IRQ_NUM             EXTINT_PORT_EIRQ3_IRQn
#define BSP_EXTINT3_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT4_IRQ_NUM             EXTINT_PORT_EIRQ4_IRQn
#define BSP_EXTINT4_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT5_IRQ_NUM             EXTINT_PORT_EIRQ5_IRQn
#define BSP_EXTINT5_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT6_IRQ_NUM             EXTINT_PORT_EIRQ6_IRQn
#define BSP_EXTINT6_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT7_IRQ_NUM             EXTINT_PORT_EIRQ7_IRQn
#define BSP_EXTINT7_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT8_IRQ_NUM             EXTINT_PORT_EIRQ8_IRQn
#define BSP_EXTINT8_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT9_IRQ_NUM             EXTINT_PORT_EIRQ9_IRQn
#define BSP_EXTINT9_IRQ_PRIO            DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT10_IRQ_NUM            EXTINT_PORT_EIRQ10_IRQn
#define BSP_EXTINT10_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT11_IRQ_NUM            EXTINT_PORT_EIRQ11_IRQn
#define BSP_EXTINT11_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT12_IRQ_NUM            EXTINT_PORT_EIRQ12_IRQn
#define BSP_EXTINT12_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT13_IRQ_NUM            EXTINT_PORT_EIRQ13_IRQn
#define BSP_EXTINT13_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT14_IRQ_NUM            EXTINT_PORT_EIRQ14_IRQn
#define BSP_EXTINT14_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
#define BSP_EXTINT15_IRQ_NUM            EXTINT_PORT_EIRQ15_IRQn
#define BSP_EXTINT15_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

/* DMA1 ch0 */
#define BSP_DMA1_CH0_IRQ_NUM            DMA1_TC0_BTC0_IRQn
#define BSP_DMA1_CH0_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA1 ch1 */
#define BSP_DMA1_CH1_IRQ_NUM            DMA1_TC1_BTC1_IRQn
#define BSP_DMA1_CH1_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA1 ch2 */
#define BSP_DMA1_CH2_IRQ_NUM            DMA1_TC2_BTC2_IRQn
#define BSP_DMA1_CH2_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA1 ch3 */
#define BSP_DMA1_CH3_IRQ_NUM            DMA1_TC3_BTC3_IRQn
#define BSP_DMA1_CH3_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA1 ch4 */
#define BSP_DMA1_CH4_IRQ_NUM            DMA1_TC4_BTC4_IRQn
#define BSP_DMA1_CH4_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA1 ch5 */
#define BSP_DMA1_CH5_IRQ_NUM            DMA1_TC5_BTC5_IRQn
#define BSP_DMA1_CH5_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

/* DMA2 ch0 */
#define BSP_DMA2_CH0_IRQ_NUM            DMA2_TC0_BTC0_IRQn
#define BSP_DMA2_CH0_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA2 ch1 */
#define BSP_DMA2_CH1_IRQ_NUM            DMA2_TC1_BTC1_IRQn
#define BSP_DMA2_CH1_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA2 ch2 */
#define BSP_DMA2_CH2_IRQ_NUM            DMA2_TC2_BTC2_IRQn
#define BSP_DMA2_CH2_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA2 ch3 */
#define BSP_DMA2_CH3_IRQ_NUM            DMA2_TC3_BTC3_IRQn
#define BSP_DMA2_CH3_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA2 ch4 */
#define BSP_DMA2_CH4_IRQ_NUM            DMA2_TC4_BTC4_IRQn
#define BSP_DMA2_CH4_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT
/* DMA2 ch5 */
#define BSP_DMA2_CH5_IRQ_NUM            DMA2_TC5_BTC5_IRQn
#define BSP_DMA2_CH5_IRQ_PRIO           DDL_IRQ_PRIO_DEFAULT

//todo UART IRQn definition
#if defined(BSP_USING_UART1)
#define BSP_UART1_IRQ_NUM               USART1_IRQn
#define BSP_UART1_IRQ_PRIO              DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_UART1_RX_USING_DMA)
#define BSP_UART1_RXTO_IRQ_NUM          USART1_IRQn
#define BSP_UART1_RXTO_IRQ_PRIO         DDL_IRQ_PRIO_DEFAULT
#endif
#if defined(BSP_UART1_TX_USING_DMA)
#define BSP_UART1_TX_CPLT_IRQ_NUM       USART1_IRQn
#define BSP_UART1_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
#define BSP_UART2_IRQ_NUM               USART2_IRQn
#define BSP_UART2_IRQ_PRIO              DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_UART2_RX_USING_DMA)
#define BSP_UART2_RXTO_IRQ_NUM          USART2_IRQn
#define BSP_UART2_RXTO_IRQ_PRIO         DDL_IRQ_PRIO_DEFAULT
#endif
#if defined(BSP_UART2_TX_USING_DMA)
#define BSP_UART2_TX_CPLT_IRQ_NUM       USART2_TCI_IRQn
#define BSP_UART2_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
#define BSP_UART3_IRQ_NUM               USART3_IRQn
#define BSP_UART3_IRQ_PRIO              DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_UART3_RX_USING_DMA)
#define BSP_UART3_RXTO_IRQ_NUM          USART3_IRQn
#define BSP_UART3_RXTO_IRQ_PRIO         DDL_IRQ_PRIO_DEFAULT
#endif
#if defined(BSP_UART3_TX_USING_DMA)
#define BSP_UART3_TX_CPLT_IRQ_NUM       USART3_IRQn
#define BSP_UART3_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif
#endif /* BSP_USING_UART3 */

#if defined(BSP_USING_UART4)
#define BSP_UART4_IRQ_NUM               USART4_IRQn
#define BSP_UART4_IRQ_PRIO              DDL_IRQ_PRIO_DEFAULT

#if defined(BSP_UART4_RX_USING_DMA)
#define BSP_UART4_RXTO_IRQ_NUM          USART4_IRQn
#define BSP_UART4_RXTO_IRQ_PRIO         DDL_IRQ_PRIO_DEFAULT
#endif
#if defined(BSP_UART4_TX_USING_DMA)
#define BSP_UART4_TX_CPLT_IRQ_NUM       USART4_IRQn
#define BSP_UART4_TX_CPLT_IRQ_PRIO      DDL_IRQ_PRIO_DEFAULT
#endif
#endif /* BSP_USING_UART4 */

#if defined(BSP_USING_CAN1)
#define BSP_CAN1_IRQ_NUM                MCAN1_INT0_IRQn
#define BSP_CAN1_IRQ_PRIO               DDL_IRQ_PRIO_DEFAULT
#endif/* BSP_USING_CAN1 */

#if defined(RT_USING_ALARM)
#define BSP_RTC_ALARM_IRQ_NUM           RTC_IRQn
#define BSP_RTC_ALARM_IRQ_PRIO          DDL_IRQ_PRIO_DEFAULT
#endif/* RT_USING_ALARM */

#ifdef __cplusplus
}
#endif

#endif /* __IRQ_CONFIG_H__ */
