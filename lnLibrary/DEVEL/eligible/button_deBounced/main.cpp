/*
// updated by ...: Loreto Notarantonio
// Date .........: 21-06-2025 16.40.36
*/
#ifdef __ln_MODULE_DEBUG_TEST__   // definito in platformio.ini

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@debouncedButton.h"




//###############################################################
//#################### T E S T  #################################
//#################### T E S T  #################################
//#################### T E S T  #################################
//###############################################################


#define pressControlRelay_pin    16
#define startButton_pin          21
deBouncedButton_t startButton;

void debouncedButtonTest() {
    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (read_deBouncedButton(&startButton, 300)) {
        Serial.printf("[%s] Rilasciato!\n", startButton.name);

        static bool relayState = false; // Stato attuale del relè (false = spento, true = acceso).
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
    setup_deBouncedButton(&startButton,  startButton_pin, "startButton", LOW);

    Serial.printf("%s: %d\n", startButton.name, startButton.pin);
}

void loop() {
    debouncedButtonTest();
    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);

}

#endif