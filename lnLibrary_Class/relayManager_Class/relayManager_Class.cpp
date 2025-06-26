//
// updated by ...: Loreto Notarantonio
// Date .........: 26-06-2025 17.17.28
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

#include "relayManager_Class.h" // Include l'header della classe

// Costruttore
// pin: Il numero del GPIO a cui è collegato il relè.
// activationLevel: Il livello fisico (HIGH o LOW) che attiva il relè.
RelayManager::RelayManager(int pin, int activationLevel) :
                            m_relayPin(pin),
                            m_pressedLevel(activationLevel),
                            m_pulseActive(false),
                            m_relayState(false) // Inizialmente spento
                        {
    pinMode(m_relayPin, OUTPUT);
    // Imposta il relè allo stato iniziale (spento)
    digitalWrite(m_relayPin, !m_pressedLevel);
}

// --- Metodi Pubblici ---

// Imposta lo stato del relè (true = acceso, false = spento)
void RelayManager::setRelay(bool state) {
    m_relayState = state;
    digitalWrite(m_relayPin, m_relayState ? m_pressedLevel : !m_pressedLevel);
}

// Avvia un pulsetime per il relè
// duration_ms: Durata del pulsetime in millisecondi.
void RelayManager::startPulse(uint32_t duration_ms) {
    m_pulseStartTime = millis();
    m_pulseDuration = duration_ms;
    m_pulseActive = true;
    setRelay(true); // Attiva il relè per il pulsetime
}

// Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
uint32_t RelayManager::getRemainingPulseTime() {
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
void RelayManager::update() {
    // Gestione del pulsetime
    if (m_pulseActive) {
        if (millis() - m_pulseStartTime >= m_pulseDuration) {
            m_pulseActive = false;
            setRelay(false); // Disattiva il relè al termine del pulsetime
            // Puoi aggiungere qui una callback o un flag per notificare la fine del pulsetime
        }
    }
}

// Ritorna lo stato attuale del relè (true = acceso, false = spento)
bool RelayManager::getRelayState() {
    return m_relayState;
}

// Ritorna il pin del relè (utile per debug)
int RelayManager::getRelayPin() {
    return m_relayPin;
}

