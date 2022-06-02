#include <string.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "Device_Info.h"
#include "mqtt.h"
#include "event.h"
#include "lvgl_gui.h"
#include "ntp.h"
#include "wifi.h"

static const char *TAG = "esp32_wifi";

void wifi_icon_display(int32_t state);
int32_t wifi_state = WIFI_EVENT_STA_DISCONNECTED;
//设置ssid和password
wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = "",
            .scan_method = WIFI_ALL_CHANNEL_SCAN,
            .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK
        }
    };

esp_err_t wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
    static int retry_num = 0;           /* 记录wifi重连次数 */
    static int wifi_connected_bit = 0;
    if (event_base == WIFI_EVENT) 
    {
        switch (event_id) 
        {
            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
                wifi_state = WIFI_EVENT_STA_START;
                wifi_icon_display(event_id);
                // esp_wifi_connect();
                break;
            //case IP_EVENT_STA_GOT_IP:
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
                wifi_state = WIFI_EVENT_STA_CONNECTED;
                wifi_icon_display(event_id);
                retry_num = 0;  
                wifi_connected_bit = 1;
                /* ESP_LOGI(TAG, "Got IP: %s\n",
                        esp_ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip)); */
                ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data; /* 获取IP地址信息*/
                ESP_LOGI(TAG,"got ip:%d.%d.%d.%d" , IP2STR(&event->ip_info.ip));  /* 打印ip地址*/
                ntp_init();
                //mqtt_init();
                Device_Event_init();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
                wifi_state = WIFI_EVENT_STA_DISCONNECTED;
                wifi_icon_display(event_id);
                if(wifi_connected_bit == 1)
                {
                    mqtt_uninit();
                    Device_Event_uninit();
                    printf("wifi disconnect , all task uninit\n");
                }
                wifi_connected_bit = 0;
                if (retry_num < 10)  /* WiFi重连次数小于10 */
                {
                    esp_wifi_connect();
                    retry_num++;
                    printf("retry to connect to the AP %d times. \n",retry_num);
                } 
                
                break;
            default:
                wifi_state = WIFI_EVENT_STA_DISCONNECTED;
                wifi_icon_display(event_id);
                break;
        }
    }
    return ESP_OK;
}

// void wifi_task()
// {
//     while(1)
//     {
//         wifi_config_t wifi_config = { 0 };
//         memset(&wifi_config,0,sizeof(wifi_config));
//         memcpy(wifi_config.sta.ssid, WIFI_SSID, strlen(WIFI_SSID));
//         memcpy(wifi_config.sta.password,WIFI_PASSWD,strlen(WIFI_PASSWD));

//         ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA));
//         ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
//         ESP_ERROR_CHECK( esp_wifi_connect());
//     }
// }

void wifi_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* 初始化底层TCP/IP堆栈。在应用程序启动时，应该调用此函数一次。*/
    ESP_ERROR_CHECK(esp_netif_init());
    
    /* 创建默认事件循环,*/
    ESP_ERROR_CHECK(esp_event_loop_create_default());    

    /* 创建一个默认的WIFI-STA网络接口，如果初始化错误，此API将中止。*/
    esp_netif_t *wifi_sta = esp_netif_create_default_wifi_sta();


    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();//设置默认的wifi栈参数
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));    //初始化WiFi Alloc资源为WiFi驱动，如WiFi控制结构，RX / TX缓冲区，WiFi NVS结构等，此WiFi也启动WiFi任务。
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));//设置WIFI模式

    //ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));//创建事件的任务
    /* 将事件处理程序注册到系统默认事件循环，分别是WiFi事件和IP地址事件 */
    // ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    // ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip));
    
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);

    esp_netif_set_hostname(wifi_sta, WIFI_HOST_NAME);//接入点设备名更变

    //printf("ERROR:%s\n" , esp_err_to_name(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config)));

    //printf("ERROR:%d\n" , ESP_ERR_WIFI_NOT_INIT);
    //esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));// Set the WiFi API configuration storage type
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    //ESP_ERROR_CHECK(esp_wifi_connect());

    //xTaskCreate(&wifi_task, "wifi_task", 2048, NULL, 15, NULL);//创建任务
}

void wifi_icon_display(int32_t state)
{
    if(state == WIFI_EVENT_STA_CONNECTED)
    {
        if(screen == USER_SCREEN)
        {
            lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_RELEASED, &icon_wifi_32);
            lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_PRESSED, &icon_wifi_32);
            lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_CHECKED_RELEASED, &icon_wifi_32);
            lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_CHECKED_PRESSED, &icon_wifi_32);
        }
        else if(screen == SETUP_SCREEN)
        {
            lv_img_set_src(lvgl_gui->setup_img_wifi, &icon_wifi_32);
        }
    }
    else
    {
        if(screen == USER_SCREEN)
        {
            lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_RELEASED, &icon_nowifi_32);
            lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_PRESSED, &icon_nowifi_32);
            lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_CHECKED_RELEASED, &icon_nowifi_32);
            lv_imgbtn_set_src(lvgl_gui->user_imgbtn_wifi, LV_BTN_STATE_CHECKED_PRESSED, &icon_nowifi_32);
        }
        else if(screen == SETUP_SCREEN)
        {
            lv_img_set_src(lvgl_gui->setup_img_wifi, &icon_nowifi_32);
        }
    }
}