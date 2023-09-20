/*
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author               Notes
 * 2022-04-28     CDT                  first version
 * 2022-06-07     xiaoxiaolisunny      add hc32f460 series
 * 2022-06-08     CDT                  fix a bug of RT_CAN_CMD_SET_FILTER
 * 2022-06-15     lianghongquan        fix bug, FILTER_COUNT, RT_CAN_CMD_SET_FILTER, interrupt setup and processing.
 */

#include "drv_can.h"
#include <drv_config.h>
#include <board_config.h>

#if defined(BSP_USING_CAN)
#define LOG_TAG    "drv_can"

#if defined(BSP_USING_CAN1) || defined(BSP_USING_CAN2)


#if defined(RT_CAN_USING_CANFD) && defined(HC32F460)
    #error "Selected mcu does not support canfd!"
#endif

#define TSEG1_MIN_FOR_CAN2_0                                (2U)
#define TSEG1_MAX_FOR_CAN2_0                                (65U)
#define TSEG1_MIN_FOR_CANFD_NORMINAL                        (2U)
#define TSEG1_MAX_FOR_CANFD_NORMINAL                        (65U)
#define TSEG1_MIN_FOR_CANFD_DATA                            (2U)
#define TSEG1_MAX_FOR_CANFD_DATA                            (17U)

#define TSEG2_MIN_FOR_CAN2_0                                (1U)
#define TSEG2_MAX_FOR_CAN2_0                                (8U)
#define TSEG2_MIN_FOR_CANFD_NORMINAL                        (1U)
#define TSEG2_MAX_FOR_CANFD_NORMINAL                        (32U)
#define TSEG2_MIN_FOR_CANFD_DATA                            (1U)
#define TSEG2_MAX_FOR_CANFD_DATA                            (8U)

#define TSJW_MIN_FOR_CAN2_0                                 (1U)
#define TSJW_MAX_FOR_CAN2_0                                 (16U)
#define TSJW_MIN_FOR_CANFD_NORMINAL                         (1U)
#define TSJW_MAX_FOR_CANFD_NORMINAL                         (16U)
#define TSJW_MIN_FOR_CANFD_DATA                             (1U)
#define TSJW_MAX_FOR_CANFD_DATA                             (8U)

#define NUM_TQ_MIN_FOR_CAN2_0                               (8U)
#define NUM_TQ_MAX_FOR_CAN2_0                               (TSEG1_MAX_FOR_CAN2_0 + TSEG2_MAX_FOR_CAN2_0)
#define NUM_TQ_MIN_FOR_CANFD_NOMINAL                        (8U)
#define NUM_TQ_MAX_FOR_CANFD_NOMINAL                        (TSEG1_MAX_FOR_CANFD_NORMINAL + TSEG2_MAX_FOR_CANFD_NORMINAL)
#define NUM_TQ_MIN_FOR_CANFD_DATA                           (8U)
#define NUM_TQ_MAX_FOR_CANFD_DATA                           (TSEG1_MAX_FOR_CANFD_DATA + TSEG2_MAX_FOR_CANFD_DATA)

#define NUM_PRESCALE_MAX                                    (256U)
#define MIN_TQ_MUL_PRESCALE                                 (4U)

#define CAN_BIT_TIMING_CAN2_0                               (1U << 0)
#define CAN_BIT_TIMING_CANFD_NORMINAL                       (1U << 1)
#define CAN_BIT_TIMING_CANFD_DATA                           (1U << 2)

#if defined(HC32F4A0) || defined(HC32F4A2)
    #define FILTER_COUNT                                    (16U)
    #define CAN1_INT_SRC                                    (INT_SRC_CAN1_HOST)
    #define CAN2_INT_SRC                                    (INT_SRC_CAN2_HOST)
#endif

#if defined (HC32F460)
    #define FILTER_COUNT                                    (8U)
    #define CAN1_INT_SRC                                    (INT_SRC_CAN_INT)
#endif

enum
{
#ifdef BSP_USING_CAN1
    CAN1_INDEX,
#endif
#ifdef BSP_USING_CAN2
    CAN2_INDEX,
#endif
    CAN_INDEX_MAX,
};

struct can_baud_rate_tab
{
    rt_uint32_t baud_rate;
    stc_can_bit_time_config_t ll_sbt;
};

struct canfd_baud_rate_tab
{
    rt_uint32_t clk_src;
    rt_uint8_t phase;
    rt_uint32_t baud;
    stc_can_bit_time_config_t ll_bt;
};

typedef struct
{
    uint8_t tq_min;
    uint8_t tq_max;
    uint8_t seg1_min;
    uint8_t seg1_max;
    uint8_t seg2_min;
    uint8_t seg2_max;
    uint8_t sjw_min;
    uint8_t sjw_max;
    uint8_t min_diff_seg1_minus_seg2;
} can_bit_timing_table_t;

#ifndef RT_CAN_USING_CANFD
static const struct can_baud_rate_tab g_baudrate_tab[] =
{
    {CAN1MBaud,   CAN_BIT_TIME_CONFIG_1M_BAUD},
    {CAN800kBaud, CAN_BIT_TIME_CONFIG_800K_BAUD},
    {CAN500kBaud, CAN_BIT_TIME_CONFIG_500K_BAUD},
    {CAN250kBaud, CAN_BIT_TIME_CONFIG_250K_BAUD},
    {CAN125kBaud, CAN_BIT_TIME_CONFIG_125K_BAUD},
    {CAN100kBaud, CAN_BIT_TIME_CONFIG_100K_BAUD},
    {CAN50kBaud,  CAN_BIT_TIME_CONFIG_50K_BAUD},
    {CAN20kBaud,  CAN_BIT_TIME_CONFIG_20K_BAUD},
    {CAN10kBaud,  CAN_BIT_TIME_CONFIG_10K_BAUD},
};
#endif

typedef struct
{
    struct rt_can_device rt_can;
    struct can_dev_init_params init;
    CM_CAN_TypeDef *instance;
    stc_can_init_t ll_init;
} can_device;

