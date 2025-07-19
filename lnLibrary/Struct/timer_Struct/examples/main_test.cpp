//
// updated by ...: Loreto Notarantonio
// Date .........: 18-07-2025 08.42.10
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
    while(!Serial && millis() < 5000); // Optional: attende la seriale (con timeout)
    delay(1000);
    myLog.begin();
    LOG_INFO("--- ESP32 Millis Timer Test Sketch ---");



    // Avvia il primo timer per 5 secondi con una callback
    myTimer01.start(10000, onMyTimer01Complete);
    myTimer02.start(15000, onMyTimer02Complete);

    // Avvia il secondo timer per 2 secondi senza callback iniziale
    // myTimer02.start(2000);
}




void loop() {
    // *** È FONDAMENTALE chiamare update() per ogni timer nel loop principale ***
    myTimer01.update();
    myTimer02.update();


    // Esempi di query dello stato del timer
    static unsigned long lastQueryTime = 0;
    if (millis() - lastQueryTime >= 1000) { // Ogni secondo, stampa lo stato
        lastQueryTime = millis();

        if (myTimer01.isRunning()) {
            // LOG_INFO("Running, Elapsed: %lu ms, Remaining: %lu ms" , myTimer01.getElapsedTime(), myTimer01.getRemainingTime());
        } else if (myTimer01.hasCompleted()) {
            myTimer01.stop();  // clean timer01
            myTimer02.start(10000);  // start timer 2
        }

        if (myTimer02.isRunning()) {
            // LOG_INFO("Running, Elapsed: %lu ms, Remaining: %lu ms" , myTimer01.getElapsedTime(), myTimer01.getRemainingTime());
        } else if (myTimer02.hasCompleted()) {
            myTimer02.stop();  // clean timer01
            myTimer01.start(10000);  // start timer 1
        }
    }

    // Breve delay per non sovraccaricare la seriale e permettere al monitor di leggerla
    // In un progetto reale, cerca di minimizzare l'uso di delay() o usalo solo per debug.
    delay(10);
}

#endif