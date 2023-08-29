/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-01     CDT          first version
 */


/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <rtthread.h>


#if defined (BSP_USING_EXMC)
#if defined (BSP_USING_NAND)

#if defined (PKG_USING_LPM)
#include <lpm.h>
#endif

#include "drv_nand.h"
#include "board_config.h"
#include "nand_port.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//#define DRV_DEBUG
#define LOG_TAG "drv.nand"
#include <drv_log.h>

/* Nand status */
#define NAND_BUSY                  0x00000000U
#define NAND_FAIL                  0x00000001U
#define NAND_READY                 0x00000040U
#define NAND_VALID_ADDRESS         0x00000100U
#define NAND_INVALID_ADDRESS       0x00000200U
#define NAND_TIMEOUT_ERROR         0x00000400U

#define NAND_ERASE_TIMEOUT         2000000UL
#define NAND_READ_TIMEOUT          2000000UL
#define NAND_WRITE_TIMEOUT         2000000UL
#define NAND_RESET_TIMEOUT         2000000UL

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#if defined (BSP_USING_NAND)
extern rt_err_t rt_hw_board_nand_init(void);
#endif

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

static rt_err_t _nand_verify_clock_frequency(void)
{
    rt_err_t ret = RT_EOK;

#if defined (HC32F4A0)
    /* EXCLK max frequency for Nand: 60MHz */
    if (CLK_GetBusClockFreq(CLK_BUS_EXCLK) > (60 * 1000000))
    {
        ret = -RT_ERROR;
    }
#endif

    return ret;
}

static rt_err_t _nand_init(void)
{
    rt_uint8_t au8DevId[4];
    rt_err_t ret = -RT_ERROR;
    stc_exmc_nfc_init_t nfc_init_params;

    /* verify nand clock frequency */
    if (_nand_verify_clock_frequency() != RT_EOK)
    {
        LOG_E("EXMC clock frequency is over limit for NAND!");
        return -RT_ERROR;
    }

    /* Initialize nand port.*/
    rt_hw_board_nand_init();

    /* Enable NFC module clock */
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_NFC, ENABLE);

    /* Enable NFC. */
    EXMC_NFC_Cmd(ENABLE);

    /* Configure NFC base parameters. */
    nfc_init_params.u32OpenPage = EXMC_NFC_OPEN_PAGE_DISABLE;
    nfc_init_params.stcBaseConfig.u32CapacitySize = NAND_EXMC_NFC_BANK_CAPACITY;
    nfc_init_params.stcBaseConfig.u32MemoryWidth = NAND_EXMC_NFC_MEMORY_WIDTH;
    nfc_init_params.stcBaseConfig.u32BankNum = EXMC_NFC_1BANK;
    nfc_init_params.stcBaseConfig.u32PageSize = NAND_EXMC_NFC_PAGE_SIZE;
    nfc_init_params.stcBaseConfig.u32WriteProtect = EXMC_NFC_WR_PROTECT_DISABLE;
    nfc_init_params.stcBaseConfig.u32EccMode = EXMC_NFC_1BIT_ECC;
    nfc_init_params.stcBaseConfig.u32RowAddrCycle = NAND_EXMC_NFC_ROW_ADDR_CYCLE;
    nfc_init_params.stcBaseConfig.u8SpareSizeForUserData = 0U;

    /* Configure NFC timing */
    nfc_init_params.stcTimingReg0.u32TS = NAND_TS;
    nfc_init_params.stcTimingReg0.u32TWP = NAND_TWP;
    nfc_init_params.stcTimingReg0.u32TRP = NAND_TRP;

    nfc_init_params.stcTimingReg0.u32TH = NAND_TH;

    nfc_init_params.stcTimingReg1.u32TWH = NAND_TWH;
    nfc_init_params.stcTimingReg1.u32TRH = NAND_TRH;
    nfc_init_params.stcTimingReg1.u32TRR = NAND_TRR;
    nfc_init_params.stcTimingReg1.u32TWB = NAND_TWB;

    nfc_init_params.stcTimingReg2.u32TCCS = NAND_TCCS;
    nfc_init_params.stcTimingReg2.u32TWTR = NAND_TWTR;
    nfc_init_params.stcTimingReg2.u32TRTW = NAND_TRTW;
    nfc_init_params.stcTimingReg2.u32TADL = NAND_TADL;
    if (LL_OK == EXMC_NFC_Init(&nfc_init_params)) {
        /* Reset NFC device. */
        if (LL_OK == EXMC_NFC_Reset(NAND_EXMC_NFC_BANK, NAND_RESET_TIMEOUT)) {
            EXMC_NFC_ReadId(NAND_EXMC_NFC_BANK, 0UL, au8DevId, sizeof(au8DevId), NAND_READ_TIMEOUT);

            LOG_D("Nand Flash ID = 0x%02X,0x%02X,0x%02X,0x%02X",
                  au8DevId[0], au8DevId[1], au8DevId[2], au8DevId[3]);

            ret = RT_EOK;
        }
    }

    return ret;
}

