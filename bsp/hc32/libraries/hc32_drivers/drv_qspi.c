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

/* QSPI read/write function */
#define QSPI_READ_FUNC                  (0U)
#define QSPI_WRITE_FUNC                 (1U)

#define QSPI_BASE_BLK_SIZE              (0x4000000UL)
#define QSPI_MAX_FLASH_ADDR             (0xFC000000UL)

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
    /* Config Dma */
    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn         = DMA_INT_ENABLE;
    stcDmaInit.u32DataWidth     = DMA_DATAWIDTH_8BIT;
    /* Init Dma */
    if (LL_OK != DMA_Init(qspi_dma->Instance, qspi_dma->channel, &stcDmaInit))
    {
        return -RT_ERROR;
    }
    DMA_Cmd(qspi_dma->Instance, ENABLE);
    NVIC_EnableIRQ(qspi_dma->irq_config.irq_num);
#endif /* BSP_QSPI_USING_DMA */

    return result;
}

static int32_t hc32_qspi_send_cmd(struct hc32_qspi_bus *qspi_bus, struct rt_qspi_message *message, uint8_t u8Func)
{
    uint32_t u32ReadMode = QSPI_RD_MD_STD_RD;
    uint32_t u32DummyCycle = 0U;
    uint32_t u32AddrWidth;

    RT_ASSERT(qspi_bus != RT_NULL);
    RT_ASSERT(message != RT_NULL);

    CM_QSPI_TypeDef *qspi_instance = qspi_bus->config->Instance;
    if (QSPI_READ_FUNC == u8Func)
    {
        if ((message->instruction.qspi_lines != 1) ||
            ((message->address.qspi_lines != 1) && (message->address.qspi_lines != 2) && (message->address.qspi_lines != 4)) ||
            ((message->qspi_data_lines != 1) && (message->qspi_data_lines != 2) && (message->qspi_data_lines != 4)))
        {
            return LL_ERR_INVD_PARAM;
        }
        if (((message->address.qspi_lines == 2) && ((message->qspi_data_lines == 1) || (message->qspi_data_lines == 4))) ||
            ((message->address.qspi_lines == 4) && ((message->qspi_data_lines == 1) || (message->qspi_data_lines == 2))))
        {
            return LL_ERR_INVD_PARAM;
        }
        if ((message->dummy_cycles < 3) || (message->dummy_cycles > 18))
        {
            return LL_ERR_INVD_PARAM;
        }

        u32DummyCycle = message->dummy_cycles << QSPI_FCR_DMCYCN_POS;
        if ((message->instruction.qspi_lines == 1) && (message->address.qspi_lines == 1) && (message->qspi_data_lines == 1))
        {
            if (message->dummy_cycles != 0)
            {
                u32ReadMode += 1U;
            }
        }
        else
        {
            u32ReadMode = message->qspi_data_lines;
            if (message->address.qspi_lines == message->qspi_data_lines)
            {
                u32ReadMode += 1U;
            }
        }
        if (message->address.size == 24)
        {
            u32AddrWidth = QSPI_ADDR_WIDTH_24BIT;
        }
        else
        {
            u32AddrWidth = QSPI_ADDR_WIDTH_32BIT_INSTR_32BIT;
        }
        /* configure register */
        MODIFY_REG32(qspi_instance->FCR, (QSPI_FCR_DMCYCN | QSPI_FCR_FOUR_BIC | QSPI_FCR_AWSL),
                     (u32DummyCycle | u32AddrWidth));
        QSPI_SetReadMode(u32ReadMode);
    }
    else
    {
        if ((message->instruction.qspi_lines != 1) ||
            (message->address.qspi_lines != 1) ||
            (message->qspi_data_lines != 1))
        {
            return LL_ERR_INVD_PARAM;
        }

        if (message->address.size == 24)
        {
            u32AddrWidth = QSPI_ADDR_WIDTH_24BIT;
        }
        else
        {
            u32AddrWidth = QSPI_ADDR_WIDTH_32BIT_INSTR_32BIT;
        }
        /* configure register */
        MODIFY_REG32(qspi_instance->FCR, (QSPI_FCR_FOUR_BIC | QSPI_FCR_AWSL), u32AddrWidth);
    }

    return LL_OK;
}



