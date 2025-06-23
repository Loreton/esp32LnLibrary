//
// updated by ...: Loreto Notarantonio
// Date .........: 23-06-2025 18.09.45
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__


#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__
#include "@logMacros.h"
#include "@SerialRead.h"
#include "@pinController_sc.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "@pinLongPress_sc.h"


#include "@main_test.h"
/*
    test di due pin come input con tempi diversi
*/

pinLongPress_sc startButton;
pinLongPress_sc pumpState;
pinController_sc activeBuzzer;


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


void myButtonHandler(pinLongPress_sc* btn) {
    Serial.printf("pinLongPress Callback: %s\n", btn->_pinID);
}


void setup() {
    Serial.begin(115200);
    while (!Serial) {};
    delay(2000);
    printf0_FN("Avvio test pulsante con debounce e gestione del reset dei livelli nella funzione chiamante.\n");

    digitalWrite(pressControlRelay_pin, HIGH); // Assumiamo un relè che si attiva con LOW
    pinMode(pressControlRelay_pin, OUTPUT);
    pinMode(activeBuzzer_pin, OUTPUT);
    pinMode(passiveBuzzer_pin, OUTPUT);

    // initialize pins
    activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);
    startButton.init("startButton",  startButton_pin, LOW, START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS, myButtonHandler);
    pumpState.init("pumpState",      pumpState_pin,   LOW, PUMP_STATE_THRESHOLDS, NUM_PUMP_STATE_THRESHOLDS, myButtonHandler);
    pinLongPressStatus(&startButton, false);
    pinLongPressStatus(&pumpState, false);


    printf0_FN("Pulsante su GPIO %d, Relè su GPIO %d\n", startButton_pin, pressControlRelay_pin);
    printf0_FN("Premi il pulsante e rilascia al livello MEDIUM_PRESS per attivare/disattivare il relè.\n");
    waitForEnter();

}




void loop() {
    static bool first_run=true;
    if (first_run) {
        activeBuzzer.blinking_dc(300, .25, 3);
        first_run=false;
    }

    // Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
    // notifyCurrentButtonLevel(&startButton, &activeBuzzer);
    notifyCurrentButtonLevel(&startButton);
    notifyCurrentButtonLevel(&pumpState, &activeBuzzer);
    activeBuzzer.update();

    if (startButton.read()) { // state is changed
        processButton(&startButton);
    }
    if (pumpState.read()) { // state is changed
        processButton(&pumpState);
    }


    delay(100);
}

#endif // #ifdef __ln_MODULE_DEBUG_TEST__
