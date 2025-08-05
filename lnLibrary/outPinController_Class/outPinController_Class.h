//
// updated by ...: Loreto Notarantonio
// Date .........: 05-08-2025 18.08.29
//
#pragma once
#include <Arduino.h> // in testa anche per le definizioni dei type

// --- Definizione della Classe del LED ---

// Se volessi mantenere una callback, la definizione dovrebbe essere adattata:
// typedef void (*pinControllerCallback)(class outPinController_Class* self);
// Anche se in un contesto di classe, le callback sono meno comuni,
// potresti considerare l'uso di puntatori a funzione membro o std::function.

class outPinController_Class {
private:
    uint8_t   m_pin;
    const char* m_name;
    char      m_pinID[21]; // conterrà [pin:%02d.%-15s] p->pin, p->name,

    uint8_t   m_activeLevel = HIGH;
    uint8_t   m_on = HIGH;
    uint8_t   m_off = LOW;

    // Blinking parameters
    uint32_t  m_onTime = 500;
    uint32_t  m_offTime = 500;
    bool      m_blinking = false;
    int8_t    m_numCycles = 0;
    bool      m_temporaryBlinking = false;

    // Fixed on parameters
    bool      m_fixed = true;
    bool      m_pulseOn = false;
    uint32_t  m_pulseOnStart = 0;
    uint32_t  m_pulseOnDuration = 0;

    // Internals
    bool      m_ledState = false;
    uint32_t  m_lastToggle = 0;

    // Funzione interna per impostare lo stato del LED
    void _set(uint8_t state);

public:
    outPinController_Class(void); // Costruttore

    void init(const char *name, uint8_t pin, uint8_t active_level = HIGH);
    void update();

    void blinking(uint32_t onMs, uint32_t offMs, int8_t cycles = 0);
    void blinking_dutyCycle(uint32_t period, float duty_cycle, int8_t cycles = 0); // con duty_cycle
    void pulse(uint32_t duration);

    // Metodi inline per accendere e spegnere
    inline void on() { _set(true); };
    inline void off() { _set(false); };
    inline const char* pinID(void) const { return m_pinID; }
    bool isPlayingSomething(void);



    void reset();
};