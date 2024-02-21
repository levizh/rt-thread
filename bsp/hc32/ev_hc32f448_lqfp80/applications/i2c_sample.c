/*
 * 程序清单：这是 I2C 设备使用例程。
 * 例程导出了 i2c_sample 到控制终端。
 * 命令调用格式：i2c_sample
 * 命令解释：
 * 程序功能：查找I2C模块，读写I2C设备。
 * 注意：测试要用逻辑分析仪或示波器抓取信号
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#if defined(RT_USING_I2C)

/* 
    to readout/write  from/into i2c device, user could use this macro to choice 
    either rt_i2c_master_send & rt_i2c_master_recv or rt_i2c_transfer 
*/

//#define USING_RT_I2C_TRANSFER

/* defined the LED_GREEN pin: PC9 */
#define LED_GREEN_PIN GET_PIN(C, 9)

#define EE_ADDR                 0x50
#define TEST_PAGE_CNT           8               // Test 8 pages

#define EE24C256

#if defined (EE24C1024)
/* 24C1024 not tested yet */
#define EE_PAGE_SIZE            256     // 24C1024
#define EE_WORD_ADR_SIZE        2       // 2 word addr

#elif defined (EE24C256)
/* 24C256 */
#define EE_PAGE_SIZE            64      // 24C256
#define EE_WORD_ADR_SIZE        2       // 2 word addr

#elif defined (EE24C02)
/* 24C02 */
#define EE_PAGE_SIZE            8     // 24C02
#define EE_WORD_ADR_SIZE        1     // 1 word addr
#endif

#define HW_I2C          "i2c1"

static rt_uint8_t trans_buf[EE_PAGE_SIZE * TEST_PAGE_CNT];
static rt_uint8_t recv_buf[EE_PAGE_SIZE * TEST_PAGE_CNT];

void i2c_test(void)
{
    struct rt_i2c_bus_device *hc32_i2c = RT_NULL;
    static rt_uint8_t send_buf0[0x10], send_buf1[0x10];
    
    static uint8_t i = 0;

#if defined (BSP_USING_I2C_HW)
    hc32_i2c = rt_i2c_bus_device_find(HW_I2C);      //hw i2c
#else
    hc32_i2c = rt_i2c_bus_device_find("i2c1_sw");   //sw sim i2c
#endif
    RT_ASSERT(hc32_i2c != RT_NULL);

#define TCA9539_ADDRESS                 (0x74)       // TCA9539 chip address on I2C bus
#define WM8731_REG_SAMPLING_CTRL        (0x08)       // Sampling Control Register

#define TCA9539_REG_INPUT_PORT0         (0x00U)
#define TCA9539_REG_INPUT_PORT1         (0x01U)
#define TCA9539_REG_OUTPUT_PORT0        (0x02U)
#define TCA9539_REG_OUTPUT_PORT1        (0x03U)
#define TCA9539_REG_INVERT_PORT0        (0x04U)
#define TCA9539_REG_INVERT_PORT1        (0x05U)
#define TCA9539_REG_CONFIG_PORT0        (0x06U)
#define TCA9539_REG_CONFIG_PORT1        (0x07U)

    struct rt_i2c_msg msg[2];

//    uint8_t u8RegAddr = TCA9539_REG_OUTPUT_PORT1;
//    uint16_t u16Cmd = 4;

    send_buf0[0] =  TCA9539_REG_CONFIG_PORT1;
    send_buf0[1] = (uint8_t)(0x1F);
    msg[0].addr  = TCA9539_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = 2;
    msg[0].buf   = send_buf0;

    rt_i2c_transfer(hc32_i2c, &msg[0], 1);
    
    send_buf0[0] =  TCA9539_REG_OUTPUT_PORT1;
    if (i%2) {
        send_buf0[1] = (uint8_t)(0xE0);
    } else {
        send_buf0[1] = (uint8_t)(0x00);
    }
    i++;
    
    msg[0].addr  = TCA9539_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = 2;
    msg[0].buf   = send_buf0;

    rt_i2c_transfer(hc32_i2c, &msg[0], 1);

    /* read */
    send_buf1[0] = TCA9539_REG_OUTPUT_PORT1;

    msg[0].addr  = TCA9539_ADDRESS;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = 1;
    msg[0].buf   = send_buf1;

    msg[1].addr  = TCA9539_ADDRESS;
    msg[1].flags = RT_I2C_RD;
    msg[1].len   = 1;
    msg[1].buf   = recv_buf;

    rt_i2c_transfer(hc32_i2c, &msg[0], 2);
    rt_kprintf("recv: %x\r\n", recv_buf[0]);

}

