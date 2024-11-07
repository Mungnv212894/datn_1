#include "thingsboard.h"

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