#ifdef RT_CAN_USING_CANFD
static const can_bit_timing_table_t s_can_bit_timing_tbl[3] =
{
    {
        .tq_min = NUM_TQ_MIN_FOR_CAN2_0,
        .tq_max = NUM_TQ_MAX_FOR_CAN2_0,
        .seg1_min = TSEG1_MIN_FOR_CAN2_0,
        .seg1_max = TSEG1_MAX_FOR_CAN2_0,
        .seg2_min = TSEG2_MIN_FOR_CAN2_0,
        .seg2_max = TSEG2_MAX_FOR_CAN2_0,
        .sjw_min = TSJW_MIN_FOR_CAN2_0,
        .sjw_max = TSJW_MAX_FOR_CAN2_0,
        .min_diff_seg1_minus_seg2 = 2,
    },
    {
        .tq_min = NUM_TQ_MIN_FOR_CANFD_NOMINAL,
        .tq_max = NUM_TQ_MAX_FOR_CANFD_NOMINAL,
        .seg1_min = TSEG1_MIN_FOR_CANFD_NORMINAL,
        .seg1_max = TSEG1_MAX_FOR_CANFD_NORMINAL,
        .seg2_min = TSEG2_MIN_FOR_CANFD_NORMINAL,
        .seg2_max = TSEG2_MAX_FOR_CANFD_NORMINAL,
        .sjw_min = TSJW_MIN_FOR_CANFD_NORMINAL,
        .sjw_max = TSJW_MAX_FOR_CANFD_NORMINAL,
        .min_diff_seg1_minus_seg2 = 2,
    },
    {
        .tq_min = NUM_TQ_MIN_FOR_CANFD_DATA,
        .tq_max = NUM_TQ_MAX_FOR_CANFD_DATA,
        .seg1_min = TSEG1_MIN_FOR_CANFD_DATA,
        .seg1_max = TSEG1_MAX_FOR_CANFD_DATA,
        .seg2_min = TSEG2_MIN_FOR_CANFD_DATA,
        .seg2_max = TSEG2_MAX_FOR_CANFD_DATA,
        .sjw_min = TSJW_MIN_FOR_CANFD_DATA,
        .sjw_max = TSJW_MAX_FOR_CANFD_DATA,
        .min_diff_seg1_minus_seg2 = 1,
    }
};

static const struct canfd_baud_rate_tab g_baudrate_fd[] =
{
    {CAN_CLOCK_SRC_20M, CAN_BIT_TIMING_CANFD_NORMINAL, CANFD_ARBITRATION_BAUD_250K, 1U, 64U, 16U, 16U},
    {CAN_CLOCK_SRC_20M, CAN_BIT_TIMING_CANFD_NORMINAL, CANFD_ARBITRATION_BAUD_500K, 1U, 32U, 8U, 8U},
    {CAN_CLOCK_SRC_20M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_1M, 1U, 16U, 4U, 4U},
    {CAN_CLOCK_SRC_20M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_2M, 1U, 8U, 2U, 2U},
    {CAN_CLOCK_SRC_20M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_4M, 1U, 4U, 1U, 1U},
    {CAN_CLOCK_SRC_20M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_5M, 1U, 3U, 1U, 1U},
    {CAN_CLOCK_SRC_40M, CAN_BIT_TIMING_CANFD_NORMINAL, CANFD_ARBITRATION_BAUD_250K, 2U, 64U, 16U, 16U},
    {CAN_CLOCK_SRC_40M, CAN_BIT_TIMING_CANFD_NORMINAL, CANFD_ARBITRATION_BAUD_500K, 1U, 64U, 16U, 16U},
    {CAN_CLOCK_SRC_40M, CAN_BIT_TIMING_CANFD_NORMINAL, CANFD_DATA_BAUD_1M, 1U, 32U, 8U, 8U},
    {CAN_CLOCK_SRC_40M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_2M, 1U, 16U, 4U, 4U},
    {CAN_CLOCK_SRC_40M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_4M, 1U, 8U, 2U, 2U},
    {CAN_CLOCK_SRC_40M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_5M, 1U, 6U, 2U, 2U},
    {CAN_CLOCK_SRC_40M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_8M, 1U, 4U, 1U, 1U},
    {CAN_CLOCK_SRC_80M, CAN_BIT_TIMING_CANFD_NORMINAL, CANFD_ARBITRATION_BAUD_250K, 4U, 64U, 16U},
    {CAN_CLOCK_SRC_80M, CAN_BIT_TIMING_CANFD_NORMINAL, CANFD_ARBITRATION_BAUD_500K, 2U, 64U, 16U},
    {CAN_CLOCK_SRC_80M, CAN_BIT_TIMING_CANFD_NORMINAL, CANFD_DATA_BAUD_1M, 2U, 32U, 8U, 8U},
    {CAN_CLOCK_SRC_80M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_2M, 2U, 16U, 4U, 4U},
    {CAN_CLOCK_SRC_80M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_4M, 1U, 16U, 4U, 4U},
    {CAN_CLOCK_SRC_80M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_5M, 1U, 12U, 4U, 4U},
    {CAN_CLOCK_SRC_80M, CAN_BIT_TIMING_CANFD_NORMINAL | CAN_BIT_TIMING_CANFD_DATA, CANFD_DATA_BAUD_8M, 1U, 8U, 2U, 2U},
};
#endif

static can_device g_can_dev_array[] =
{
#if defined(HC32F4A0) || defined(HC32F4A2)
#ifdef BSP_USING_CAN1
    {
        {0},
        CAN1_INIT_PARAMS,
        .instance = CM_CAN1,
    },
#endif
#ifdef BSP_USING_CAN2
    {
        {0},
        CAN2_INIT_PARAMS,
        .instance = CM_CAN2,
    },
#endif
#endif

#if defined (HC32F460)
#ifdef BSP_USING_CAN1
    {
        {0},
        CAN1_INIT_PARAMS,
        .instance = CM_CAN,
    },
#endif
#endif
};

