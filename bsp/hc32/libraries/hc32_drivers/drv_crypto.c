/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-10     CDT          first version
 */
#include "board.h"

#if defined(BSP_USING_HWCRYPTO)

#define DRV_DEBUG
#define LOG_TAG             "drv_crypto"
#include <drv_log.h>

struct hc32_hwcrypto_device
{
    struct rt_hwcrypto_device dev;
    struct rt_mutex mutex;
};

#if defined(BSP_USING_CRC)

#define DEFAULT_CRC16_X25_POLY      0x1021       /*!<  X^16 + X^12 + X^5 + 1 */
#define DEFAULT_CRC32_POLY          0x04C11DB7   /*!<  X^32 + X^26 + X^23 + X^22 + X^16 + X^12 + X^11 + X^10 +X^8 + X^7 + X^5 + X^4 + X^2+ X + 1 */

static stc_crc_init_t stcCrcInit;
static stc_crc_init_t stcCrcInit_bk;

static rt_uint32_t _crc_update(struct hwcrypto_crc *ctx, const rt_uint8_t *in, rt_size_t length)
{
    rt_uint32_t result = 0;
    struct hc32_hwcrypto_device *hc32_hw_dev = (struct hc32_hwcrypto_device *)ctx->parent.device->user_data;

    rt_mutex_take(&hc32_hw_dev->mutex, RT_WAITING_FOREVER);

    if (ctx ->crc_cfg.poly != DEFAULT_CRC16_X25_POLY && ctx ->crc_cfg.poly != DEFAULT_CRC32_POLY)
    {
        LOG_E("CRC polynomial only support 0x1021/0x04C11DB7U.");
        goto _exit;
    }

#if defined(HC32F460)
    switch (ctx ->crc_cfg.flags)
    {
    case 0:
        stcCrcInit.u32RefIn = CRC_REFIN_DISABLE;
        stcCrcInit.u32RefOut = CRC_REFOUT_DISABLE;
        break;
    case CRC_FLAG_REFIN:
        stcCrcInit.u32RefIn = CRC_REFIN_ENABLE;
        break;
    case CRC_FLAG_REFOUT:
        stcCrcInit.u32RefOut = CRC_REFOUT_ENABLE;
        break;
    case CRC_FLAG_REFIN | CRC_FLAG_REFOUT:
        stcCrcInit.u32RefIn = CRC_REFIN_ENABLE;
        stcCrcInit.u32RefOut = CRC_REFOUT_ENABLE;
        break;
    default :
        LOG_E("crc flag parameter error.");
        goto _exit;
    }
#endif

    switch(ctx ->crc_cfg.width)
    {
    case 16:
        stcCrcInit.u32Protocol = CRC_CRC16;
        break;
    case 32:
        stcCrcInit.u32Protocol = CRC_CRC32;
        break;
    default :
        LOG_E("crc width only support 16/32.");
        goto _exit;
    }

    stcCrcInit.u32InitValue = ctx ->crc_cfg.last_val;

    if (rt_memcmp(&stcCrcInit_bk, &stcCrcInit, sizeof(stc_crc_init_t)))
    {
        if (CRC_Init(&stcCrcInit) != LL_OK)
        {
            LOG_E("crc init error.");
            goto _exit;
        }
        LOG_D("CRC_Init.");
        rt_memcpy(&stcCrcInit_bk, &stcCrcInit, sizeof(stc_crc_init_t));
    }

    result = CRC_AccumulateData8(in, length);

_exit:
    rt_mutex_release(&hc32_hw_dev->mutex);

    return result;
}

static const struct hwcrypto_crc_ops crc_ops =
{
    .update = _crc_update,
};
#endif /* BSP_USING_CRC */

#if defined(BSP_USING_RNG)
static rt_uint32_t _rng_rand(struct hwcrypto_rng *ctx)
{
    rt_uint32_t gen_random = 0;

    if (TRNG_GenerateRandom(&gen_random, 1U) != LL_OK)
    {
        return 0;
    }

    return gen_random;
}

static const struct hwcrypto_rng_ops rng_ops =
{
    .update = _rng_rand,
};
#endif /* BSP_USING_RNG */

#if defined(BSP_USING_HASH)

#define HASH_MSG_DIGEST_SIZE        (32U)

