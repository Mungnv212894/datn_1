#include "adc.h"

// GPIO labels mapped to their corresponding channels
typedef struct {
    const char *label;
    adc1_channel_t channel;
} ADC_Channel;

// Define the ADC channels and labels
ADC_Channel adc_channels[] = {
    {"top", ADC1_CHANNEL_3},    // GPIO39
    {"right", ADC1_CHANNEL_4},  // GPIO32
    {"bottom", ADC1_CHANNEL_5}, // GPIO33
    {"left", ADC1_CHANNEL_6},   // GPIO34
    {"center", ADC1_CHANNEL_7}  // GPIO35
};

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
void read_adc_values(int adc_values[5][NUM_READINGS]) {
    for (int i = 0; i < NUM_READINGS; i++) {
        for (int j = 0; j < 5; j++) {
            adc_values[j][i] = adc1_get_raw(adc_channels[j].channel);
        }
        vTaskDelay(pdMS_TO_TICKS(50));  // Delay of 100ms between readings
    }
}

// Create JSON array of readings for each ADC channel
char *create_json_body(int adc_values[5][NUM_READINGS]) {
    time_t timestamp = get_current_timestamp();
    cJSON *root = cJSON_CreateArray();
    if (root == NULL) return NULL;

    for (int i = 0; i < 5; i++) {
        cJSON *adc_entry = cJSON_CreateObject();
        if (adc_entry == NULL) {
            cJSON_Delete(root);
            return NULL;
        }

        // Add label for the ADC channel
        cJSON_AddStringToObject(adc_entry, "label", adc_channels[i].label);

        // Add timestamp (example using time since boot)
        cJSON_AddNumberToObject(adc_entry, "timestamp", timestamp);

        // Create an array of readings for this ADC channel
        cJSON *vols_array = cJSON_CreateIntArray(adc_values[i], NUM_READINGS);
        if (vols_array == NULL) {
            cJSON_Delete(adc_entry);
            cJSON_Delete(root);
            return NULL;
        }
        cJSON_AddItemToObject(adc_entry, "vols", vols_array);

        // Add this channel's entry to the root array
        cJSON_AddItemToArray(root, adc_entry);
    }

    char *out = cJSON_Print(root);
    cJSON_Delete(root);  // Clean up cJSON object after use
    return out;
}

// Print JSON for ADC values
void print_adc_values() {
    int adc_values[5][NUM_READINGS];
    read_adc_values(adc_values);
    char *payload = create_json_body(adc_values);
    
    if (payload) {
        ESP_LOGI(TAG_JSON, "%s", payload);
        free(payload);  // Free the JSON string after use
    } else {
        ESP_LOGE(TAG_JSON, "Failed to create JSON payload");
    }
}