#ifndef RT_CAN_USING_CANFD
static rt_uint32_t _get_can_baud_index(rt_uint32_t baud)
{
    rt_uint32_t len, index;

    len = sizeof(g_baudrate_tab) / sizeof(g_baudrate_tab[0]);
    for (index = 0; index < len; index++)
    {
        if (g_baudrate_tab[index].baud_rate == baud)
            return index;
    }

    return 0; /* default baud is CAN1MBaud */
}
#endif

static rt_uint32_t _get_can_work_mode(rt_uint32_t mode)
{
    rt_uint32_t work_mode;
    switch (mode)
    {
    case RT_CAN_MODE_NORMAL:
        work_mode = CAN_WORK_MD_NORMAL;
        break;
    case RT_CAN_MODE_LISTEN:
        work_mode = CAN_WORK_MD_SILENT;
        break;
    case RT_CAN_MODE_LOOPBACK:
        work_mode = CAN_WORK_MD_ELB;
        break;
    case RT_CAN_MODE_LOOPBACKANLISTEN:
        work_mode = CAN_WORK_MD_ELB_SILENT;
        break;
    default:
        work_mode = CAN_WORK_MD_NORMAL;
        break;
    }

    return work_mode;
}

#ifdef RT_CAN_USING_CANFD
static uint32_t _can_get_clk_src(CM_CAN_TypeDef *CANx)
{
    uint32_t can_clk = 0;
    switch ((rt_uint32_t)CANx)
    {
#ifdef BSP_USING_CAN1
    case (rt_uint32_t)CM_CAN1:
        can_clk = CAN1_CLOCK_SEL;
        break;
#endif
#ifdef BSP_USING_CAN2
    case (rt_uint32_t)CM_CAN2:
        can_clk = CAN2_CLOCK_SEL;
        break;
#endif
    default:
        break;
    }
    return can_clk;
}

static rt_bool_t _can_get_default_bit_timing(uint32_t can_clk, rt_uint32_t baud, rt_uint32_t option,
        stc_can_bit_time_config_t *p_stc_bit_cfg)
{
    rt_uint32_t len, index;
    rt_bool_t found = RT_FALSE;

    len = sizeof(g_baudrate_fd) / sizeof(g_baudrate_fd[0]);
    for (index = 0; index < len; index++)
    {
        if ((g_baudrate_fd[index].clk_src == can_clk) && \
                ((g_baudrate_fd[index].phase & option) == option) \
           )
        {
            if (g_baudrate_fd[index].baud == baud)
            {
                found = RT_TRUE;
                break;
            }
        }
    }
    if (found)
    {
        rt_memcpy(p_stc_bit_cfg, &g_baudrate_fd[index].ll_bt, sizeof(stc_can_bit_time_config_t));
    }

    return found;
}

static rt_err_t _can_find_closest_prescaler(uint32_t num_tq_mul_prescaler, uint32_t start_prescaler,
        uint32_t max_tq, uint32_t min_tq)
{
    rt_bool_t has_found = RT_FALSE;
    uint32_t prescaler = start_prescaler;

    while (!has_found)
    {
        if ((num_tq_mul_prescaler / prescaler > max_tq) || (num_tq_mul_prescaler % prescaler != 0))
        {
            ++prescaler;
            continue;
        }
        else
        {
            has_found = RT_TRUE;
            break;
        }
    }

    uint32_t tq = num_tq_mul_prescaler / prescaler;
    if (tq * prescaler == num_tq_mul_prescaler)
    {
        has_found = RT_TRUE;
    }
    else if (tq < min_tq)
    {
        has_found = RT_FALSE;
    }

    return has_found ? prescaler : 0U;
}

static rt_err_t _can_get_bit_timing(CM_CAN_TypeDef *CANx, int option, uint32_t baudrate,
                                    stc_can_bit_time_config_t *p_stc_bit_cfg)
{
    rt_err_t status = -RT_ERROR;
    uint32_t can_clk = _can_get_clk_src(CANx);
    if (_can_get_default_bit_timing(can_clk, baudrate, option, p_stc_bit_cfg) == RT_TRUE)
    {
        status = RT_EOK;
        return status;
    }

    do
    {
        uint32_t idx = 0;
        for (int i = 0; i < 3; i++)
        {
            if (option & 1 << i)
            {
                idx = i;
                break;
            }
        }
        if ((idx > CAN_BIT_TIMING_CANFD_DATA) || (baudrate == 0U) ||
                (can_clk / baudrate < MIN_TQ_MUL_PRESCALE) || (p_stc_bit_cfg == NULL))
        {
            break;
        }

        const can_bit_timing_table_t *tbl = &s_can_bit_timing_tbl[(uint8_t) idx];
        if (can_clk / baudrate < tbl->tq_min)
        {
            break;
        }

        uint32_t num_tq_mul_prescaler = can_clk / baudrate;
        uint32_t start_prescaler = 1U;
        uint32_t num_seg1, num_seg2;
        rt_bool_t has_found = RT_FALSE;

        /* Find out the minimum prescaler */
        uint32_t current_prescaler;
        while (!has_found)
        {
            current_prescaler = _can_find_closest_prescaler(num_tq_mul_prescaler, start_prescaler,
                                tbl->tq_max,
                                tbl->tq_min);
            if ((current_prescaler < start_prescaler) || (current_prescaler > NUM_PRESCALE_MAX))
            {
                break;
            }
            uint32_t num_tq = num_tq_mul_prescaler / current_prescaler;

            num_seg2 = (num_tq - tbl->min_diff_seg1_minus_seg2) / 2U;
            num_seg1 = num_tq - num_seg2;
            while (num_seg2 > tbl->seg2_max)
            {
                num_seg2--;
                num_seg1++;
            }

            /* Recommended sample point is 75% - 80% */
            while ((num_seg1 * 1000U) / num_tq < CAN_SAMPLEPOINT_MIN)
            {
                ++num_seg1;
                --num_seg2;
            }

            if ((num_seg1 * 1000U) / num_tq > CAN_SAMPLEPOINT_MAX)
            {
                break;
            }

            if ((num_seg2 >= tbl->seg2_min) && (num_seg1 <= tbl->seg1_max))
            {
                has_found = RT_TRUE;
            }
            else
            {
                start_prescaler = current_prescaler + 1U;
            }
        }

        if (has_found)
        {
            uint32_t num_sjw = LL_MIN(tbl->sjw_max, num_seg2);
            p_stc_bit_cfg->u32TimeSeg1 = num_seg1;
            p_stc_bit_cfg->u32TimeSeg2 = num_seg2;
            p_stc_bit_cfg->u32SJW = num_sjw;
            p_stc_bit_cfg->u32Prescaler = current_prescaler;
            status = RT_EOK;
        }
    }
    while (RT_FALSE);

    return status;
}
#else
static rt_err_t _can20_config_baud(can_device *p_can_dev, void *arg)
{
    rt_uint32_t argval = *(rt_uint32_t *)arg;
    rt_uint32_t baud_index;
    rt_err_t rt_ret = RT_EOK;

    if (argval != CAN1MBaud &&
            argval != CAN800kBaud &&
            argval != CAN500kBaud &&
            argval != CAN250kBaud &&
            argval != CAN125kBaud &&
            argval != CAN100kBaud &&
            argval != CAN50kBaud  &&
            argval != CAN20kBaud  &&
            argval != CAN10kBaud)
    {
        return -RT_ERROR;
    }
    if (argval == p_can_dev->rt_can.config.baud_rate)
    {
        return rt_ret;
    }

    baud_index = _get_can_baud_index(argval);
    p_can_dev->ll_init.stcBitCfg = g_baudrate_tab[baud_index].ll_sbt;

    /* init can */
    int32_t ret = CAN_Init(p_can_dev->instance, &p_can_dev->ll_init);
    if (ret != LL_OK)
    {
        rt_ret = -RT_EINVAL;
    }
    else
    {
        p_can_dev->rt_can.config.baud_rate = argval;
    }

    return rt_ret;
}
#endif

