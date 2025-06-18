#include "RelayManager.h"

void relayInit(Relay* relay, uint8_t pin, const char* name, uint8_t pressedLevel) {
    relay->pin = pin;
    relay->name = name;
    relay->pressedLevel = pressedLevel;
    relay->state = !pressedLevel;
    relay->pulseActive = false;
    relay->pulseEndMs = 0;
    relay->pulseRemainMs = 0;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, relay->state);
}

void relaySet(Relay* relay, uint8_t level) {
    relay->state = level;
    digitalWrite(relay->pin, level);
}

void relayStartPulse(Relay* relay, uint32_t ms) {
    relay->pulseActive = true;
    uint32_t now = millis();
    relay->pulseEndMs = now + ms;
    relay->pulseRemainMs = ms;
    relaySet(relay, relay->pressedLevel);
}

void relayUpdate(Relay* relay, uint32_t nowMs) {
    if (relay->pulseActive) {
        if (nowMs >= relay->pulseEndMs) {
            relaySet(relay, !relay->pressedLevel);
            relay->pulseActive = false;
            relay->pulseRemainMs = 0;
        } else {
            relay->pulseRemainMs = relay->pulseEndMs - nowMs;
        }
    }
}

uint8_t relayGetState(const Relay* relay) {
    return relay->state;
}