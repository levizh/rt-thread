/*
 * 程序清单：这是 Crypto 设备使用例程。
 * 例程导出了 crypto_sample 到控制终端。
 * 命令调用格式：crypto_sample cmd_id [options]
 * 程序功能：。
 * 注意：测试结果要与标准工具进行对比
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "rtconfig.h"
#include "rtdef.h"
/* rand() located in */
#include "stdlib.h"
#if defined (RT_USING_HWCRYPTO)

enum MODULE_CMD{
    CMD_AES = 0x01,
    CMD_CRC,
    CMD_HASH,
    CMD_RNG
};

/* local macros */
#define RT_HWCRYPTO_IV_MAX_SIZE 16
#define RT_HWCRYPTO_KEYBIT_MAX_SIZE 256

/* crypto device init and data init */
void cypto_data_init(rt_uint8_t *in, rt_int16_t len)
{
    rt_uint16_t i;
    if(NULL == in || len <= 0)
    {
        rt_kprintf("error para\n");
        return;
    }

    for(i = 0; i < len; i++)
    {
        in[i] = rand() % 256;
    }
}

/* aes related test interface */
#if defined (RT_HWCRYPTO_USING_AES)

#define AES256_UNIT_BIT     (256U)
#define AES256_UNIT_BYTE    (AES256_UNIT_BIT / 8U)
/* local varible */
/* F4A0 aes key only support 16, 24 and 32 bytes array */
static const rt_uint8_t
cy_key[] = {
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
        0x39, 0x30, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x49, 0x40, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
        };
rt_uint8_t plain_text[AES256_UNIT_BYTE];
rt_uint8_t cipher_text[AES256_UNIT_BYTE];

/* aes driver test */
static void hw_aes_crypt(const rt_uint8_t in[AES256_UNIT_BYTE],
                        rt_uint8_t out[AES256_UNIT_BYTE], hwcrypto_mode mode)
{
    rt_err_t ret;
    struct rt_hwcrypto_ctx *ctx;

    /* create an AES-CBC ctx */
    ctx = rt_hwcrypto_symmetric_create(rt_hwcrypto_dev_default(),
                                        HWCRYPTO_TYPE_AES_ECB);
    if (ctx == RT_NULL)
    {
        rt_kprintf("create AES-CBC context err!\n");
        return;
    }
    /* set key into AES-EBC */
    ret = rt_hwcrypto_symmetric_setkey(ctx, cy_key, sizeof(cy_key) / sizeof(cy_key[0]) * 8);
    if(RT_EOK != ret)
    {
        rt_kprintf("failed to set aes key\n");
        goto _exit;
    }
    /* AES-CBC encrypt/decrypt */
    rt_hwcrypto_symmetric_crypt(ctx, mode, AES256_UNIT_BYTE, in, out);
    if(RT_EOK != ret)
    {
        rt_kprintf("failed to aes crtpy key\n");
        goto _exit;
    }
    /* delete ctx */
_exit:
    rt_hwcrypto_symmetric_destroy(ctx);
}

