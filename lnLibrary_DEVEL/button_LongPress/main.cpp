//
// updated by ...: Loreto Notarantonio
// Date .........: 20-06-2025 16.30.14
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//


#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__
#include "@logMacros.h"
#include "@button_LongPress.h"

/*
    test di due pin come input con tempi diversi
*/

ButtonState_t startButton;
ButtonState_t pumpState;


const unsigned long START_BUTTON_THRESHOLDS[] = {
                                                  200,
                                                  800,
                                                  2000,
                                                  5000,
                                                };

const unsigned long PUMP_STATE_THRESHOLDS[] = {
                                                  200,
                                                  5000,
                                                  15000,
                                                  20000,
                                                  25000,
                                                };

const size_t NUM_START_BUTTON_THRESHOLDS = sizeof(START_BUTTON_THRESHOLDS) / sizeof(START_BUTTON_THRESHOLDS[0]);
const size_t NUM_PUMP_STATE_THRESHOLDS = sizeof(PUMP_STATE_THRESHOLDS) / sizeof(PUMP_STATE_THRESHOLDS[0]);


// #########################################
// # se non c'è ln_time.cpp mi serve una dummy_Now()
// #########################################
const int8_t DUMMY_TIME_BUFFER_LENGTH = 20;
char  PROGMEM temp_buffer_time[DUMMY_TIME_BUFFER_LENGTH];
char *nowTime() {
    snprintf(temp_buffer_time, DUMMY_TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return temp_buffer_time;
}




void setup() {
    Serial.begin(115200);
    while (!Serial) {};
    delay(2000);
    printf0_FN("Avvio test pulsante con debounce e gestione del reset dei livelli nella funzione chiamante.\n");

    digitalWrite(RELAY_PIN, HIGH); // Assumiamo un relè che si attiva con LOW
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(activeBuzzer_pin, OUTPUT);
    pinMode(passiveBuzzer_pin, OUTPUT);

    setupButton(&startButton, startButton_pin, "startButton",  LOW, START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS);
    setupButton(&pumpState,   pumpState_pin,   "pumpState",    LOW, PUMP_STATE_THRESHOLDS, NUM_PUMP_STATE_THRESHOLDS);
    buttonLP_pinStatus(&startButton, false);
    buttonLP_pinStatus(&pumpState, true);


    printf0_FN("Pulsante su GPIO %d, Relè su GPIO %d\n", startButton_pin, RELAY_PIN);
    printf0_FN("Premi il pulsante e rilascia al livello MEDIUM_PRESS per attivare/disattivare il relè.\n");
    attachBuzzer(passiveBuzzer_pin, 10, 0);
    waitForEnter();
}






void loop() {

    if (readButton(&startButton)) { // state is changed
        processButton(&startButton);
    }
    if (readButton(&pumpState)) { // state is changed
        processButton(&pumpState);
    }

    // Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
    notifyCurrentButtonLevel(&startButton, activeBuzzer_pin);
    notifyCurrentButtonLevel(&pumpState, activeBuzzer_pin);

    delay(10);
}