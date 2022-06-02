#ifndef _LVGL_GUI_H_
#define _LVGL_GUI_H_

#include "lvgl.h"

#define LOGIN_SCREEN        0
#define USER_SCREEN         1
#define RESERVATION_SCREEN  2
#define SETUP_SCREEN        3

extern unsigned char screen;
extern unsigned char screen_flush;
extern char mqttserver[32];

LV_FONT_DECLARE(ziyuanyuanti_16);
LV_FONT_DECLARE(ziyuanyuanti_24);
LV_FONT_DECLARE(yumo_16);
LV_IMG_DECLARE(logo_80);
LV_IMG_DECLARE(logo_name_30);
LV_IMG_DECLARE(login_phone_150);
LV_IMG_DECLARE(sw_off);
LV_IMG_DECLARE(sw_on);
LV_IMG_DECLARE(V_30);
LV_IMG_DECLARE(A_30);
LV_IMG_DECLARE(W_30);
LV_IMG_DECLARE(headimg_46);
LV_IMG_DECLARE(charger_on);
LV_IMG_DECLARE(charger_off);
LV_IMG_DECLARE(lamp_on);
LV_IMG_DECLARE(lamp_off);
LV_IMG_DECLARE(solder_off);
LV_IMG_DECLARE(solder_up);
LV_IMG_DECLARE(solder_down);
LV_IMG_DECLARE(solder_steady);
LV_IMG_DECLARE(unknown);
LV_IMG_DECLARE(icon_wifi_32);
LV_IMG_DECLARE(icon_nowifi_32);
LV_IMG_DECLARE(icon_mqtt_32);
LV_IMG_DECLARE(icon_nomqtt_32);
LV_IMG_DECLARE(icon_notice_32);

extern uint8_t headimg_46_map[4232];

typedef struct
{
    lv_obj_t *all_label_notice;   
    lv_obj_t *login_screen;
    lv_obj_t *login_qr;
    lv_obj_t *login_btn;
    lv_obj_t *login_btn_label;
    lv_obj_t *login_btn2;
    lv_obj_t *login_img1;
    lv_obj_t *login_img2;
    lv_obj_t *login_img3;
    lv_obj_t *reservation_screen;
    lv_obj_t *reservation_img1;
    lv_obj_t *reservation_img2;
    lv_obj_t *reservation_label1;
    lv_obj_t *reservation_label2;
    lv_obj_t *reservation_img_head;
    lv_obj_t *reservation_label_name;
    lv_obj_t *reservation_label_ID;
    lv_obj_t *user_screen;
    lv_obj_t *user_imgbtn_wifi;
    lv_obj_t *user_img_mqtt;
    lv_obj_t *user_imgbtn_notice;
    lv_obj_t *user_msgbox_notice;
    lv_obj_t *user_tabview;
    lv_obj_t *user_tabview_tab1;
    lv_obj_t *user_tabview_tab1_imgbtn;
    lv_obj_t *user_tabview_tab2;
    lv_obj_t *user_tabview_tab2_chart_volt;
    lv_chart_series_t *user_tabview_tab2_chart_series_volt;
    lv_obj_t *user_tabview_tab2_img_volt;
    lv_obj_t *user_tabview_tab2_label_volt;
    lv_obj_t *user_tabview_tab2_label_volt_num;
    lv_obj_t *user_tabview_tab2_chart_curr;
    lv_chart_series_t *user_tabview_tab2_chart_series_curr;
    lv_obj_t *user_tabview_tab2_img_curr;
    lv_obj_t *user_tabview_tab2_label_curr;
    lv_obj_t *user_tabview_tab2_label_curr_num;
    lv_obj_t *user_tabview_tab2_chart_power;
    lv_chart_series_t *user_tabview_tab2_chart_series_power;
    lv_obj_t *user_tabview_tab2_img_power;
    lv_obj_t *user_tabview_tab2_label_power;
    lv_obj_t *user_tabview_tab2_label_power_num;
    lv_obj_t *user_tabview_tab3;
    lv_obj_t *user_tabview_tab3_img_head;
    lv_obj_t *user_tabview_tab3_label_username;
    lv_obj_t *user_tabview_tab3_label_username_value;
    lv_obj_t *user_tabview_tab3_label_ID;
    lv_obj_t *user_tabview_tab3_label_ID_value;
    lv_obj_t *user_tabview_tab3_img_charger;
    lv_obj_t *user_tabview_tab3_img_lamp;
    lv_obj_t *user_tabview_tab3_img_solder;
    lv_obj_t *user_tabview_tab3_img_unknown;
    lv_obj_t *setup_screen;
    lv_obj_t *setup_page;
    lv_obj_t *setup_kb;
    lv_obj_t *setup_text_wifissid;
    lv_obj_t *setup_label_wifissid;
    lv_obj_t *setup_text_wifipsw;
    lv_obj_t *setup_label_wifipsw;
    lv_obj_t *setup_text_mqttserver;
    lv_obj_t *setup_label_mqttserver;
    lv_obj_t *setup_text_mqttport;
    lv_obj_t *setup_label_mqttport;
    lv_obj_t *setup_text_mqttid;
    lv_obj_t *setup_label_mqttid;
    lv_obj_t *setup_text_mqttpsw;
    lv_obj_t *setup_label_mqttpsw;
    lv_obj_t *setup_btn_wifi;
    lv_obj_t *setup_btn_label_wifi;
    lv_obj_t *setup_btn_unwifi;
    lv_obj_t *setup_btn_label_unwifi;
    lv_obj_t *setup_btn_mqtt;
    lv_obj_t *setup_btn_label_mqtt;
    lv_obj_t *setup_btn_unmqtt;
    lv_obj_t *setup_btn_label_unmqtt;
    lv_obj_t *setup_btn_exit;
    lv_obj_t *setup_btn_label_exit;
    lv_obj_t *setup_img_wifi;
    lv_obj_t *setup_img_mqtt;
}lvgl_gui_t;

lvgl_gui_t *lvgl_gui;


void login_srceen_events_init();
void user_screen_events_init();
void setup_screen_events_init();

void lvgl_init();
void lvgl_gui_login_screen();
void lvgl_gui_reservation_screen();
void lvgl_gui_user_screen();
void lvgl_gui_user_msgbox_notice();
void lvgl_gui_setup_screen();
void lvgl_gui_start();

void init_img46();

#endif // _LVGL_GUI_H_
