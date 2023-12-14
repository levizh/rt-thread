/*
* 功能
*   展示 CAN1、CAN2 接收消息和回发消息。
* 代码使用方法
*   在 main 函数 while 循环之前调用can_sample_init()
*
* 默认波特率
*   仲裁段:波特率500K,采样率80%,
*   数据段:波特率为4M,采样率80%
*
* 接收和发送消息
*  CAN1:
*  仅接收满足以下过滤条件的消息，并发送接收到的消息
*   1）标准帧：match ID:0x100~0x1ff
*   2）扩展帧：match ID:0x12345100~0x123451ff
*   3）固定ID帧: match ID: 0x555
*  测试设备发送满足以上过滤条件的消息后，需确认可接收到相同的消息，否则测试失败。
*
*  CAN2:
*  仅接收满足以下过滤条件的消息，并发送接收到的消息
*   1）标准帧：match ID:0x200~0x2ff
*   2）扩展帧：match ID:0x1abcd100~0x1abcd1ff
*   3）固定ID帧: match ID: 0x666
*  测试设备发送满足以上过滤条件的消息后，需确认可接收到相同的消息，否则测试失败。
*
* 命令行命令
*   1）设置时序：
*       注意：使用此项设置前，需修改 MSH 最大参数格式为 20
*           （menuconfig-->RT-Thread Compone-->MSH: command shell-->The number of arguments for a shell command）
*       格式：
*           can set_bittiming <channel> <count> <rt_can_bit_timing_norminal> <rt_can_bit_timing_data>
*       示例:
*           MSH >can set_bittiming 2 1 1 64 16 16 0 (设置can2 仲裁段波特率500K)
*           MSH >can set_bittiming 2 2 1 64 16 16 0 1 16 4 4 16 (设置can2 仲裁段波特率500K,数据段波特率2M)
*   2）设置仲裁段波特率：
*       格式：
*           can set_baud <channel> <baud>
*       示例：
*           MSH >can set_baud 1 1000000 （设置can1仲裁段波特率1M）
*   3）设置数据段波特率：
*       格式：
*           can set_baudfd <channel> <baudfd>
*       示例：
*           MSH >can set_baudfd 1 2000000 （设置can1数据段波特率2M）
*/

#include <rtthread.h>
#include "rtdevice.h"
#include "drv_can.h"

#if defined(BSP_USING_CAN)

#define CAN_DEV_CH_1                    (0)
#define CAN_DEV_CH_2                    (1)
#define CAN_DEV_COUNT                   (2)

#define MSH_USAGE_CAN_SET_BAUD          "can set_baud <channel> <baud>                - set can channel baud\n"
#define MSH_USAGE_CAN_SET_BAUDFD        "can set_baudfd <channel> <baudfd>            - set can channel baudfd\n"
#define MSH_USAGE_CAN_SET_BITTIMING     "can set_bittiming <channel> <count> <rt_can_bit_timing_norminal> <rt_can_bit_timing_data>  - set can bit timing,\n"

#define MSH_RESULT_STR(result)          ((result == RT_EOK) ? "success" : "failure")

static struct rt_semaphore rx_sem[CAN_DEV_COUNT] = {RT_NULL, RT_NULL};
static rt_device_t can_dev[CAN_DEV_COUNT] = {RT_NULL, RT_NULL};
static rt_mutex_t can_mutex[CAN_DEV_COUNT] = {RT_NULL, RT_NULL};
static uint32_t can_dev_ch[CAN_DEV_COUNT] = {CAN_DEV_CH_1, CAN_DEV_CH_2};

static rt_err_t can1_rx_call(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem[0]);

    return RT_EOK;
}

static rt_err_t can2_rx_call(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem[1]);

    return RT_EOK;
}

static void _set_tx_indicate(uint32_t ch)
{
    if (ch == CAN_DEV_CH_1)
    {
        rt_device_set_rx_indicate(can_dev[ch], can1_rx_call);
    }
    else
    {
        rt_device_set_rx_indicate(can_dev[ch], can2_rx_call);
    }
}

