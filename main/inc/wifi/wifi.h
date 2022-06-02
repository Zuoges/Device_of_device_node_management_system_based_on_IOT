#ifndef _WIFI_H_
#define _WIFI_H_

#include "esp_wifi.h"

extern wifi_config_t wifi_config;
extern int32_t wifi_state;

void wifi_init();
void wifi_icon_display(int32_t state);

#endif /* _WIFI_H_ */