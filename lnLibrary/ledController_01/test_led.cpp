/*
// updated by ...: Loreto Notarantonio
// Date .........: 22-06-2025 17.51.52
*/



#ifdef __ln_myLed1.TEST_CPP__



#include "@ledController_01.h"

// --- Funzioni Principali (setup e loop) per dimostrazione ---

// Ogni variabile 'myLedX' è una specifica istanza di led_sClass
#define LED1_pin 25
#define LED2_pin 26
led_sClass myLed1;
led_sClass myLed2;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Avvio dimostrazione modulo LED con puntatori...");

    // Inizializza i tuoi LED passando l'indirizzo della struttura (&myLedX)
    myLed1.init(LED1_pin, 3000, (uint8_t) 30, HIGH);  // LED 1: Pin 2, 10% duty, logica inversa
    myLed2.init(LED2_pin, 5000, (uint8_t) 50, HIGH); // LED 2: Pin 4, 50% duty, logica normale

    Serial.println("Inizializzazione completata. Avvio loop...");
}

void loop() {
    static unsigned long lastChangeTime = 0;
    // Chiamate a myLed1.update() per ciascun LED che vuoi gestire
    // DEVI chiamare questa funzione per OGNI LED individualmente
    myLed1.update();
    myLed2.update();

    // esempi di comandi:
    // led.stopBlinking();
    // led.turnOnFor(3000); // acceso per 3s

}

#endif
#endif