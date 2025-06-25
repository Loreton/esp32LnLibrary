//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.56.12
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type

// --- Definizione della Struttura del LED ---

typedef void (*pinControllerCallback)(struct pinController_Struct* self);  // se volessi usare una callback
/** esempio:
void myLedControllerHandler(pinController_Struct* led) {
    Serial.printf("ledController Callback: %s\n", led->_pinID);
}
*/


typedef struct pinController_Struct {
    uint8_t        pin_;
    const   char*  name_;
    char           pinID_[21];  // conterrà [pin:%02d.%-15s] p->pin, p->name,

    uint8_t        on_level_        = HIGH;
    uint8_t        on_              = HIGH;
    uint8_t        off_             = LOW;

    // Blinking parameters
    uint32_t       onTime_      = 500;
    uint32_t       offTime_     = 500;
    bool           blinking_        = false;
    int8_t         num_cycles_       = 0;
    int8_t         temporaryBlinking_       = false;

    // Fixed on parameters
    bool           fixed_           = true;
    bool           pulseOn_         = false;
    uint32_t       pulseOnStart_    = 0;
    uint32_t       pulseOnDuration_ = 0;

    // Internals
    bool           ledState_        = false;
    uint32_t       lastToggle_      = 0;
    // bool           isBusy_          = false;



    void init(const char *name, uint8_t pin, uint8_t active_level=HIGH);
    void update();

    void blinking(uint32_t onMs, uint32_t offMs, int8_t cycles=0);
    void blinking_dc(uint32_t period, float duty_cycle, int8_t cycles=0); // con duty_cycle
    void pulse(uint32_t duration);
    void off();
    void off_ifBlinking();
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


} pinController_Struct;


