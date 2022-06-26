#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lvgl.h"

#include "wifi.h"
#include "mqtt.h"
#include "Euclidean.h"
#include "server_receive_transmit.h"
#include "Device_Info.h"
#include "lvgl_gui.h"

unsigned char screen = LOGIN_SCREEN;
unsigned char screen_flush = 0;
char msgbox_str[300];

void lvgl_gui_login_screen()
{
    screen = LOGIN_SCREEN;
    //字体风格
    static lv_style_t font_16_style;
    lv_style_set_text_font(&font_16_style, LV_STATE_DEFAULT, &yumo_16);

    //创建登录屏幕
    lvgl_gui->login_screen = lv_obj_create(NULL, NULL);

    //二维码
    const char * data = "https://mp.weixin.qq.com/a/~md70breAAFuM8Br_a7D8zA~~";
    lvgl_gui->login_qr = lv_qrcode_create(lvgl_gui->login_screen , 100 , lv_color_hex3(0x33f) , lv_color_hex3(0xeef));
    lv_qrcode_update(lvgl_gui->login_qr , data , strlen(data));
    lv_obj_align(lvgl_gui->login_qr , NULL , LV_ALIGN_CENTER , -55 , 25);

    //图标
    lvgl_gui->login_img1 = lv_img_create(lvgl_gui->login_screen , NULL);
    lv_img_set_src(lvgl_gui->login_img1 , &logo_80);
    lv_obj_set_pos(lvgl_gui->login_img1 , 0 , 0);

    //学校名字
    lvgl_gui->login_img2 = lv_img_create(lvgl_gui->login_screen , NULL);
    lv_img_set_src(lvgl_gui->login_img2 , &logo_name_30);
    lv_obj_set_pos(lvgl_gui->login_img2 , 80 , 10);

    //扫一扫图片
    lvgl_gui->login_img3 = lv_img_create(lvgl_gui->login_screen , NULL);
    lv_img_set_src(lvgl_gui->login_img3 , &login_phone_150);
    lv_obj_align(lvgl_gui->login_img3 , NULL , LV_ALIGN_CENTER , 75 , 20);

    // //按键(调试用)
    // lvgl_gui->login_btn = lv_btn_create(lvgl_gui->login_screen , NULL);
    // lv_obj_align(lvgl_gui->login_btn , NULL , LV_ALIGN_CENTER , -55 , 100);
    // lv_obj_set_size(lvgl_gui->login_btn , 100 , 30);
    // lvgl_gui->login_btn_label = lv_label_create(lvgl_gui->login_btn , NULL);
    // lv_obj_add_style(lvgl_gui->login_btn_label , LV_LABEL_PART_MAIN , &font_16_style);
    // lv_obj_align(lvgl_gui->login_btn_label , NULL , LV_ALIGN_CENTER , 0 , 0);
    // lv_label_set_text(lvgl_gui->login_btn_label , "进入");

    // //按键2(调试用)
    // lvgl_gui->login_btn2 = lv_btn_create(lvgl_gui->login_screen , NULL);
    // lv_obj_align(lvgl_gui->login_btn2 , NULL , LV_ALIGN_CENTER , 30 , 100);
    // lv_obj_set_size(lvgl_gui->login_btn2 , 100 , 30);

    //全局通知
    lvgl_gui->all_label_notice = lv_label_create(lvgl_gui->login_screen , NULL);
    lv_obj_add_style(lvgl_gui->all_label_notice , LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_long_mode(lvgl_gui->all_label_notice , LV_LABEL_LONG_SROLL_CIRC); 
    lv_obj_set_width(lvgl_gui->all_label_notice , 320);
    lv_label_set_text(lvgl_gui->all_label_notice , "食堂三楼营养猪肚鸡超级好吃                              ");
    lv_obj_align(lvgl_gui->all_label_notice , NULL , LV_ALIGN_IN_BOTTOM_MID , 0 , 0);
    

    login_srceen_events_init();

}

void lvgl_gui_reservation_screen()
{
    //字体风格
    static lv_style_t font_16_style;
    lv_style_set_text_font(&font_16_style, LV_STATE_DEFAULT, &yumo_16);

    static lv_style_t font_24_style;
    lv_style_set_text_font(&font_24_style, LV_STATE_DEFAULT, &ziyuanyuanti_24);
    
    //创建预约屏幕
    lvgl_gui->reservation_screen = lv_obj_create(NULL, NULL);
    
    //图标
    lvgl_gui->reservation_img1 = lv_img_create(lvgl_gui->reservation_screen , NULL);
    lv_img_set_src(lvgl_gui->reservation_img1 , &logo_80);
    lv_obj_set_pos(lvgl_gui->reservation_img1 , 0 , 0);

    //学校名字
    lvgl_gui->reservation_img2 = lv_img_create(lvgl_gui->reservation_screen , NULL);
    lv_img_set_src(lvgl_gui->reservation_img2 , &logo_name_30);
    lv_obj_set_pos(lvgl_gui->reservation_img2 , 80 , 10);

    //被预约文字    "该设备已被预约"
    lvgl_gui->reservation_label1 = lv_label_create(lvgl_gui->reservation_screen , NULL);
    lv_obj_add_style(lvgl_gui->reservation_label1 , LV_LABEL_PART_MAIN , &font_24_style);
    lv_obj_align(lvgl_gui->reservation_label1 , NULL , LV_ALIGN_CENTER , -57 , -30);
    lv_label_set_text(lvgl_gui->reservation_label1 , "该设备已被预约");

    //提醒使用文字  "请在微信小程序上开启使用"
    lvgl_gui->reservation_label2 = lv_label_create(lvgl_gui->reservation_screen , NULL);
    lv_obj_add_style(lvgl_gui->reservation_label2 , LV_LABEL_PART_MAIN , &font_16_style);
    lv_obj_align(lvgl_gui->reservation_label2 , NULL , LV_ALIGN_CENTER , -76 , -8);
    lv_label_set_text(lvgl_gui->reservation_label2 , "请在微信小程序上开启使用");

    //预约人头像
    // lvgl_gui->reservation_img_head = lv_img_create(lvgl_gui->reservation_screen , NULL);
    // lv_img_set_src(lvgl_gui->reservation_img_head , &headimg_46);
    // lv_obj_align(lvgl_gui->reservation_img_head , NULL , LV_ALIGN_CENTER , 0 , 30);

    //预约人名字
    lvgl_gui->reservation_label_name = lv_label_create(lvgl_gui->reservation_screen , NULL);
    lv_obj_add_style(lvgl_gui->reservation_label_name , LV_LABEL_PART_MAIN , &font_16_style);
    lv_obj_align(lvgl_gui->reservation_label_name , NULL , LV_ALIGN_CENTER , 0 , 10);
    //lv_label_set_align(lvgl_gui->reservation_label_name , LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(lvgl_gui->reservation_label_name , http_rec.state_user);

    //预约人学号
    lvgl_gui->reservation_label_ID = lv_label_create(lvgl_gui->reservation_screen , NULL);
    lv_obj_add_style(lvgl_gui->reservation_label_ID , LV_LABEL_PART_MAIN , &font_16_style);
    lv_obj_align(lvgl_gui->reservation_label_ID , NULL , LV_ALIGN_CENTER , -17 , 30);
    //lv_label_set_align(lvgl_gui->reservation_label_ID , LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(lvgl_gui->reservation_label_ID , http_rec.state_uuid);

    //全局通知
    lvgl_gui->all_label_notice = lv_label_create(lvgl_gui->reservation_screen , NULL);
    lv_obj_add_style(lvgl_gui->all_label_notice , LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_long_mode(lvgl_gui->all_label_notice , LV_LABEL_LONG_SROLL_CIRC); 
    lv_obj_set_width(lvgl_gui->all_label_notice , 320);
    lv_label_set_text(lvgl_gui->all_label_notice , "食堂三楼营养猪肚鸡超级好吃                              ");
    lv_obj_align(lvgl_gui->all_label_notice , NULL , LV_ALIGN_IN_BOTTOM_MID , 0 , 0);

}

void lvgl_gui_user_screen()
{

    //字体风格
    static lv_style_t font_16_style;
    lv_style_set_text_font(&font_16_style, LV_STATE_DEFAULT, &yumo_16);

    static lv_style_t lv_font_20_style;
    lv_style_set_text_font(&lv_font_20_style, LV_STATE_DEFAULT, &lv_font_montserrat_20);

    static lv_style_t font_24_style;
    lv_style_set_text_font(&font_24_style, LV_STATE_DEFAULT, &ziyuanyuanti_24);

    static lv_style_t lv_font_24_style;
    lv_style_set_text_font(&lv_font_24_style, LV_STATE_DEFAULT, &lv_font_montserrat_24);

    static lv_style_t chart_font_style;
    lv_style_set_text_font(&chart_font_style, LV_STATE_DEFAULT, &lv_font_montserrat_10);

    static lv_style_t img_style;
    lv_style_init(&img_style);

    //创建用户屏幕
    lvgl_gui->user_screen = lv_obj_create(NULL, NULL);

    //标签栏
    lvgl_gui->user_tabview = lv_tabview_create(lvgl_gui->user_screen , NULL);
    lv_obj_add_style(lvgl_gui->user_tabview , LV_LABEL_PART_MAIN , &font_16_style);
    lvgl_gui->user_tabview_tab1 = lv_tabview_add_tab(lvgl_gui->user_tabview , "设备状态");
    lvgl_gui->user_tabview_tab2 = lv_tabview_add_tab(lvgl_gui->user_tabview , "数据统计");
    lvgl_gui->user_tabview_tab3 = lv_tabview_add_tab(lvgl_gui->user_tabview , "状态分析");

    //设备状态 标签1
        //图片按键
    lvgl_gui->user_tabview_tab1_imgbtn = lv_imgbtn_create(lvgl_gui->user_tabview_tab1 , NULL);
    lv_obj_set_size(lvgl_gui->user_tabview_tab1_imgbtn , 100 , 100);
    lv_obj_align(lvgl_gui->user_tabview_tab1_imgbtn , NULL , LV_ALIGN_CENTER , 0 , 0);
    lv_imgbtn_set_src(lvgl_gui->user_tabview_tab1_imgbtn, LV_BTN_STATE_RELEASED, &sw_off);
    lv_imgbtn_set_src(lvgl_gui->user_tabview_tab1_imgbtn, LV_BTN_STATE_PRESSED, &sw_off);
    lv_imgbtn_set_src(lvgl_gui->user_tabview_tab1_imgbtn, LV_BTN_STATE_CHECKED_RELEASED, &sw_on);
    lv_imgbtn_set_src(lvgl_gui->user_tabview_tab1_imgbtn, LV_BTN_STATE_CHECKED_PRESSED, &sw_on);
    lv_imgbtn_set_checkable(lvgl_gui->user_tabview_tab1_imgbtn , true);
        
    //数据统计 标签2
        //电压折线图
    lvgl_gui->user_tabview_tab2_chart_volt = lv_chart_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_obj_set_size(lvgl_gui->user_tabview_tab2_chart_volt, 185, 45);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_chart_volt, 120, 10);
    lv_chart_set_update_mode(lvgl_gui->user_tabview_tab2_chart_volt, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(lvgl_gui->user_tabview_tab2_chart_volt, 8);
    lv_chart_set_range(lvgl_gui->user_tabview_tab2_chart_volt, 210, 230);
    lvgl_gui->user_tabview_tab2_chart_series_volt = lv_chart_add_series(lvgl_gui->user_tabview_tab2_chart_volt, LV_COLOR_YELLOW);
    lv_obj_add_style(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_AXIS_PRIMARY_Y , &chart_font_style);
    lv_obj_set_style_local_pad_left(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_BG , LV_STATE_DEFAULT , 50);
    lv_chart_set_y_tick_texts(lvgl_gui->user_tabview_tab2_chart_volt , "230\n220\n210" , 0 , LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_obj_set_style_local_line_width(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , 1);
    lv_obj_set_style_local_size(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , 0);
    lv_obj_set_style_local_bg_opa(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_40);
    lv_obj_set_style_local_bg_main_stop(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_100);
    lv_obj_set_style_local_bg_grad_stop(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_bg_grad_dir(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_GRAD_DIR_VER);
    lv_obj_set_style_local_line_opa(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_SERIES_BG , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_bg_opa(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_BG , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_border_opa(lvgl_gui->user_tabview_tab2_chart_volt , LV_CHART_PART_BG , LV_STATE_DEFAULT , LV_OPA_0);
        //电压数值
    lvgl_gui->user_tabview_tab2_img_volt = lv_img_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_img_set_src(lvgl_gui->user_tabview_tab2_img_volt , &V_30);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_img_volt , 10 , 18);
    // lvgl_gui->user_tabview_tab2_label_volt = lv_label_create(lvgl_gui->user_tabview_tab2 , NULL);
    // lv_obj_add_style(lvgl_gui->user_tabview_tab2_label_volt , LV_LABEL_PART_MAIN , &font_24_style);
    // lv_label_set_text(lvgl_gui->user_tabview_tab2_label_volt , "电压：");
    // lv_obj_set_pos(lvgl_gui->user_tabview_tab2_label_volt , 15 , 10);
    lvgl_gui->user_tabview_tab2_label_volt_num = lv_label_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_obj_add_style(lvgl_gui->user_tabview_tab2_label_volt_num , LV_LABEL_PART_MAIN , &lv_font_20_style);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_label_volt_num , 45 , 22);

        //电流折线图
    lvgl_gui->user_tabview_tab2_chart_curr = lv_chart_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_obj_set_size(lvgl_gui->user_tabview_tab2_chart_curr, 185, 45);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_chart_curr, 120, 65);
    lv_chart_set_update_mode(lvgl_gui->user_tabview_tab2_chart_curr, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(lvgl_gui->user_tabview_tab2_chart_curr, 8);
    lv_chart_set_range(lvgl_gui->user_tabview_tab2_chart_curr, 0, 2);
    lvgl_gui->user_tabview_tab2_chart_series_curr = lv_chart_add_series(lvgl_gui->user_tabview_tab2_chart_curr, LV_COLOR_BLUE);
    lv_obj_add_style(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_AXIS_PRIMARY_Y , &chart_font_style);
    lv_obj_set_style_local_pad_left(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_BG , LV_STATE_DEFAULT , 50);
    lv_chart_set_y_tick_texts(lvgl_gui->user_tabview_tab2_chart_curr , "2\n1\n0" , 0 , LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_obj_set_style_local_line_width(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , 1);
    lv_obj_set_style_local_size(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , 0);
    lv_obj_set_style_local_bg_opa(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_40);
    lv_obj_set_style_local_bg_main_stop(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_100);
    lv_obj_set_style_local_bg_grad_stop(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_bg_grad_dir(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_GRAD_DIR_VER);
    lv_obj_set_style_local_line_opa(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_SERIES_BG , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_bg_opa(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_BG , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_border_opa(lvgl_gui->user_tabview_tab2_chart_curr , LV_CHART_PART_BG , LV_STATE_DEFAULT , LV_OPA_0);
        //电流数值
    lvgl_gui->user_tabview_tab2_img_curr = lv_img_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_img_set_src(lvgl_gui->user_tabview_tab2_img_curr , &A_30);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_img_curr , 10 , 73);
    // lvgl_gui->user_tabview_tab2_label_curr = lv_label_create(lvgl_gui->user_tabview_tab2 , NULL);
    // lv_obj_add_style(lvgl_gui->user_tabview_tab2_label_curr , LV_LABEL_PART_MAIN , &font_24_style);
    // lv_label_set_text(lvgl_gui->user_tabview_tab2_label_curr , "电压：");
    // lv_obj_set_pos(lvgl_gui->user_tabview_tab2_label_curr , 15 , 65);
    lvgl_gui->user_tabview_tab2_label_curr_num = lv_label_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_obj_add_style(lvgl_gui->user_tabview_tab2_label_curr_num , LV_LABEL_PART_MAIN , &lv_font_20_style);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_label_curr_num , 45 , 77);

        //功率折线图
    lvgl_gui->user_tabview_tab2_chart_power = lv_chart_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_obj_set_size(lvgl_gui->user_tabview_tab2_chart_power, 185, 45);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_chart_power, 120, 120);
    lv_chart_set_update_mode(lvgl_gui->user_tabview_tab2_chart_power, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(lvgl_gui->user_tabview_tab2_chart_power, 8);
    lv_chart_set_range(lvgl_gui->user_tabview_tab2_chart_power, 0, 100);
    lvgl_gui->user_tabview_tab2_chart_series_power = lv_chart_add_series(lvgl_gui->user_tabview_tab2_chart_power, LV_COLOR_ORANGE);
    lv_obj_add_style(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_AXIS_PRIMARY_Y , &chart_font_style);
    lv_obj_set_style_local_pad_left(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_BG , LV_STATE_DEFAULT , 50);
    lv_chart_set_y_tick_texts(lvgl_gui->user_tabview_tab2_chart_power , "100\n50\n0" , 0 , LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_obj_set_style_local_line_width(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , 1);
    lv_obj_set_style_local_size(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , 0);
    lv_obj_set_style_local_bg_opa(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_40);
    lv_obj_set_style_local_bg_main_stop(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_100);
    lv_obj_set_style_local_bg_grad_stop(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_bg_grad_dir(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_SERIES , LV_STATE_DEFAULT , LV_GRAD_DIR_VER);
    lv_obj_set_style_local_line_opa(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_SERIES_BG , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_bg_opa(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_BG , LV_STATE_DEFAULT , LV_OPA_0);
    lv_obj_set_style_local_border_opa(lvgl_gui->user_tabview_tab2_chart_power , LV_CHART_PART_BG , LV_STATE_DEFAULT , LV_OPA_0);
        //功率数值
    lvgl_gui->user_tabview_tab2_img_power = lv_img_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_img_set_src(lvgl_gui->user_tabview_tab2_img_power , &W_30);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_img_power , 10 , 128);
    // lvgl_gui->user_tabview_tab2_label_power = lv_label_create(lvgl_gui->user_tabview_tab2 , NULL);
    // lv_obj_add_style(lvgl_gui->user_tabview_tab2_label_power , LV_LABEL_PART_MAIN , &font_24_style);
    // lv_label_set_text(lvgl_gui->user_tabview_tab2_label_power , "电压：");
    // lv_obj_set_pos(lvgl_gui->user_tabview_tab2_label_power , 15 , 120);
    lvgl_gui->user_tabview_tab2_label_power_num = lv_label_create(lvgl_gui->user_tabview_tab2 , NULL);
    lv_obj_add_style(lvgl_gui->user_tabview_tab2_label_power_num , LV_LABEL_PART_MAIN , &lv_font_20_style);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab2_label_power_num , 45 , 132);


    //用户信息 标签3
        //用户姓名
    // lvgl_gui->user_tabview_tab3_label_username = lv_label_create(lvgl_gui->user_tabview_tab3 , NULL);
    // lv_obj_add_style(lvgl_gui->user_tabview_tab3_label_username , LV_LABEL_PART_MAIN , &font_16_style);
    // lv_label_set_text(lvgl_gui->user_tabview_tab3_label_username , "姓名：");
    // lv_obj_set_pos(lvgl_gui->user_tabview_tab3_label_username , 10 , 15);
    lvgl_gui->user_tabview_tab3_label_username_value = lv_label_create(lvgl_gui->user_tabview_tab3 , NULL);
    lv_obj_add_style(lvgl_gui->user_tabview_tab3_label_username_value , LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->user_tabview_tab3_label_username_value , soldering_state);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab3_label_username_value , 10 , 70);
    //     //用户学号
    // lvgl_gui->user_tabview_tab3_label_ID = lv_label_create(lvgl_gui->user_tabview_tab3 , NULL);
    // lv_obj_add_style(lvgl_gui->user_tabview_tab3_label_ID , LV_LABEL_PART_MAIN , &font_16_style);
    // lv_label_set_text(lvgl_gui->user_tabview_tab3_label_ID , "学号：");
    // lv_obj_set_pos(lvgl_gui->user_tabview_tab3_label_ID , 10 , 40);
    lvgl_gui->user_tabview_tab3_label_ID_value = lv_label_create(lvgl_gui->user_tabview_tab3 , NULL);
    lv_obj_add_style(lvgl_gui->user_tabview_tab3_label_ID_value , LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->user_tabview_tab3_label_ID_value , elecapp_state);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab3_label_ID_value , 10 , 90);
        //设备状态分析图片表示
    lvgl_gui->user_tabview_tab3_img_solder = lv_img_create(lvgl_gui->user_tabview_tab3 , NULL);
    lv_img_set_src(lvgl_gui->user_tabview_tab3_img_solder , &solder_off);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab3_img_solder , 10 , 10);

    lvgl_gui->user_tabview_tab3_img_charger = lv_img_create(lvgl_gui->user_tabview_tab3 , NULL);
    lv_img_set_src(lvgl_gui->user_tabview_tab3_img_charger , &charger_off);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab3_img_charger , 70 , 10);

    lvgl_gui->user_tabview_tab3_img_lamp = lv_img_create(lvgl_gui->user_tabview_tab3 , NULL);
    lv_img_set_src(lvgl_gui->user_tabview_tab3_img_lamp , &lamp_off);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab3_img_lamp , 130 , 10);

    lvgl_gui->user_tabview_tab3_img_unknown = lv_img_create(lvgl_gui->user_tabview_tab3 , NULL);
    lv_img_set_src(lvgl_gui->user_tabview_tab3_img_unknown , &unknown);
    lv_obj_set_pos(lvgl_gui->user_tabview_tab3_img_unknown , 100 , 10); 
    lv_obj_set_size(lvgl_gui->user_tabview_tab3_img_unknown , 0 , 0);

    // //全局通知
    // lvgl_gui->all_label_notice = lv_label_create(lvgl_gui->user_screen , NULL);
    // lv_obj_add_style(lvgl_gui->all_label_notice , LV_LABEL_PART_MAIN , &font_16_style);
    // lv_label_set_long_mode(lvgl_gui->all_label_notice , LV_LABEL_LONG_SROLL_CIRC); 
    // lv_obj_set_width(lvgl_gui->all_label_notice , 320);
    // lv_label_set_text(lvgl_gui->all_label_notice , "食堂三楼营养猪肚鸡超级好吃                              ");
    // lv_obj_align(lvgl_gui->all_label_notice , NULL , LV_ALIGN_IN_BOTTOM_MID , 0 , 0);

    //全局状态图标
    lvgl_gui->user_imgbtn_wifi = lv_imgbtn_create(lvgl_gui->user_screen , NULL);
    if(wifi_state == WIFI_EVENT_STA_CONNECTED)
    {
        lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_RELEASED, &icon_wifi_32);
        lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_PRESSED, &icon_wifi_32);
        lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_CHECKED_RELEASED, &icon_wifi_32);
        lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_CHECKED_PRESSED, &icon_wifi_32);
    }
    else
    {
        lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_RELEASED, &icon_nowifi_32);
        lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_PRESSED, &icon_nowifi_32);
        lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_CHECKED_RELEASED, &icon_nowifi_32);
        lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_CHECKED_PRESSED, &icon_nowifi_32);
    }
    lv_obj_align(lvgl_gui->user_imgbtn_wifi , NULL , LV_ALIGN_IN_BOTTOM_RIGHT , -10 , 0);

    lvgl_gui->user_img_mqtt = lv_img_create(lvgl_gui->user_screen , NULL);
    if(mqtt_state == MQTT_EVENT_CONNECTED)
    {
        lv_img_set_src(lvgl_gui->user_img_mqtt, &icon_mqtt_32);
    }
    else if(mqtt_state == MQTT_EVENT_DISCONNECTED || mqtt_state == MQTT_EVENT_ERROR)
    {
        lv_img_set_src(lvgl_gui->user_img_mqtt, &icon_nomqtt_32);
    }
    lv_obj_align(lvgl_gui->user_img_mqtt , NULL , LV_ALIGN_IN_BOTTOM_RIGHT , -52 , 0);

    lvgl_gui->user_imgbtn_notice = lv_imgbtn_create(lvgl_gui->user_screen , NULL);
    lv_imgbtn_set_src(lvgl_gui->user_imgbtn_notice, LV_BTN_STATE_RELEASED, &icon_notice_32);
    lv_imgbtn_set_src(lvgl_gui->user_imgbtn_notice, LV_BTN_STATE_PRESSED, &icon_notice_32);
    lv_imgbtn_set_src(lvgl_gui->user_imgbtn_notice, LV_BTN_STATE_CHECKED_RELEASED, &icon_notice_32);
    lv_imgbtn_set_src(lvgl_gui->user_imgbtn_notice, LV_BTN_STATE_CHECKED_PRESSED, &icon_notice_32);
    lv_obj_align(lvgl_gui->user_imgbtn_notice , NULL , LV_ALIGN_IN_BOTTOM_RIGHT , -94 , 0);

    user_screen_events_init();

    screen = USER_SCREEN;
}