static const rt_uint8_t *hash_in = RT_NULL;
static rt_size_t hash_length = 0;

#if defined(BSP_HASH_USING_DMA)
#include "drv_dma.h"
#include "dma_config.h"

/* 64byte = 16word = 512bit */
#define HASH_GROUP_SIZE             (64U)
#define HASH_LAST_GROUP_SIZE_MAX    (56U)

#define HASH_DMA_CONFIG                                  \
    {                                                    \
        .Instance       = HASH_DMA_INSTANCE,             \
        .channel        = HASH_DMA_CHANNEL,              \
        .clock          = HASH_DMA_CLOCK,                \
        .trigger_select = HASH_DMA_TRIG_SELECT,          \
        .trigger_event  = EVT_SRC_AOS_STRG,              \
    }

static struct dma_config hash_dma = HASH_DMA_CONFIG;

static uint8_t  u8TempFlag = 0U;
static uint8_t  u8FillFlag = 0U;
static uint32_t u32Temp = 0U;
static uint32_t u32FillData = 0U;
static uint32_t u32Pos = 0U;
static uint32_t u32TsfCnt = 0U;
static uint32_t u32FillBuffer[HASH_GROUP_SIZE / 4];

static rt_err_t DMA_Config(void)
{
    rt_err_t res = RT_EOK;
    stc_dma_init_t stcDmaInit;

    /* Enable DMA. */
    FCG_Fcg0PeriphClockCmd(hash_dma.clock, ENABLE);

    (void)DMA_StructInit(&stcDmaInit);
    /* Configures DMA */
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_32BIT;
    stcDmaInit.u32BlockSize = (HASH_GROUP_SIZE / 4);
    stcDmaInit.u32IntEn     = DMA_INT_DISABLE;
    /* Set transfer count */
    stcDmaInit.u32TransCount = 1U;
    stcDmaInit.u32SrcAddr = (uint32_t)(&u32FillBuffer[0]);
    stcDmaInit.u32DestAddr    = (uint32_t)(&CM_HASH->DR15);
    stcDmaInit.u32SrcAddrInc  = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;
    if (DMA_Init(hash_dma.Instance, hash_dma.channel, &stcDmaInit) != LL_OK) {
        LOG_E("hash dma init failed.");
        FCG_Fcg0PeriphClockCmd(hash_dma.clock, DISABLE);
        res = -RT_ERROR;
        return res;
    }

    /* Set DMA trigger source */
    AOS_SetTriggerEventSrc(hash_dma.trigger_select, hash_dma.trigger_event);
    /* DMA module enable */
    DMA_Cmd(hash_dma.Instance, ENABLE);
    /* DMA channel enable */
    DMA_ChCmd(hash_dma.Instance, hash_dma.channel, ENABLE);

    return res;
}

static void DMA_Reconfig(void)
{
    DMA_SetTransCount(hash_dma.Instance, hash_dma.channel, 1U);
    DMA_SetSrcAddr(hash_dma.Instance, hash_dma.channel, (uint32_t)(&u32FillBuffer[0U]));
    DMA_SetDestAddr(hash_dma.Instance, hash_dma.channel, (uint32_t)(&CM_HASH->DR15));
    /* DMA channel enable */
    DMA_ChCmd(hash_dma.Instance, hash_dma.channel, ENABLE);
}

