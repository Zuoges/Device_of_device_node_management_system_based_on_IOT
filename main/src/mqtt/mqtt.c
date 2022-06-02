#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h" 
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "mqtt_client.h"

#include "Euclidean.h"
#include "lvgl_gui.h"
#include "Device_Info.h"
#include "mqtt.h"

static const char *TAG = "esp32_mqtt";

static EventGroupHandle_t mqtt_event_group;
esp_mqtt_client_handle_t client;

char mqtt_receive_topic[200] = {'\0'};
char mqtt_receive_data[200] = {'\0'}; 
char mqtt_json_receive_data[200] = {'\0'}; 

unsigned char MQTT_REC_BIT = 0;
int32_t mqtt_state = MQTT_EVENT_DISCONNECTED;


void mqtt_icon_display(int32_t state);

void mqtt_subscribe(char *my_mqtt_topic)        //MQTT��������
{
    int msg_id;
    msg_id = esp_mqtt_client_subscribe(client, my_mqtt_topic, 1);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
}

void mqtt_unsubscribe(char *my_mqtt_topic)      //MQTTȡ����������
{
    int msg_id;
    msg_id = esp_mqtt_client_unsubscribe(client, my_mqtt_topic);
    ESP_LOGI(TAG, "cancel subscribe successful, msg_id=%d", msg_id);
}

void mqtt_send_message(char *my_mqtt_topic , char *my_mqtt_data , int message_qos)      //MQTT������Ϣ
{
    int msg_id;
    msg_id = esp_mqtt_client_publish(client, my_mqtt_topic, my_mqtt_data, 0, message_qos, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
}

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    client = event->client;
    //int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED://MQTT�����¼�
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            mqtt_state = MQTT_EVENT_CONNECTED;
            mqtt_icon_display(event->event_id);
            xEventGroupSetBits(mqtt_event_group, CONNECTED_BIT);
            //���Ͷ���
            //mqtt_subscribe("testesp32mqtt");
            break;
        case MQTT_EVENT_DISCONNECTED://MQTT�Ͽ������¼�
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            mqtt_state = MQTT_EVENT_DISCONNECTED;
            mqtt_icon_display(event->event_id);
            //mqtt�����¼�
            xEventGroupClearBits(mqtt_event_group, CONNECTED_BIT);
            break;
        case MQTT_EVENT_SUBSCRIBED://MQTT���Ͷ����¼�
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            //msg_id = esp_mqtt_client_publish(client, "testesp32mqtt", "���ĳɹ�", 0, 0, 0);
            //ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED://MQTTȡ�������¼�
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED://MQTT�����¼�
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA://MQTT���������¼�
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);   //����
            sprintf(mqtt_receive_topic,"%.*s", event->topic_len, event->topic);
            //mqtt_receive_topic=event->topic;
            // printf("DATA=%.*s\r\n", event->data_len, event->data);      //����
            // sprintf(mqtt_receive_data,"%.*s", event->data_len, event->data);
            //printf("%s\n",mqtt_receive_data);
            MQTT_REC_BIT = 1;

            // if((strncmp(mqtt_receive_topic , MQTT_RECEIVE_ADDRESS , strlen(MQTT_RECEIVE_ADDRESS))) == 0)
            // {
            //     sprintf(mqtt_json_receive_data,"%.*s", event->data_len, event->data);
            // }

            break;
        case MQTT_EVENT_ERROR://MQTT�����¼�
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            mqtt_state = MQTT_EVENT_ERROR;
            xEventGroupClearBits(mqtt_event_group, CONNECTED_BIT);
            break;
        default:break;
    }
    return ESP_OK;
}

void mqtt_app_start(char* server, int serverport)
{
    mqtt_event_group = xEventGroupCreate();
    esp_mqtt_client_config_t mqtt_cfg = {
        .host = server,            //MQTT������IP
        .event_handle = mqtt_event_handler, //MQTT�¼�
        .port = serverport,                         //�˿�
        .username = MQTT_USER_NAME,                //�û���
        .password = MQTT_PASSWORD,               //����
        .client_id = MQTT_ID,
        .disable_auto_reconnect = false,//���ñ�������
        .keepalive = 10,//�����������
        .transport = MQTT_TRANSPORT_OVER_TCP,//��������Э��
        .out_buffer_size = 2048,//���÷��ͻ�������С
        .buffer_size = 2048,    //���ý��ջ�������С
        // .user_context = (void *)your_context
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
    //��mqtt����
    xEventGroupWaitBits(mqtt_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}

void mqtt_init(char* server, int serverport)
{
    mqtt_app_start(server, serverport);
    mqtt_subscribe(MQTT_RECEIVE_ADDRESS);
    mqtt_subscribe(MQTT_PUBLISH_ADDRESS);
    //mqtt_cfg = (esp_mqtt_client_config_t*)malloc(sizeof(esp_mqtt_client_config_t));
}

// void mqtt_init()
// {
//     mqtt_app_start();
//     mqtt_subscribe(MQTT_RECEIVE_ADDRESS);
//     mqtt_subscribe(MQTT_PUBLISH_ADDRESS);
// }

void mqtt_uninit()
{
    esp_mqtt_client_stop(client);
}

void mqtt_icon_display(int32_t state)
{
    if(state == MQTT_EVENT_CONNECTED)
    {
        if(screen == USER_SCREEN)
        {
            lv_img_set_src(lvgl_gui->user_img_mqtt, &icon_mqtt_32);
        }
        else if(screen == SETUP_SCREEN)
        {
            lv_img_set_src(lvgl_gui->setup_img_mqtt, &icon_mqtt_32);
        }
    }
    else if(state == MQTT_EVENT_DISCONNECTED || state == MQTT_EVENT_ERROR)
    {
        if(screen == USER_SCREEN)
        {
            lv_img_set_src(lvgl_gui->user_img_mqtt, &icon_nomqtt_32);
        }
        else if(screen == SETUP_SCREEN)
        {
            lv_img_set_src(lvgl_gui->setup_img_mqtt, &icon_nomqtt_32);
        }
    }
}
