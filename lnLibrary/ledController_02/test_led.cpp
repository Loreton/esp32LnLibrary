/*
// updated by ...: Loreto Notarantonio
// Date .........: 22-06-2025 19.57.55
*/



#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>

#include "@ledController_02.h"

// --- Funzioni Principali (setup e loop) per dimostrazione ---

// Ogni variabile 'myLedX' è una specifica istanza di led_sClass
// #define LED1_pin 25
#define LED1_pin 23
#define LED2_pin 26
ledController_sc myLed1;
ledController_sc myLed2;
// led_sClass myLed2;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Avvio dimostrazione modulo LED con puntatori...");

    // Inizializza i tuoi LED passando l'indirizzo della struttura (&myLedX)
    myLed1.init(LED1_pin, HIGH);
    myLed2.init(LED2_pin, HIGH);


    Serial.println("Inizializzazione completata. Avvio loop...");
}

void loop() {
    static bool first_run=true;
    static uint32_t startedMillis;
    uint32_t now = millis() - startedMillis;
    // Chiamate a myLed1.update() per ciascun LED che vuoi gestire
    // DEVI chiamare questa funzione per OGNI LED individualmente
    myLed1.update();
    myLed2.update();

    if (first_run) {
        myLed1.startBlinking(1000, 500, 4);
        myLed2.pulseOn(3000);
        first_run=false;
        startedMillis=millis();
    }
    // esempi di comandi:

    if (now > 20000 && now < 20100) {
        myLed2.startBlinking(1000, 500);
        myLed1.pulseOn(3000);
    }

    if (now > 24000 && now < 24100) {
    // if (millis() - startedMillis > 14000) {
        myLed1.off();
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