#include <rtthread.h>
#include <rtdevice.h>
#include <drv_wktm.h>

#define WDT_DEVICE_NAME    "wktm"

static void _wktm_cmd_print_usage(void)
{
    rt_kprintf("wktm [option]\n");
    rt_kprintf("  start: start wktm.\n");
    rt_kprintf("  stop : stop wktm.\n");
    rt_kprintf("  get_f: get wktm frequency in Hz.\n");
}

static int wktm_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;

    if (argc != 2)
    {
        _wktm_cmd_print_usage();
        return -RT_ERROR;
    }

    if (!rt_strcmp("start", argv[1]))
    {
        hc32_wktm_start(0xFFFU);
    } else if (!rt_strcmp("stop", argv[1]))
    {
        hc32_wktm_stop();
    } else if (!rt_strcmp("get_f", argv[1]))
    {
        rt_kprintf("wktm frequency is %d Hz.\n", hc32_wktm_get_countfreq());
    }
    else
    {
        _wktm_cmd_print_usage();
        return -RT_ERROR;
    }

    return ret;
}

MSH_CMD_EXPORT_ALIAS(wktm_sample, wktm, wktm [option]);