static rt_err_t _can_config(struct rt_can_device *can, struct can_configure *cfg)
{
    can_device *p_can_dev;
    rt_err_t rt_ret = RT_EOK;

    RT_ASSERT(can);
    RT_ASSERT(cfg);
    p_can_dev = (can_device *)rt_container_of(can, can_device, rt_can);
    RT_ASSERT(p_can_dev);

    p_can_dev->ll_init.u8WorkMode = _get_can_work_mode(cfg->mode);
#ifdef RT_CAN_USING_CANFD
    rt_ret = _can_get_bit_timing(p_can_dev->instance, \
                                 CAN_BIT_TIMING_CANFD_NORMINAL, \
                                 p_can_dev->rt_can.config.baud_rate, \
                                 &p_can_dev->ll_init.stcBitCfg);
    if (rt_ret != RT_EOK)
    {
        return rt_ret;
    }
    rt_ret = _can_get_bit_timing(p_can_dev->instance, \
                                 CAN_BIT_TIMING_CANFD_DATA, \
                                 p_can_dev->rt_can.config.baud_rate_fd, \
                                 &p_can_dev->ll_init.pstcCanFd->stcBitCfg);
    if (rt_ret != RT_EOK)
    {
        return rt_ret;
    }
    p_can_dev->ll_init.pstcCanFd->u8SSPOffset = p_can_dev->ll_init.pstcCanFd->stcBitCfg.u32TimeSeg1;
#else
    rt_uint32_t baud_index = _get_can_baud_index(cfg->baud_rate);
    p_can_dev->ll_init.stcBitCfg = g_baudrate_tab[baud_index].ll_sbt;
#endif
    /* init can */
    int32_t ret = CAN_Init(p_can_dev->instance, &p_can_dev->ll_init);
    if (ret != LL_OK)
    {
        rt_ret = -RT_EINVAL;
    }
#ifdef RT_CAN_USING_CANFD
    p_can_dev->rt_can.config.baud_rate_fd = cfg->baud_rate_fd;
#endif
    p_can_dev->rt_can.config.baud_rate = cfg->baud_rate;

    return rt_ret;
}

static uint16_t _get_filter_idx(struct rt_can_filter_config *filter_cfg)
{
    uint16_t u16FilterSelected = 0;

    for (int i = 0; i < filter_cfg->count; i++)
    {
        if (filter_cfg->items[i].hdr != -1)
        {
            u16FilterSelected |= 1 << filter_cfg->items[i].hdr;
        }
    }

    for (int i = 0; i < filter_cfg->count; i++)
    {
        if (filter_cfg->items[i].hdr == -1)
        {
            for (int j = 0; j < FILTER_COUNT; j++)
            {
                if ((u16FilterSelected & 1 << j) == 0)
                {
                    filter_cfg->items[i].hdr = j;
                    u16FilterSelected |= 1 << filter_cfg->items[i].hdr;
                    break;
                }
            }
        }
    }

    return u16FilterSelected;
}

static rt_err_t _can_config_filter(can_device *p_can_dev, void *arg)
{
    struct rt_can_filter_config *filter_cfg;
    filter_cfg = (struct rt_can_filter_config *)arg;

    if (filter_cfg == NULL || filter_cfg->count == 0)
    {
        return -RT_EINVAL;
    }
    RT_ASSERT(filter_cfg->count <= FILTER_COUNT);

    /* get default filter */
    if (p_can_dev->ll_init.pstcFilter)
    {
        p_can_dev->ll_init.u16FilterSelect = _get_filter_idx(filter_cfg);
        for (int i = 0; i < filter_cfg->count; i++)
        {
            p_can_dev->ll_init.pstcFilter[i].u32ID = filter_cfg->items[i].id & 0x1FFFFFFF;
            /* rt-thread CAN mask, 1 mean filer, 0 mean ignore. *
                * HDSC HC32 CAN mask, 0 mean filer, 1 mean ignore. */
            p_can_dev->ll_init.pstcFilter[i].u32IDMask = (~filter_cfg->items[i].mask) & 0x1FFFFFFF;
            switch (filter_cfg->items[i].ide)
            {
            case (RT_CAN_STDID):
                p_can_dev->ll_init.pstcFilter[i].u32IDType = CAN_ID_STD;
                break;
            case (RT_CAN_EXTID):
                p_can_dev->ll_init.pstcFilter[i].u32IDType = CAN_ID_EXT;
                break;
            default:
                p_can_dev->ll_init.pstcFilter[i].u32IDType = CAN_ID_STD_EXT;
                break;
            }
        }
    }
    (void)CAN_Init(p_can_dev->instance, &p_can_dev->ll_init);

    return RT_EOK;
}

