//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.31.53
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

#include "relay_class.h" // Include l'header della classe

// Costruttore
// pin: Il numero del GPIO a cui è collegato il relè.
// activationLevel: Il livello fisico (HIGH o LOW) che attiva il relè.
RelayManager::RelayManager(int pin, int activationLevel) :
                            relayPin_(pin),
                            pressedLevel_(activationLevel),
                            pulseActive_(false),
                            relayState_(false) // Inizialmente spento
                        {
    pinMode(relayPin_, OUTPUT);
    // Imposta il relè allo stato iniziale (spento)
    digitalWrite(relayPin_, !pressedLevel_);
}

// --- Metodi Pubblici ---

// Imposta lo stato del relè (true = acceso, false = spento)
void RelayManager::setRelay(bool state) {
    relayState_ = state;
    digitalWrite(relayPin_, relayState_ ? pressedLevel_ : !pressedLevel_);
}

// Avvia un pulsetime per il relè
// duration_ms: Durata del pulsetime in millisecondi.
void RelayManager::startPulse(uint32_t duration_ms) {
    pulseStartTime_ = millis();
    pulseDuration_ = duration_ms;
    pulseActive_ = true;
    setRelay(true); // Attiva il relè per il pulsetime
}

// Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
uint32_t RelayManager::getRemainingPulseTime() {
    if (pulseActive_) {
        uint32_t elapsed = millis() - pulseStartTime_;
        if (elapsed < pulseDuration_) {
            return pulseDuration_ - elapsed;
        } else {
            return 0; // Pulsetime scaduto
        }
    }
    return 0;
}

// Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
void RelayManager::update() {
    // Gestione del pulsetime
    if (pulseActive_) {
        if (millis() - pulseStartTime_ >= pulseDuration_) {
            pulseActive_ = false;
            setRelay(false); // Disattiva il relè al termine del pulsetime
            // Puoi aggiungere qui una callback o un flag per notificare la fine del pulsetime
        }
    }
}

// Ritorna lo stato attuale del relè (true = acceso, false = spento)
bool RelayManager::getRelayState() {
    return relayState_;
}

// Ritorna il pin del relè (utile per debug)
int RelayManager::getRelayPin() {
    return relayPin_;
}

