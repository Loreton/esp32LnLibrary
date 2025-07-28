#include <Arduino.h>
#include "driver/ledc.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Configura il timer LEDC
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 1000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configura il canale LEDC
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = GPIO_NUM_4,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 512,
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    Serial.println("PWM attivo sul GPIO 4 (50% duty, 1kHz)");
}

void loop() {}