/* this API is for eeprom size is smaller than 24C16 (256Bytes) */
static void eeprom_page_write(uint32_t page, uint8_t *pBuf)
{
    struct rt_i2c_bus_device *hc32_i2c = RT_NULL;

#if defined (BSP_USING_I2C_HW)
    hc32_i2c = rt_i2c_bus_device_find(HW_I2C);      //hw i2c
#else
    hc32_i2c = rt_i2c_bus_device_find("i2c1_sw");   //sw sim i2c
#endif
/*
    START --- ADR_W --- WORD_ADR(1 byte) --- DATAn --- STOP
*/
    uint8_t TxBuf[EE_PAGE_SIZE + EE_WORD_ADR_SIZE] = {0};
    struct rt_i2c_msg msg[1];
    /* write byte */
    if (EE_WORD_ADR_SIZE == 2)
    {
      TxBuf[0] = (page * EE_PAGE_SIZE) / 256;           // addrH
      TxBuf[1] = page * EE_PAGE_SIZE;                   // addrL
    } else
    {
      TxBuf[0] = page * EE_PAGE_SIZE;
    }
    for (int i = 0; i < EE_PAGE_SIZE; i++)   // data field
    {
      TxBuf[i + EE_WORD_ADR_SIZE] = *pBuf++;
    }
    msg[0].addr  = EE_ADDR;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = EE_PAGE_SIZE + EE_WORD_ADR_SIZE;
    msg[0].buf   = TxBuf;
    
#if defined(USING_RT_I2C_TRANSFER)
    rt_i2c_transfer(hc32_i2c, &msg[0], 1);
#else
    rt_i2c_master_send(hc32_i2c,EE_ADDR,RT_I2C_NO_STOP,TxBuf,msg[0].len/2);
    rt_i2c_master_send(hc32_i2c,EE_ADDR,RT_I2C_NO_START,TxBuf + msg[0].len/2,msg[0].len - msg[0].len/2);
#endif
    /* write cycle 5ms */
    rt_thread_mdelay(5);
}

static void eeprom_page_read(uint32_t page, uint8_t *pBuf)
{
    struct rt_i2c_bus_device *hc32_i2c = RT_NULL;

#if defined(USING_RT_I2C_TRANSFER)
    struct rt_i2c_msg msg[2];
#endif
    uint8_t readAddr[EE_WORD_ADR_SIZE];
    if (EE_WORD_ADR_SIZE == 2)
    {
      readAddr[0] = (page * EE_PAGE_SIZE) / 256;        // addrH
      readAddr[1] = page * EE_PAGE_SIZE;                // addrL
    } else
    {
      readAddr[0] = page * EE_PAGE_SIZE;
    }

#if defined (BSP_USING_I2C_HW)
    hc32_i2c = rt_i2c_bus_device_find(HW_I2C);          //hw i2c
#else
    hc32_i2c = rt_i2c_bus_device_find("i2c1_sw");   //sw sim i2c
#endif

#if defined(USING_RT_I2C_TRANSFER)    
    msg[0].addr  = EE_ADDR;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = EE_WORD_ADR_SIZE;
    msg[0].buf   = readAddr;

    msg[1].addr  = EE_ADDR;
    msg[1].flags = RT_I2C_RD;
    msg[1].len   = EE_PAGE_SIZE;
    msg[1].buf   = pBuf;
    rt_i2c_transfer(hc32_i2c, &msg[0], 2);
#else
    rt_i2c_master_send(hc32_i2c,EE_ADDR,RT_I2C_NO_STOP,readAddr,EE_WORD_ADR_SIZE);
    rt_i2c_master_recv(hc32_i2c,EE_ADDR,0,pBuf,EE_PAGE_SIZE);
#endif
}

void eeprom_test(void)
{
    uint32_t page;

    for (int32_t i = 0; i < sizeof(trans_buf); i++)
    {
        trans_buf[i] = i;
    }

    for (page = 0; page < TEST_PAGE_CNT; page++)
    {
        eeprom_page_write(page, trans_buf + EE_PAGE_SIZE * page);
    }
    for (int32_t i = 0; i < sizeof(trans_buf); i++)
    {
        recv_buf[i] = 0;
    }
    for (page = 0; page < TEST_PAGE_CNT; page++)
    {
        eeprom_page_read(page, recv_buf + EE_PAGE_SIZE * page);
        rt_kprintf("page %2d: ", page);
        for (uint32_t i = 0; i < EE_PAGE_SIZE; i++)
        {
            rt_kprintf("%2X ", recv_buf[i + (page * EE_PAGE_SIZE)]);
        }
        rt_kprintf("\r\n");
    }
    rt_kprintf("\r\n");
}

static void i2c_sample(int argc, char *argv[])
{
    /* set LED_GREEN_PIN pin mode to output */
    rt_pin_mode(LED_GREEN_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED_GREEN_PIN, PIN_HIGH);
    rt_thread_mdelay(500);
    rt_pin_write(LED_GREEN_PIN, PIN_LOW);
    rt_thread_mdelay(500);
    i2c_test();
    eeprom_test();
}

MSH_CMD_EXPORT(i2c_sample, i2c sample);

#endif/* RT_USING_I2C */

/*
 EOF
*/
