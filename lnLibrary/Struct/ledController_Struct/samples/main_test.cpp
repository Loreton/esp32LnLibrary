/*
// updated by ...: Loreto Notarantonio
// Date .........: 11-07-2025 09.44.24
*/



#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>

#include "../LedController_Struct.h"

// --- Funzioni Principali (setup e loop) per dimostrazione ---

// Ogni variabile 'myLedX' è una specifica istanza di led_sClass
// #define LED1_pin 25
#define activeBuzzer_pin 23
#define LED2_pin 26
LedController_Struct activeBuzzer;
LedController_Struct myLed2;



void setup() {
    Serial.begin(115200);
    delay(1000);
    // loreto._pin=12;
    Serial.println("Avvio dimostrazione modulo LED con puntatori...");

    // Inizializza i tuoi LED passando l'indirizzo della struttura (&myLedX)
    activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);
    myLed2.init("Led", LED2_pin, HIGH);

    Serial.println("Inizializzazione completata. Avvio loop...");
}



void loop() {
    static bool first_run=true;
    static uint32_t startedMillis;
    uint32_t now ;

    if (first_run) {
        first_run=false;
        // activeBuzzer.blinking(1000, 500, 4);
        activeBuzzer.blinking(1000, .50, 4);
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
        myLed2.blinking_dc(1000, 0.10);
        activeBuzzer.pulse(3000);
    }

    if (now > 24000 && now < 24100) {
        activeBuzzer.blinking_dc(1000, .70, 5);
    }

    if (now > 30000 && now < 30100) {
        activeBuzzer.off();
    }

    // startBlinking(1000, 500);
    // stopBlinking();
    // turnOnFor(unsigned long duration);
    // setLed(bool on);
    // isOn() const;
    // isBlinking() const;
    // isFixedOn() const;
    delay(50);
}

#endif