#include <stdio.h>
#include "driver/gpio.h"

#include "gpio.h"

int relay_state = 0;

void gpio_init()
{
    gpio_set_direction(GPIO_NUM_26 , GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_26 , 1);
    relay_state = 0;
}

void relay_on()
{
    //gpio_set_level(GPIO_NUM_26 , 0); //低电平开继电器
    gpio_set_level(GPIO_NUM_26 , 0);    //高电平开继电器
    relay_state = 1;
}

void relay_off()
{
    //gpio_set_level(GPIO_NUM_26 , 1);
    gpio_set_level(GPIO_NUM_26 , 1);
    relay_state = 0;
}