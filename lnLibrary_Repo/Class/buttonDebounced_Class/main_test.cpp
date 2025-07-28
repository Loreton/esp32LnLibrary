/*
// updated by ...: Loreto Notarantonio
// Date .........: 29-06-2025 18.03.35
*/

#ifdef __ln_MODULE_DEBUG_TEST__   // definito in platformio.ini


#include <Arduino.h>    // in testa anche per le definizioni dei type

#define __I_AM_MAIN_CPP__

#include "@buttonDebounced_Class.h"



#define pressControlRelay_pin   16
#define startButton_pin         21

// Creazione di istanze della classe ButtonDebounced_Class.
// L'inizializzazione avviene tramite il costruttore.
ButtonDebounced_Class startButton;
// ButtonDebounced_Class startButton("startButton", startButton_pin, LOW);
// ButtonDebounced_Class startButton2("startButton2", 22, LOW);
// ButtonDebounced_Class startButton3("startButton3", 23, LOW);

void debouncedButtonTest() {
    // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
    if (startButton.read(300)) {
        Serial.printf("[%s] Rilasciato!\n", startButton.pinID()); // Usiamo getName()
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

const int8_t DUMMY_TIME_BUFFER_LENGTH = 20;
char  PROGMEM temp_buffer_time[DUMMY_TIME_BUFFER_LENGTH];
char *nowTime() {
    snprintf(temp_buffer_time, DUMMY_TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return temp_buffer_time;
}


void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(pressControlRelay_pin, OUTPUT);
    // L'inizializzazione ora avviene nel costruttore, non c'è più bisogno di chiamare init().

    startButton.init("startButton", startButton_pin, LOW);
    Serial.printf("%s\n", startButton.pinID());
}

void loop() {
    debouncedButtonTest();
    // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
    delay(10);
}

#endif