static void FillData(void)
{
    uint8_t i;
    uint32_t u32BitLenHigh;
    uint32_t u32BitLenLow;
    uint8_t au8Buffer[64U] = {0U};

    u32BitLenHigh = (hash_length >> 29U) & 0x7U;
    u32BitLenLow  = (hash_length << 3U);
    if ((u32FillData >= HASH_LAST_GROUP_SIZE_MAX) && (u8TempFlag == 0U)) {
        (void)memcpy(au8Buffer, &hash_in[HASH_GROUP_SIZE * u32TsfCnt], u32FillData);
        au8Buffer[u32FillData] = 0x80U;
        for (i = 0U; i < HASH_GROUP_SIZE; i += 4U) {
            u32Temp = *((uint32_t *)&au8Buffer[i]);
            u32FillBuffer[i / 4U] = (uint32_t)__REV(u32Temp);
        }
        u8FillFlag = 0U;
        u32FillData = 0U;
        u8TempFlag = 1U;
    } else {
        if (u8TempFlag == 0U) {
            (void)memcpy(au8Buffer, &hash_in[HASH_GROUP_SIZE * u32TsfCnt], u32FillData);
            au8Buffer[u32FillData] = 0x80U;
        }
        au8Buffer[63U] = (uint8_t)(u32BitLenLow);
        au8Buffer[62U] = (uint8_t)(u32BitLenLow >> 8U);
        au8Buffer[61U] = (uint8_t)(u32BitLenLow >> 16U);
        au8Buffer[60U] = (uint8_t)(u32BitLenLow >> 24U);
        au8Buffer[59U] = (uint8_t)(u32BitLenHigh);
        au8Buffer[58U] = (uint8_t)(u32BitLenHigh >> 8U);
        au8Buffer[57U] = (uint8_t)(u32BitLenHigh >> 16U);
        au8Buffer[56U] = (uint8_t)(u32BitLenHigh >> 24U);
        u8FillFlag = 1U;
        for (i = 0U; i < HASH_GROUP_SIZE; i += 4U) {
            u32Temp = *((uint32_t *)&au8Buffer[i]);
            u32FillBuffer[i / 4U] = (uint32_t)__REV(u32Temp);
        }
    }
}
#endif

static void HASH_Config(void)
{
    /* Enable HASH. */
    FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_HASH, ENABLE);
    /* Configures HASH. SHA256 operating mode */
    (void)HASH_SetMode(HASH_MD_SHA256);
#if defined(BSP_HASH_USING_DMA)
    /* Disable interrupt function */
    (void)HASH_IntCmd(HASH_INT_GRP, DISABLE);
    /* Enable AOS. */
    FCG_Fcg0PeriphClockCmd(PWC_FCG0_AOS, ENABLE);
    /* AOS_HASH_A: HASH_TRIG_EVT_XXX_BTCn */
    AOS_SetTriggerEventSrc(AOS_HASH_A, HASH_A_DMA_TRIG_EVT);
#endif
}

static rt_err_t _hash_update(struct hwcrypto_hash *ctx, const rt_uint8_t *in, rt_size_t length)
{
    rt_uint32_t result = RT_EOK;
    uint32_t i = 0;

    struct hc32_hwcrypto_device *hc32_hw_dev = (struct hc32_hwcrypto_device *)ctx->parent.device->user_data;
    rt_mutex_take(&hc32_hw_dev->mutex, RT_WAITING_FOREVER);

    /* Start HASH computation using DMA transfer */
    switch (ctx->parent.type)
    {
    case HWCRYPTO_TYPE_SHA256:
        hash_in = in;
        hash_length = length;
#if defined(BSP_HASH_USING_DMA)
        /* Set the first group. */
        (void)HASH_SetMsgGroup(HASH_MSG_GRP_FIRST);
        /* Start HASH */
        (void)HASH_Start();
        do {
            /* clear the DMA transfer completed flag */
            // DMA_ClearTransCompleteStatus(hash_dma.Instance, HASH_DMA_FLAG);     //todo
            if (strlen((char *)hash_in) <= HASH_LAST_GROUP_SIZE_MAX) {
                /* Set the last group. */
                (void)HASH_SetMsgGroup(HASH_MSG_GRP_END);
            }
            u32FillData = strlen((char *)hash_in) - HASH_GROUP_SIZE * u32TsfCnt;
            if (u32FillData >= HASH_GROUP_SIZE) {
                for (i = 0U; i < (HASH_GROUP_SIZE / 4); i++) {
                    u32Temp = *((uint32_t *)&hash_in[u32Pos]);
                    u32FillBuffer[i] = (uint32_t)__REV(u32Temp);
                    u32Pos += 4U;
                }
                u32TsfCnt++;
            } else {
                FillData();
                if (u8FillFlag == 1U) {
                    /* AOS_HASH_B: HASH_TRIG_EVT_XXX_TCn */
                    AOS_SetTriggerEventSrc(AOS_HASH_B, HASH_B_DMA_TRIG_EVT);
                }
            }
            /* DMA Reconfiguration */
            DMA_Reconfig();
            /* AOS trigger for DMA */
            AOS_SW_Trigger();
        } while (u8FillFlag == 0U);
#endif
       break;
    default :
        LOG_E("not support hash type: %x", ctx->parent.type);
        result = RT_ERROR;
        break;
    }

    rt_mutex_release(&hc32_hw_dev->mutex);

    return  result;
}

