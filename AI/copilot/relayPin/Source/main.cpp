#include <stdio.h>
#include "relay_manager.h"
#include "esp_timer.h"

#define RELAY_GPIO 16

relay_t relay;

void app_main(void) {
    relay_init(&relay, RELAY_GPIO, "RELAY_PIN", 1); // pressed_level=1 se il relè è attivo alto

    relay_start_pulse(&relay, 3000); // avvia pulsetime di 3 secondi

    while (1) {
        uint32_t now = esp_timer_get_time() / 1000; // ms
        relay_update(&relay, now);
        printf("Relay %s stato: %d, tempo rimanente: %d ms\n", relay.name, relay_get_state(&relay), relay.pulse_remain_ms);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}