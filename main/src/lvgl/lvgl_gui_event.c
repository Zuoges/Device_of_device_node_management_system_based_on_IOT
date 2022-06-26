#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "lvgl.h"

#include "wifi.h"
#include "mqtt.h"
#include "server_receive_transmit.h"
#include "Device_Info.h"
#include "gpio.h"
#include "lvgl_gui.h"

uint8_t kb_state = 0;
char mqttserver[32];
// static void login_btn_event_handler(lv_obj_t* obj, lv_event_t event)
// {
//     switch(event)
//     {
//         case LV_EVENT_CLICKED:
//         {
//             printf("login_btn_clicked\n");
//             // lv_obj_clean(lv_scr_act());
//             // lvgl_gui_user_screen();
//             // lv_scr_load(lvgl_gui->user_screen);

//             strcpy(http_rec.state_state ,"2");

//             break;
//         }
//         default:break;
//     }
// }

// static void login_btn2_event_handler(lv_obj_t* obj, lv_event_t event)
// {
//     switch(event)
//     {
//         case LV_EVENT_CLICKED:
//         {
//             printf("login_btn_clicked\n");
//             // lv_obj_clean(lv_scr_act());
//             // lvgl_gui_user_screen();
//             // lv_scr_load(lvgl_gui->user_screen);

//             strcpy(http_rec.state_state ,"1");

//             break;
//         }
//         default:break;
//     }
// }

static void user_tabview_tab1_imgbtn_event_handler(lv_obj_t* obj, lv_event_t event)
{
    switch (event)
    {
        case LV_EVENT_VALUE_CHANGED:
        {
            if(lv_btn_get_state(obj) == LV_BTN_STATE_CHECKED_RELEASED)
            {
                relay_on();
            }
            else if(lv_btn_get_state(obj) == LV_BTN_STATE_RELEASED)
            {
                relay_off();
            }
            break;
        }
        default:break;
    }
}

static void user_imgbtn_notice_event_handler(lv_obj_t* obj, lv_event_t event)
{
    switch (event)
    {
        case LV_EVENT_SHORT_CLICKED:
        {
            lvgl_gui_user_msgbox_notice();
            break;
        }
        default:break;
    }
}

static void user_imgbtn_wifi_event_handler(lv_obj_t* obj, lv_event_t event)
{
    switch (event)
    {
        case LV_EVENT_SHORT_CLICKED:
        {
            while(screen_flush == 1);
            lv_obj_clean(lv_scr_act());
            lvgl_gui_setup_screen();
            lv_scr_load(lvgl_gui->setup_screen);
            break;
        }
        default:break;
    }
}

static void setup_btn_exit_event_handler(lv_obj_t* obj, lv_event_t event)
{
    switch (event)
    {
        case LV_EVENT_SHORT_CLICKED:
        {
            while(screen_flush == 1);
            lv_obj_clean(lv_scr_act());
            lvgl_gui_user_screen();
            lv_scr_load(lvgl_gui->user_screen);
            break;
        }
        default:break;
    }
}

static void setup_kb_event_handler(lv_obj_t* obj, lv_event_t event)
{
    lv_keyboard_def_event_cb(obj, event);
    switch (event)
    {
        case LV_EVENT_CANCEL:
        {
            if(obj != NULL)
            {
                lv_obj_del(obj);
                obj = NULL; 
                kb_state = 0;
            }
        }
    }
}

static void setup_page_event_handler(lv_obj_t* obj, lv_event_t event)
{
    switch (event)
    {
        case LV_EVENT_CLICKED:
        {
            if(lvgl_gui->setup_kb != NULL && kb_state == 1)
            {
                lv_event_send(lvgl_gui->setup_kb, LV_EVENT_CANCEL, NULL);
            }
            break;
        }
        default:break;
    }
}

static void setup_text_event_handler(lv_obj_t* obj, lv_event_t event)
{
    switch (event)
    {
        case LV_EVENT_RELEASED:
        {
            if(lvgl_gui->setup_kb == NULL || kb_state == 0)
            {
                lvgl_gui->setup_kb = lv_keyboard_create(lv_scr_act(), NULL);
                lv_obj_set_event_cb(lvgl_gui->setup_kb, setup_kb_event_handler);
                lv_indev_wait_release(lv_indev_get_act());
                lv_textarea_set_cursor_hidden(obj, false);
                lv_keyboard_set_textarea(lvgl_gui->setup_kb, obj);
                kb_state = 1;
            }
            break;
        }
        case LV_EVENT_DEFOCUSED:
        {
            lv_textarea_set_cursor_hidden(obj, true);
            break;
        }
        default:break;
    }
}

static void setup_btn_wifi_event_handler(lv_obj_t* obj, lv_event_t event)
{
    char ssid[32];
    char psw[64];
    switch (event)
    {
        case LV_EVENT_CLICKED:
        {
            strcpy(ssid, lv_textarea_get_text(lvgl_gui->setup_text_wifissid));
            strcpy(psw, lv_textarea_get_text(lvgl_gui->setup_text_wifipsw));
            strcpy((char*)wifi_config.sta.ssid, ssid);
            strcpy((char*)wifi_config.sta.password, psw);
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        }
        default:break;
    }
}

static void setup_btn_unwifi_event_handler(lv_obj_t* obj, lv_event_t event)
{
    switch (event)
    {
        case LV_EVENT_CLICKED:
        {
            ESP_ERROR_CHECK(esp_wifi_disconnect());
            break;
        }
        default:break;
    }
}

static void setup_btn_mqtt_event_handler(lv_obj_t* obj, lv_event_t event)
{
    char port[10];
    switch (event)
    {
        case LV_EVENT_CLICKED:
        {
            strcpy(mqttserver, lv_textarea_get_text(lvgl_gui->setup_text_mqttserver));
            strcpy(port, lv_textarea_get_text(lvgl_gui->setup_text_mqttport));
            mqtt_init(mqttserver, atoi(port));
            break;
        }
        default:break;
    }
}

static void setup_btn_unmqtt_event_handler(lv_obj_t* obj, lv_event_t event)
{
    switch (event)
    {
        case LV_EVENT_CLICKED:
        {
            mqtt_uninit();
            break;
        }
        default:break;
    }
}

void login_srceen_events_init()
{
    // lv_obj_set_event_cb(lvgl_gui->login_btn , login_btn_event_handler);
    // lv_obj_set_event_cb(lvgl_gui->login_btn2 , login_btn2_event_handler);
}

void user_screen_events_init()
{
    lv_obj_set_event_cb(lvgl_gui->user_tabview_tab1_imgbtn , user_tabview_tab1_imgbtn_event_handler);
    lv_obj_set_event_cb(lvgl_gui->user_imgbtn_notice , user_imgbtn_notice_event_handler);
    lv_obj_set_event_cb(lvgl_gui->user_imgbtn_wifi , user_imgbtn_wifi_event_handler);
}

void setup_screen_events_init()
{
    lv_obj_set_event_cb(lvgl_gui->setup_btn_exit , setup_btn_exit_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_page , setup_page_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_text_wifissid , setup_text_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_text_wifipsw , setup_text_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_text_mqttserver , setup_text_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_text_mqttport , setup_text_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_btn_wifi , setup_btn_wifi_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_btn_unwifi , setup_btn_unwifi_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_btn_mqtt , setup_btn_mqtt_event_handler);
    lv_obj_set_event_cb(lvgl_gui->setup_btn_unmqtt , setup_btn_unmqtt_event_handler);
}