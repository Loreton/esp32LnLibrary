//
// updated by ...: Loreto Notarantonio
// Date .........: 13-07-2025 14.23.43
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type

// --- Definizione della Struttura del LED ---

typedef void (*pinControllerCallback)(struct LedController_Struct* self);  // se volessi usare una callback
/** esempio:
void myLedControllerHandler(LedController_Struct* led) {
    Serial.printf("ledController Callback: %s\n", led->_pinID);
}
*/


typedef struct LedController_Struct {
    uint8_t        m_pin;
    const   char*  m_name;
    char           m_pinID[21];  // conterrà [pin:%02d.%-15s] p->pin, p->name,

    uint8_t        m_activeLevel    = HIGH;
    uint8_t        m_on             = HIGH;
    uint8_t        m_off            = LOW;

    // uint8_t         m_activeLevel=LOW; //              Livello che attiva il relè (HIGH o LOW)
    // uint8_t         m_On=LOW; //              Livello che attiva il relè (HIGH o LOW)
    // uint8_t         m_Off=LOW; //              Livello che attiva il relè (HIGH o LOW)

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

    LedController_Struct(void); // costructor

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


} LedController_Struct;


