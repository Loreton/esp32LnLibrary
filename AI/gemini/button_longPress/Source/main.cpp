//
// updated by ...: Loreto Notarantonio
// Date .........: 18-06-2025 13.29.13
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@button_structures.h"

#define RELAY_PIN     16
#define BUTTON_PIN    21




// --- Variabili Globali per il Test ---

bool relayState = false;

ButtonState_t testButton;

const unsigned long TEST_BUTTON_THRESHOLDS[] = {
  50,
  800,
  2000,
  5000
};
const size_t NUM_TEST_BUTTON_THRESHOLDS = sizeof(TEST_BUTTON_THRESHOLDS) / sizeof(TEST_BUTTON_THRESHOLDS[0]);

// --- Setup e Loop Principale ---

void setup() {
    Serial.begin(115200);
    Serial.println("Avvio test pulsante con debounce e gestione del reset dei livelli nella funzione chiamante.");

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); // Assumiamo un relè che si attiva con LOW

    setupButton(&testButton, BUTTON_PIN, "TestButton", LOW, TEST_BUTTON_THRESHOLDS, NUM_TEST_BUTTON_THRESHOLDS);

    Serial.printf("Pulsante su GPIO %d, Relè su GPIO %d\n", BUTTON_PIN, RELAY_PIN);
    Serial.println("Premi il pulsante e rilascia al livello MEDIUM_PRESS per attivare/disattivare il relè.");
}

void loop() {
    if (readButton(&testButton, 300)) { // 300ms di debounce
        Serial.print("[");
        Serial.print(testButton.name);
        Serial.print("] Rilasciato! Durata: ");
        Serial.print(testButton.pressDuration);
        Serial.print(" ms. Livello finale raggiunto: ");

        switch (testButton.currentPressLevel) {
            case SHORT_PRESS: Serial.println("SHORT_PRESS"); break;
            case MEDIUM_PRESS: Serial.println("MEDIUM_PRESS"); break;
            case LONG_PRESS: Serial.println("LONG_PRESS"); break;
            case VERY_LONG_PRESS: Serial.println("VERY_LONG_PRESS"); break;
            default: Serial.println("Sconosciuto/Non Qualificato"); break;
        }

        // --- LOGICA DI TEST PER IL RELÈ ---
        if (testButton.currentPressLevel == MEDIUM_PRESS) {
            relayState = !relayState;
            if (relayState) {
                digitalWrite(RELAY_PIN, LOW);
                Serial.println("  --> Relè ACCESO!");
            } else {
                digitalWrite(RELAY_PIN, HIGH);
                Serial.println("  --> Relè SPENTO!");
            }
        } else {
            Serial.println("  --> Livello di pressione non MEDIUM_PRESS. Nessuna azione sul relè.");
        }

        // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
        // Dopo aver processato i dati, li resettiamo per la prossima pressione.
        testButton.currentPressLevel = NO_PRESS;
        testButton.pressDuration = 0;
        testButton.maxLevelReachedAndNotified = false;
        // testButton.pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.
    }

    // Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
    if (testButton.buttonPressed == testButton.pressedLogicLevel) {
        notifyCurrentButtonLevel(&testButton);

    } else {
        static ButtonPressType lastPrintedLevel = NO_PRESS;
        lastPrintedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
    }

    delay(10);
}