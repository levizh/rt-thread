#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_WDT

#define WDT_DEVICE_NAME    "wdt"

static rt_device_t wdg_dev;

static void idle_hook(void)
{
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
}

static void _wdt_cmd_print_usage(void)
{
    rt_kprintf("wdt [option]\n");
    rt_kprintf("  set_timeout: set wdt timeout, rang(0-3).\n");
    rt_kprintf("    e.g. MSH >wdt set_timeout 2\n");
}

static int wdt_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t timeout = atoi(argv[2]);

    if (argc != 3)
    {
        _wdt_cmd_print_usage();
        return -RT_ERROR;
    }

    if (!rt_strcmp("set_timeout", argv[1]) && timeout <= 3)
    {
        wdg_dev = rt_device_find(WDT_DEVICE_NAME);
        if (!wdg_dev)
        {
            rt_kprintf("find %s failed!\n", WDT_DEVICE_NAME);
            return RT_ERROR;
        }

        ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
        if (ret != RT_EOK)
        {
            rt_kprintf("set %s timeout failed!\n", WDT_DEVICE_NAME);
            return RT_ERROR;
        }

        ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
        if (ret != RT_EOK)
        {
            rt_kprintf("start %s failed!\n", WDT_DEVICE_NAME);
            return -RT_ERROR;
        }

        rt_thread_idle_sethook(idle_hook);
    }
    else 
    {
        _wdt_cmd_print_usage();
        return -RT_ERROR;
    }

    return ret;
}

MSH_CMD_EXPORT_ALIAS(wdt_sample, wdt, wdt [option]);

#endif
