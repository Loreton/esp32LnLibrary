/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.45.27
*/

#ifdef __ln_MODULE_DEBUG_TEST__   // definito in platformio.ini
#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@debouncedButton_Struct.h"


/*
Checking size .pio/build/ln_esp32/firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]   6.6% (used 21488 bytes from 327680 bytes)
Flash: [==        ]  20.6% (used 269529 bytes from 1310720 bytes)
6061940 Jun 21 16:23 .pio/build/ln_esp32/firmware.elf
*/


//###############################################################
//#################### T E S T  #################################
//#################### T E S T  #################################
//#################### T E S T  #################################
//###############################################################


#define pressControlRelay_pin    16
#define startButton_pin          21
deBouncedButton_sc startButton;
deBouncedButton_sc startButton2;
deBouncedButton_sc startButton3;

void debouncedButtonTest() {
    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (startButton.read(300)) {
        Serial.printf("[%s] Rilasciato!\n", startButton.name_);

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
    startButton.init(startButton_pin, "startButton", LOW);
    startButton2.init(23, "startButton2", LOW);
    startButton3.init(23, "startButton3", LOW);

    Serial.printf("%s: %d\n", startButton.name_, startButton.pin_);
}

void loop() {
    debouncedButtonTest();
    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);

}

#endif