static rt_err_t _hash_finish(struct hwcrypto_hash *ctx, rt_uint8_t *out, rt_size_t length)
{
    rt_uint32_t result = RT_EOK;
    rt_uint32_t timeout = 1000000U;

    struct hc32_hwcrypto_device *hc32_hw_dev = (struct hc32_hwcrypto_device *)ctx->parent.device->user_data;
    rt_mutex_take(&hc32_hw_dev->mutex, RT_WAITING_FOREVER);
    
    if (hash_in == RT_NULL || hash_length == 0)
    {
        LOG_E("no data input.");
            result = RT_ERROR;
        goto _exit;
    }

    /* Get the computed digest value */
    switch (ctx->parent.type)
    {
    case HWCRYPTO_TYPE_SHA256:
        if (length == HASH_MSG_DIGEST_SIZE)
        {
#if defined(BSP_HASH_USING_DMA)
            /* Wait for the message operation to complete */
            while (HASH_GetStatus(HASH_FLAG_CYC_END) == RESET && timeout--);
            /* Get the digest result */
            HASH_GetMsgDigest(out);
            /* Clear the flag */
            (void)HASH_ClearStatus(HASH_FLAG_CYC_END);
#else
            result = HASH_Calculate(hash_in, hash_length, out);
#endif
        }
        else 
        {
            LOG_E("The out size must be 32 bytes");
        }
        break;

    default :
        LOG_E("not support hash type: %x", ctx->parent.type);
            result = RT_ERROR;
        break;
    }

_exit:
    rt_mutex_release(&hc32_hw_dev->mutex);

    return result;
}

static const struct hwcrypto_hash_ops hash_ops =
{
    .update = _hash_update,
    .finish  = _hash_finish
};

#endif /* BSP_USING_HASH */

#if defined(BSP_USING_AES)
static rt_err_t _cryp_crypt(struct hwcrypto_symmetric *ctx,
                            struct hwcrypto_symmetric_info *info)
{
    rt_uint32_t result = RT_EOK;
    struct hc32_hwcrypto_device *hc32_hw_dev = (struct hc32_hwcrypto_device *)ctx->parent.device->user_data;
    rt_mutex_take(&hc32_hw_dev->mutex, RT_WAITING_FOREVER);

    switch (ctx->parent.type)
    {
    case HWCRYPTO_TYPE_AES_ECB:
       LOG_D("AES type is ECB.");
       break;
    case HWCRYPTO_TYPE_AES_CBC:
    case HWCRYPTO_TYPE_AES_CTR:
    case HWCRYPTO_TYPE_DES_ECB:
    case HWCRYPTO_TYPE_DES_CBC:
    default :
        LOG_E("not support cryp type: %x", ctx->parent.type);
        break;
    }

#if defined (HC32F460)
    if (ctx->key_bitlen != (AES_KEY_SIZE_16BYTE*8))
    {
        LOG_E("not support key bitlen: %d", ctx->key_bitlen);
        result = RT_ERROR;
        goto _exit;
    }
#elif defined (HC32F4A0)
    if (ctx->key_bitlen != (AES_KEY_SIZE_16BYTE*8) && ctx->key_bitlen != (AES_KEY_SIZE_24BYTE*8) && ctx->key_bitlen != (AES_KEY_SIZE_32BYTE*8))
    {
        LOG_E("not support key bitlen: %d", ctx->key_bitlen);
        result = RT_ERROR;
        goto _exit;
    }
#endif

    if (info->length != 16U)
    {
        LOG_E("aes only support input data length: 16");
        result = RT_ERROR;
        goto _exit;
    }

    if (info->mode == HWCRYPTO_MODE_ENCRYPT)
    {
        /* AES encryption. */
        result = AES_Encrypt(info->in, info->length, ctx->key, (ctx->key_bitlen/8), info->out);
    }
    else if (info->mode == HWCRYPTO_MODE_DECRYPT)
    {
        /* AES decryption */
        result = AES_Decrypt(info->in, info->length, ctx->key, (ctx->key_bitlen/8), info->out);
    }
    else
    {
        rt_kprintf("error cryp mode : %02x!\n", info->mode);
        result = RT_ERROR;
        goto _exit;
    }

_exit:
    rt_mutex_release(&hc32_hw_dev->mutex);

