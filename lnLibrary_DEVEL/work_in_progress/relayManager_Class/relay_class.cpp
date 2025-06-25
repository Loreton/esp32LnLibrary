//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 12.10.34
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

#include "relay_class.h" // Include l'header della classe

// Costruttore
// pin: Il numero del GPIO a cui è collegato il relè.
// activationLevel: Il livello fisico (HIGH o LOW) che attiva il relè.
RelayManager::RelayManager(int pin, int activationLevel) :
    relayPin(pin),
    pressedLevel(activationLevel),
    pulseActive(false),
    relayState(false) // Inizialmente spento
{
    pinMode(relayPin, OUTPUT);
    // Imposta il relè allo stato iniziale (spento)
    digitalWrite(relayPin, !pressedLevel);
}

// --- Metodi Pubblici ---

// Imposta lo stato del relè (true = acceso, false = spento)
void RelayManager::setRelay(bool state) {
    relayState = state;
    digitalWrite(relayPin, relayState ? pressedLevel : !pressedLevel);
}

// Avvia un pulsetime per il relè
// duration_ms: Durata del pulsetime in millisecondi.
void RelayManager::startPulse(unsigned long duration_ms) {
    pulseStartTime = millis();
    pulseDuration = duration_ms;
    pulseActive = true;
    setRelay(true); // Attiva il relè per il pulsetime
}

// Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
unsigned long RelayManager::getRemainingPulseTime() {
    if (pulseActive) {
        unsigned long elapsed = millis() - pulseStartTime;
        if (elapsed < pulseDuration) {
            return pulseDuration - elapsed;
        } else {
            return 0; // Pulsetime scaduto
        }
    }
    return 0;
}

// Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
void RelayManager::update() {
    // Gestione del pulsetime
    if (pulseActive) {
        if (millis() - pulseStartTime >= pulseDuration) {
            pulseActive = false;
            setRelay(false); // Disattiva il relè al termine del pulsetime
            // Puoi aggiungere qui una callback o un flag per notificare la fine del pulsetime
        }
    }
}

// Ritorna lo stato attuale del relè (true = acceso, false = spento)
bool RelayManager::getRelayState() {
    return relayState;
}

// Ritorna il pin del relè (utile per debug)
int RelayManager::getRelayPin() {
    return relayPin;
}

