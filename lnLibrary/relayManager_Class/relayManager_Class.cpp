//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 17.17.17
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include <lnSetPinID.h>

#include <LnTime_Class.h>
extern LnTime_Class lnTime;

#include "relayManager_Class.h" // Include l'header della classe



// Costruttore
RelayManager_Class::RelayManager_Class() {
    // Inizializza i membri con valori predefiniti
    m_pin = 99;
    m_name = nullptr;
    m_pinID[0] = '\0'; // Inizializza la stringa vuota
    m_activeLevel = LOW;
    m_On = LOW;
    m_Off = LOW;
    m_pulseStartTime = 0;
    m_pulseDuration = 0;
    m_pulseActive = false;
    m_relayState = false;
}

// Inizializza il relè.
void RelayManager_Class::init(const char *name, uint8_t pin, uint8_t activeLevel) {
    m_pin = pin;
    m_name = name;
    m_activeLevel = activeLevel;
    m_Off = !activeLevel; // Il livello OFF è l'opposto del livello attivo
    m_On = activeLevel;   // Il livello ON è il livello attivo
    m_pulseActive = false;

    // Utilizza la funzione setPinID per creare l'ID del pin
    setPinID(m_pinID, sizeof(m_pinID) - 1, m_name, m_pin);

    // Imposta il pin come OUTPUT e lo porta allo stato OFF iniziale
    pinMode(m_pin, OUTPUT);
    // digitalWrite(m_pin, m_Off);
    updateState(m_Off); // set default at start time


    LOG_TRACE("[%s] initialized. active level: %s", m_pinID, (m_activeLevel == HIGH ? "HIGH" : "LOW"));
}



void RelayManager_Class::updateState(uint8_t reqState) {
    if (reqState == m_On || reqState == m_Off ) {
        digitalWrite(m_pin, reqState);
        delay(20);
    }
    m_relayState = (digitalRead(m_pin) == m_activeLevel) ? m_On : m_Off;
};




// #####################################################
// #
// #####################################################
void RelayManager_Class::on(uint32_t duration_ms) {
    if (m_relayState != m_On) {
        updateState(m_On); // turnON and update m_relayState
        if (!m_pulseActive) {
            startPulse(duration_ms);
        }
    } else {
        LOG_NOTIFY("[%s] already ON", m_pinID);
    }
}





// #####################################################
// #
// #####################################################
void RelayManager_Class::off() {
    m_pulseActive = false;
    m_pulseDuration = 0;
    updateState(m_Off); // turnOFF and update m_relayState
}



// #####################################################
// #
// #####################################################
void RelayManager_Class::toggle(uint32_t duration_ms) {
    m_relayState == m_On ? off() : on(duration_ms) ; // Inverte lo stato logico e lo applica
}





// #####################################################
// # Avvia un pulsetime per il relè
// #####################################################
void RelayManager_Class::startPulse(uint32_t duration_ms) {
    const char *hmsTime;

    if (!m_pulseActive) {
        m_pulseStartTime = millis();
        m_pulseDuration = (duration_ms != 0) ? duration_ms : m_defaultPulseTime;
        m_pulseActive = true;

        hmsTime = lnTime.msecToHMS(m_pulseDuration);
        LOG_NOTIFY("[%s] Pulsetime started! [%s]", m_pinID, hmsTime);
        if (m_relayState != m_On) {
            on();
        }  // Accende il relè all'avvio del pulsetime
    }
    #if LOG_MODULE_LEVEL >= LOG_LEVEL_TRACE
    else {
        hmsTime = lnTime.msecToHMS(m_pulseDuration - (millis() - m_pulseStartTime));
        LOG_TRACE("[%s] Pulsetime already active [%s]", m_pinID, hmsTime);
    }
    #endif
}





// #####################################################
// # Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
// #####################################################
uint32_t RelayManager_Class::remainingPulseTime() {
    uint32_t remaining = 0;

    if (m_pulseActive) {
        uint32_t elapsed = millis() - m_pulseStartTime;
        remaining =  (elapsed < m_pulseDuration) ? m_pulseDuration - elapsed : 0;
    }


    #ifdef XXXXXX__
    if (elapsed % 10000 < 100 && remaining != 0) { // ogni 10 secondo
        LOG_SPEC("");
        LOG_SPEC("m_pulseActive:      %d", m_pulseActve);
        LOG_SPEC("m_pulseDuration:    %lu", m_pulseDuration);
        LOG_SPEC("m_pulseStartTime:   %lu", m_pulseStartTime);
        LOG_SPEC("elapsed:            %lu", elapsed);
        LOG_SPEC("remaining:          %lu", remaining);
        LOG_SPEC("");
    }
    #endif


    return remaining;
}

// #####################################################
// # Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
// #####################################################
void RelayManager_Class::update() {
    updateState();
    if (m_pulseActive && remainingPulseTime() == 0) {
        off(); // Spegne il relè alla fine del pulsetime
        LOG_NOTIFY("[%s] Pulsetime terminato.", m_pinID);
    }
}


// #####################################################
// # Ritorna vero se il relè è fisicamente attivo (a livello di pin)
// #####################################################
bool RelayManager_Class::isActive() const {
    return digitalRead(m_pin) == m_activeLevel;
}

