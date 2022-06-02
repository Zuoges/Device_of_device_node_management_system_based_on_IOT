#include <stdio.h>

#include "wifi.h"
#include "gpio.h"
#include "key.h"
#include "im1281b.h"
#include "event.h"
#include "Euclidean.h"
#include "lvgl_gui.h"
#include "Device_Info.h"

void app_main(void)
{
    gpio_init();
    key_init();
    im1281b_init();
    euclidean_data_init();
#ifdef USE_NETWORK
    wifi_init();   
#endif
    lvgl_init();
    //Device_Event_init();
    
}
