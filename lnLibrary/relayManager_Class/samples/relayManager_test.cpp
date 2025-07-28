//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 17.05.25
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#ifdef __ln_MODULE_DEBUG_TEST__
#include "relayManager_Class.h"

#define magnetoTermicoRelay_pin     17  // OUTPUT


RelayManager_Class myRelay; // Creazione di un'istanza della classe


void setup() {
    Serial.begin(115200);
    // Inizializza il relè collegato al pin 2, attivo a HIGH
    myRelay.init("PortaGarage", magnetoTermicoRelay_pin, HIGH);
}

void loop() {
    myRelay.update(); // Chiamata essenziale per gestire il pulsetime

    // Esempio di utilizzo:
    if (Serial.available()) {
        char command = Serial.read();
        if (command == 'o') {
            myRelay.on();
            Serial.println("Relè ON");
        } else if (command == 'f') {
            myRelay.off();
            Serial.println("Relè OFF");
        } else if (command == 't') {
            myRelay.toggle();
            Serial.println("Relè TOGGLED");
        } else if (command == 'p') {
            myRelay.startPulse(2000); // Pulsetime di 2 secondi
            Serial.println("Relè Pulse 2s");
        }
    }

    // Monitoraggio dello stato
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime > 1000) {
        Serial.printf("[%s] State: %s, Active: %s, Pulse Remaining: %lu ms\n",
                      myRelay.pinID(),
                      myRelay.state() ? "ON" : "OFF",
                      myRelay.isActive() ? "YES" : "NO",
                      myRelay.getRemainingPulseTime());
        lastPrintTime = millis();
    }
}
#endif