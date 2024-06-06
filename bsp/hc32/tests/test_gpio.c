/*
 * 程序清单：这是一个 PIN 设备使用例程
 * 例程导出了 pin_beep_sample 命令到控制终端
 * 命令调用格式：pin_beep_sample
 * 程序功能：通过按键控制蜂鸣器对应引脚的电平状态控制蜂鸣器
*/

#include <rtthread.h>
#include <rtdevice.h>
#include "board_config.h"

#if defined(BSP_USING_GPIO)
#include "drv_gpio.h"


#define LED1_PIN_NUM            GET_PIN(C, 9)
#define KEY1_PIN_NUM            GET_PIN(A, 0)


static uint8_t u8LedState = 1;


void led_control(void *args)
{
    u8LedState = !u8LedState;
    if (0 == u8LedState)
    {
        rt_pin_write(LED1_PIN_NUM, PIN_LOW);
    }
    else
    {
        rt_pin_write(LED1_PIN_NUM, PIN_HIGH);
    }
}

static void pin_beep_sample(void)
{
    /* LED引脚为输出模式 */
    rt_pin_mode(LED1_PIN_NUM, PIN_MODE_OUTPUT);
    /* 默认高电平 */
    rt_pin_write(LED1_PIN_NUM, PIN_HIGH);

    /* 按键1引脚为输入模式 */
    rt_pin_mode(KEY1_PIN_NUM, PIN_MODE_INPUT_PULLUP);
    /* 绑定中断，下降沿模式，回调函数名为led_control */
    // rt_pin_attach_irq(KEY1_PIN_NUM, PIN_IRQ_MODE_RISING, led_control, RT_NULL);
    // rt_pin_attach_irq(KEY1_PIN_NUM, PIN_IRQ_MODE_FALLING, led_control, RT_NULL);
    rt_pin_attach_irq(KEY1_PIN_NUM, PIN_IRQ_MODE_RISING_FALLING, led_control, RT_NULL);
    /* 使能中断 */
    rt_pin_irq_enable(KEY1_PIN_NUM, PIN_IRQ_ENABLE);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(pin_beep_sample, pin beep sample);

#endif
