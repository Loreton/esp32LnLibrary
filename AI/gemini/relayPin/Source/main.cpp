//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 16.25.30
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "RelayManager.h" // Assicurati che RelayManager.h sia nella stessa cartella o in una libreria

#define RELAY_PIN 16

// Creazione di un'istanza di RelayManager
RelayManager::PinConfig relay1Config = {RELAY_PIN, "Relay1", LOW}; // Il relè è attivo quando il pin è LOW
RelayManager relay1(relay1Config);

unsigned long lastToggleTime = 0;
const unsigned long toggleInterval = 5000; // 5 secondi

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.printf("\n--- ESP32 Relay Management Example ---\n");

    // Puoi impostare lo stato iniziale del relè
    relay1.setRelayState(LOW); // Assicurati che sia spento all'inizio
}

void loop() {
    // Chiamare regolarmente la funzione update per gestire il pulsetime
    relay1.update();

    unsigned long currentTime = millis();

    // Esempio di utilizzo: attiva il relè per 2 secondi ogni 5 secondi
    if (currentTime - lastToggleTime >= toggleInterval) {
        lastToggleTime = currentTime;

        if (!relay1.isPulseTimeActive()) {
            RelayManager::PulseTimeConfig pulseConfig = {2000, true}; // 2 secondi, attiva il relè
            relay1.startPulseTime(pulseConfig);
        }
    }

    // Puoi anche leggere lo stato del relè
    // Serial.printf("Stato corrente Relay1: %s\n", relay1.getRelayState() == HIGH ? "ON" : "OFF");
    // Serial.printf("Pulsetime attivo Relay1: %s, Rimanente: %lu ms\n", relay1.isPulseTimeActive() ? "SI" : "NO", relay1.getPulseTimeRemaining());

    delay(10); // Piccolo ritardo per non sovraccaricare la CPU, ma il codice rimane non bloccante
}