{
    /* Init QSPI */
    stcQspiInit.u32ReadMode       = QSPI_RD_MD_STD_RD;
    stcQspiInit.u32DummyCycle     = QSPI_DUMMY_CYCLE4;
    stcQspiInit.u32AddrWidth      = QSPI_ADDR_WIDTH_24BIT;




    /* set QSPI cmd struct */
    Cmdhandler.Instruction = message->instruction.content;
    Cmdhandler.Address = message->address.content;
    Cmdhandler.DummyCycles = message->dummy_cycles;
    // if (message->instruction.qspi_lines == 0)
    // {
    //     Cmdhandler.InstructionMode = QSPI_INSTRUCTION_NONE;
    // }
    // else if (message->instruction.qspi_lines == 1)
    // {
    //     Cmdhandler.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    // }
    // else if (message->instruction.qspi_lines == 2)
    // {
    //     Cmdhandler.InstructionMode = QSPI_INSTRUCTION_2_LINES;
    // }
    // else if (message->instruction.qspi_lines == 4)
    // {
    //     Cmdhandler.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    // }


    // if (message->address.qspi_lines == 0)
    // {
    //     Cmdhandler.AddressMode = QSPI_ADDRESS_NONE;
    // }
    // else if (message->address.qspi_lines == 1)
    // {
    //     Cmdhandler.AddressMode = QSPI_ADDRESS_1_LINE;
    // }
    // else if (message->address.qspi_lines == 2)
    // {
    //     Cmdhandler.AddressMode = QSPI_ADDRESS_2_LINES;
    // }
    // else if (message->address.qspi_lines == 4)
    // {
    //     Cmdhandler.AddressMode = QSPI_ADDRESS_4_LINES;
    // }


    // if (message->address.size == 24)
    // {
    //     Cmdhandler.AddressSize = QSPI_ADDRESS_24_BITS;
    // }
    // else
    // {
    //     Cmdhandler.AddressSize = QSPI_ADDRESS_32_BITS;
    // }


    // if (message->qspi_data_lines == 0)
    // {
    //     Cmdhandler.DataMode = QSPI_DATA_NONE;
    // }
    // else if (message->qspi_data_lines == 1)
    // {
    //     Cmdhandler.DataMode = QSPI_DATA_1_LINE;
    // }
    // else if (message->qspi_data_lines == 2)
    // {
    //     Cmdhandler.DataMode = QSPI_DATA_2_LINES;
    // }
    // else if (message->qspi_data_lines == 4)
    // {
    //     Cmdhandler.DataMode = QSPI_DATA_4_LINES;
    // }


    Cmdhandler.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    Cmdhandler.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    Cmdhandler.DdrMode = QSPI_DDR_MODE_DISABLE;
    Cmdhandler.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    Cmdhandler.NbData = message->parent.length;
    HAL_QSPI_Command(&qspi_bus->QSPI_Handler, &Cmdhandler, 5000);
}


static int32_t hc32_qspi_write(uint32_t u32Addr, uint8_t *pu8WriteBuf, uint32_t u32Size)
{
    uint32_t u32TempSize;
    uint8_t u8AddrBuf[4U];
    uint32_t u32AddrOffset = 0U;
    int32_t i32Ret = LL_OK;
    uint8_t u8InstrVal;

    if ((NULL == pu8WriteBuf) || (0UL == u32Size) || ((u32Addr % W25Q64_PAGE_SIZE) != 0U))
    {
        i32Ret = LL_ERR_INVD_PARAM;
    }
    else
    {
        while (u32Size != 0UL)
        {
            if (u32Size >= W25Q64_PAGE_SIZE)
            {
                u32TempSize = W25Q64_PAGE_SIZE;
            }
            else
            {
                u32TempSize = u32Size;
            }
            QSPI_FLASH_WriteInstr(W25Q64_WR_ENABLE, NULL, 0U, NULL, 0U);
            QSPI_FLASH_WordToByte(u32Addr, u8AddrBuf);
#if (QSPI_FLASH_RD_MD == QSPI_RD_MD_QUAD_OUTPUT_FAST_RD) || (QSPI_FLASH_RD_MD == QSPI_RD_MD_QUAD_IO_FAST_RD)
            u8InstrVal = W25Q64_QUAD_INPUT_PAGE_PROGRAM;
#else
            u8InstrVal = W25Q64_PAGE_PROGRAM;
#endif
            QSPI_FLASH_WriteInstr(u8InstrVal, u8AddrBuf, (QSPI_FLASH_ADDR_WIDTH + 1U),
                                  &pu8WriteBuf[u32AddrOffset], u32TempSize);
            i32Ret = QSPI_FLASH_CheckProcessDone(500U);
            if (i32Ret != LL_OK)
            {
                break;
            }
            u32Addr       += u32TempSize;
            u32AddrOffset += u32TempSize;
            u32Size       -= u32TempSize;
        }
    }

    return i32Ret;
}