static void crypto_aes_sample(void)
{
    rt_uint16_t i;

    cypto_data_init(plain_text, AES256_UNIT_BYTE);
    rt_kprintf("\norigin plain text(hex):\n");
    for(i = 0; i < AES256_UNIT_BYTE; i++)
    {
        /* print ascii symbol for the convenience of online-tool validation */
        rt_kprintf("%02x ", plain_text[i]);
        if((i + 1) % 16 == 0)
        {
            rt_kprintf("\n");
        }
    }

    rt_kprintf("key(asiic):\n");
    for(i = 0; i < sizeof(cy_key) / sizeof(cy_key[0]); i++)
    {
        rt_kprintf("%02c ", cy_key[i]);
        if((i + 1) % 16 == 0)
        {
            rt_kprintf("\n");
        }
    }

    rt_memset(cipher_text, 0, AES256_UNIT_BYTE);
    /* aes encrypt */
    hw_aes_crypt(plain_text, cipher_text, HWCRYPTO_MODE_ENCRYPT);
    /* print cipher text and compare with standard tool's output */
    rt_kprintf("\naes cipher text(hex):\n");
    for(i = 0; i < AES256_UNIT_BYTE; i++)
    {
        rt_kprintf("%02x ", cipher_text[i]);
        if((i + 1) % 16 == 0)
        {
            rt_kprintf("\n");
        }
    }

    rt_memset(plain_text, 0, AES256_UNIT_BYTE);
    hw_aes_crypt(cipher_text, plain_text, HWCRYPTO_MODE_DECRYPT);

    rt_kprintf("\naes decrypt plain text(hex):\n");
    for(i = 0; i < AES256_UNIT_BYTE; i++)
    {
        rt_kprintf("%02x ", plain_text[i]);
        if((i + 1) % 16 == 0)
        {
            rt_kprintf("\n");
        }
    }
    rt_kprintf("\n");
}
#endif /* RT_HWCRYPTO_USING_AES */

/* crc related test interface */
#if defined (RT_HWCRYPTO_USING_CRC)
/* define crc input data length */
#define CRC_ARRAY_BYTE      (128U)

static uint32_t crypto_crc_calculate(hwcrypto_crc_mode mode,
                            rt_uint8_t *in, rt_uint16_t len)
{
    rt_uint32_t result = 0;
    struct rt_hwcrypto_ctx *ctx;
    struct hwcrypto_crc_cfg cfg;

    switch(mode)
    {
        case HWCRYPTO_CRC_CRC16:
            cfg.last_val = 0xFFFFU;
            cfg.poly = 0x1021U;
            cfg.width = 16;
            cfg.xorout = 0xFFFFU;
            break;
        case HWCRYPTO_CRC_CRC32:
        default:
            cfg.last_val = 0xFFFFFFFFUL,
            cfg.poly = 0x04C11DB7UL;
            cfg.width = 32;
            cfg.xorout = 0xFFFFFFFFUL;
            break;
    }
    /* f4a0 hw use CRC_FLAG_REFIN | CRC_FLAG_REFOUT as default config,
        and can't be changed */
    cfg.flags = 0;

    ctx = rt_hwcrypto_crc_create(rt_hwcrypto_dev_default(), mode);
    if(ctx == RT_NULL)
    {
        rt_kprintf("failed to create crc\n");
        return 0;
    }
    /* config crc */
    rt_hwcrypto_crc_cfg(ctx, &cfg);
    /* set data */
    result = rt_hwcrypto_crc_update(ctx, in, len);
    rt_hwcrypto_crc_destroy(ctx);
    return result;
}

static void crypto_crc_sample(void)
{
    rt_uint32_t i, result;
    rt_uint8_t crc_array[CRC_ARRAY_BYTE];

    /* initialize array with rand value */
    cypto_data_init(crc_array, CRC_ARRAY_BYTE);
    rt_kprintf("origin hex code:\n");

    for(i = 0; i < CRC_ARRAY_BYTE; i++)
    {
        rt_kprintf("%02x ", crc_array[i]);
        if((i + 1) % 16 == 0)
        {
              rt_kprintf("\n");
        }
    }
    /* calculate crc */
    result = crypto_crc_calculate(HWCRYPTO_CRC_CRC16,
        crc_array, CRC_ARRAY_BYTE);
    rt_kprintf("crc-16 result %04x\n", result);

    result = crypto_crc_calculate(HWCRYPTO_CRC_CRC32,
        crc_array, CRC_ARRAY_BYTE);
    rt_kprintf("crc-32 result %08x\n", result);
}
#endif /* RT_HWCRYPTO_USING_CRC */

/* hash(sha-256) related test interface */
#if defined (RT_HWCRYPTO_USING_SHA2)

#define SHA256_UNIT_BIT     (256U)
#define SHA256_UNIT_BYTE    (SHA256_UNIT_BIT / 8)

