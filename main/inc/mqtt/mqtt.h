#ifndef _MQTT_H_
#define _MQTT_H_

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h" 
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "mqtt_client.h"

#include "mqtt.h"

#define CONNECTED_BIT BIT0
#define FAIL_BIT      BIT1

extern unsigned char MQTT_REC_BIT;

void mqtt_subscribe(char *my_mqtt_topic);
void mqtt_unsubscribe(char *my_mqtt_topic);
void mqtt_send_message(char *my_mqtt_topic , char *my_mqtt_data , int message_qos);
esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);
void mqtt_app_start();
void mqtt_init();


#endif /* _MQTT_H_ */