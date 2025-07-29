//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 16.57.13
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__


#include <Arduino.h>    // in testa anche per le definizioni dei type

#define pressControlRelay_pin       16
#define pumpState_pin               19
#define activeBuzzer_pin            23  // OUTPUT
#define passiveBuzzer_pin           22  // OUTPUT
#define startButton_pin             21  // INPUT



#define __I_AM_MAIN_CPP__
#include "lnGlobalVars.h" // printf:XFN()
#include "lnSerialRead.h" // waitForEnter()

#include "outPinController_Class.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "ButtonLongPress_Class.h"
#include "callBackFunctions.h" // Function protopypes





void processButton(ButtonLongPress_Class *p);
size_t initialMemory = ESP.getFreeHeap();


ButtonLongPress_Class startButton;
ButtonLongPress_Class pumpState;
outPinController_Class activeBuzzer;


// const unsigned long START_BUTTON_THRESHOLDS[] = {
const PROGMEM uint32_t START_BUTTON_THRESHOLDS[] = {
                                                  200,
                                                  800,
                                                  2000,
                                                  5000,
                                                };

const PROGMEM uint32_t PUMP_STATE_THRESHOLDS[] = {
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



void setup() {
    Serial.begin(115200);
    // while (!Serial) {};
    delay(2000);
    lnLog.init();
    printSizeOf();

    // LOG_INFO("Avvio test pulsante con debounce e gestione del reset dei livelli nella funzione chiamante.");

    pinMode(pressControlRelay_pin, OUTPUT);
    digitalWrite(pressControlRelay_pin, HIGH); // Assumiamo un relè che si attiva con LOW
    pinMode(activeBuzzer_pin, OUTPUT);
    pinMode(passiveBuzzer_pin, OUTPUT);


    // initialize pins
    activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);

    startButton.init("startButton",  startButton_pin, LOW, START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS);
    // showStatus(&startButton);
    startButton.showStatus();
    // startButton.showStatus(showStatusCB);

    pumpState.init("pumpState",      pumpState_pin,   LOW, PUMP_STATE_THRESHOLDS, NUM_PUMP_STATE_THRESHOLDS);
    // pumpState.showStatus(showStatusCB);
    // showStatus(&pumpState);
    pumpState.showStatus();



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
    activeBuzzer.update();

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
    // startButton.pressingLevelNotification(startButtonNotificationHandlerCB);
    // pumpState.pressingLevelNotification(pumpStateNotificationHandlerCB);
    startButton.pressingLevelNotification(beepNotification);
    pumpState.pressingLevelNotification(beepNotification);

    // startButton.read(startButtonHandlerCB);
    // pumpState.read(pumpStateHandlerCB);


    if (startButton.read()) { // state is changed
        processButton(&startButton);
    }
    if (pumpState.read()) { // state is changed
        processButton(&pumpState);
    }
/*
    // pressingLevelNotification(&startButton);

*/

    // delay(10);
}

#endif // #ifdef __ln_MODULE_DEBUG_TEST__
