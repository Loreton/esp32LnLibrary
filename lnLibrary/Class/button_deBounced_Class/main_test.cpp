/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 17.52.53
*/

#ifdef __ln_MODULE_DEBUG_TEST__   // definito in platformio.ini
#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@debouncedButton_Class.h"


#define pressControlRelay_pin   16
#define startButton_pin         21

// Creazione di istanze della classe DebouncedButton_Class.
// L'inizializzazione avviene tramite il costruttore.
DebouncedButton_Class startButton(startButton_pin, "startButton", LOW);
DebouncedButton_Class startButton2(23, "startButton2", LOW);
DebouncedButton_Class startButton3(23, "startButton3", LOW);

void debouncedButtonTest() {
    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (startButton.read(300)) {
        Serial.printf("[%s] Rilasciato!\n", startButton.getName()); // Usiamo getName()
        static bool relayState = true; // Stato attuale del relè
        // Toggle dello stato del relè.
        relayState = !relayState;
        if (relayState) {
            digitalWrite(pressControlRelay_pin, LOW); // Accendi il relè (adatta a HIGH se il tuo relè è "attivo alto").
            Serial.println("  --> Relè ACCESO!");
        } else {
            digitalWrite(pressControlRelay_pin, HIGH); // Spegni il relè (adatta a LOW se il tuo relè è "attivo alto").
            Serial.println("  --> Relè SPENTO!");
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(pressControlRelay_pin, OUTPUT);
    // L'inizializzazione ora avviene nel costruttore, non c'è più bisogno di chiamare init().

    Serial.printf("%s: %d\n", startButton.getName(), startButton.getPin());
}

void loop() {
    debouncedButtonTest();
    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);
}

#endif