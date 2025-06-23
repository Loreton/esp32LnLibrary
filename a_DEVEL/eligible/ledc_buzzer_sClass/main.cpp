//
// updated by ...: Loreto Notarantonio
// Date .........: 21-06-2025 19.03.23
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//


#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()
#include "@SerialRead.h" // waitForEnter()

#include "ledc_buzzer_sClass.h"

#define passiveBuzzer_pin   22  // OUTPUT


Buzzer_func_inside myBuzzer; // Istanza globale del buzzer

void setup() {
    Serial.begin(115200);
    while(!Serial) {};
    Serial.println("ESP32 Buzzer External Functions Example");

    myBuzzer.init(passiveBuzzer_pin, 0); // Inizializza il buzzer
}

void loop() {
    myBuzzer.handle(); // Gestisce la fine del tono

    static unsigned long lastActionTime = 0;
    static int step = 0;
    const unsigned long PAUSE_BETWEEN_ACTIONS = 1500; // 1.5 secondi

    if (!myBuzzer.isPlayingTone() && (millis() - lastActionTime >= PAUSE_BETWEEN_ACTIONS)) {
        switch (step) {
            case 0:
                myBuzzer.playTone(440, 500); // Suona un La per 500ms
                step = 1;
                break;
            case 1:
                myBuzzer.playTone(523, 750); // Suona un Do per 750ms
                step = 2;
                break;
            case 2:
                myBuzzer.playTone(659, 1000); // Suona un Mi per 1000ms
                step = 3;
                break;
            default:
                myBuzzer.noTone();
                step = 0;
                Serial.println("Sequenza completata, riavvio.");
                waitForEnter();
                break;
        }
        lastActionTime = millis();
    }
    delay(10); // per liberare la CPU

}



#endif