//
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 09.37.31
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type

// --- Definizione della Struttura del LED ---

typedef void (*pinControllerCallback)(struct PinController_Struct* self);  // se volessi usare una callback
/** esempio:
void myLedControllerHandler(PinController_Struct* led) {
    Serial.printf("ledController Callback: %s\n", led->_pinID);
}
*/


typedef struct PinController_Struct {
    uint8_t        m_pin;
    const   char*  m_name;
    char           m_pinID[21];  // conterrà [pin:%02d.%-15s] p->pin, p->name,

    uint8_t        m_onLevel       = HIGH;
    uint8_t        m_on             = HIGH;
    uint8_t        m_off            = LOW;

    // Blinking parameters
    uint32_t       m_onTime     = 500;
    uint32_t       m_offTime    = 500;
    bool           m_blinking       = false;
    int8_t         m_numCycles      = 0;
    int8_t         m_temporaryBlinking      = false;

    // Fixed on parameters
    bool           m_fixed          = true;
    bool           m_pulseOn        = false;
    uint32_t       m_pulseOnStart   = 0;
    uint32_t       m_pulseOnDuration= 0;

    // Internals
    bool           m_ledState       = false;
    uint32_t       m_lastToggle     = 0;
    // bool           m_isBusy         = false;



    void init(const char *name, uint8_t pin, uint8_t active_level=HIGH);
    void updateStatus();

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


} PinController_Struct;