static rt_err_t _can_config_work_mode(can_device *p_can_dev, void *arg)
{
    rt_err_t rt_ret = RT_EOK;
    rt_uint32_t argval = (rt_uint32_t) arg;

    if (argval != RT_CAN_MODE_NORMAL &&
            argval != RT_CAN_MODE_LISTEN &&
            argval != RT_CAN_MODE_LOOPBACK &&
            argval != RT_CAN_MODE_LOOPBACKANLISTEN)
    {
        return -RT_ERROR;
    }
    if (argval == p_can_dev->rt_can.config.mode)
    {
        return rt_ret;
    }

    p_can_dev->ll_init.u8WorkMode = _get_can_work_mode(argval);
    /* init can */
    int32_t ret = CAN_Init(p_can_dev->instance, &p_can_dev->ll_init);
    if (ret != LL_OK)
    {
        rt_ret = -RT_EINVAL;
    }

    return rt_ret;
}

static uint8_t _can_get_data_bytes_len(uint32_t dlc)
{
    uint8_t data_bytes = 0;

    dlc &= 0xFU;
    if (dlc <= 8U)
    {
        data_bytes = dlc;
    }
    else
    {
        switch (dlc)
        {
        case CAN_DLC12:
            data_bytes = 12U;
            break;
        case CAN_DLC16:
            data_bytes = 16U;
            break;
        case CAN_DLC20:
            data_bytes = 20U;
            break;
        case CAN_DLC24:
            data_bytes = 24U;
            break;
        case CAN_DLC32:
            data_bytes = 32U;
            break;
        case CAN_DLC48:
            data_bytes = 48U;
            break;
        case CAN_DLC64:
            data_bytes = 64U;
            break;
        default:
            /* Code should never touch here */
            break;
        }
    }

    return data_bytes;
}

#ifdef RT_CAN_USING_CANFD
static void _canfd_init(uint32_t idx)
{
    g_can_dev_array[idx].ll_init.pstcCanFd = (stc_canfd_config_t *)rt_malloc(sizeof(stc_canfd_config_t));
    RT_ASSERT((g_can_dev_array[idx].ll_init.pstcCanFd != RT_NULL));

    CAN_FD_StructInit(g_can_dev_array[idx].ll_init.pstcCanFd);
    switch ((rt_uint32_t)g_can_dev_array[idx].instance)
    {
#ifdef BSP_USING_CAN1
    case (rt_uint32_t)CM_CAN1:
        g_can_dev_array[idx].ll_init.pstcCanFd->u8Mode = CAN1_CANFD_MODE;
        break;
#endif
#ifdef BSP_USING_CAN2
    case (rt_uint32_t)CM_CAN2:
        g_can_dev_array[idx].ll_init.pstcCanFd->u8Mode = CAN2_CANFD_MODE;
        break;
#endif
    default:
        break;
    }
}

static rt_bool_t _can_is_valid_tdc(CM_CAN_TypeDef *CANx, struct rt_can_bit_timing_config *cfg)
{
    if (cfg->items[0].num_sspoff > 1)
    {
        return RT_FALSE;
    }
    if (cfg->count == 1)
    {
        return RT_TRUE;
    }

    uint32_t can_clk = _can_get_clk_src(CANx);
    uint32_t baud_rate = can_clk / (cfg->items[1].prescaler * \
                                    (cfg->items[1].num_seg1 + cfg->items[1].num_seg2));
    return (baud_rate >= CANFD_DATA_BAUD_1M);
}

static rt_err_t _can_config_timing(can_device *p_can_dev, void *arg)
{
    rt_err_t rt_ret = RT_EOK;

    struct rt_can_bit_timing_config *timing_configs = (struct rt_can_bit_timing_config *)arg;
    RT_ASSERT(_can_is_valid_tdc(p_can_dev->instance, timing_configs));
    if ((timing_configs == RT_NULL) || (timing_configs->count < 1) || (timing_configs->count > 2))
    {
        return -RT_ERROR;
    }

    p_can_dev->rt_can.config.can_timing = timing_configs->items[0];
    p_can_dev->ll_init.stcBitCfg.u32Prescaler = p_can_dev->rt_can.config.can_timing.prescaler;
    p_can_dev->ll_init.stcBitCfg.u32TimeSeg1 = p_can_dev->rt_can.config.can_timing.num_seg1;
    p_can_dev->ll_init.stcBitCfg.u32TimeSeg2 = p_can_dev->rt_can.config.can_timing.num_seg2;
    p_can_dev->ll_init.stcBitCfg.u32SJW = p_can_dev->rt_can.config.can_timing.num_sjw;
    if (timing_configs->count == 2)
    {
        p_can_dev->rt_can.config.canfd_timing = timing_configs->items[1];
        p_can_dev->ll_init.pstcCanFd->stcBitCfg.u32Prescaler = p_can_dev->rt_can.config.canfd_timing.prescaler;
        p_can_dev->ll_init.pstcCanFd->stcBitCfg.u32TimeSeg1 = p_can_dev->rt_can.config.canfd_timing.num_seg1;
        p_can_dev->ll_init.pstcCanFd->stcBitCfg.u32TimeSeg2 = p_can_dev->rt_can.config.canfd_timing.num_seg2;
        p_can_dev->ll_init.pstcCanFd->stcBitCfg.u32SJW = p_can_dev->rt_can.config.canfd_timing.num_sjw;
        p_can_dev->ll_init.pstcCanFd->u8SSPOffset = p_can_dev->rt_can.config.canfd_timing.num_sspoff;
        if (p_can_dev->rt_can.config.canfd_timing.num_sspoff)
        {
            p_can_dev->ll_init.pstcCanFd->u8TDC = CAN_FD_TDC_ENABLE;
        }
    }
    /* init can */
    int32_t ll_ret = CAN_Init(p_can_dev->instance, &p_can_dev->ll_init);
    if (ll_ret != LL_OK)
    {
        rt_ret = -RT_EINVAL;
    }

    return rt_ret;
}


