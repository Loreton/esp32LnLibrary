//
// updated by ...: Loreto Notarantonio
// Date .........: 19-06-2025 17.12.55
//


#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__
#include "@led_structure.h"



// --- Funzioni Principali (setup e loop) per dimostrazione ---

// Dichiarazione delle istanze delle strutture LED_t
// Ogni variabile 'myLedX' è una specifica istanza di LED_t
LED_t myLed1;
LED_t myLed2;
// LED_t myLed3; // Puoi aggiungere quanti LED vuoi, dichiarandoli singolarmente

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Avvio dimostrazione modulo LED con puntatori...");

    // Inizializza i tuoi LED passando l'indirizzo della struttura (&myLedX)
    LED_init(&myLed1, 25, 3000, 30, HIGH);  // LED 1: Pin 2, 10% duty, logica inversa
    LED_init(&myLed2, 26, 5000, 50, HIGH); // LED 2: Pin 4, 50% duty, logica normale
    // LED_init(&myLed3, 16, 200, 800, false); // LED 3: Pin 16, 20% duty, logica normale

    Serial.println("Inizializzazione completata. Avvio loop...");
}

void loop() {
  // Chiamate a LED_update() per ciascun LED che vuoi gestire
  // DEVI chiamare questa funzione per OGNI LED individualmente
  LED_update(&myLed1);
  // LED_update(&myLed2);
  // LED_update(&myLed3); // Aggiorna anche il terzo LED

  // --- Esempio di altre attività non bloccanti ---

  // Esempio: cambia il duty cycle del LED 1 dopo un po' di tempo
  static unsigned long lastChangeTime = 0;
  if (millis() - lastChangeTime > 10000) { // Ogni 10 secondi
    lastChangeTime = millis();
    // Cambia il duty cycle del LED 1
    if (myLed1.onTime == 100) {
      LED_setDutyCycle(&myLed1, 7000, 75); // 70% ON
    } else {
      LED_setDutyCycle(&myLed1, 3000, 25); // Torna a 10% ON
    }
  }

  // Esempio: forzare lo stato del LED 2 per 2 secondi ogni 5 secondi
  static unsigned long lastForceTime = 0;
  static bool forcedOn = false;
  if (millis() - lastForceTime > 5000) { // Ogni 5 secondi
    lastForceTime = millis();
    if (!forcedOn) {
      LED_setState(&myLed2, true); // Forzo ON
      forcedOn = true;
        Serial.println("LED (Pin: 4) controllo forzato.");
    } else {
      // Aspetto 2 secondi prima di rilasciare il controllo
      if (millis() - (lastForceTime + 2000) > 0) { // Rilascia dopo 2 secondi forzati
         // Non chiamo setState(false) qui, lascio che LED_update riprenda il controllo
         // myLed2.state verrà aggiornato correttamente al prossimo ciclo di LED_update
         forcedOn = false;
         Serial.println("LED (Pin: 4) rilasciato dal controllo forzato.");
      }
    }
  }
}