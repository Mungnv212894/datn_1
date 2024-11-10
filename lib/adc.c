#include "adc.h"
#include "freertos/queue.h"
#include <stdlib.h>

// GPIO labels mapped to their corresponding channels

// Initialize ADC configuration
void init_adc() {
    adc1_config_width(ADC_WIDTH_BIT_12);  
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_12);  // Top
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_12);  // Right
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_12);  // Bottom
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_12);  // Left
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_12);  // Center
}

// Read ADC values for each channel over multiple readings
char *createJsonBody(int64_t timestamp, char *label, int adc_vol[100]) {
    //printf("%s:%d\n",label, timestamp);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "timestamp", timestamp);
    cJSON_AddStringToObject(root, "label", label);
    cJSON *adc_array = cJSON_CreateIntArray(adc_vol, 100);  // Mảng 100 giá trị ADC
    if (adc_array == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    // Thêm mảng ADC vào đối tượng gốc với tên là "vols"
    cJSON_AddItemToObject(root, "vols", adc_array);

    // In chuỗi JSON
    char *out = cJSON_Print(root);
    cJSON_Delete(root);  // Giải phóng bộ nhớ sau khi sử dụng
    return out;
}

void adc_task(void *pvParameters) {
    // Khai báo mảng lưu trữ giá trị ADC cho mỗi cảm biến
    int pir_vol_0[100];
    int pir_vol_1[100];
    int pir_vol_2[100];
    int pir_vol_3[100];
    int pir_vol_4[100];

    // Kiểm tra và tạo hàng đợi chỉ một lần
    if (queue == NULL) {
        queue = xQueueCreate(ITEMS_NUM, ITEM_SIZE);
        if (queue == NULL) {
            printf("Failed to create queue\n");
            return;
        }
    }

    while (1) {
        // Lấy thời gian hiện tại (timestamp)
        time_t timestamps = get_current_timestamp();
        int64_t timestamp = (int64_t)timestamps * 1000;


        // Đọc 100 lần ADC và lưu vào mảng
        for (int i = 0; i < 100; i++) {
            pir_vol_0[i] = adc1_get_raw(ADC1_CHANNEL_3);  // Top sensor (GPIO39)
            pir_vol_1[i] = adc1_get_raw(ADC1_CHANNEL_4);  // Right sensor (GPIO32)
            pir_vol_2[i] = adc1_get_raw(ADC1_CHANNEL_5);  // Bottom sensor (GPIO33)
            pir_vol_3[i] = adc1_get_raw(ADC1_CHANNEL_6);  // Left sensor (GPIO34)
            pir_vol_4[i] = adc1_get_raw(ADC1_CHANNEL_7);  // Center sensor (GPIO35)

            vTaskDelay(pdMS_TO_TICKS(10));  // Delay giữa các lần đọc ADC
        }

        // Tạo và gửi JSON cho từng cảm biến vào hàng đợi
        char *pir_json_0 = createJsonBody(timestamp, "top", pir_vol_0);
        xQueueSend(queue, (void *)pir_json_0, (TickType_t)0);
        free(pir_json_0);
        pir_json_0 = NULL;

        char *pir_json_1 = createJsonBody(timestamp, "right", pir_vol_1);
        xQueueSend(queue, (void *)pir_json_1, (TickType_t)0);
        free(pir_json_1);
        pir_json_1 = NULL;

        char *pir_json_2 = createJsonBody(timestamp, "bottom", pir_vol_2);
        xQueueSend(queue, (void *)pir_json_2, (TickType_t)0);
        free(pir_json_2);
        pir_json_2 = NULL;

        char *pir_json_3 = createJsonBody(timestamp, "left", pir_vol_3);
        xQueueSend(queue, (void *)pir_json_3, (TickType_t)0);
        free(pir_json_3);
        pir_json_3 = NULL;

        char *pir_json_4 = createJsonBody(timestamp, "center", pir_vol_4);
        xQueueSend(queue, (void *)pir_json_4, (TickType_t)0);
        free(pir_json_4);
        pir_json_4 = NULL;
    }
}

void print_task(void *pvParameters) {
    // Kiểm tra nếu queue chưa được tạo thì tạo queue mới
    if (queue == NULL) {
        queue = xQueueCreate(ITEMS_NUM, ITEM_SIZE);
    }

    char rxbuff[700];

    while (1) {
        // Nhận dữ liệu từ queue và in ra độ dài
        if (xQueueReceive(queue, &rxbuff, (TickType_t)1)) {
            printf("%s\n", rxbuff);
        }
    }
}