static rt_err_t _canfd_control(can_device *p_can_dev, int cmd, void *arg)
{
    rt_uint32_t argval;
    rt_err_t timing_stat;
    switch (cmd)
    {
    case RT_CAN_CMD_SET_BAUD:
        argval = *(rt_uint32_t *) arg;
        if (p_can_dev->rt_can.config.baud_rate == argval)
        {
            break;
        }
        timing_stat = _can_get_bit_timing(p_can_dev->instance, \
                                          CAN_BIT_TIMING_CANFD_NORMINAL, \
                                          argval, \
                                          &p_can_dev->ll_init.stcBitCfg);
        if (timing_stat != RT_EOK)
        {
            return timing_stat;
        }
        CAN_Init(p_can_dev->instance, &p_can_dev->ll_init);
        p_can_dev->rt_can.config.baud_rate = argval;
        break;
    case RT_CAN_CMD_SET_CANFD:
        if (p_can_dev->rt_can.config.enable_canfd == argval)
        {
            break;
        }
        p_can_dev->rt_can.config.enable_canfd = (rt_uint32_t) argval;
        break;
    case RT_CAN_CMD_SET_BAUD_FD:
        argval = *(rt_uint32_t *) arg;
        if (p_can_dev->rt_can.config.baud_rate_fd == argval)
        {
            break;
        }
        timing_stat = _can_get_bit_timing(p_can_dev->instance, \
                                          CAN_BIT_TIMING_CANFD_DATA, \
                                          argval, \
                                          &p_can_dev->ll_init.pstcCanFd->stcBitCfg);
        if (timing_stat != RT_EOK)
        {
            return timing_stat;
        }
        p_can_dev->ll_init.pstcCanFd->u8SSPOffset = p_can_dev->ll_init.pstcCanFd->stcBitCfg.u32TimeSeg1;
        CAN_Init(p_can_dev->instance, &p_can_dev->ll_init);
        p_can_dev->rt_can.config.baud_rate_fd = argval;
        break;
    case RT_CAN_CMD_SET_BITTIMING:
        return _can_config_timing(p_can_dev, arg);
        break;
    default:
        break;
    }

    return RT_EOK;
}
#endif

static rt_err_t _can_control(struct rt_can_device *can, int cmd, void *arg)
{
    can_device *p_can_dev;
    rt_uint32_t argval;

    RT_ASSERT(can != RT_NULL);
    p_can_dev = (can_device *)rt_container_of(can, can_device, rt_can);
    RT_ASSERT(p_can_dev);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        argval = (rt_uint32_t) arg;
        switch (argval)
        {
        case RT_DEVICE_FLAG_INT_RX:
            CAN_IntCmd(p_can_dev->instance, CAN_INT_RX, DISABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_RX_BUF_WARN, DISABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_RX_BUF_FULL, DISABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_RX_OVERRUN, DISABLE);
            break;
        case RT_DEVICE_FLAG_INT_TX:
            CAN_IntCmd(p_can_dev->instance, CAN_INT_STB_TX, DISABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_PTB_TX, DISABLE);
            break;
        case RT_DEVICE_CAN_INT_ERR:
            CAN_IntCmd(p_can_dev->instance, CAN_INT_ERR_INT, DISABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_ARBITR_LOST, DISABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_ERR_PASSIVE, DISABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_BUS_ERR, DISABLE);
            break;
        default:
            break;
        }
        break;
    case RT_DEVICE_CTRL_SET_INT:
        argval = (rt_uint32_t) arg;
        switch (argval)
        {
        case RT_DEVICE_FLAG_INT_RX:
            CAN_IntCmd(p_can_dev->instance, CAN_INT_RX, ENABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_RX_BUF_WARN, ENABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_RX_BUF_FULL, ENABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_RX_OVERRUN, ENABLE);
            break;
        case RT_DEVICE_FLAG_INT_TX:
            CAN_IntCmd(p_can_dev->instance, CAN_INT_STB_TX, ENABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_PTB_TX, ENABLE);
            break;
        case RT_DEVICE_CAN_INT_ERR:
            CAN_IntCmd(p_can_dev->instance, CAN_INT_ERR_INT, ENABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_ARBITR_LOST, ENABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_ERR_PASSIVE, ENABLE);
            CAN_IntCmd(p_can_dev->instance, CAN_INT_BUS_ERR, ENABLE);
            break;
        default:
            break;
        }
        break;
    case RT_CAN_CMD_SET_FILTER:
        return _can_config_filter(p_can_dev, arg);
        break;
    case RT_CAN_CMD_SET_MODE:
        return _can_config_work_mode(p_can_dev, arg);
        break;
    case RT_CAN_CMD_SET_BAUD:
#ifdef RT_CAN_USING_CANFD
        return _canfd_control(p_can_dev, cmd, arg);
#else
        return _can20_config_baud(p_can_dev, arg);
#endif
        break;
    case RT_CAN_CMD_SET_PRIV:
        argval = (rt_uint32_t) arg;
        if (argval != RT_CAN_MODE_PRIV &&
                argval != RT_CAN_MODE_NOPRIV)
        {
            return -RT_ERROR;
        }
        if (argval != p_can_dev->rt_can.config.privmode)
        {
            p_can_dev->rt_can.config.privmode = argval;
            return RT_EOK;
        }
        break;
    case RT_CAN_CMD_GET_STATUS:
    {
        struct rt_can_status *rt_can_stat = (struct rt_can_status *)arg;
        stc_can_error_info_t stcErr = {0};
        CAN_GetErrorInfo(p_can_dev->instance, &stcErr);
        rt_can_stat->rcverrcnt = stcErr.u8RxErrorCount;
        rt_can_stat->snderrcnt = stcErr.u8TxErrorCount;
        rt_can_stat->lasterrtype = stcErr.u8ErrorType;
        rt_can_stat->errcode = CAN_GetStatusValue(p_can_dev->instance);
    }
    break;
#ifdef RT_CAN_USING_CANFD
    case RT_CAN_CMD_SET_CANFD:
    case RT_CAN_CMD_SET_BAUD_FD:
    case RT_CAN_CMD_SET_BITTIMING:
        return _canfd_control(p_can_dev, cmd, arg);
#endif
    default:
        break;

    }
    return RT_EOK;
}

