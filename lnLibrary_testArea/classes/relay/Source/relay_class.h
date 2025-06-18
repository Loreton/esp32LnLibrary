//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 20.58.09
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>


class Relay {
    private:
        int             _state;
        unsigned long   _pulseTimeStartTime;
        unsigned long   _pulseTimeRemaining;
        bool            _isPulseTimeActive;
        uint8_t         _pressedLevel; // Livello HIGH o LOW per considerare il relè "attivo"_activeLevel;
        uint8_t         _pin;
        const char*     _pinName;

    public:
        // Costruttore
        Relay(uint8_t pin, String name, uint8_t activeLevel) {
            _pin = pin;
            _pinName = name;
            _pressedLevel = activeLevel;
            _pulseTimeStartTime = 0;
            _pulseTimeRemaining = 0;
            _isPulseTimeActive = false;

        }

        // Funzione per ottenere lo stato corrente del relè
        void state(void) const {
            return _state;
        }


        // Metodo per accendere il LED
        void on(void) {
            digitalWrite(_pin, _state == _pressedLevel ? _pressedLevel : !_pressedLevel);
        }


        // Metodo per spegnere il LED
        void off(void) {
            digitalWrite(_pin, _state == _pressedLevel ? _pressedLevel : !_pressedLevel);
        }
        }

        // Metodo per invertire lo stato del LED
        void toggle(void);

        // Metodo per ottenere lo stato attuale del LED
        bool isActive(void);
};


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
    RelayManager(const PinConfig& config)
        : _pinConfig(config),
          _relayState(LOW), // Inizializza il relè spento
          _pulseTimeStartTime(0),
          _pulseTimeRemaining(0),
          _isPulseTimeActive(false) {
        pinMode(_pinConfig.pinNumber, OUTPUT);
        digitalWrite(_pinConfig.pinNumber, !_pinConfig.pressedLevel); // Assicurati che il relè sia spento all'avvio
        Serial.printf("RelayManager: Pin %d (%s) configurato. Livello attivo: %s\n",
                      _pinConfig.pinNumber, _pinConfig.pinName,
                      _pinConfig.pressedLevel == HIGH ? "HIGH" : "LOW");
    }

    // Funzione per impostare lo stato del relè
    void setRelayState(int state) {
        if (_relayState != state) {
            _relayState = state;
            digitalWrite(_pinConfig.pinNumber, _relayState == _pinConfig.pressedLevel ? _pinConfig.pressedLevel : !_pinConfig.pressedLevel);

        }
    }


    // Funzione per avviare un pulsetime
    void startPulseTime(const PulseTimeConfig& config) {
        _pulseTimeStartTime = millis();
        _pulseTimeRemaining = config.duration;
        _isPulseTimeActive = true;
        if (config.activateRelay) {
            setRelayState(_pinConfig.pressedLevel); // Attiva il relè
        }
        Serial.printf("RelayManager: Pulsetime avviato per %lu ms su %s\n",
                      config.duration, _pinConfig.pinName);
    }


    // Funzione di lettura (da chiamare frequentemente nel loop)
    void update() {
        if (_isPulseTimeActive) {
            unsigned long currentTime = millis();
            unsigned long elapsedTime = currentTime - _pulseTimeStartTime;

            if (elapsedTime >= _pulseTimeRemaining) {
                // Pulsetime completato
                setRelayState(!_pinConfig.pressedLevel); // Spegni il relè
                _isPulseTimeActive = false;
                _pulseTimeRemaining = 0;
                Serial.printf("RelayManager: Pulsetime completato per %s. Relè spento.\n", _pinConfig.pinName);
            } else {
                _pulseTimeRemaining = _pulseTimeStartTime + _pulseTimeRemaining - currentTime;
                // Serial.printf("RelayManager: Pulsetime rimanente per %s: %lu ms\n", _pinConfig.pinName, _pulseTimeRemaining);
            }
        }
    }

    // Funzione per verificare se un pulsetime è attivo
    bool isPulseTimeActive() const {
        return _isPulseTimeActive;
    }

    // Funzione per ottenere il tempo rimanente del pulsetime
    unsigned long getPulseTimeRemaining() const {
        return _pulseTimeRemaining;
    }
