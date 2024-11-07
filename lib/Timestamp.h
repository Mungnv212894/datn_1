#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "esp_log.h"
#include "esp_event.h"
#include "esp_sntp.h"
#include "time.h"
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Callback function for time synchronization event
void time_sync_notification_cb(struct timeval *tv);

// Function to initialize SNTP
void initialize_sntp();

// Function to wait for time to be set
void wait_for_time();

// Function to get the current timestamp
time_t get_current_timestamp();

#ifdef __cplusplus
}
#endif

#endif /* TIMESTAMP_H */
