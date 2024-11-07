#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "wifi.h"
#include "Timestamp.h"
#include "esp_log.h"  // Đảm bảo bạn đã bao gồm thư viện này

#define HOST "http://192.168.0.101:8080/api/v1/sq7zjUpuojMMZooJY233/telemetry"
#define TAG_HTTP_POST "HTTP_POST"

// Khởi tạo ADC
void init_adc() {
    adc1_config_width(ADC_WIDTH_BIT_12);  
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_12);  // tren
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_12);  // phai
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_12);  // duoi
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_12);  // trai
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_12);  // giua
}

// Đọc giá trị ADC
void read_adc_values(int adc_values[5]) {
    adc_values[0] = adc1_get_raw(ADC1_CHANNEL_3);
    adc_values[1] = adc1_get_raw(ADC1_CHANNEL_4);
    adc_values[2] = adc1_get_raw(ADC1_CHANNEL_5);
    adc_values[3] = adc1_get_raw(ADC1_CHANNEL_6);
    adc_values[4] = adc1_get_raw(ADC1_CHANNEL_7);
}

// Tạo JSON body
char *create_json_body(int adc_values[5]) {
    time_t timestamp = get_current_timestamp();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "timestamp", timestamp);
    cJSON_AddNumberToObject(root, "adc_channel_3", adc_values[0]);
    cJSON_AddNumberToObject(root, "adc_channel_4", adc_values[1]);
    cJSON_AddNumberToObject(root, "adc_channel_5", adc_values[2]);
    cJSON_AddNumberToObject(root, "adc_channel_6", adc_values[3]);
    cJSON_AddNumberToObject(root, "adc_channel_7", adc_values[4]);

    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    return out;
}

// Hàm xử lý sự kiện HTTP
esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            //ESP_LOGI(TAG_HTTP_POST, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            //ESP_LOGI(TAG_HTTP_POST, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            //ESP_LOGI(TAG_HTTP_POST, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            //ESP_LOGI(TAG_HTTP_POST, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG_HTTP_POST, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            //ESP_LOGI(TAG_HTTP_POST, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            //ESP_LOGI(TAG_HTTP_POST, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            //ESP_LOGI(TAG_HTTP_POST, "HTTP_EVENT_REDIRECT");
            break;
        default:
            //ESP_LOGI(TAG_HTTP_POST, "Unhandled event ID: %d", evt->event_id);
            break;
    }
    return ESP_OK;
}

// Gửi dữ liệu JSON qua HTTP POST
int http_post_json_handle(char *payload) {
    esp_http_client_config_t config = {
        .url = HOST,
        .event_handler = _http_event_handler,
        .method = HTTP_METHOD_POST,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG_HTTP_POST, "Failed to initialize HTTP client");
        return 1;
    }

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, payload, strlen(payload));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        // ESP_LOGI(TAG_HTTP_POST, "HTTP POST Status = %d, content_length = %lld",
        //          esp_http_client_get_status_code(client),
        //          esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG_HTTP_POST, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    return (err == ESP_OK) ? 0 : 1;
}

// Gửi dữ liệu ADC lên ThingsBoard
void send_to_thingsboard(int adc_values[5]) {
    char *payload = create_json_body(adc_values);
    if (payload) {
        ESP_LOGI(TAG_HTTP_POST, "Sending JSON: %s", payload);
        if (http_post_json_handle(payload) == 0) {
           // ESP_LOGI(TAG_HTTP_POST, "Data sent successfully");
        } else {
            ESP_LOGE(TAG_HTTP_POST, "Failed to send data");
        }
        free(payload);  // Giải phóng bộ nhớ
    } else {
        ESP_LOGE(TAG_HTTP_POST, "Failed to create JSON payload");
    }
}

// Hàm chính
void app_main(void) {
    connect_to_wifi();
    initialize_sntp();
    wait_for_time();
    init_adc();

    int adc_values[5];
    while (1) {
        read_adc_values(adc_values);
        send_to_thingsboard(adc_values);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Gửi mỗi giây
        }
}
