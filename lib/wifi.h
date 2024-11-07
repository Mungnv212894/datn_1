#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_SSID "T3"
#define WIFI_PASS "123.123.123"

void wifi_event_group_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

void connect_to_wifi();

#ifdef __cplusplus
}
#endif

#endif /* WIFI_CONNECT_H */
