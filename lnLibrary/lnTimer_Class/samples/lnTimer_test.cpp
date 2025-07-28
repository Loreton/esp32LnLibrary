#include "lnTimer_Class.h" // Includi il nuovo header
#include "lnLogger_Class.h"    // E il tuo logger

MillisTimer myTimer("MyFirstTimer"); // Crea un'istanza della classe


void myTimerCallBack() {
    LOG_INFO("Callback executed! Timer completed.");
}





void setup() {
  Serial.begin(115200);
  // lnLogger.setLogLevel(LOG_LEVEL_DEBUG); // Esempio di inizializzazione del logger
  delay(1000);
  lnLog.init();

  // Avvia il timer per 5 secondi con una lambda function come callback
  // myTimer.start(5000, [](){
  //   LOG_INFO("Callback executed! Timer completed.");
  // });

  myTimer.start(5000, myTimerCallBack); // Avvia il timer per 5 secondi con una lambda function come callback
}

void loop() {
  myTimer.update(); // Aggiorna lo stato del timer ad ogni loop

  if (myTimer.hasCompleted()) {
    // Questo blocco verrà eseguito una sola volta dopo che il timer è completato e la callback (se presente) è stata chiamata.
    // L'istanza myTimer non è più isCompleted a meno che non venga riavviata con start().
  }

  // Puoi anche interrogare lo stato del timer in qualsiasi momento:
  if (myTimer.isRunning()) {
    LOG_NOTIFY("Timer running. Remaining: %lu ms", myTimer.getRemainingTime());
  }
  delay(10);
}