static int _can_sendmsg(struct rt_can_device *can, const void *buf, rt_uint32_t box_num)
{
    struct rt_can_msg *pmsg = (struct rt_can_msg *) buf;
    stc_can_tx_frame_t stc_tx_frame = {0};
    int32_t ll_ret;

    RT_ASSERT(can != RT_NULL);
    can_device *p_can_dev = (can_device *)rt_container_of(can, can_device, rt_can);
    RT_ASSERT(p_can_dev);

    stc_tx_frame.u32ID = pmsg->id;
    if (RT_CAN_DTR == pmsg->rtr)
    {
        stc_tx_frame.RTR = 0;
    }
    else
    {
        stc_tx_frame.RTR = 1;
    }
#ifdef RT_CAN_USING_CANFD
    if (pmsg->fd_frame != 0)
    {
        RT_ASSERT(pmsg->len <= CAN_DLC64);
    }
    else
    {
        RT_ASSERT(pmsg->len <= CAN_DLC8);
    }
    stc_tx_frame.FDF = pmsg->fd_frame;
    stc_tx_frame.BRS = pmsg->brs;
#endif
    stc_tx_frame.DLC = pmsg->len & 0x0FU;
    /* Set up the IDE */
    stc_tx_frame.IDE = pmsg->ide;
    /* Set up the data field */
    uint32_t msg_len = _can_get_data_bytes_len(stc_tx_frame.DLC);
    rt_memcpy(&stc_tx_frame.au8Data, pmsg->data, msg_len);
    ll_ret = CAN_FillTxFrame(p_can_dev->instance, CAN_TX_BUF_PTB, &stc_tx_frame);
    if (ll_ret != LL_OK)
    {
        return -RT_ERROR;
    }
    /* Request transmission */
    CAN_StartTx(p_can_dev->instance, CAN_TX_REQ_PTB);

    return RT_EOK;
}

static int _can_recvmsg(struct rt_can_device *can, void *buf, rt_uint32_t fifo)
{
    int32_t ll_ret;
    struct rt_can_msg *pmsg;
    stc_can_rx_frame_t ll_rx_frame;

    RT_ASSERT(can != RT_NULL);
    can_device *p_can_dev = (can_device *)rt_container_of(can, can_device, rt_can);
    RT_ASSERT(p_can_dev);

    pmsg = (struct rt_can_msg *) buf;
    /* get data */
    ll_ret = CAN_GetRxFrame(p_can_dev->instance, &ll_rx_frame);
    if (ll_ret != LL_OK)
        return -RT_ERROR;

    /* get id */
    if (0 == ll_rx_frame.IDE)
    {
        pmsg->ide = RT_CAN_STDID;
    }
    else
    {
        pmsg->ide = RT_CAN_EXTID;
    }
    pmsg->id = ll_rx_frame.u32ID;
    /* get type */
    if (0 == ll_rx_frame.RTR)
    {
        pmsg->rtr = RT_CAN_DTR;
    }
    else
    {
        pmsg->rtr = RT_CAN_RTR;
    }
    /* get len */
    pmsg->len = ll_rx_frame.DLC;
    /* get hdr_index */
    pmsg->hdr = 0;
#ifdef RT_CAN_USING_CANFD
    pmsg->fd_frame = ll_rx_frame.FDF;
    pmsg->brs = ll_rx_frame.BRS;
#endif
    uint32_t msg_len = _can_get_data_bytes_len(ll_rx_frame.DLC);
    rt_memcpy(pmsg->data, &ll_rx_frame.au8Data, msg_len);

    return RT_EOK;
}

static const struct rt_can_ops _can_ops =
{
    _can_config,
    _can_control,
    _can_sendmsg,
    _can_recvmsg,
};

