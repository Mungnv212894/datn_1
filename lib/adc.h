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

#define TAG_JSON "ADC_JSON"
#define NUM_READINGS 20  // Number of readings to take per ADC channel

void init_adc();
void read_adc_values(int adc_values[5][NUM_READINGS]);
char *create_json_body(int adc_values[5][NUM_READINGS]);
void print_adc_values();

#endif