    return result;
}

static const struct hwcrypto_symmetric_ops cryp_ops =
{
    .crypt = _cryp_crypt
};
#endif

static rt_err_t _crypto_create(struct rt_hwcrypto_ctx *ctx)
{
    rt_err_t res = RT_EOK;

    switch (ctx->type & HWCRYPTO_MAIN_TYPE_MASK)
    {
#if defined(BSP_USING_RNG)
    case HWCRYPTO_TYPE_RNG:
    {
        /* Enable TRNG. */
        FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_TRNG, ENABLE);
        /* TRNG initialization configuration. */
        TRNG_Init(TRNG_SHIFT_CNT64, TRNG_RELOAD_INIT_VAL_ENABLE);

        // ctx->contex = RT_NULL;
        ((struct hwcrypto_rng *)ctx)->ops = &rng_ops;

        break;
    }
#endif /* BSP_USING_RNG */

#if defined(BSP_USING_CRC)
    case HWCRYPTO_TYPE_CRC:
    {
        /* Enable CRC module clock. */
        FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_CRC, ENABLE);
        /* Initialize CRC16 */
        CRC_StructInit(&stcCrcInit);
#if defined(BSP_USING_CRC32)
        stcCrcInit.u32Protocol = CRC_CRC32;
        stcCrcInit.u32InitValue = 0xFFFFFFFF;
#else
        stcCrcInit.u32Protocol = CRC_CRC16;
        stcCrcInit.u32InitValue = 0xFFFF;
#endif
        if (CRC_Init(&stcCrcInit) != LL_OK)
        {
            LOG_E("crc init error.");
            res = -RT_ERROR;
            break;
        }
        rt_memcpy(&stcCrcInit_bk, &stcCrcInit, sizeof(stc_crc_init_t));
        // ctx->contex = hcrc;
        ((struct hwcrypto_crc *)ctx)->ops = &crc_ops;

        break;
    }
#endif /* BSP_USING_CRC */

#if defined(BSP_USING_HASH)
    case HWCRYPTO_TYPE_MD5:
    case HWCRYPTO_TYPE_SHA1:
    case HWCRYPTO_TYPE_SHA2:
    {
        if (ctx->type == HWCRYPTO_TYPE_SHA256)
        {
            /* Configures HASH */
            HASH_Config();
        #if defined(BSP_HASH_USING_DMA)
            /* Configures DMA */
            res = DMA_Config();
        #endif
            // ctx->contex = hash;
            ((struct hwcrypto_hash *)ctx)->ops = &hash_ops;
        }
        else
        {
            LOG_E("not support hash type.");
            res = -RT_ERROR;
        }
        break;
    }
#endif /* BSP_USING_HASH */

#if defined(BSP_USING_AES)
    case HWCRYPTO_TYPE_AES:
    case HWCRYPTO_TYPE_DES:
    case HWCRYPTO_TYPE_3DES:
    case HWCRYPTO_TYPE_RC4:
    case HWCRYPTO_TYPE_GCM:
    {
        /* Enable AES peripheral clock. */
        FCG_Fcg0PeriphClockCmd(PWC_FCG0_AES, ENABLE);

        // ctx->contex = cryp;
        ((struct hwcrypto_symmetric *)ctx)->ops = &cryp_ops;

        break;
    }
#endif  /* BSP_USING_AES */

    default:
        res = -RT_ERROR;
        break;
    }
    return res;
}

static void _crypto_destroy(struct rt_hwcrypto_ctx *ctx)
{
    switch (ctx->type & HWCRYPTO_MAIN_TYPE_MASK)
    {
#if defined(BSP_USING_RNG)
    case HWCRYPTO_TYPE_RNG:
        FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_TRNG, DISABLE);
        break;
#endif /* BSP_USING_RNG */

#if defined(BSP_USING_CRC)
    case HWCRYPTO_TYPE_CRC:
        CRC_DeInit();
        FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_CRC, DISABLE);
        break;
#endif /* BSP_USING_CRC */

#if defined(BSP_USING_HASH)
    case HWCRYPTO_TYPE_MD5:
    case HWCRYPTO_TYPE_SHA1:
    case HWCRYPTO_TYPE_SHA2:
        FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_HASH, DISABLE);
        break;
