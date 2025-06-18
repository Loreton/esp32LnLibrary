#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t pin;
    const char* name;
    uint8_t pressed_level;
    uint8_t state;
    bool pulse_active;
    uint32_t pulse_end_ms;
    uint32_t pulse_remain_ms;
} relay_t;

void relay_init(relay_t* relay, uint8_t pin, const char* name, uint8_t pressed_level);
void relay_set(relay_t* relay, uint8_t level);
void relay_start_pulse(relay_t* relay, uint32_t ms);
void relay_update(relay_t* relay, uint32_t now_ms);
uint8_t relay_get_state(const relay_t* relay);