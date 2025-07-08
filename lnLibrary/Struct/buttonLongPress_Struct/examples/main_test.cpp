//
// updated by ...: Loreto Notarantonio
// Date .........: 08-07-2025 09.31.25
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__


#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__
#include "lnLogger.h" // printf:XFN()
#include "lnGlobalVars.h" // printf:XFN()
#include "lnSerialRead.h" // waitForEnter()

#include "PinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "ButtonLongPress_Struct.h"
#include "callBackFunctions.h" // Function protopypes

void processButton(ButtonLongPress_Struct *p);
size_t initialMemory = ESP.getFreeHeap();


ButtonLongPress_Struct startButton;
ButtonLongPress_Struct pumpState;
PinController_Struct activeBuzzer;


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
size_t buttonMemory = ESP.getFreeHeap();

size_t finalMemory;

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


    // LOG_INFO("Avvio test pulsante con debounce e gestione del reset dei livelli nella funzione chiamante.");

    pinMode(pressControlRelay_pin, OUTPUT);
    digitalWrite(pressControlRelay_pin, HIGH); // Assumiamo un relè che si attiva con LOW
    pinMode(activeBuzzer_pin, OUTPUT);
    pinMode(passiveBuzzer_pin, OUTPUT);


    // initialize pins
    activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);

    startButton.init("startButton",  startButton_pin, LOW, START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS);
    // showStatusCB(&startButton);
    startButton.showStatus(showStatusCB);

    pumpState.init("pumpState",      pumpState_pin,   LOW, PUMP_STATE_THRESHOLDS, NUM_PUMP_STATE_THRESHOLDS);
    pumpState.showStatus(showStatusCB);
    // showStatusCB(&pumpState);


    // finalMemory = ESP.getFreeHeap();
    // DEBUG_LOG("Pulsante su GPIO %d, Relè su GPIO %d", startButton_pin, pressControlRelay_pin);
    // DEBUG_LOG("Premi il pulsante e rilascia al livello MEDIUM_PRESS per attivare/disattivare il relè.");

    // DEBUG_LOG("initial Memory:     %ld bytes", initialMemory); // Stima RAM allocata
    // DEBUG_LOG("button Memory:      %ld bytes", buttonMemory - initialMemory); // Stima RAM allocata
    // DEBUG_LOG("memoria occupata:   %ld bytes", finalMemory - initialMemory); // Stima RAM allocata

    // waitForEnter();
}




void loop() {
    static bool first_run=true;
    static uint32_t startedMillis;
    uint32_t elapsed;
    uint32_t now;

    if (first_run) {
        first_run=false;
        // activeBuzzer.blinking_dc(400, .75, 4);
        startedMillis=millis();
        LOG_INFO("system ready!");
    }
    now = millis() - startedMillis;
    activeBuzzer.updateStatus();

    /*
    elapsed=5000;
    if (now > elapsed && now < (elapsed+100)) {
        // myLed2.startBlinkingDC(1000, 10);
        activeBuzzer.off();
    }

    elapsed=10000;
    if (now > elapsed && now < (elapsed+100)) {
        // myLed2.startBlinkingDC(1000, 10);
        activeBuzzer.blinking(300, 100, 4);
    }

*/
    // Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
    // startButton.notifyCurrentButtonLevel(startButtonNotificationHandlerCB);
    // pumpState.notifyCurrentButtonLevel(pumpStateNotificationHandlerCB);
    startButton.notifyCurrentButtonLevel(beepNotification);
    pumpState.notifyCurrentButtonLevel(beepNotification);

    startButton.read(startButtonHandlerCB);
    pumpState.read(pumpStateHandlerCB);


    // if (startButton.read()) { // state is changed
    //     processButton(&startButton);
    // }
    // if (pumpState.read()) { // state is changed
    //     processButton(&pumpState);
    // }
/*
    // notifyCurrentButtonLevel(&startButton);

*/

    // delay(10);
}

#endif // #ifdef __ln_MODULE_DEBUG_TEST__
