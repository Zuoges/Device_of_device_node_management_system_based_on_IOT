#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "lvgl.h"
#include "wifi.h"
#include "uart.h"
#include "mqtt.h"
#include "gpio.h"
#include "server_receive_transmit.h"
#include "Euclidean.h"
#include "im1281b.h"
#include "lvgl_gui.h"
#include "Device_Info.h"
#include "event.h"

TaskHandle_t Device_Event_Handle;

char label_volt_str[10];
char label_curr_str[10];
char label_power_str[10];

void Device_MQTT_send()
{
    char data[1024];
    server_data_build(data);
    printf("MQTT send : %s\r\n" , data);
    mqtt_send_message(MQTT_PUBLISH_ADDRESS , data , 0);
}

void Device_main_task()
{
    static int mqtt_count = 0;
    while(1)
    {
        //lv_label_set_text(lvgl_gui->all_label_notice , http_rec.control);
        
        sprintf(label_volt_str , "%.2fV" , im1281b_data->voltage);
        sprintf(label_curr_str , "%.2fA" , im1281b_data->current);
        sprintf(label_power_str , "%.1fW" , im1281b_data->power);
        lv_label_set_text(lvgl_gui->user_tabview_tab2_label_volt_num , label_volt_str);
        lv_label_set_text(lvgl_gui->user_tabview_tab2_label_curr_num , label_curr_str);
        lv_label_set_text(lvgl_gui->user_tabview_tab2_label_power_num , label_power_str);
        lv_chart_set_next(lvgl_gui->user_tabview_tab2_chart_volt , lvgl_gui->user_tabview_tab2_chart_series_volt , im1281b_data->voltage);
        lv_chart_set_next(lvgl_gui->user_tabview_tab2_chart_curr , lvgl_gui->user_tabview_tab2_chart_series_curr , im1281b_data->current);
        lv_chart_set_next(lvgl_gui->user_tabview_tab2_chart_power , lvgl_gui->user_tabview_tab2_chart_series_power , im1281b_data->power);
        
        // printf("U: %.4f\nI: %.4f\nP: %.4f\nPF: %.4f\nE: %.4f\n\n" , im1281b_data->voltage , im1281b_data->current , im1281b_data->power , im1281b_data->power_factor , im1281b_data->electricity);
        //printf("%.4f, %.4f, %.4f, %.4f, %.4f, \n" , im1281b_data->voltage , im1281b_data->current , im1281b_data->power , im1281b_data->power_factor , im1281b_data->electricity);

        status_parse(im1281b_data);
        //打印顺序分别为 电压,电流,功率,功率因数,电量,(可忽略),(可忽略),电烙铁状态编号(可忽略)
        printf("%.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %d, %d, \n" , im1281b_data->voltage , im1281b_data->current , im1281b_data->power , im1281b_data->power_factor , im1281b_data->electricity , min_d , min_i , soldering_state_bit);

        lv_label_set_text(lvgl_gui->user_tabview_tab3_label_username_value , soldering_state);
        lv_label_set_text(lvgl_gui->user_tabview_tab3_label_ID_value , elecapp_state);
#ifdef USE_NETWORK
        mqtt_count++;
        if(mqtt_count >= 5)
        {
            Device_MQTT_send();
            //printf("     esp_get_free_heap_size : %d  \n", esp_get_free_heap_size());
            mqtt_count = 0;
        }
#endif

        //lv_label_set_text(lvgl_gui->user_tabview_tab3_label_username_value , http_rec.state_user);
        vTaskDelay(200 / portTICK_PERIOD_MS);

    }
}

void Device_Event_init()
{
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    xTaskCreate(Device_main_task, 
                "Device_main", 
                4096 * 6, 
                NULL, 
                7, 
                &Device_Event_Handle);
}

void Device_Event_uninit()
{
    vTaskDelete(Device_Event_Handle);
}