//
// updated by ...: Loreto Notarantonio
// Date .........: 22-06-2025 19.55.56
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type

// --- Definizione della Struttura del LED ---

typedef struct ledController_sc {
    uint8_t             _pin;
    uint8_t             _on_level        = HIGH;
    uint8_t             _on              = HIGH;
    uint8_t             _off             = LOW;
    // bool                _inverted        = false;

    // Blinking parameters
    unsigned long       _onDuration      = 500;
    unsigned long       _offDuration     = 500;
    bool                _blinking        = false;
    int8_t             _num_cycles       = 0;
    int8_t             _temporaryBlinking       = false;

    // Fixed on parameters
    bool                _fixed           = true;
    // unsigned long       _fixedOnStart    = 0;
    // unsigned long       _fixedOnDuration = 0;
    bool                _pulseOn         = false;
    unsigned long       _pulseOnStart    = 0;
    unsigned long       _pulseOnDuration = 0;

    // Internals
    bool                _ledState        = false;
    unsigned long       _lastToggle      = 0;



    void init(uint8_t pin, uint8_t active_level=HIGH);
    void update();
    // void startBlinking(unsigned long onMs, unsigned long offMs);
    void startBlinking(unsigned long onMs, unsigned long offMs, int8_t cycles=0);
    // void stopBlinking();
    void pulseOn(unsigned long duration);
    void _setLed(bool on);
    // bool isOn() const;
    void off();
    void on();
    void set(uint8_t state);

    void _setFixed();
    void _setBlinking(int8_t cycles=0);
    void _setPulse();
    void _clearAll();

    // bool isBlinking() const;
    // bool isFixedOn() const;


} ledController_sc;


