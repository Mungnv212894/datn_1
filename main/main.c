#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "adc.h"
#include "wifi.h"
#include "Timestamp.h"
#include "thingsboard.h"


QueueHandle_t queue;
void app_main() {
    // Kết nối WiFi và cấu hình thời gian
    connect_to_wifi();
    initialize_sntp();
    wait_for_time();
    
    // Khởi tạo ADC
    init_adc();

    // Tạo hàng đợi (queue) để truyền dữ liệu giữa các task
    // queue = xQueueCreate(10, sizeof(int));  // Tạo queue chứa tối đa 10 phần tử, mỗi phần tử là int
    // if (queue == NULL) {
    //     printf("Không thể tạo queue\n");
    //     return;
    // }

    // Tạo 2 task: đọc ADC và in giá trị
    xTaskCreatePinnedToCore(adc_task, "adc_task", 6000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(send_to_thingsboard, "print_task",6000, NULL, 1, NULL, 1);
}