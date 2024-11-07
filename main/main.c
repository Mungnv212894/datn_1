#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "adc.h"
//#include "thingsboard.h"
#include "wifi.h"
#include "Timestamp.h"

void app_main(void) {
    connect_to_wifi();
    initialize_sntp();
    wait_for_time();
    init_adc();

    // Declare adc_values as a 2D array with correct dimensions
    int adc_values[5][NUM_READINGS];
    
    while (1) {
        // Read ADC values into the 2D array
        read_adc_values(adc_values);
        
        // Print the ADC values as JSON (assuming print_adc_values is updated to accept adc_values)
        print_adc_values(adc_values);

        // Delay for 1000 ms (1 second)
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    }
