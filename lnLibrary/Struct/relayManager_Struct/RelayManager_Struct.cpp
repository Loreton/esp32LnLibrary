//
// updated by ...: Loreto Notarantonio
// Date .........: 19-07-2025 18.55.56
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

#include "lnGlobalVars.h"
#include "lnLogger.h"
#include "lnSetPinID.h"
#include "RelayManager_Struct.h" // Include l'header della classe

// Costruttore
RelayManager_Struct::RelayManager_Struct(void) {}

// pin: Il numero del GPIO a cui è collegato il relè.
// activationLevel: Il livello fisico (HIGH o LOW) che attiva il relè.

void RelayManager_Struct::init(const char *name, uint8_t pin, uint8_t activeLevel) {
        m_pin = pin;
        m_name = name;
        m_activeLevel = activeLevel;
        m_Off = !activeLevel;
        m_On = activeLevel;
        m_pulseActive = false;
        m_relayState = m_Off; // Inizialmente spento

    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

    // Imposta il relè allo stato iniziale (spento)
    pinMode(m_pin, OUTPUT);
    // digitalWrite(m_pin, m_Off);
    setRelay(OFF);
    LOG_DEBUG("%s inizializzato. active level: %s", m_pinID,  m_activeLevel ? "ON" : "OFF");

}


// Avvia un pulsetime per il relè
// duration_ms: Durata del pulsetime in millisecondi.
void RelayManager_Struct::startPulse(uint32_t duration_ms) {
    m_pulseStartTime = millis();
    m_pulseDuration = duration_ms;
    m_pulseActive = true;
    LOG_NOTIFY("[%s] Pulsetime avviato.", m_pinID);
    on();
}

// Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
uint32_t RelayManager_Struct::getRemainingPulseTime() {
    if (m_pulseActive) {
        uint32_t elapsed = millis() - m_pulseStartTime;
        if (elapsed < m_pulseDuration) {
            return m_pulseDuration - elapsed;
        } else {
            return 0; // Pulsetime scaduto
            LOG_NOTIFY("[%s] Pulsetime ended.", m_pinID);
        }
    }
    return 0;
}

// Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
void RelayManager_Struct::update() {
    if (m_pulseActive) {
        if (millis() - m_pulseStartTime >= m_pulseDuration) {
            m_pulseActive = false;
            off();
            // Puoi aggiungere qui una callback o un flag per notificare la fine del pulsetime
        }
    }
}

// Ritorna il pin del relè (utile per debug)
// int RelayManager_Struct::pin() {
//     return m_pin;
// }

// Ritorna il pin del relè (utile per debug)
const char * RelayManager_Struct::pinID() {
    return m_pinID;
}

// Ritorna lo stato attuale del relè (true = acceso, false = spento)
bool RelayManager_Struct::state() {
    // uint8_t state=digitalRead(m_pin);
    uint8_t state = (digitalRead(m_pin) == m_activeLevel) ? true : false;

    LOG_DEBUG("state: %s", state ? "ON" : "OFF");
    return state;
}


// Imposta lo stato del relè (true = acceso, false = spento)
void RelayManager_Struct::setRelay(bool req_state) {
    m_relayState = req_state;
    digitalWrite(m_pin, m_relayState ? m_activeLevel : !m_activeLevel);
    LOG_DEBUG("[%s] - %s", m_pinID, str_OnOff[m_relayState]);
}

void RelayManager_Struct::on() {
    setRelay(true);
    // digitalWrite(m_pin, m_On);
}

void RelayManager_Struct::off() {
    setRelay(false);
    // digitalWrite(m_pin, m_Off);
    // LOG_DEBUG("[%s] - OFF", m_pinID);
}

void RelayManager_Struct::toggle() {
    setRelay(!m_relayState);
    // m_relayState = digitalRead(m_pin) ? m_activeLevel : !m_activeLevel

    // digitalWrite(m_pin, m_relayState ? m_activeLevel : !m_activeLevel);
    // LOG_DEBUG("[%s] - OFF", m_pinID);
}

