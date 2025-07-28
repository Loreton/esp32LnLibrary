/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 15.34.54
*/



#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>


#define __I_AM_MAIN_CPP__
#include "lnGlobalVars.h"
#include "outPinController_Class.h"


#define activeBuzzer_pin 23
#define LED2_pin 26
outPinController_Class activeBuzzer;
outPinController_Class myLed2;



void setup() {
    Serial.begin(115200);
    delay(1000);
    lnLog.init();

    LOG_INFO("Avvio dimostrazione modulo LED con puntatori...");

    // Inizializza i tuoi LED passando l'indirizzo della struttura (&myLedX)
    activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);
    myLed2.init("Led", LED2_pin, HIGH);

    LOG_INFO("Inizializzazione completata. Avvio loop...");
}



void loop() {
    static bool first_run=true;
    static uint32_t startedMillis;
    uint32_t now ;

    if (first_run) {
        first_run=false;

        activeBuzzer.blinking(1000, .50, 4); // 4 cicli
        myLed2.pulse(3000);
        startedMillis=millis();
    }

    now = millis() - startedMillis;
    // Chiamate a activeBuzzer.update() per ciascun LED che vuoi gestire
    // DEVI chiamare questa funzione per OGNI LED individualmente
    activeBuzzer.update();
    myLed2.update();

    // esempi di comandi:

    if (now > 20000 && now < 20100) {
        // myLed2.startBlinkingDC(1000, 10);
        myLed2.blinking_dutyCycle(1000, 0.10);
        activeBuzzer.pulse(3000);
    }

    if (now > 24000 && now < 24100) {
        activeBuzzer.blinking_dutyCycle(1000, .70, 5);
    }

    if (now > 30000 && now < 30100) {
        activeBuzzer.off();
    }

    delay(50);

}


#if 0
outPinController_Class myLed; // Crea un'istanza della classe

void setup() {
  Serial.begin(115200);
  delay(1000);
  lnLog.init();
  LOG_NOTIFY("starting process");

  myLed.init("BuiltInLED", 25, LOW); // Inizializza il LED
  myLed.blinking(200, 800); // Fai lampeggiare 5 volte
}

void loop() {
  // LOG_NOTIFY("looping...");
  myLed.update(); // Aggiorna lo stato del LED nel loop
  delay(1000);
}

#endif

#endif