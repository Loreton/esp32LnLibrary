//
// updated by ...: Loreto Notarantonio
// Date .........: 22-06-2025 16.59.04
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
// #include <Arduino.h>    // in testa anche per le definizioni dei type

// --- Definizione della Struttura del LED ---
typedef struct led_sClass_t {
    int             _pin;            // Pin GPIO a cui è collegato il LED
    bool            _state;          // Stato corrente del LED (true = ON, false = OFF)
    unsigned long   _previousMillis; // Ultimo istante in cui lo stato del LED è stato cambiato
    unsigned long   _onTime;         // Tempo di accensione del LED (in millisecondi)
    unsigned long   _offTime;        // Tempo di spegnimento del LED (in millisecondi)
    float           _dutyCycle;      // valore della percentuale 0-100
    uint8_t         _activeLevel;    // per gestire eventuale logica inversa
    bool            _on;             // True se si usa logica inversa (LOW = ON, HIGH = OFF)
    bool            _off;            // True se si usa logica inversa (LOW = ON, HIGH = OFF)

    void init(int ledPin, unsigned long period, uint8_t duty_cycle, bool inverseLog);
    void init(int ledPin, unsigned long onTime, unsigned long offTime, bool active_level);
    void update();
    void setState(bool turnOn);
    void setDutyCycle(unsigned long period, uint8_t duty_cycle);
} led_sClass_t;


