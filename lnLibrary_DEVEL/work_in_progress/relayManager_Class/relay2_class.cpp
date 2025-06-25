//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 20.29.26
//

#include "relay_class.h" // Include l'header della classe
// Non serve #include <Arduino.h> qui, dato che è già in led_class.h
// Se però ci fossero funzioni di Arduino usate solo nell'implementazione, andrebbe qui.

// Implementazione del Costruttore
Relay::Relay(uint8_t relayPin, String relayName, uint8_t level) {
    pin = relayPin;
    name = relayName;
    pressedLevel = level;
    pinMode(pin, OUTPUT);
    off();
  }

void Relay::on() {
    digitalWrite(pin, pressedLevel);
    isOn = true;
  }

void Relay::off() {
    digitalWrite(pin, !pressedLevel);
    isOn = false;
    pulseActive = false; // se lo spegni manualmente, disattivi eventuale pulse
  }

void Relay::toggle() {
    isOn ? off() : on();
  }

void Relay::startPulse(unsigned long durationMs) {
    on();
    pulseDuration = durationMs;
    pulseStartTime = millis();
    pulseActive = true;
  }

void Relay::update() {
    if (pulseActive && millis() - pulseStartTime >= pulseDuration) {
      off();
    }
  }

bool Relay::isActive() {
    return isOn;
  }