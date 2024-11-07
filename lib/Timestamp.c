#include "Timestamp.h"

static const char *TAG = "timestamp";

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

void initialize_sntp() {
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

void wait_for_time() {
    time_t now = 0;
    struct tm timeinfo = { 0 };
    while (timeinfo.tm_year < (2016 - 1900)) {
        printf("Waiting for time set...\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
}

time_t get_current_timestamp() {
    time_t now;
    time(&now);
    return now;
}
