/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-06-15     CDT          first version
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_QSPI

#if defined(BSP_USING_QSPI)

#include "drv_qspi.h"
#include "board_config.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
// #define DRV_DEBUG
#define LOG_TAG                         "drv.qspi"
#include <drv_log.h>

#define BSP_QSPI_USING_DMA

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
extern rt_err_t rt_hw_qspi_board_init(void);

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void qspi_err_irq_handle(void);
static void qspi_dma_irq_handle(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
struct rt_spi_bus spi_bus_obj;
struct hc32_qspi_bus qspi_bus_obj;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static int hc32_qspi_init(struct rt_qspi_device *device, struct rt_qspi_configuration *qspi_cfg)
{
    int result = RT_EOK;
    uint32_t u32Cnt = 1;
    uint32_t u32BusFreq;
    stc_qspi_init_t stcQspiInit = QSPI_INIT_PARAMS;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(qspi_cfg != RT_NULL);

    struct rt_spi_configuration *cfg = &qspi_cfg->parent;
    struct hc32_qspi_bus *qspi_bus = device->parent.bus->parent.user_data;
    CM_QSPI_TypeDef *qspi_instance = qspi_bus->config->Instance;
    /* Get BUS clock */
    u32BusFreq = CLK_GetBusClockFreq(CLK_BUS_HCLK);
    while (cfg->max_hz < u32BusFreq / (u32Cnt + 1U))
    {
        u32Cnt++;
        if (u32Cnt == 0x3FU)    /* Div64 */
        {
            break;
        }
    }
    stcQspiInit.u32ClockDiv = (u32Cnt << QSPI_CR_DIV_POS);

    if (!(cfg->mode & RT_SPI_CPOL))
    {
        /* QSPI MODE0 */
        stcQspiInit.u32SpiMode  = QSPI_SPI_MD0;
    }
    else
    {
        /* QSPI MODE3 */
        stcQspiInit.u32SpiMode  = QSPI_SPI_MD3;
    }

    /* Enable qspi clock */
    FCG_Fcg1PeriphClockCmd(qspi_bus->config->clock, ENABLE);
    /* qspi port init */
    rt_hw_qspi_board_init();
    /* Init QSPI */
    stcQspiInit.u32ReadMode       = QSPI_RD_MD_STD_RD;
    stcQspiInit.u32DummyCycle     = QSPI_DUMMY_CYCLE4;
    stcQspiInit.u32AddrWidth      = QSPI_ADDR_WIDTH_24BIT;
    (void)QSPI_Init(&stcQspiInit);
    QSPI_SetWpPinLevel(QSPI_WP_PIN_LEVEL);
    /* Enable error interrupt */
    NVIC_EnableIRQ(qspi_bus->config->err_irq.irq_config.irq_num);
    LOG_D("qspi init success!");

#ifdef BSP_QSPI_USING_DMA
    struct dma_config *qspi_dma;
    stc_dma_init_t stcDmaInit;

    /* Get qspi dma */
    qspi_dma = qspi_bus->config->dma_qspi;
    /* Enable Dma clock */
    FCG_Fcg0PeriphClockCmd(qspi_dma->clock, ENABLE);
    AOS_SetTriggerEventSrc(qspi_dma->trigger_select, qspi_dma->trigger_event);
    /* Config Dma */
    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn         = DMA_INT_ENABLE;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
    /* Init Dma */
    if (LL_OK != DMA_Init(qspi_dma->Instance, qspi_dma->channel, &stcDmaInit))
    {
        return -RT_ERROR;
    }
    NVIC_EnableIRQ(qspi_dma->irq_config.irq_num);
#endif /* BSP_QSPI_USING_DMA */

    return result;
}

static void qspi_send_cmd(struct hc32_qspi_bus *qspi_bus, struct rt_qspi_message *message)
{
    RT_ASSERT(qspi_bus != RT_NULL);
    RT_ASSERT(message != RT_NULL);

    QSPI_CommandTypeDef Cmdhandler;

    /* set QSPI cmd struct */
    Cmdhandler.Instruction = message->instruction.content;
    Cmdhandler.Address = message->address.content;
    Cmdhandler.DummyCycles = message->dummy_cycles;
    if (message->instruction.qspi_lines == 0)
    {
        Cmdhandler.InstructionMode = QSPI_INSTRUCTION_NONE;
    }
    else if (message->instruction.qspi_lines == 1)
    {
        Cmdhandler.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    }
    else if (message->instruction.qspi_lines == 2)
    {
        Cmdhandler.InstructionMode = QSPI_INSTRUCTION_2_LINES;
    }
    else if (message->instruction.qspi_lines == 4)
    {
        Cmdhandler.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    }
    if (message->address.qspi_lines == 0)
    {
        Cmdhandler.AddressMode = QSPI_ADDRESS_NONE;
    }
    else if (message->address.qspi_lines == 1)
    {
        Cmdhandler.AddressMode = QSPI_ADDRESS_1_LINE;
    }
    else if (message->address.qspi_lines == 2)
    {
        Cmdhandler.AddressMode = QSPI_ADDRESS_2_LINES;
    }
    else if (message->address.qspi_lines == 4)
    {
        Cmdhandler.AddressMode = QSPI_ADDRESS_4_LINES;
    }
    if (message->address.size == 24)
    {
        Cmdhandler.AddressSize = QSPI_ADDRESS_24_BITS;
    }
    else
    {
        Cmdhandler.AddressSize = QSPI_ADDRESS_32_BITS;
    }
    if (message->qspi_data_lines == 0)
    {
        Cmdhandler.DataMode = QSPI_DATA_NONE;
    }
    else if (message->qspi_data_lines == 1)
    {
        Cmdhandler.DataMode = QSPI_DATA_1_LINE;
    }
    else if (message->qspi_data_lines == 2)
    {
        Cmdhandler.DataMode = QSPI_DATA_2_LINES;
    }
    else if (message->qspi_data_lines == 4)
    {
        Cmdhandler.DataMode = QSPI_DATA_4_LINES;
    }

    Cmdhandler.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    Cmdhandler.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    Cmdhandler.DdrMode = QSPI_DDR_MODE_DISABLE;
    Cmdhandler.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    Cmdhandler.NbData = message->parent.length;
    HAL_QSPI_Command(&qspi_bus->QSPI_Handler, &Cmdhandler, 5000);
}

static rt_uint32_t qspixfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);

    rt_size_t len = 0;
    struct rt_qspi_message *qspi_message = (struct rt_qspi_message *)message;
    struct hc32_qspi_bus *qspi_bus = device->bus->parent.user_data;