void lvgl_gui_user_msgbox_notice()
{
    //字体风格
    static lv_style_t font_16_style;
    lv_style_set_text_font(&font_16_style, LV_STATE_DEFAULT, &yumo_16);
    
    static const char* btn_str[] = { "关闭", "" };
    lvgl_gui->user_msgbox_notice = lv_msgbox_create(lv_scr_act(), NULL);
    lv_obj_add_style(lvgl_gui->user_msgbox_notice, LV_LABEL_PART_MAIN , &font_16_style);
    sprintf(msgbox_str, "这里是浙理工科艺学院\n2018级电子信息工程1班\n陈豪佐 的本科毕设作品\n基于物联网的设备节点管理系统设备端\n点击WIFI图标可以配置网络\nWIFI : %s\nMQTT服务器 : %s", (char*)wifi_config.sta.ssid, mqttserver);
    lv_msgbox_set_text(lvgl_gui->user_msgbox_notice, msgbox_str);
    lv_msgbox_add_btns(lvgl_gui->user_msgbox_notice, btn_str);
    lv_obj_align(lvgl_gui->user_msgbox_notice, NULL, LV_ALIGN_CENTER, 0, 0);

}

void lvgl_gui_setup_screen()
{
    screen = SETUP_SCREEN;
    lvgl_gui->setup_kb = NULL;
    
    //字体风格
    static lv_style_t font_16_style;
    lv_style_set_text_font(&font_16_style, LV_STATE_DEFAULT, &yumo_16);

    static lv_style_t lv_font_20_style;
    lv_style_set_text_font(&lv_font_20_style, LV_STATE_DEFAULT, &lv_font_montserrat_20);

    static lv_style_t font_24_style;
    lv_style_set_text_font(&font_24_style, LV_STATE_DEFAULT, &ziyuanyuanti_24);

    static lv_style_t lv_font_24_style;
    lv_style_set_text_font(&lv_font_24_style, LV_STATE_DEFAULT, &lv_font_montserrat_24);

    static lv_style_t img_style;
    lv_style_init(&img_style);

    //创建设置屏幕
    lvgl_gui->setup_screen = lv_obj_create(NULL, NULL);

    //设置页面
    lvgl_gui->setup_page = lv_page_create(lvgl_gui->setup_screen, NULL);
    lv_obj_set_size(lvgl_gui->setup_page, 320, 240);
    lv_obj_align(lvgl_gui->setup_page, NULL, LV_ALIGN_CENTER, 0, 0);
    // lv_page_set_edge_flash(lvgl_gui->setup_page, true);

    //创建WIFI设置文字框
    lvgl_gui->setup_label_wifissid = lv_label_create(lvgl_gui->setup_page, NULL);
    lv_obj_add_style(lvgl_gui->setup_label_wifissid, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_label_wifissid , "WIFI名称:");
    lv_obj_set_pos(lvgl_gui->setup_label_wifissid , 10 , 10);

    lvgl_gui->setup_text_wifissid = lv_textarea_create(lvgl_gui->setup_page, NULL);
    lv_obj_add_style(lvgl_gui->setup_text_wifissid, LV_LABEL_PART_MAIN , &font_16_style);
    lv_textarea_set_text(lvgl_gui->setup_text_wifissid, "HUAWEI-JQV4BC");
    lv_textarea_set_placeholder_text(lvgl_gui->setup_text_wifissid, "WIFI SSID");
    lv_obj_set_pos(lvgl_gui->setup_text_wifissid, 10, 30);
    lv_obj_set_size(lvgl_gui->setup_text_wifissid, 200, 40);
    lv_textarea_set_one_line(lvgl_gui->setup_text_wifissid, true);
    lv_textarea_set_cursor_hidden(lvgl_gui->setup_text_wifissid, true);

    lvgl_gui->setup_label_wifipsw = lv_label_create(lvgl_gui->setup_page, NULL);
    lv_obj_add_style(lvgl_gui->setup_label_wifipsw, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_label_wifipsw , "WIFI密码:");
    lv_obj_set_pos(lvgl_gui->setup_label_wifipsw , 10 , 70);

    lvgl_gui->setup_text_wifipsw = lv_textarea_create(lvgl_gui->setup_page, NULL);
    lv_obj_add_style(lvgl_gui->setup_text_wifipsw, LV_LABEL_PART_MAIN , &font_16_style);
    lv_textarea_set_text(lvgl_gui->setup_text_wifipsw, "422wifi.");
    lv_textarea_set_placeholder_text(lvgl_gui->setup_text_wifipsw, "WIFI Password");
    lv_textarea_set_pwd_mode(lvgl_gui->setup_text_wifipsw, true);
    lv_obj_set_pos(lvgl_gui->setup_text_wifipsw, 10, 90);
    lv_obj_set_size(lvgl_gui->setup_text_wifipsw , 200 , 40);
    lv_textarea_set_one_line(lvgl_gui->setup_text_wifipsw, true);
    lv_textarea_set_cursor_hidden(lvgl_gui->setup_text_wifipsw, true);

    lvgl_gui->setup_btn_wifi = lv_btn_create(lvgl_gui->setup_page, NULL);
    lv_obj_set_size(lvgl_gui->setup_btn_wifi , 100 , 30);
    lv_obj_set_pos(lvgl_gui->setup_btn_wifi, 10, 130);
    lvgl_gui->setup_btn_label_wifi = lv_label_create(lvgl_gui->setup_btn_wifi , NULL);
    lv_obj_add_style(lvgl_gui->setup_btn_label_wifi, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_btn_label_wifi , "连接WIFI");

    lvgl_gui->setup_btn_unwifi = lv_btn_create(lvgl_gui->setup_page, NULL);
    lv_obj_set_size(lvgl_gui->setup_btn_unwifi , 100 , 30);
    lv_obj_set_pos(lvgl_gui->setup_btn_unwifi, 110, 130);
    lvgl_gui->setup_btn_label_unwifi = lv_label_create(lvgl_gui->setup_btn_unwifi , NULL);
    lv_obj_add_style(lvgl_gui->setup_btn_label_unwifi, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_btn_label_unwifi , "断开WIFI");

    //创建MQTT设置文字框
    lvgl_gui->setup_label_mqttserver = lv_label_create(lvgl_gui->setup_page, NULL);
    lv_obj_add_style(lvgl_gui->setup_label_mqttserver, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_label_mqttserver , "MQTT服务器地址:");
    lv_obj_set_pos(lvgl_gui->setup_label_mqttserver , 10 , 170);

    lvgl_gui->setup_text_mqttserver = lv_textarea_create(lvgl_gui->setup_page, NULL);
    lv_obj_add_style(lvgl_gui->setup_text_mqttserver, LV_LABEL_PART_MAIN , &font_16_style);
    lv_textarea_set_text(lvgl_gui->setup_text_mqttserver, "192.168.3.2");
    lv_textarea_set_placeholder_text(lvgl_gui->setup_text_mqttserver, "MQTT Server");
    lv_obj_set_pos(lvgl_gui->setup_text_mqttserver, 10, 190);
    lv_obj_set_size(lvgl_gui->setup_text_mqttserver , 200 , 30);
    lv_textarea_set_one_line(lvgl_gui->setup_text_mqttserver, true);
    lv_textarea_set_cursor_hidden(lvgl_gui->setup_text_mqttserver, true);

    lvgl_gui->setup_label_mqttport = lv_label_create(lvgl_gui->setup_page, NULL);
    lv_obj_add_style(lvgl_gui->setup_label_mqttport, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_label_mqttport , "MQTT端口号:");
    lv_obj_set_pos(lvgl_gui->setup_label_mqttport , 10 , 230);

    lvgl_gui->setup_text_mqttport = lv_textarea_create(lvgl_gui->setup_page, NULL);
    lv_obj_add_style(lvgl_gui->setup_text_mqttport, LV_LABEL_PART_MAIN , &font_16_style);
    lv_textarea_set_text(lvgl_gui->setup_text_mqttport, "1883");
    lv_textarea_set_placeholder_text(lvgl_gui->setup_text_mqttport, "MQTT Port");
    lv_textarea_set_accepted_chars(lvgl_gui->setup_text_mqttport, "0123456789");
    lv_obj_set_pos(lvgl_gui->setup_text_mqttport, 10, 250);
    lv_obj_set_size(lvgl_gui->setup_text_mqttport , 200 , 30);
    lv_textarea_set_one_line(lvgl_gui->setup_text_mqttport, true);
    lv_textarea_set_cursor_hidden(lvgl_gui->setup_text_mqttport, true);

    lvgl_gui->setup_btn_mqtt = lv_btn_create(lvgl_gui->setup_page, NULL);
    lv_obj_set_size(lvgl_gui->setup_btn_mqtt , 100 , 30);
    lv_obj_set_pos(lvgl_gui->setup_btn_mqtt, 10, 290);
    lvgl_gui->setup_btn_label_mqtt = lv_label_create(lvgl_gui->setup_btn_mqtt , NULL);
    lv_obj_add_style(lvgl_gui->setup_btn_label_mqtt, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_btn_label_mqtt , "连接MQTT");
    
    lvgl_gui->setup_btn_unmqtt = lv_btn_create(lvgl_gui->setup_page, NULL);
    lv_obj_set_size(lvgl_gui->setup_btn_unmqtt , 100 , 30);
    lv_obj_set_pos(lvgl_gui->setup_btn_unmqtt, 110, 290);
    lvgl_gui->setup_btn_label_unmqtt = lv_label_create(lvgl_gui->setup_btn_unmqtt , NULL);
    lv_obj_add_style(lvgl_gui->setup_btn_label_unmqtt, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_btn_label_unmqtt , "断开MQTT");

    //离开设置界面按键
    lvgl_gui->setup_btn_exit = lv_btn_create(lvgl_gui->setup_page, NULL);
    lv_obj_set_size(lvgl_gui->setup_btn_exit , 100 , 30);
    lv_obj_set_pos(lvgl_gui->setup_btn_exit, 10, 330);
    lvgl_gui->setup_btn_label_exit = lv_label_create(lvgl_gui->setup_btn_exit , NULL);
    lv_obj_add_style(lvgl_gui->setup_btn_label_exit, LV_LABEL_PART_MAIN , &font_16_style);
    lv_label_set_text(lvgl_gui->setup_btn_label_exit , "离开设置");

    //全局状态图标
    lvgl_gui->setup_img_wifi = lv_img_create(lvgl_gui->setup_screen , NULL);

    if(wifi_state == WIFI_EVENT_STA_CONNECTED)
    {
        lv_img_set_src(lvgl_gui->setup_img_wifi, &icon_wifi_32);
    }
    else
    {
        lv_img_set_src(lvgl_gui->setup_img_wifi, &icon_nowifi_32);
    }
    lv_obj_align(lvgl_gui->setup_img_wifi , NULL , LV_ALIGN_IN_BOTTOM_RIGHT , -10 , 0);

    lvgl_gui->setup_img_mqtt = lv_img_create(lvgl_gui->setup_screen , NULL);
    if(mqtt_state == MQTT_EVENT_CONNECTED)
    {
        lv_img_set_src(lvgl_gui->setup_img_mqtt, &icon_mqtt_32);
    }
    else if(mqtt_state == MQTT_EVENT_DISCONNECTED || mqtt_state == MQTT_EVENT_ERROR)
    {
        lv_img_set_src(lvgl_gui->setup_img_mqtt, &icon_nomqtt_32);
    }
    lv_obj_align(lvgl_gui->setup_img_mqtt , NULL , LV_ALIGN_IN_BOTTOM_RIGHT , -52 , 0);

    
    setup_screen_events_init();
}

void lvgl_gui_start()
{
    lvgl_gui = (lvgl_gui_t*)malloc(sizeof(lvgl_gui_t));
    lvgl_gui_user_screen();
    lv_scr_load(lvgl_gui->user_screen);
}