static rt_err_t _nand_get_status(rt_uint32_t nfc_bank, rt_uint32_t timeout)
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t to = 0UL;
    rt_uint32_t status = 0UL;

    do {
        /* Block checking flag if timeout value is NAND_WRITE_TIMEOUT */
        if (to++ > timeout) {
            ret = -RT_ETIMEOUT;
            LOG_E("get nand status timeout!");
            break;
        }

        status = EXMC_NFC_ReadStatus(nfc_bank);
    } while (0UL == (status & NAND_READY));

    if (RT_ETIMEOUT != ret) {
        if (0UL != (status & NAND_FAIL)) {
            ret = -RT_ERROR;
            LOG_E("nand status error!");
        }
    }

    return ret;
}

static rt_err_t _nand_erase_block(rt_uint32_t bank, rt_uint32_t row_addr, rt_uint32_t timeout)
{
    rt_err_t ret = -RT_ERROR;

    if (LL_OK == EXMC_NFC_EraseBlock(bank,  row_addr, timeout)) {
        ret = _nand_get_status(bank, timeout);
    }

    return ret;
}

rt_err_t  _nand_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

rt_err_t _nand_control(rt_device_t dev, int cmd, void *args)
{
    if(RT_DEVICE_CTRL_BLK_GETGEOME == cmd)
    {
        struct rt_device_blk_geometry *geometry = (struct rt_device_blk_geometry *)args;
        geometry->bytes_per_sector = NAND_BYTES_PER_PAGE;
        geometry->sector_count = NAND_DEVICE_PAGES;
        geometry->block_size = NAND_BYTES_PER_BLOCK;
        return RT_EOK;
    }
    else if(RT_DEVICE_CTRL_BLK_ERASE == cmd)
    {
        uint32_t *blk = (uint32_t *)args;
        _nand_erase_block(NAND_EXMC_NFC_BANK, *blk, NAND_ERASE_TIMEOUT);
        return RT_EOK;
    }
    else
    {
        LOG_E("unsupport command!");
    }

    return -RT_ERROR;
}

/* pos: sector offset   size: page count */
rt_ssize_t _nand_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    int32_t nfc_ret;
    rt_ssize_t i;
    rt_uint32_t read_page = pos;
    rt_uint8_t *read_buf = (rt_uint8_t *)buffer;

    for (i = 0; i < size; i++)
    {
        nfc_ret = EXMC_NFC_ReadPageMeta(NAND_EXMC_NFC_BANK, read_page, read_buf, NAND_BYTES_PER_PAGE, NAND_READ_TIMEOUT);
        if (LL_OK != nfc_ret)
        {
            LOG_E("nand read error!");
            break;
        }

        read_page ++;
        read_buf += NAND_BYTES_PER_PAGE;
    }

    return i;
}