static void _can_isr(can_device *p_can_dev)
{
    stc_can_error_info_t stcErr;

    (void)CAN_GetErrorInfo(p_can_dev->instance, &stcErr);

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_BUS_OFF) == SET)
    {
        /* BUS OFF. */
    }
    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_ERR_INT) == SET)
    {
        /* ERROR. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_ERR_INT);
    }
    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_BUS_ERR) == SET)
    {
        /* BUS ERROR. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_BUS_ERR);
    }
    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_ERR_PASSIVE) == SET)
    {
        /* error-passive to error-active or error-active to error-passive. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_ERR_PASSIVE);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_RX_BUF_OVF) == SET)
    {
        /* RX overflow. */
        rt_hw_can_isr(&p_can_dev->rt_can, RT_CAN_EVENT_RXOF_IND);
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_RX_BUF_OVF);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_TX_BUF_FULL) == SET)
    {
        /* TX buffer full. */
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_TX_ABORTED) == SET)
    {
        /* TX aborted. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_TX_ABORTED);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_ARBITR_LOST) == SET)
    {
        rt_hw_can_isr(&p_can_dev->rt_can, RT_CAN_EVENT_TX_FAIL);
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_ARBITR_LOST);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_STB_TX) == SET)
    {
        /* STB transmitted. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_STB_TX);
        rt_hw_can_isr(&p_can_dev->rt_can, RT_CAN_EVENT_TX_DONE);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_PTB_TX) == SET)
    {
        /* PTB transmitted. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_PTB_TX);
        rt_hw_can_isr(&p_can_dev->rt_can, RT_CAN_EVENT_TX_DONE);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_RX) == SET)
    {
        /* Received a frame. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_RX);
        rt_hw_can_isr(&p_can_dev->rt_can, RT_CAN_EVENT_RX_IND);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_RX_BUF_WARN) == SET)
    {
        /* RX buffer warning. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_RX_BUF_WARN);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_RX_BUF_FULL) == SET)
    {
        /* RX buffer full. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_RX_BUF_FULL);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_RX_OVERRUN) == SET)
    {
        /* RX buffer overrun. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_RX_OVERRUN);
    }

    if (CAN_GetStatus(p_can_dev->instance, CAN_FLAG_TEC_REC_WARN) == SET)
    {
        /* TEC or REC reached warning limit. */
        CAN_ClearStatus(p_can_dev->instance, CAN_FLAG_TEC_REC_WARN);
    }

    if (CAN_TTC_GetStatus(p_can_dev->instance, CAN_TTC_FLAG_TIME_TRIG) == SET)
    {
        /* Time trigger interrupt. */
        CAN_TTC_ClearStatus(p_can_dev->instance, CAN_TTC_FLAG_TIME_TRIG);
    }

    if (CAN_TTC_GetStatus(p_can_dev->instance, CAN_TTC_FLAG_TRIG_ERR) == SET)
    {
        /* Trigger error interrupt. */
    }

    if (CAN_TTC_GetStatus(p_can_dev->instance, CAN_TTC_FLAG_WATCH_TRIG) == SET)
    {
        /* Watch trigger interrupt. */
        CAN_TTC_ClearStatus(p_can_dev->instance, CAN_TTC_FLAG_WATCH_TRIG);
    }
}
#if defined(BSP_USING_CAN1)
static void _can1_irq_handler(void)
{
    rt_interrupt_enter();
    _can_isr(&g_can_dev_array[CAN1_INDEX]);
    rt_interrupt_leave();
}
#endif

#if defined(BSP_USING_CAN2)
static void _can2_irq_handler(void)
{
    rt_interrupt_enter();
    _can_isr(&g_can_dev_array[CAN2_INDEX]);
    rt_interrupt_leave();
}
#endif

static void _can_clock_enable(void)
{
#if defined(HC32F4A0) || defined(HC32F4A2)
#if defined(BSP_USING_CAN1)
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN1, ENABLE);
#endif
#if   defined(BSP_USING_CAN2)
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN2, ENABLE);
#endif
#endif

#if defined(HC32F460)
#if defined(BSP_USING_CAN1)
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN, ENABLE);
#endif
#endif
}

static void _can_irq_config(void)
{
    struct hc32_irq_config irq_config;
#if defined(BSP_USING_CAN1)
    irq_config.irq_num = BSP_CAN1_IRQ_NUM;
    irq_config.int_src = CAN1_INT_SRC;
    irq_config.irq_prio = BSP_CAN1_IRQ_PRIO;
    /* register interrupt */
    hc32_install_irq_handler(&irq_config,
                             _can1_irq_handler,
                             RT_TRUE);
#endif
#if defined(BSP_USING_CAN2)
    irq_config.irq_num = BSP_CAN2_IRQ_NUM;
    irq_config.int_src = CAN2_INT_SRC;
    irq_config.irq_prio = BSP_CAN2_IRQ_PRIO;
    /* register interrupt */
    hc32_install_irq_handler(&irq_config,
                             _can2_irq_handler,
                             RT_TRUE);
#endif
}

extern rt_err_t rt_hw_board_can_init(CM_CAN_TypeDef *CANx);
extern void CanPhyEnable(void);
int rt_hw_can_init(void)
{
    struct can_configure rt_can_config  = CANDEFAULTCONFIG;
    rt_can_config.privmode = RT_CAN_MODE_NOPRIV;
    rt_can_config.ticks = 50;
#ifdef RT_CAN_USING_HDR
    rt_can_config.maxhdr = FILTER_COUNT;
#endif
#ifdef RT_CAN_USING_CANFD
    rt_can_config.baud_rate_fd = CANFD_DATA_BAUD_1M;
#endif

    _can_irq_config();
    _can_clock_enable();
    CanPhyEnable();
    int result = RT_EOK;
    uint32_t i = 0;
    for (; i < CAN_INDEX_MAX; i++)
    {
        CAN_StructInit(&g_can_dev_array[i].ll_init);
        if (g_can_dev_array[i].ll_init.pstcFilter == RT_NULL)
        {
            g_can_dev_array[i].ll_init.pstcFilter = (stc_can_filter_config_t *)rt_malloc(sizeof(stc_can_filter_config_t) * FILTER_COUNT);
        }
        RT_ASSERT((g_can_dev_array[i].ll_init.pstcFilter != RT_NULL));

        rt_memset(g_can_dev_array[i].ll_init.pstcFilter, 0, sizeof(stc_can_filter_config_t) * FILTER_COUNT);
        g_can_dev_array[i].ll_init.pstcFilter[0].u32ID = 0U;
        g_can_dev_array[i].ll_init.pstcFilter[0].u32IDMask = 0x1FFFFFFF;
        g_can_dev_array[i].ll_init.pstcFilter[0].u32IDType = CAN_ID_STD_EXT;
        g_can_dev_array[i].ll_init.u16FilterSelect = CAN_FILTER1;
        if(g_can_dev_array[i].init.single_trans_mode)
        {
            g_can_dev_array[i].ll_init.u8PTBSingleShotTx = CAN_PTB_SINGLESHOT_TX_ENABLE;
        }
        g_can_dev_array[i].rt_can.config = rt_can_config;
#ifdef RT_CAN_USING_CANFD
        _canfd_init(i);
#endif
        /* register CAN device */
        rt_hw_board_can_init(g_can_dev_array[i].instance);
        rt_hw_can_register(&g_can_dev_array[i].rt_can,
                           g_can_dev_array[i].init.name,
                           &_can_ops,
                           &g_can_dev_array[i]);
    }

    return result;
}

INIT_DEVICE_EXPORT(rt_hw_can_init);
#endif

#endif /* BSP_USING_CAN */

/************************** end of file ******************/
