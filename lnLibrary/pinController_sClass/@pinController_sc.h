//
// updated by ...: Loreto Notarantonio
// Date .........: 23-06-2025 20.07.51
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type

// --- Definizione della Struttura del LED ---

typedef void (*pinControllerCallback)(struct pinController_sc* self);  // se volessi usare una callback
/** esempio:
void myLedControllerHandler(pinController_sc* led) {
    Serial.printf("ledController Callback: %s\n", led->_pinID);
}
*/


typedef struct pinController_sc {
    uint8_t        _pin;
    const   char*  _name;
    char           _pinID[21];  // conterrà [pin:%02d.%-15s] p->pin, p->name,

    uint8_t        _on_level        = HIGH;
    uint8_t        _on              = HIGH;
    uint8_t        _off             = LOW;

    // Blinking parameters
    uint32_t       _onTime      = 500;
    uint32_t       _offTime     = 500;
    bool           _blinking        = false;
    int8_t         _num_cycles       = 0;
    int8_t         _temporaryBlinking       = false;

    // Fixed on parameters
    bool           _fixed           = true;
    bool           _pulseOn         = false;
    uint32_t       _pulseOnStart    = 0;
    uint32_t       _pulseOnDuration = 0;

    // Internals
    bool           _ledState        = false;
    uint32_t       _lastToggle      = 0;
    // bool           _isBusy          = false;



    void init(const char *name, uint8_t pin, uint8_t active_level=HIGH);
    void update();

    void blinking(uint32_t onMs, uint32_t offMs, int8_t cycles=0);
    void blinking_dc(uint32_t period, float duty_cycle, int8_t cycles=0); // con duty_cycle
    void pulse(uint32_t duration);
    void off();
    void on();
    void set(uint8_t state);


    // ========================================
    // - Internal use funtcions
    // ========================================
    void _setLed(bool on);
    void _setFixed();
    void _setBlinking(int8_t cycles=0);
    void _setPulse();
    void _clearAll();


} pinController_sc;


