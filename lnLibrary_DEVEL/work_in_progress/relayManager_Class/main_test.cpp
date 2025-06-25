//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 12.20.47
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@SerialRead.h"
#include "relay_class.h"


// --- Esempio di utilizzo nel file .ino ---

// Crea istanze della classe RelayManager
// Relay 1: Pin 16, si attiva con HIGH
RelayManager relay1(16, LOW);
// Relay 2: Pin 17, si attiva con LOW (esempio di relè a logica inversa)
RelayManager relay2(17, LOW);
// Puoi aggiungere altri relè qui, ad esempio RelayManager relay3(18, HIGH);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Gestione Multi-Relè (solo Pulsetime) Avviata!");

    // Esempi di avvio per relay1
    relay1.startPulse(30000); // Pulsetime di 5 secondi
    Serial.println("Relay 1: Pulsetime avviato.");

    // Esempi di avvio per relay2
    relay2.setRelay(true); // Accende il relè 2
    Serial.println("Relay 2: Acceso.");
    delay(1000);
}

void loop() {
    static bool first_run=true;
    static uint32_t startedMillis;
    uint32_t now, elapsed;
        if (first_run) {
        first_run=false;
        startedMillis=millis();
    }

    now = millis() - startedMillis;

    // È FONDAMENTALE chiamare update() per ogni istanza di RelayManager
    // affinché la logica del pulsetime venga elaborata.
    relay1.update();
    relay2.update();
    // Se hai altri relè, aggiungi le loro chiamate update() qui:
    // relay3.update();

    // --- Debugging e Stampa Stato (Non bloccante) ---
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime >= 2000) { // Stampa ogni 2 secondi
        lastPrintTime = millis();
        Serial.println("--- Stato Relè ---");
        Serial.printf("Relay 1 (Pin %d): Stato: %s, Pulsetime Rimanente: %lu ms\n",
                      relay1.getRelayPin(), relay1.getRelayState() ? "ACCESO" : "SPENTO",
                      relay1.getRemainingPulseTime());
        Serial.printf("Relay 2 (Pin %d): Stato: %s, Pulsetime Rimanente: %lu ms\n",
                      relay2.getRelayPin(), relay2.getRelayState() ? "ACCESO" : "SPENTO",
                      relay2.getRemainingPulseTime());
    }

    // Altre operazioni non bloccanti del tuo programma principale qui
    // ...
    elapsed = now - startedMillis;
    if (elapsed >60000 && elapsed<60100) {
        relay1.setRelay(false); // Accende il relè 2
        Serial.println("Relay 2: Spento.");
        waitForEnter();
    }
}