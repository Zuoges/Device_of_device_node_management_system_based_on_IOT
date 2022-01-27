/*****************************************************************************
    作者：Zuoge
    时间：2021年10月2日
    说明：按键扫描，目前仅debug使用
    已知问题：
    备注：
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "server_receive_transmit.h"
#include "Euclidean.h"

#include "key.h"

void Key34_Onclick()
{
    // printf("Key34 Press\r\n");
    // strcpy(http_rec.state_state ,"1");
    static int num = 0;
    num = num + 1;
    sprintf(elecapp_state , "%d" , num);
}

void Key35_Onclick()
{
    // printf("Key35 Press\r\n");
    // strcpy(http_rec.state_state ,"2");
}

void Key_Scan_Task()
{
    static unsigned char K34_Now = 0 , K35_Now = 0;
    unsigned char K34_Past = 0 , K35_Past = 0;
    while(1)
    {
        K34_Past = 0;
        K34_Past = 0;

        K34_Past = K34_Now;
        K35_Past = K35_Now;

        if(gpio_get_level(GPIO_NUM_34) != 0)
            K34_Now = 1;
        else
            K34_Now = 0;

        if(gpio_get_level(GPIO_NUM_35) != 0)
            K35_Now = 1;
        else
            K35_Now = 0;

        if((K34_Past == 1) && (K34_Now == 0))   Key34_Onclick();
        if((K35_Past == 1) && (K35_Now == 0))   Key35_Onclick();

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void key_init()
{
    gpio_set_direction(GPIO_NUM_34 , GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_35 , GPIO_MODE_INPUT);

    xTaskCreate(Key_Scan_Task,
                "Key_Scan",
                2048,
                NULL,
                20,
                NULL);
}