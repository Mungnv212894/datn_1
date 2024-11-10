#pragma once
#ifndef __ADC__H_
#define __ADC__H_

#include <stdio.h>
#include "driver/adc.h"
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"
#include "freertos/FreeRTOS.h"  // For FreeRTOS types and macros
#include "freertos/task.h"      // For vTaskDelay
#include "Timestamp.h"
#include "queueService.h"  // Thêm dòng này để kết nối với queueService.h

#define TAG_JSON "ADC_JSON"




extern QueueHandle_t queue;  // Đảm bảo khai báo queue đúng cách

void init_adc();
char *createJsonBody(int64_t timestamp, char *label, int adc_vol[100]);
void adc_task(void *pvParameters);
void print_task(void *pvParameters);  // Khai báo task để in dữ liệu

#endif