static void _set_default_filter(uint32_t ch)
{
    struct rt_can_filter_item can1_items[5] =
    {
        RT_CAN_FILTER_ITEM_INIT(0x100, 0, 0, 0, 0x700, RT_NULL, RT_NULL), /* std,match ID:0x100~0x1ff，hdr= - 1， */
        RT_CAN_FILTER_ITEM_INIT(0x12345100, 1, 0, 0, 0xFFFFFF00, RT_NULL, RT_NULL), /* ext,match ID:0x12345100~0x123451ff，hdr= - 1， */
        {0x555, 0, 0, 0, 0x7ff, 7,}                                       /* std,match ID:0x555，hdr= 7 */
    };
    struct rt_can_filter_item can2_items[5] =
    {
        RT_CAN_FILTER_ITEM_INIT(0x200, 0, 0, 0, 0x700, RT_NULL, RT_NULL), /* std,match ID:0x100~0x1ff，hdr= - 1， */
        RT_CAN_FILTER_ITEM_INIT(0x1abcd100, 1, 0, 0, 0xFFFFFF00, RT_NULL, RT_NULL), /* ext,match ID:0x1abcd100~0x1abcd1ff，hdr= - 1， */
        {0x666, 0, 0, 0, 0x7ff, 7,}                                       /* std,match ID:0x666，hdr= 7 */
    };
    struct rt_can_filter_config cfg = {3, 1, can1_items};
    rt_err_t res;

    if (ch == CAN_DEV_CH_2)
    {
        cfg.items = can2_items;
    }
    res = rt_device_control(can_dev[ch], RT_CAN_CMD_SET_FILTER, &cfg);
    RT_ASSERT(res == RT_EOK);
}

static void can_rx_thread(void *parameter)
{
    int i;
    struct rt_can_msg rxmsg = {0};

    uint32_t ch = *(uint32_t *)parameter;

    rt_size_t  size;
    while (1)
    {
        rt_memset(&rxmsg, 0, sizeof(struct rt_can_msg));
        rt_sem_take(&rx_sem[ch], RT_WAITING_FOREVER);
        rt_mutex_take(can_mutex[ch], RT_WAITING_FOREVER);
        rt_device_read(can_dev[ch], 0, &rxmsg, sizeof(rxmsg));
        size = rt_device_write(can_dev[ch], 0, &rxmsg, sizeof(rxmsg));
        rt_mutex_release(can_mutex[ch]);
        if (size == 0)
        {
            rt_kprintf("can dev write data failed!\n");
        }
        rt_kprintf("ID:%x", rxmsg.id);
        for (i = 0; i < 8; i++)
        {
            rt_kprintf("%2x", rxmsg.data[i]);
        }

        rt_kprintf("\n");
    }
}

static rt_err_t _msh_cmd_parse_ch(char *ch, uint32_t *ch_out)
{
    rt_err_t result = -RT_ERROR;
    uint32_t ch_temp = atoi(ch) - 1;
    if (ch_temp > CAN_DEV_COUNT)
    {
        rt_kprintf("param error: channel exceed max value %d \n", CAN_DEV_CH_2);
        return result;
    }
    *ch_out = ch_temp;
    return RT_EOK;
}

static void _msh_cmd_set_baud(int argc, char **argv)
{
    rt_err_t result;
    if (argc == 4)
    {
        uint32_t ch;
        if (_msh_cmd_parse_ch(argv[2], &ch) != RT_EOK)
        {
            return;
        }
        uint32_t baud = atoi(argv[3]);
        rt_mutex_take(can_mutex[ch], RT_WAITING_FOREVER);
        result = rt_device_control(can_dev[ch], RT_CAN_CMD_SET_BAUD, &baud);
        rt_mutex_release(can_mutex[ch]);
        rt_kprintf("set %s \n", MSH_RESULT_STR(result));
    }
    else
    {
        rt_kprintf(MSH_USAGE_CAN_SET_BAUD);
        rt_kprintf("    e.g. MSH >can set_baud 1 500000\n");
    }
}

void _msh_cmd_set_timing(int argc, char **argv)
{
    rt_err_t result;
    if (argc == 9 || argc == 14)
    {
        uint32_t ch;
        if (_msh_cmd_parse_ch(argv[2], &ch) != RT_EOK)
        {
            return;
        }
        uint32_t count = atoi(argv[3]);
        if (count > 2)
        {
            rt_kprintf("param error: count exceed max value 2 \n");
            return;
        }

        struct rt_can_bit_timing items[2];
        struct rt_can_bit_timing_config cfg;
        uint32_t pos = 4;
        items[0].prescaler = atoi(argv[pos++]);
        items[0].num_seg1 =  atoi(argv[pos++]);
        items[0].num_seg2 =  atoi(argv[pos++]);
        items[0].num_sjw =  atoi(argv[pos++]);
        items[0].num_sspoff =  atoi(argv[pos++]);
        if (count > 1)
        {
            items[1].prescaler =  atoi(argv[pos++]);
            items[1].num_seg1 =  atoi(argv[pos++]);
            items[1].num_seg2 =  atoi(argv[pos++]);
            items[1].num_sjw =  atoi(argv[pos++]);
            items[1].num_sspoff =  atoi(argv[pos]);
        }
        cfg.count = count;
        cfg.items = items;
        rt_mutex_take(can_mutex[ch], RT_WAITING_FOREVER);
        result = rt_device_control(can_dev[ch], RT_CAN_CMD_SET_BITTIMING, &cfg);
        rt_mutex_release(can_mutex[ch]);
        rt_kprintf("set %s \n", MSH_RESULT_STR(result));
    }
    else
    {
        rt_kprintf(MSH_USAGE_CAN_SET_BITTIMING);
        rt_kprintf("    e.g. MSH >can set_bittiming 2 1 1 64 16 16 0\n");
        rt_kprintf("    e.g. MSH >can set_bittiming 2 2 1 64 16 16 0 1 16 4 4 16\n");
    }
}