#ifdef BSP_QSPI_USING_SOFT_CS
    struct hc32_hw_qspi_cs *cs = device->parent.user_data;
#endif
    const rt_uint8_t *tx_buf = message->send_buf;
    rt_uint8_t *rx_buf = message->recv_buf;
    rt_int32_t length = message->length;

#ifdef BSP_QSPI_USING_SOFT_CS
    if (message->cs_take)
    {
        rt_pin_write(cs->pin, 0);
    }
#endif
    /* send data */
    if (tx_buf != NULL)
    {
        qspi_send_cmd(qspi_bus, qspi_message);
        if (qspi_message->parent.length != 0)
        {
            if (HAL_QSPI_Transmit(&qspi_bus->QSPI_Handler, (rt_uint8_t *)tx_buf, 5000) == HAL_OK)
            {
                len = length;
            }
            else
            {
                LOG_E("QSPI send data failed(%d)!", qspi_bus->QSPI_Handler.ErrorCode);
                qspi_bus->QSPI_Handler.State = HAL_QSPI_STATE_READY;
                goto __exit;
            }
        }
        else
        {
            len = 1;
        }
    }
    else if (rx_buf != NULL)     /* recv data */
    {
        qspi_send_cmd(qspi_bus, qspi_message);
#ifdef BSP_QSPI_USING_DMA
        if (HAL_QSPI_Receive_DMA(&qspi_bus->QSPI_Handler, rx_buf) == HAL_OK)
#else
        if (HAL_QSPI_Receive(&qspi_bus->QSPI_Handler, rx_buf, 5000) == HAL_OK)
#endif
        {
            len = length;
#ifdef BSP_QSPI_USING_DMA
            while (qspi_bus->QSPI_Handler.RxXferCount != 0);
#endif
        }
        else
        {
            LOG_E("QSPI recv data failed(%d)!", qspi_bus->QSPI_Handler.ErrorCode);
            qspi_bus->QSPI_Handler.State = HAL_QSPI_STATE_READY;
            goto __exit;
        }
    }
__exit:
#ifdef BSP_QSPI_USING_SOFT_CS
    if (message->cs_release)
    {
        rt_pin_write(cs->pin, 1);
    }
#endif

    return len;
}

static rt_err_t qspi_configure(struct rt_spi_device *device, struct rt_spi_configuration *configuration)
{
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(configuration != RT_NULL);

    struct rt_qspi_device *qspi_device = (struct rt_qspi_device *)device;
    return hc32_qspi_init(qspi_device, &qspi_device->config);
}

static const struct rt_spi_ops hc32_qspi_ops =
{
    .configure = qspi_configure,
    .xfer = qspixfer,
};

