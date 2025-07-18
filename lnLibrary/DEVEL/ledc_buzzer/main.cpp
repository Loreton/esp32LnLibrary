//
// updated by ...: Loreto Notarantonio
// Date .........: 09-07-2025 13.55.58
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "lnLogger.h" // printf:XFN()
#include "lnSerialRead.h" // waitForEnter()

#include "ledc_buzzer.h"

#define passiveBuzzer_pin   22  // OUTPUT

// --- Esempio di utilizzo globale ---
Buzzer_t myBuzzer; // Istanza globale del buzzer

void setup() {
    Serial.begin(115200);
    while(!Serial) {};
    LOG_INFO("ESP32 Buzzer External Functions Example");

    buzzer_init(&myBuzzer, passiveBuzzer_pin, 0); // Inizializza il buzzer
}

void loop() {
    buzzer_handle(&myBuzzer); // Gestisce la fine del tono

    static unsigned long lastActionTime = 0;
    static int step = 0;
    const unsigned long PAUSE_BETWEEN_ACTIONS = 1500; // 1.5 secondi

    if (!buzzer_isPlayingTone(&myBuzzer) && (millis() - lastActionTime >= PAUSE_BETWEEN_ACTIONS)) {
        switch (step) {
            case 0:
                buzzer_playTone(&myBuzzer, 440, 500); // Suona un La per 500ms
                step = 1;
                break;
            case 1:
                buzzer_playTone(&myBuzzer, 523, 750); // Suona un Do per 750ms
                step = 2;
                break;
            case 2:
                buzzer_playTone(&myBuzzer, 659, 1000); // Suona un Mi per 1000ms
                step = 3;
                break;
            default:
                buzzer_noTone(&myBuzzer);
                step = 0;
                LOG_INFO("Sequenza completata, riavvio.");
                waitForEnter();
                break;
        }
        lastActionTime = millis();
    }
    delay(10); // per liberare la CPU

}

