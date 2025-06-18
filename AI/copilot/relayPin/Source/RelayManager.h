#pragma once
#include <Arduino.h>

struct Relay {
    uint8_t pin;
    const char* name;
    uint8_t pressedLevel;
    uint8_t state;
    bool pulseActive;
    uint32_t pulseEndMs;
    uint32_t pulseRemainMs;
};

void relayInit(Relay* relay, uint8_t pin, const char* name, uint8_t pressedLevel);
void relaySet(Relay* relay, uint8_t level);
void relayStartPulse(Relay* relay, uint32_t ms);
void relayUpdate(Relay* relay, uint32_t nowMs);
uint8_t relayGetState(const Relay* relay);