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
#include "adc.h"


#define HOST "http://tbc7.hust-2slab.org:8080/api/v1/79JKq5zbOWu3HMpohVNR/telemetry"
#define TAG_HTTP_POST "HTTP_POST"


esp_err_t _http_event_handler(esp_http_client_event_t *evt);
int http_post_json_handle(char *payload);
void send_to_thingsboard(void *pvParameters);
//void print_task(void *pvParameters);  // Khai báo task để in dữ liệu


#endif // THINGSBOARD_H