void _msh_cmd_set_baudfd(int argc, char **argv)
{
    rt_err_t result;


    if (argc == 4)
    {
        uint32_t ch;
        if (_msh_cmd_parse_ch(argv[2], &ch) != RT_EOK)
        {
            return;
        }
        uint32_t baudfd = atoi(argv[3]);
        rt_mutex_take(can_mutex[ch], RT_WAITING_FOREVER);
        result = rt_device_control(can_dev[ch], RT_CAN_CMD_SET_BAUD_FD, &baudfd);
        rt_mutex_release(can_mutex[ch]);
        rt_kprintf("set %s \n", MSH_RESULT_STR(result));
    }
    else
    {
        rt_kprintf(MSH_USAGE_CAN_SET_BAUDFD);
        rt_kprintf("    e.g. MSH >can set_baudfd 1 4000000\n");
    }
}

void _show_usage(void)
{
    rt_kprintf("Usage: \n");
    rt_kprintf(MSH_USAGE_CAN_SET_BAUD);
    rt_kprintf(MSH_USAGE_CAN_SET_BAUDFD);
    rt_kprintf(MSH_USAGE_CAN_SET_BITTIMING);
}

int can(int argc, char **argv)
{
    if (argc == 1)
    {
        _show_usage();
        return 0;
    }

    else if (!strcmp(argv[1], "set_baud"))
    {
        _msh_cmd_set_baud(argc, argv);
    }
    else if (!strcmp(argv[1], "set_baudfd"))
    {
        _msh_cmd_set_baudfd(argc, argv);
    }
    else if (!strcmp(argv[1], "set_bittiming"))
    {
        _msh_cmd_set_timing(argc, argv);
    }

    return 0;
}

int can_sample_init(void)
{
    char can_name[RT_NAME_MAX] = "can";
    char sem_name[RT_NAME_MAX] = "c_sem";
    char mutex_name[RT_NAME_MAX] = "c_mtx";
    rt_thread_t thread;
    rt_err_t res;

    for (int ch = 0; ch < CAN_DEV_COUNT; ch++)
    {
        if (ch == 0)
        {
            can_name[3] = '1';
            sem_name[6] = '0';
            mutex_name[6] = '0';
        }
        else if (ch == 1)
        {
            can_name[3] = '2';
            sem_name[6] = '1';
            mutex_name[6] = '1';
        }
        can_dev[ch] = rt_device_find(can_name);
        if (!can_dev[ch])
        {
            rt_kprintf("find %s failed!\n", can_name);
            return RT_ERROR;
        }
        rt_sem_init(&rx_sem[ch], sem_name, 0, RT_IPC_FLAG_FIFO);
        can_mutex[ch] = rt_mutex_create(mutex_name, RT_IPC_FLAG_FIFO);

        res = rt_device_open(can_dev[ch], RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
        RT_ASSERT(res == RT_EOK);
        res = rt_device_control(can_dev[ch], RT_CAN_CMD_SET_BAUD, (void *)CANFD_ARBITRATION_BAUD_500K);
        RT_ASSERT(res == RT_EOK);
        res = rt_device_control(can_dev[ch], RT_CAN_CMD_SET_BAUD_FD, (void *)CANFD_DATA_BAUD_4M);
        RT_ASSERT(res == RT_EOK);

        _set_tx_indicate(ch);
        _set_default_filter(ch);

        thread = rt_thread_create("can_rx", can_rx_thread, &can_dev_ch[ch], 1024, 25, 10);
        if (thread != RT_NULL)
        {
            rt_thread_startup(thread);
        }
        else
        {
            rt_kprintf("create can_rx thread failed!\n");
        }
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(can, can [option]);

#endif 
