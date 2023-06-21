/*
 * Copyright (c) 2006-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#include <board.h>
#include <drv_spi.h>
#include <rtdevice.h>
#include <rthw.h>
#include <finsh.h>
#include <dfs_fs.h>
#include <fal.h>

#ifdef BSP_USING_SPI_FLASH

#include "spi_flash.h"
#ifdef RT_USING_SFUD
    #include "spi_flash_sfud.h"
#endif

#define SPI_BUS_NAME                    "spi1"
#define SPI_FLASH_DEVICE_NAME           "spi10"
#define SPI_FLASH_CHIP                  "w25q64"
#define SPI_FLASH_SS_PORT               GPIO_PORT_C
#define SPI_FLASH_SS_PIN                GPIO_PIN_07
/* Partition Name */
#define FS_PARTITION_NAME              "filesystem"

#ifdef RT_USING_SFUD
static int rt_hw_spi_flash_with_sfud_init(void)
{
    rt_hw_spi_device_attach(SPI_BUS_NAME, SPI_FLASH_DEVICE_NAME, SPI_FLASH_SS_PORT, SPI_FLASH_SS_PIN);

    if (RT_NULL == rt_sfud_flash_probe(SPI_FLASH_CHIP, SPI_FLASH_DEVICE_NAME))
    {
        return RT_ERROR;
    }

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_with_sfud_init);

static int rt_hw_fs_init(void)
{
    struct rt_device *mtd_dev = RT_NULL;

    /* 初始化 fal */
    fal_init();
    /* 生成 mtd 设备 */
    mtd_dev = fal_mtd_nor_device_create(FS_PARTITION_NAME);
    if (!mtd_dev)
    {
        LOG_E("Can't create a mtd device on '%s' partition.", FS_PARTITION_NAME);
        return RT_ERROR;
    }
    else
    {
        /* 挂载 littlefs */
        if (RT_EOK == dfs_mount(FS_PARTITION_NAME, "/", "lfs", 0, 0))
        {
            LOG_I("Filesystem initialized!");
            return RT_EOK;
        }
        else
        {
            /* 格式化文件系统 */
            if (RT_EOK == dfs_mkfs("lfs", FS_PARTITION_NAME))
            {
                /* 挂载 littlefs */
                if (RT_EOK == dfs_mount(FS_PARTITION_NAME, "/", "lfs", 0, 0))
                {
                    LOG_I("Filesystem initialized!");
                    return RT_EOK;
                }
                else
                {
                    LOG_E("Failed to initialize filesystem!");
                    return RT_ERROR;
                }
            }
            else
            {
                LOG_E("Failed to Format fs!");
                return RT_ERROR;
            }
        }
    }
}
INIT_APP_EXPORT(rt_hw_fs_init);

#endif /* RT_USING_SFUD */

#endif /* BSP_USING_SPI_FLASH */