static int hc32_qspi_register_bus(struct hc32_qspi_bus *qspi_bus, const char *name)
{
    RT_ASSERT(qspi_bus != RT_NULL);
    RT_ASSERT(name != RT_NULL);

    spi_bus_obj.parent.user_data = qspi_bus;
    return rt_qspi_bus_register(&spi_bus_obj, name, &hc32_qspi_ops);
}

static void qspi_err_irq_handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    QSPI_ClearStatus(QSPI_FLAG_ROM_ACCESS_ERR);
    /* leave interrupt */
    rt_interrupt_leave();
}

#ifdef BSP_QSPI_USING_DMA
static void qspi_dma_irq_handle(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMA_IRQHandler(&qspi_bus_obj.hdma_quadspi);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_QSPI_USING_DMA */

/**
  * @brief  This function attach device to QSPI bus.
  * @param  bus_name                    QSPI bus name
  * @param  device_name                 QSPI device name
  * @param  pin                         QSPI cs pin number
  * @param  data_line_width             QSPI data lines width, such as 1, 2, 4
  * @param  enter_qspi_mode             Callback function that lets FLASH enter QSPI mode
  * @param  exit_qspi_mode              Callback function that lets FLASH exit QSPI mode
  * @retval 0 : success
  *        -1 : failed
  */
rt_err_t rt_hw_qspi_bus_attach_device(const char *bus_name, const char *device_name, rt_uint32_t pin, rt_uint8_t data_line_width, void (*enter_qspi_mode)(), void (*exit_qspi_mode)())
{
    struct rt_qspi_device *qspi_device = RT_NULL;
    struct hc32_hw_qspi_cs *cs_pin = RT_NULL;
    rt_err_t result = RT_EOK;

    RT_ASSERT(bus_name != RT_NULL);
    RT_ASSERT(device_name != RT_NULL);
    RT_ASSERT(data_line_width == 1 || data_line_width == 2 || data_line_width == 4);

    qspi_device = (struct rt_qspi_device *)rt_malloc(sizeof(struct rt_qspi_device));
    if (qspi_device == RT_NULL)
    {
        LOG_E("no memory, qspi bus attach device failed!");
        result = RT_ENOMEM;
        goto __exit;
    }
    cs_pin = (struct hc32_hw_qspi_cs *)rt_malloc(sizeof(struct hc32_hw_qspi_cs));
    if (qspi_device == RT_NULL)
    {
        LOG_E("no memory, qspi bus attach device failed!");
        result = RT_ENOMEM;
        goto __exit;
    }
    qspi_device->enter_qspi_mode        = enter_qspi_mode;
    qspi_device->exit_qspi_mode         = exit_qspi_mode;
    qspi_device->config.qspi_dl_width   = data_line_width;

    cs_pin->pin = pin;
#ifdef BSP_QSPI_USING_SOFT_CS
    rt_pin_mode(pin, PIN_MODE_OUTPUT);
    rt_pin_write(pin, 1);
#endif

    result = rt_spi_bus_attach_device(&qspi_device->parent, device_name, bus_name, (void *)cs_pin);
__exit:
    if (result != RT_EOK)
    {
        if (qspi_device)
        {
            rt_free(qspi_device);
        }
        if (cs_pin)
        {
            rt_free(cs_pin);
        }
    }

    return  result;
}

static void hc32_get_qspi_info(void)
{
    static struct hc32_qspi_config qspi_config = QSPI_BUS_CONFIG;
    qspi_config.err_irq.irq_callback = qspi_err_irq_handler;
#ifdef BSP_QSPI_USING_DMA
    static struct dma_config qspi_dma = QSPI_DMA_CONFIG;
    qspi_dma.irq_callback   = qspi_dma_irq_handle;
    qspi_config.dma_qspi    = &qspi_dma;
#endif
    qspi_bus_obj.config = &qspi_config;
}

static int rt_hw_qspi_bus_init(void)
{
    hc32_get_qspi_info();
    /* register the handle */
    hc32_install_irq_handler(&qspi_bus_obj.config->err_irq.irq_config, qspi_bus_obj.config->err_irq.irq_callback, RT_FALSE);
#ifdef BSP_QSPI_USING_DMA
    hc32_install_irq_handler(&qspi_bus_obj.config->dma_qspi->irq_config, qspi_bus_obj.config->dma_qspi->irq_callback, RT_FALSE);
#endif
    return hc32_qspi_register_bus(&qspi_bus_obj, "qspi1");
}
INIT_BOARD_EXPORT(rt_hw_qspi_bus_init);

#endif /* BSP_USING_QSPI */
#endif /* RT_USING_QSPI */
