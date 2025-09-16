


#include "lnTimer_Class.h" // Includi il nuovo header
#include "lnLogger_Class.h"    // E il tuo logger

// MillisTimer myTimer("MyFirstTimer"); // Crea un'istanza della classe
MillisTimer myTimer; // Crea un'istanza della classe
MillisTimer myTimerCB; // Crea un'istanza della classe
// MillisTimer myTimer2("MySecondTimer"); // Crea un'istanza della classe


// void myTimerCallBack() {
//     LOG_INFO("Callback executed! Timer completed.");
//     delay(2000);
//     // myTimer.start(5000, myTimerCallBack); // Avvia il timer per 5 secondi con una lambda function come callback
//     myTimer.restart(); // Avvia il timer per 5 secondi con una lambda function come callback
// }



void myTimerCallBack(MillisTimer *t) {
    LOG_INFO("Callback executed! Timer completed for timer: %s", t->name());
    delay(2000);
    t->restart(); // riAvvia il timer
    // t->restart(2000); // riAvvia il timer
}





void setup() {
    Serial.begin(115200);
    // lnLogger.setLogLevel(LOG_LEVEL_DEBUG); // Esempio di inizializzazione del logger
    delay(1000);
    lnLog.init();
    myTimer.init("myTimer", 5000); // Avvia il timer per 5 secondi con una lambda function come callback
    myTimerCB.init("myTimerCB", 5000, myTimerCallBack); // Avvia il timer per 5 secondi con una lambda function come callback

    // Avvia il timer
    myTimer.start(4000); // Avvia il timer per 5 secondi con una lambda function come callback
    myTimerCB.start(3000); // Avvia il timer per 5 secondi con una lambda function come callback
}

void loop() {
    myTimer.update(); // Aggiorna lo stato del timer ad ogni loop
    myTimerCB.update(); // Aggiorna lo stato del timer ad ogni loop

    if (!myTimer.hasCallBack() && myTimer.hasExpired()) {
        LOG_INFO("myTimer executed! Timer completed for timer: %s", myTimer.name());
        delay(2000);
        myTimer.restart(); // riAvvia il timer
    }


    // questo non dovrebbe essere eseguito....
    if (!myTimerCB.hasCallBack() && myTimerCB.hasExpired()) {
        LOG_INFO("myTimerCB executed! Timer completed for timer: %s", myTimerCB.name());
        delay(2000);
        myTimerCB.restart(); // riAvvia il timer
    }

    // Puoi anche interrogare lo stato del timer in qualsiasi momento:
    // if (myTimer.isRunning()) {
    //     LOG_NOTIFY("Timer running. Remaining: %lu ms", myTimer.getRemainingTime());
    // }
    delay(10);
}