#endif /* BSP_USING_HASH */

#if defined(BSP_USING_AES)
    case HWCRYPTO_TYPE_AES:
    case HWCRYPTO_TYPE_DES:
    case HWCRYPTO_TYPE_3DES:
    case HWCRYPTO_TYPE_RC4:
    case HWCRYPTO_TYPE_GCM:
        FCG_Fcg0PeriphClockCmd(PWC_FCG0_AES, DISABLE);
        break;
#endif /* BSP_USING_AES */

    default:
        break;
    }
}

static rt_err_t _crypto_clone(struct rt_hwcrypto_ctx *des, const struct rt_hwcrypto_ctx *src)
{
    rt_err_t res = RT_EOK;

    switch (src->type & HWCRYPTO_MAIN_TYPE_MASK)
    {
#if defined(BSP_USING_RNG)
    case HWCRYPTO_TYPE_RNG:
        break;
#endif /* BSP_USING_RNG */

#if defined(BSP_USING_CRC)
    case HWCRYPTO_TYPE_CRC:
        break;
#endif /* BSP_USING_CRC */

#if defined(BSP_USING_HASH)
    case HWCRYPTO_TYPE_MD5:
    case HWCRYPTO_TYPE_SHA1:
    case HWCRYPTO_TYPE_SHA2:
        break;
#endif /* BSP_USING_HASH */

#if defined(BSP_USING_AES)
    case HWCRYPTO_TYPE_AES:
    case HWCRYPTO_TYPE_DES:
    case HWCRYPTO_TYPE_3DES:
    case HWCRYPTO_TYPE_RC4:
    case HWCRYPTO_TYPE_GCM:
        break;
#endif /* BSP_USING_AES */

    default:
        res = -RT_ERROR;
        break;
    }
    return res;
}

static void _crypto_reset(struct rt_hwcrypto_ctx *ctx)
{
    switch (ctx->type & HWCRYPTO_MAIN_TYPE_MASK)
    {
#if defined(BSP_USING_RNG)
    case HWCRYPTO_TYPE_RNG:
        break;
#endif /* BSP_USING_RNG */

#if defined(BSP_USING_CRC)
    case HWCRYPTO_TYPE_CRC:
        break;
#endif /* BSP_USING_CRC */

#if defined(BSP_USING_HASH)
    case HWCRYPTO_TYPE_MD5:
    case HWCRYPTO_TYPE_SHA1:
    case HWCRYPTO_TYPE_SHA2:
        break;
#endif /* BSP_USING_HASH*/

#if defined(BSP_USING_AES)
    case HWCRYPTO_TYPE_AES:
    case HWCRYPTO_TYPE_DES:
    case HWCRYPTO_TYPE_3DES:
    case HWCRYPTO_TYPE_RC4:
    case HWCRYPTO_TYPE_GCM:
        break;
#endif /* BSP_USING_AES */

    default:
        break;
    }
}

static const struct rt_hwcrypto_ops _ops =
{
    .create = _crypto_create,
    .destroy = _crypto_destroy,
    .copy = _crypto_clone,
    .reset = _crypto_reset,
};

int hc32_hw_crypto_device_init(void)
{
    static struct hc32_hwcrypto_device _crypto_dev;

#if defined(BSP_USING_UQID)
    stc_efm_unique_id_t pstcUID;
    rt_uint32_t cpuid[3] = {0};

    EFM_GetUID(&pstcUID);
    cpuid[0] = pstcUID.u32UniqueID0;
    cpuid[1] = pstcUID.u32UniqueID1;
    rt_memcpy(&_crypto_dev.dev.id, cpuid, 8);
#endif /* BSP_USING_UQID */

    _crypto_dev.dev.ops = &_ops;
    _crypto_dev.dev.user_data = &_crypto_dev;

    if (rt_hwcrypto_register(&_crypto_dev.dev, RT_HWCRYPTO_DEFAULT_NAME) != RT_EOK)
    {
        return -RT_ERROR;
    }

    rt_mutex_init(&_crypto_dev.mutex, RT_HWCRYPTO_DEFAULT_NAME, RT_IPC_FLAG_PRIO);
    return RT_EOK;
}
INIT_DEVICE_EXPORT(hc32_hw_crypto_device_init);

#endif