/* pos: sector offset   size: page count */
rt_ssize_t _nand_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    rt_ssize_t i;
    rt_int32_t nfc_ret;
    rt_uint32_t write_page = pos;
    rt_uint8_t *write_buf = (rt_uint8_t *)buffer;

    for (i = 0; i < size; i++)
    {
        nfc_ret = EXMC_NFC_WritePageMeta(NAND_EXMC_NFC_BANK, write_page, write_buf, NAND_BYTES_PER_PAGE, NAND_WRITE_TIMEOUT);
        if (LL_OK != nfc_ret)
        {
            LOG_E("nand write error!");
            break;
        }
        else
        {
            nfc_ret = _nand_get_status(NAND_EXMC_NFC_BANK, NAND_WRITE_TIMEOUT);
            if (nfc_ret != RT_EOK)
            {
                break;
            }
        }

        write_page ++;
        write_buf += NAND_BYTES_PER_PAGE;
    }

    return i;
}

int rt_hw_nand_init(void)
{
    static struct rt_device _hw_nand;

    _nand_init();

    _hw_nand.type      = RT_Device_Class_MTD;
    _hw_nand.init      = RT_NULL;
    _hw_nand.open      = _nand_open;
    _hw_nand.close     = RT_NULL;
    _hw_nand.read      = _nand_read;
    _hw_nand.write     = _nand_write;
    _hw_nand.control   = _nand_control;
    _hw_nand.user_data = RT_NULL;

    rt_device_register(&_hw_nand, "nand1", RT_DEVICE_FLAG_RDWR);
    LOG_D("nand1 init done\n");

#ifdef PKG_USING_LPM
    lpm_init();
    lpm_dev_blk_append(&_hw_nand);
#endif

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_nand_init);

#ifdef DRV_DEBUG
#ifdef FINSH_USING_MSH
static int _nand_test(void)
{
    rt_err_t ret;
    rt_uint32_t i = 0;
    rt_off_t page_offset;
    rt_off_t block_offset;
    rt_size_t page_count;
    static uint8_t read_buffer [NAND_BYTES_PER_PAGE];
    static uint8_t write_buffer [NAND_BYTES_PER_PAGE];
    static rt_device_t nand;

    nand = rt_device_find("nand1");
    if (RT_NULL == nand)
    {
        LOG_E("nand device not found\r\n");
        return -RT_ERROR;
    }
    
    ret = rt_device_open(nand, RT_DEVICE_FLAG_RDWR);
    if (ret != RT_EOK)
    {
        LOG_E("nand device failed to open \r\n");
        return -RT_ERROR;
    }

    page_offset = 1;
    block_offset = 0;
    page_count = sizeof(write_buffer) / NAND_BYTES_PER_PAGE;

    /* Fill the buffer to send */
    for (i = 0; i < sizeof(write_buffer); i++ )
    {
        write_buffer[i] = i;
    }

    /* Erase the NAND first Block */
    _nand_control(nand, RT_DEVICE_CTRL_BLK_ERASE, &block_offset);

    /* Write data to NAND memory */
    _nand_write(nand, page_offset, write_buffer, page_count);
    LOG_D("write data to nand\r\n");

    /* Read data from NAND memory */
    _nand_read(nand, page_offset, read_buffer, page_count);
    LOG_D("read data from nand\r\n");

    if (rt_memcmp(read_buffer, write_buffer, sizeof(read_buffer)) == 0)
    {
        LOG_D("compare w/r data: consistency \r\n");
        return RT_EOK;
    }
    else
    {
        LOG_D("compare w/r data: inconsistency \r\n");
        return -RT_ERROR;
    }
}
MSH_CMD_EXPORT(_nand_test, nand test)
#endif /* FINSH_USING_MSH */
#endif /* DRV_DEBUG */

#endif /* BSP_USING_NAND */
#endif /* BSP_USING_EXMC */
