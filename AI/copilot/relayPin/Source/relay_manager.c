#include "relay_manager.h"
#include "driver/gpio.h"
#include "esp_timer.h"

void relay_init(relay_t* relay, uint8_t pin, const char* name, uint8_t pressed_level) {
    relay->pin = pin;
    relay->name = name;
    relay->pressed_level = pressed_level;
    relay->state = !pressed_level;
    relay->pulse_active = false;
    relay->pulse_end_ms = 0;
    relay->pulse_remain_ms = 0;
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_level(pin, relay->state);
}

void relay_set(relay_t* relay, uint8_t level) {
    relay->state = level;
    gpio_set_level(relay->pin, level);
}

void relay_start_pulse(relay_t* relay, uint32_t ms) {
    relay->pulse_active = true;
    uint32_t now = esp_timer_get_time() / 1000;
    relay->pulse_end_ms = now + ms;
    relay->pulse_remain_ms = ms;
    relay_set(relay, relay->pressed_level);
}

void relay_update(relay_t* relay, uint32_t now_ms) {
    if (relay->pulse_active) {
        if (now_ms >= relay->pulse_end_ms) {
            relay_set(relay, !relay->pressed_level);
            relay->pulse_active = false;
            relay->pulse_remain_ms = 0;
        } else {
            relay->pulse_remain_ms = relay->pulse_end_ms - now_ms;
        }
    }
}

uint8_t relay_get_state(const relay_t* relay) {
    return relay->state;
}