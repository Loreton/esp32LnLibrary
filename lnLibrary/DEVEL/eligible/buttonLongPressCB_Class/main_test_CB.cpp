//
// updated by ...: Loreto Notarantonio
// Date .........: 01-07-2025 16.58.36
//

#ifdef __ln_MODULE_DEBUG_TEST__
#include <Arduino.h>

#define fNO_KEYB_PROPT false
#define fKEYB_PROMPT true
#define LOG_LEVEL_0
#define __I_AM_MAIN_CPP__
#include "@globalVars.h" // printf:XFN()

#include "@pinController_Class.h" // Include the new PinController_Class header
#include "@pinLongPressCB_Class.h"  // Include the new ButtonLongPressCB_Class_Class header


#include "@main_test.h"
/*
    test di due pin come input con tempi diversi
*/


const uint32_t START_BUTTON_THRESHOLDS[] = {
    200,
    800,
    2000,
    5000,
};

const uint32_t PUMP_STATE_THRESHOLDS[] = {
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

// Callback function now receives a pointer to the ButtonLongPressCB_Class object
void myButtonHandler(ButtonLongPressCB_Class* btn) {
    Serial.printf("pinLongPress Callback: %s\n", btn->getPinID()); // Use getter
}
void myButtonNotifyCallback(ButtonLongPressCB_Class* btn) {
    Serial.printf("pinLongPress Callback: %s\n", btn->getPinID()); // Use getter
}
void myBuzzerHandler(uint16_t duration) {
    printf0_FN("pinLongPress Callback duration: %d\n", duration); // Use getter
}


// Initialize objects using their constructors
// ButtonLongPressCB_Class  startButton;
// ButtonLongPressCB_Class  pumpState;
PinController_Class activeBuzzer("Buzzer", activeBuzzer_pin, HIGH);
ButtonLongPressCB_Class  startButton("startButton", startButton_pin, LOW, START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS, myButtonNotifyCallback); // Now an object, not a struct
ButtonLongPressCB_Class  pumpState("pumpState", pumpState_pin, LOW, PUMP_STATE_THRESHOLDS, NUM_PUMP_STATE_THRESHOLDS);   // Now an object, not a struct
// PinController_Class activeBuzzer("Buzzer", activeBuzzer_pin, HIGH);

void setup() {
    Serial.begin(115200);
    // while (!Serial) {};
    delay(2000);
    printf0_FN("Avvio test pulsante con debounce e gestione del reset dei livelli nella funzione chiamante.\n");

    // Initialize objects using their constructors

    startButton.setBuzzerCallback(myBuzzerHandler);
    startButton.printStatus(fNO_KEYB_PROPT);
    pumpState.printStatus(fKEYB_PROMPT);

    digitalWrite(pressControlRelay_pin, HIGH); // Assumiamo un relè che si attiva con LOW
    pinMode(pressControlRelay_pin, OUTPUT);


    printf0_FN("Pulsante su GPIO %d, Relè su GPIO %d\n", startButton_pin, pressControlRelay_pin);
    printf0_FN("Premi il pulsante e rilascia al livello MEDIUM_PRESS per attivare/disattivare il relè.\n");
    waitForEnter();
}




void loop() {
    static bool first_run=true;
    static uint32_t startedMillis;
    if (first_run) {
        first_run=false;
        activeBuzzer.blinking_dc(1000, 0.50, 4); // Period, Duty Cycle, Cycles
        startedMillis=millis();
    }

    uint32_t now = millis() - startedMillis;

    // --------------------------------------------------
    // --- update current objects status
    activeBuzzer.update(); // Call update on the activeBuzzer object
    startButton.notifyCurrentLevel(&activeBuzzer);
    pumpState.notifyCurrentLevel(&activeBuzzer);
    // --------------



    // Call member functions directly on the objects
    if (startButton.read()) { // state is changed
        processButton(&startButton); // processButton still takes a pointer for now
        // startButton.resetState(); // resetState is now called inside processButton
    }
    if (pumpState.read()) { // state is changed
        processButton(&pumpState); // processButton still takes a pointer for now
        // pumpState.resetState(); // resetState is now called inside processButton
    }

    // delay(10);
}


#endif // #ifdef __ln_MODULE_DEBUG_TEST__
