#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include <Arduino.h>

class RelayManager {
public:
    // Struttura per i parametri di configurazione del pin
    struct PinConfig {
        int pinNumber;
        const char* pinName;
        int pressedLevel; // Livello HIGH o LOW per considerare il relè "attivo"
    };

    // Struttura per la gestione del pulsetime
    struct PulseTimeConfig {
        unsigned long duration; // Durata del pulsetime in millisecondi
        bool activateRelay;     // true per attivare il relè all'inizio del pulsetime
    };

    // Costruttore
    RelayManager(const PinConfig& config);

    // Funzione per impostare lo stato del relè
    void setRelayState(int state);

    // Funzione per ottenere lo stato corrente del relè
    int getRelayState() const;

    // Funzione per avviare un pulsetime
    void startPulseTime(const PulseTimeConfig& config);

    // Funzione di lettura (da chiamare frequentemente nel loop)
    void update();

    // Funzione per verificare se un pulsetime è attivo
    bool isPulseTimeActive() const;

    // Funzione per ottenere il tempo rimanente del pulsetime
    unsigned long getPulseTimeRemaining() const;

private:
    PinConfig _pinConfig;
    int _relayState;
    unsigned long _pulseTimeStartTime;
    unsigned long _pulseTimeRemaining;
    bool _isPulseTimeActive;
};

#endif // RELAY_MANAGER_H