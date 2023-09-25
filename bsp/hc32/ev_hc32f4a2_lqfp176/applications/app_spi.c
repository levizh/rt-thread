/*
 * 程序清单：这是一个 SPI 设备使用例程
 * 例程导出了 spi_w25q_sample 命令到控制终端
 * 命令调用格式：spi_w25q_sample spi10
 * 命令解释：命令第二个参数是要使用的SPI设备名称，为空则使用默认的SPI设备
 * 程序功能：通过SPI设备读取 w25q 的 ID 数据
*/

#include <rtthread.h>
#include <rtdevice.h>
#include "board_config.h"
#include "drv_spi.h"



#define W25Q_SPI_DEVICE_NAME     "spi10"

struct rt_spi_device *spi_dev_w25q;     /* SPI 设备句柄 */

struct rt_spi_message msg1, msg2;

rt_uint8_t w25x_read_uid = 0x4B;         /* 命令 */
rt_uint8_t u8UID[8] = {0};
rt_uint8_t txBuf[5] = {0};


static int rt_hw_spi_flash_init(void)
{
    if (RT_EOK != rt_hw_spi_device_attach("spi1", "spi10", SPI1_CS_PORT, SPI1_CS_PIN))
    {
        rt_kprintf("Failed to attach the spi device.");
        return -RT_ERROR;
    }

    return RT_EOK;
}

/* 导出到自动初始化 */
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_init);



static void spi_w25q_sample(int argc, char *argv[])
{
    char name[RT_NAME_MAX];

    if (argc == 2)
    {
        rt_strncpy(name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(name, W25Q_SPI_DEVICE_NAME, RT_NAME_MAX);
    }

    /* 查找 spi 设备获取设备句柄 */
    spi_dev_w25q = (struct rt_spi_device *)rt_device_find(name);
    if (!spi_dev_w25q)
    {
        rt_kprintf("spi sample run failed! can't find %s device!\n", name);
    }
    else
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
        cfg.max_hz = 10 * 1000 * 1000;                          /* 10M */
        rt_spi_configure(spi_dev_w25q, &cfg);

        txBuf[0] = w25x_read_uid;
        txBuf[1] = 0xFF;
        txBuf[2] = 0xFF;
        txBuf[3] = 0xFF;
        txBuf[4] = 0xFF;
        /* 方式1：使用 rt_spi_send_then_recv()发送命令读取ID */
        rt_spi_send_then_recv(spi_dev_w25q, txBuf, 5, u8UID, 8);
        rt_kprintf("use rt_spi_send_then_recv() read w25q UID is: %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\r\n",
                   u8UID[0], u8UID[1], u8UID[2], u8UID[3], u8UID[4], u8UID[5], u8UID[6], u8UID[7]);

        /* 方式2：使用 rt_spi_transfer_message()发送命令读取ID */
        struct rt_spi_message msg1, msg2;

        msg1.send_buf   = txBuf;
        msg1.recv_buf   = RT_NULL;
        msg1.length     = 5;
        msg1.cs_take    = 1;
        msg1.cs_release = 0;
        msg1.next       = &msg2;

        msg2.send_buf   = RT_NULL;
        msg2.recv_buf   = u8UID;
        msg2.length     = 8;
        msg2.cs_take    = 0;
        msg2.cs_release = 1;
        msg2.next       = RT_NULL;

        rt_spi_transfer_message(spi_dev_w25q, &msg1);
        rt_kprintf("use rt_spi_transfer_message() read w25q ID is: UID is: %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\r\n",
                   u8UID[0], u8UID[1], u8UID[2], u8UID[3], u8UID[4], u8UID[5], u8UID[6], u8UID[7]);

        /* write,read and compare data */

    }
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(spi_w25q_sample, spi w25q sample);
