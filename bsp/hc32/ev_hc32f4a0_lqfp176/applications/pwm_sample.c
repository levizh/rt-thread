/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 * Copyright (c) 2022, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#ifdef BSP_USING_PWM
/* defined the LED_GREEN pin: PC9 */
#define LED_GREEN_PIN GET_PIN(C, 9)

#define PWM_DEV_CHANNEL     1

struct rt_device_pwm *pwm_dev;


static rt_int32_t pwm_sample(int argc, char *argv[])
{
    rt_uint32_t period = 50000;
    rt_uint32_t pulse = 45000;

    /* set LED_GREEN_PIN pin mode to output */
    rt_pin_mode(LED_GREEN_PIN, PIN_MODE_OUTPUT);

    if (argc != 2)
    {
        return RT_ERROR;
    }

    pwm_dev = (struct rt_device_pwm *)rt_device_find(argv[1]);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", argv[1]);
        return RT_ERROR;
    }

    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL + 1, period, pulse);
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL + 1);
//    rt_pwm_set_period(pwm_dev,PWM_DEV_CHANNEL,100000);

    while (1)
    {
        rt_thread_mdelay(50);
        pulse += 5000;
        rt_pwm_set_pulse(pwm_dev, PWM_DEV_CHANNEL, pulse);
        rt_pwm_set_pulse(pwm_dev, PWM_DEV_CHANNEL + 1, pulse);
        if (pulse >= period)
        {
            pulse = 0;
        }
    }
}
MSH_CMD_EXPORT(pwm_sample,pwm_sample [opt])
#endif
/*
 EOF
*/
