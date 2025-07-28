//
// updated by ...: Loreto Notarantonio
// Date .........: 11-07-2025 19.12.02
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "lnLogger.h"
#include "lnSerialRead.h"
#include "lnGlobalVars.h"
#include "../RelayManager_Struct.h"


// --- Esempio di utilizzo nel file .ino ---

// Crea istanze della classe RelayManager
// Relay 1: Pin 16, si attiva con HIGH
RelayManager_Struct relay1;
RelayManager_Struct relay2;
// Relay 2: Pin 17, si attiva con LOW (esempio di relè a logica inversa)
// Puoi aggiungere altri relè qui, ad esempio RelayManager relay3(18, HIGH);

void setup() {
    Serial.begin(115200);
    delay(1000);
    LOG_INFO("Gestione Multi-Relè (solo Pulsetime) Avviata!");
    relay1.init("relay_01", 16, LOW);
    relay2.init("relay_02", 17, LOW);

    // // Esempi di avvio per relay1
    relay1.startPulse(30000); // Pulsetime di 5 secondi
    // LOG_INFO("Relay 1: Pulsetime avviato.");

    // // Esempi di avvio per relay2
    // relay2.setRelay(true); // Accende il relè 2
    relay2.on(); // Accende il relè 2
    // LOG_INFO("Relay 2: Acceso.");
    // delay(1000);
}


void loop() {
    static bool first_run=true;
    static uint32_t startedMillis;
    uint32_t now, elapsed;
        if (first_run) {
        first_run=false;
        startedMillis=millis();
        LOG_INFO("starting.");
    }

    now = millis() - startedMillis;
    elapsed = now - startedMillis;


    // È FONDAMENTALE chiamare update() per ogni istanza di RelayManager
    // affinché la logica del pulsetime venga elaborata.
    relay1.update();
    relay2.update();
    // Se hai altri relè, aggiungi le loro chiamate update() qui:
    // relay3.update();

    // --- Debugging e Stampa Stato (Non bloccante) ---
    static unsigned long lastPrintTime = 0;
    if (elapsed%2000 <= 10) { // Stampa ogni 2 secondi
        lastPrintTime = millis();
        // LOG_INFO("--- Stato Relè ---");
        // LOG_INFO("[%s] State: %s, Pulsetime Rimanente: %lu ms", relay1.pinID(), str_OnOff[relay1.state()], relay1.getRemainingPulseTime());
        LOG_INFO("[%s] State: %d, Pulsetime Rimanente: %lu ms", relay1.pinID(), relay1.state(), relay1.getRemainingPulseTime());
    }

    // Altre operazioni non bloccanti del tuo programma principale qui
    // ...
    if (elapsed >60000 && elapsed<60100) {
        // relay1.setRelay(false); // Accende il relè 2
        relay2.off();
        waitForEnter();
    }
}


#endif