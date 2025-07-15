//
// updated by ...: Loreto Notarantonio
// Date .........: 15-07-2025 11.56.56
//
// Gemini: https://g.co/gemini/share/7448d628a2a6

#if __ln_MODULE_DEBUG_TEST__
#include <Arduino.h>

#include "lnLogger.h" // Includi il tuo file header personalizzato
#include "../millisTimer_Struct.h" // Includi il tuo file header personalizzato



// Crea istanze della tua struttura MillisTimer
MillisTimer myTimer01("Timer-01");
MillisTimer myTimer02("Timer-02");


// --------------------------------
// --- Funzioni di Callback ---
// --------------------------------
// Queste funzioni verranno chiamate quando i rispettivi timer si completano.
void onMyTimer01Complete() {
    LOG_INFO("--- [CALLBACK] Timer-01 has completed! ---");
    // Esegui qui le azioni specifiche per myTimer01
}

void onMyTimer02Complete() {
  LOG_INFO("--- [CALLBACK] Timer-02 finished! ---");
    // Esegui qui le azioni specifiche per myTimer02
    // Esempio: riavvia myTimer02 per un ciclo infinito con una callback diversa
    // myTimer02.start(2500, onMyTimer02Complete);
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    LOG_INFO("\n--- ESP32 Millis Timer Test Sketch ---");


    // Avvia il primo timer per 5 secondi con una callback
    myTimer01.start(20000, onMyTimer01Complete);

    // Avvia il secondo timer per 2 secondi senza callback iniziale
    // myTimer02.start(2000);
}

void loop() {
    // *** È FONDAMENTALE chiamare check() per ogni timer nel loop principale ***
    if (myTimer01.check()) {
        // Questo blocco si esegue SOLO la prima volta che myTimer01 completa
        // LOG_INFO("Main loop detected MyTimer completion!");
        // Esempio: riavvia myTimer01 dopo 3 secondi
        myTimer01.start(30000, onMyTimer01Complete);
    }

    // Controlla myTimer02
    // Qui possiamo anche aggiungere una callback se il timer si completa senza averne una
    // if (myTimer02.check()) {
        // LOG_INFO("Main loop detected AnotherTimer completion!");
    //     if (!myTimer02.m_onCompleteCallback) { // Se non aveva una callback, la aggiungiamo per il riavvio
    //         LOG_INFO("AnotherTimer completed without a set callback. Restarting with callback.");
    //         myTimer02.start(4000, onMyTimer02Complete); // Riavvia con la callback
    //     }
    // }

    // Esempi di query dello stato del timer
    static unsigned long lastQueryTime = 0;
    if (millis() - lastQueryTime >= 1000) { // Ogni secondo, stampa lo stato
        lastQueryTime = millis();

        LOG_INFO("MyTimer Status: ");
        if (myTimer01.isTimerRunning()) {
            // LOG_INFO("Running, Elapsed: %lu ms, Remaining: %lu ms" , myTimer01.getElapsedTime(), myTimer01.getRemainingTime());
        } else if (myTimer01.hasCompleted()) {
            // LOG_INFO("Completed.");
        } else {
            LOG_INFO("Stopped/Not Started.");
        }

      // LOG_INFO("AnotherTimer Status: ");
      // if (myTimer02.isTimerRunning()) {
      //       // LOG_INFO("Running, Elapsed: %lu ms, Remaining: %lu ms" , myTimer01.getElapsedTime(), myTimer01.getRemainingTime());
      //   } else if (myTimer02.hasCompleted()) {
      //       // LOG_INFO("Completed.");
      //   } else {
      //       LOG_INFO("Stopped/Not Started.");
      // }
    }

    // Breve delay per non sovraccaricare la seriale e permettere al monitor di leggerla
    // In un progetto reale, cerca di minimizzare l'uso di delay() o usalo solo per debug.
    delay(10);
}

#endif