void crypto_sha2_sample(void)
{
    rt_uint16_t i, len;
    rt_uint8_t in[SHA256_UNIT_BYTE * 2 - 10], out[SHA256_UNIT_BYTE];
    struct rt_hwcrypto_ctx *ctx;

    /* create sha2 pointer */
    ctx = rt_hwcrypto_hash_create(rt_hwcrypto_dev_default(), HWCRYPTO_TYPE_SHA256);
    if (ctx == RT_NULL)
    {
        rt_kprintf("failed to create hash512\n");
        return;
    }
    len = sizeof(in) / sizeof(in[0]);
    cypto_data_init(in, len);
    rt_kprintf("\norigin data(hex):\n");
    for(i = 0; i < len; i++)
    {
        rt_kprintf("%02x ", in[i]);
        if((i + 1) % 16 == 0)
        {
            rt_kprintf("\n");
        }
    }
    /* input data for hash calculation */
    rt_hwcrypto_hash_update(ctx, in, len);
    /* get result out */
    rt_hwcrypto_hash_finish(ctx, out, SHA256_UNIT_BYTE);
    rt_kprintf("\nhash code(hex):\n");
    for(i = 0; i < SHA256_UNIT_BYTE; i++)
    {
        rt_kprintf("%02x ", out[i]);
        if((i + 1) % 16 == 0)
        {
            rt_kprintf("\n");
        }
    }
    /* delete context pointer */
    rt_hwcrypto_hash_destroy(ctx);
}
#endif /* RT_HWCRYPTO_USING_SHA2 */

/* rng related test interface */
#if defined (RT_HWCRYPTO_USING_RNG)
#define TRNG_TEST_TIMES     (100 * 10000)
void crypto_rng_sample(void)
{
    rt_uint32_t odd_cnt = 0, even_cnt = 0, i, result;

    rt_kprintf("rng generating...\n");
    for(i = 0; i < TRNG_TEST_TIMES; i++)
    {
        result = rt_hwcrypto_rng_update();
        result % 2 ? ++even_cnt : ++odd_cnt;
    }
    rt_kprintf("even num: %d, odd num: %d\n", even_cnt, odd_cnt);
}
#endif /* RT_HWCRYPTO_USING_RNG */

void crypto_sample(int argc, char *argv[])
{
    if(argc < 2)
    {
        rt_kprintf("crypto_sample [usage] [options]:\n");
        rt_kprintf("%*c %d aes-256 test sample\n", 4, ' ', CMD_AES);
        rt_kprintf("%*c %d crc-16, crc-32 test\n", 4, ' ', CMD_CRC);
        rt_kprintf("%*c %d hash sha256 test\n", 4, ' ', CMD_HASH);
        rt_kprintf("%*c %d trng test\n", 4, ' ', CMD_RNG);
        return;
    }
    switch(*argv[1] - '0')
    {
#if defined (RT_HWCRYPTO_USING_AES)
        case CMD_AES:
            crypto_aes_sample();
            break;
#endif /* RT_HWCRYPTO_USING_AES */

#if defined (RT_HWCRYPTO_USING_CRC)
        case CMD_CRC:
            crypto_crc_sample();
            break;
#endif /* RT_HWCRYPTO_USING_CRC */

#if defined (RT_HWCRYPTO_USING_SHA2_256)
        case CMD_HASH:
            crypto_sha2_sample();
            break;
#endif /* RT_HWCRYPTO_USING_SHA2_256 */

#if defined (RT_HWCRYPTO_USING_RNG)
        case CMD_RNG:
        crypto_rng_sample();
        break;
#endif /* RT_HWCRYPTO_USING_RNG */

        default:
            rt_kprintf("command %d under support\n", *argv[1] - '0');
            break;
    }
}
MSH_CMD_EXPORT(crypto_sample, crypto sample);

#endif /* RT_USING_HWCRYPTO */
 /*
 EOF
*/
