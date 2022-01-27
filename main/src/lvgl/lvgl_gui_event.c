#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "lvgl.h"

#include "server_receive_transmit.h"
#include "Device_Info.h"
#include "gpio.h"
#include "lvgl_gui.h"

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

void login_srceen_events_init()
{
    // lv_obj_set_event_cb(lvgl_gui->login_btn , login_btn_event_handler);
    // lv_obj_set_event_cb(lvgl_gui->login_btn2 , login_btn2_event_handler);
}

void user_screen_events_init()
{
    lv_obj_set_event_cb(lvgl_gui->user_tabview_tab1_imgbtn , user_tabview_tab1_imgbtn_event_handler);
}