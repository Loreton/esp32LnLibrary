//
// updated by ...: Loreto Notarantonio
// Date .........: 20-06-2025 07.56.54
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
// #include <Arduino.h>    // in testa anche per le definizioni dei type

// --- Definizione della Struttura del LED ---
typedef struct LED_t {
    int pin;                // Pin GPIO a cui è collegato il LED
    bool state;             // Stato corrente del LED (true = ON, false = OFF)
    unsigned long previousMillis; // Ultimo istante in cui lo stato del LED è stato cambiato
    unsigned long onTime;   // Tempo di accensione del LED (in millisecondi)
    unsigned long offTime;  // Tempo di spegnimento del LED (in millisecondi)
    float dutyCycle;      // valore della percentuale 0-100
    uint8_t activeLevel;      // per gestire eventuale logica inversa
    bool on;                // True se si usa logica inversa (LOW = ON, HIGH = OFF)
    bool off;               // True se si usa logica inversa (LOW = ON, HIGH = OFF)
} LED_t;

// ========== FUNZIONI ESTERNE ==========

void LED_init(LED_t *ledPtr, int ledPin, unsigned long period, uint8_t duty_cycle, bool inverseLog);
void LED_init(LED_t *ledPtr, int ledPin, unsigned long onTime, unsigned long offTime, bool active_level);
void LED_update(LED_t *ledPtr);
void LED_setState(LED_t *ledPtr, bool turnOn);
void LED_setDutyCycle(LED_t *ledPtr, unsigned long period, uint8_t duty_cycle);