static int32_t hc32_qspi_read(struct hc32_qspi_bus *qspi_bus, struct rt_qspi_message *message)
{
    uint32_t u32Count = 0U;
    __IO uint8_t *pu8Read;
    rt_uint8_t *rx_buf = message->recv_buf;
    rt_int32_t length = message->length;
    uint32_t u32Addr = message->address.content;
    uint32_t u32ExtBlkStartNum = 0U;
    uint32_t u32GetSize, u32RxIndex = 0U;
#ifdef BSP_QSPI_USING_DMA
    struct dma_config *qspi_dma;
    stc_dma_init_t stcDmaInit;
    uint32_t u32DmaTransSize;
#endif

    RT_ASSERT(qspi_bus != RT_NULL);
    RT_ASSERT(message != RT_NULL);

    if ((u32Addr + length) > QSPI_MAX_FLASH_ADDR)
    {
        return LL_ERR_INVD_PARAM;
    }
    u32ExtBlkStartNum = u32Addr / QSPI_BASE_BLK_SIZE;
    u32Addr = (u32Addr % QSPI_BASE_BLK_SIZE) + QSPI_ROM_BASE;
#ifdef BSP_QSPI_USING_DMA
    qspi_bus->config->dma_flag = RT_FALSE;
    qspi_dma = qspi_bus->config->dma_qspi;

    AOS_SetTriggerEventSrc(qspi_dma->trigger_select, qspi_dma->trigger_event);
    DMA_ClearTransCompleteStatus(qspi_dma->Instance, (1U << qspi_dma->channel));
    /* Config Dma */
    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn         = DMA_INT_ENABLE;
    stcDmaInit.u32DataWidth     = DMA_DATAWIDTH_8BIT;
    stcDmaInit.u32SrcAddrInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestAddrInc   = DMA_DEST_ADDR_INC;
    DMA_Init(qspi_dma->Instance, qspi_dma->channel, &stcDmaInit);
    while (length != 0)
    {
        QSPI_SelectMemoryBlock((uint8_t)u32ExtBlkStartNum);
        if ((u32Addr + length) > (QSPI_ROM_END + 1U))
        {
            u32GetSize = (QSPI_ROM_END + 1U) - u32Addr;
            length -= u32GetSize;
        }
        else
        {
            u32GetSize = length;
            length = 0U;
        }
        while (u32GetSize != 0U)
        {
            if (u32GetSize > DMA_DTCTL_BLKSIZE)
            {
                u32DmaTransSize = DMA_DTCTL_BLKSIZE;
                u32GetSize -= DMA_DTCTL_BLKSIZE;
            }
            else
            {
                u32DmaTransSize = u32GetSize;
                u32GetSize = 0U;
            }

            DMA_SetSrcAddr(qspi_dma->Instance, qspi_dma->channel, u32Addr);
            DMA_SetDestAddr(qspi_dma->Instance, qspi_dma->channel, (uint32_t)&rx_buf[u32RxIndex]);
            DMA_SetTransCount(qspi_dma->Instance, qspi_dma->channel, 1UL);
            DMA_SetBlockSize(qspi_dma->Instance, qspi_dma->channel, (uint16_t)u32DmaTransSize);
            (void)DMA_ChCmd(qspi_dma->Instance, qspi_dma->channel, ENABLE);
            AOS_SW_Trigger();   /* 1st trigger for DMA */
            while (RT_FALSE == qspi_bus->config->dma_flag);
            u32Addr += u32DmaTransSize;
            u32RxIndex += u32DmaTransSize;
        }
        if (length != 0U)
        {
            u32ExtBlkStartNum += 1U;
            u32Addr = QSPI_ROM_BASE;
        }
    }
    AOS_SetTriggerEventSrc(qspi_dma->trigger_select, EVT_SRC_MAX);
#else
    while (length != 0)
    {
        QSPI_SelectMemoryBlock((uint8_t)u32ExtBlkStartNum);
        pu8Read = (__IO uint8_t *)u32Addr;
        if ((u32Addr + length) > (QSPI_ROM_END + 1U))
        {
            u32GetSize = (QSPI_ROM_END + 1U) - u32Addr;
            length -= u32GetSize;
        }
        else
        {
            u32GetSize = length;
            length = 0U;
        }
        u32Count = 0U;
        while (u32Count < u32GetSize)
        {
            rx_buf[u32RxIndex++] = *pu8Read++;
            u32Count++;
        }
        if (length != 0U)
        {
            u32ExtBlkStartNum += 1U;
            u32Addr = QSPI_ROM_BASE;
        }
    }
#endif

    return LL_OK;
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
    // const rt_uint8_t *tx_buf = message->send_buf;
    // rt_uint8_t *rx_buf = message->recv_buf;
    rt_int32_t length = message->length;

#ifdef BSP_QSPI_USING_SOFT_CS
    if (message->cs_take)
    {
        rt_pin_write(cs->pin, 0);
    }
#endif
    if (message->send_buf != NULL)     /* send data */
    {
        if (LL_OK != hc32_qspi_send_cmd(qspi_bus, qspi_message, QSPI_WRITE_FUNC))
        {
            LOG_E("QSPI rt_qspi_message format error!");
            goto __exit;
        }

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
    else if (message->recv_buf != NULL)     /* recv data */
    {
        if (LL_OK != hc32_qspi_send_cmd(qspi_bus, qspi_message, QSPI_READ_FUNC))
        {
            LOG_E("QSPI rt_qspi_message format error!");
            goto __exit;
        }
        if (LL_OK == hc32_qspi_read(qspi_bus, qspi_message))
        {
            len = length;
        }
        else
        {
            LOG_E("QSPI recv data failed!");
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
    struct dma_config *qspi_dma;

    qspi_dma = qspi_bus_obj.config->dma_qspi;
    /* enter interrupt */
    rt_interrupt_enter();
    DMA_ClearTransCompleteStatus(qspi_dma->Instance, (1U << qspi_dma->channel));
    qspi_bus_obj.config->dma_flag = RT_TRUE;
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
