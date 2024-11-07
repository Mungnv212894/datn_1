#ifndef THINGSBOARD_H
#define THINGSBOARD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_http_client.h"
#include "esp_log.h"  
#include "cJSON.h"


#define HOST "http://192.168.0.101:8080/api/v1/sq7zjUpuojMMZooJY233/telemetry"
#define TAG_HTTP_POST "HTTP_POST"


esp_err_t _http_event_handler(esp_http_client_event_t *evt);
int http_post_json_handle(char *payload);
void send_to_thingsboard(int adc_values[5]);


#endif // THINGSBOARD_H