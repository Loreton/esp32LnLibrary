//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 14.20.00
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

#include "lnGlobalVars.h" // Assicurati che questo file esista e sia configurato
#include "lnSetPinID.h"   // Assicurati che questo file esista e sia configurato
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
    m_relayState = false; // Inizialmente spento

    // Utilizza la funzione setPinID per creare l'ID del pin
    setPinID(m_pinID, sizeof(m_pinID) - 1, m_name, m_pin);

    // Imposta il pin come OUTPUT e lo porta allo stato OFF iniziale
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, m_Off);
    setRelay(false); // Assicura che lo stato logico e fisico siano coerenti

    LOG_TRACE("[%s] initialized. active level: %s", m_pinID, (m_activeLevel == HIGH ? "HIGH" : "LOW"));
}

// Imposta lo stato fisico del relè sul pin
void RelayManager_Class::setRelay(bool req_state) {
    m_relayState = req_state; // Aggiorna lo stato logico interno

    // Scrive il livello appropriato sul pin
    digitalWrite(m_pin, m_relayState ? m_On : m_Off);

    // Se il relè viene spento, disattiva qualsiasi pulsetime in corso
    if (!m_relayState) {
        m_pulseActive = false;
    }
    LOG_DEBUG("[%s] - %s", m_pinID, (m_relayState ? "ON" : "OFF"));
}

void RelayManager_Class::on() {
    setRelay(true);
}

void RelayManager_Class::off() {
    setRelay(false);
}

void RelayManager_Class::toggle() {
    setRelay(!m_relayState); // Inverte lo stato logico e lo applica
}

// Avvia un pulsetime per il relè
void RelayManager_Class::startPulse(uint32_t duration_ms) {
    if (!m_pulseActive) {
        m_pulseStartTime = millis();
        m_pulseDuration = duration_ms;
        m_pulseActive = true;
        lnLog.timeStamp(m_timeBUFFER, m_timeBUFFER_Len, duration_ms, true);
        LOG_TRACE("[%s] Pulsetime avviato per %s", m_pinID, m_timeBUFFER, true);
        on(); // Accende il relè all'avvio del pulsetime
    } else {
        lnLog.timeStamp(m_timeBUFFER, m_timeBUFFER_Len, m_pulseDuration - (millis() - m_pulseStartTime), true);
        LOG_WARNING("[%s] Pulsetime già attivo per %lu ms (skipping...)", m_pinID, m_timeBUFFER);
    }
}

// Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
uint32_t RelayManager_Class::getRemainingPulseTime() {
    if (m_pulseActive) {
        uint32_t elapsed = millis() - m_pulseStartTime;
        if (elapsed < m_pulseDuration) {
            return m_pulseDuration - elapsed;
        } else {
            return 0; // Pulsetime scaduto
        }
    }
    return 0;
}

// Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
void RelayManager_Class::update() {
    if (m_pulseActive) {
        if (millis() - m_pulseStartTime >= m_pulseDuration) {
            m_pulseActive = false;
            off(); // Spegne il relè alla fine del pulsetime
            LOG_NOTIFY("[%s] Pulsetime terminato.", m_pinID);
        }
    }
}

// Ritorna lo stato logico attuale del relè (true = acceso, false = spento)
bool RelayManager_Class::state() const {
    return m_relayState;
}

// Ritorna vero se il relè è fisicamente attivo (a livello di pin)
bool RelayManager_Class::isActive() const {
    return digitalRead(m_pin) == m_activeLevel;
}

// Ritorna l'ID del pin (utile per debug)
// const char *RelayManager_Class::pinID() const {
//     return m_pinID;
// }