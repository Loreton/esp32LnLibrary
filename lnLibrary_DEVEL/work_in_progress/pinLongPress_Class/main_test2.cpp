//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 18.32.54
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__
#include <Arduino.h>

#define __I_AM_MAIN_CPP__
#include "@globalVars.h" // printf:XFN()

#include "@pinController_Class.h" // Include the new PinController_Class header
#include "@pinLongPress_Class.h"  // Include the new PinLongPress_Class_Class header


#include "@main_test.h"

void notifyCurrentButtonLevel(PinLongPress_Class *p, PinController_Class *buzzer);
/*
    test di due pin come input con tempi diversi
*/

// Declare instances of the classes


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


PinLongPress_Class startButton("startButton", startButton_pin, LOW, START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS); // Now an object, not a struct
PinLongPress_Class pumpState("pumpState", pumpState_pin, LOW, PUMP_STATE_THRESHOLDS, NUM_PUMP_STATE_THRESHOLDS);   // Now an object, not a struct
PinController_Class activeBuzzer("Buzzer", activeBuzzer_pin, HIGH); // Assuming pinController_Struct is also converted to PinController_Class



// #########################################
// # se non c'è ln_time.cpp mi serve una dummy_Now()
// #########################################
const int8_t DUMMY_TIME_BUFFER_LENGTH = 20;
char  PROGMEM temp_buffer_time[DUMMY_TIME_BUFFER_LENGTH];
char *nowTime() {
    snprintf(temp_buffer_time, DUMMY_TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return temp_buffer_time;
}

// // Callback function now receives a pointer to the PinLongPress_Class object
// void myButtonHandler(PinLongPress_Class* btn) {
//     Serial.printf("pinLongPress Callback: %s\n", btn->getPinID()); // Use getter
// }


void setup() {
    Serial.begin(115200);
    while (!Serial) {};
    delay(2000);
    printf0_FN("Avvio test pulsante con debounce e gestione del reset dei livelli nella funzione chiamante.\n");

    digitalWrite(pressControlRelay_pin, HIGH); // Assumiamo un relè che si attiva con LOW
    pinMode(pressControlRelay_pin, OUTPUT);
    pinMode(activeBuzzer_pin, OUTPUT);
    pinMode(passiveBuzzer_pin, OUTPUT);

    // Initialize objects using their constructors
    // activeBuzzer = PinController_Class("Buzzer", activeBuzzer_pin, HIGH); // Assuming a constructor for PinController_Class
    // startButton = PinLongPress_Class("startButton", startButton_pin, LOW, START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS, myButtonHandler);
    // pumpState = PinLongPress_Class();

    // Call member functions using the dot operator
    startButton.printStatus(false);
    pumpState.printStatus(false);


    printf0_FN("Pulsante su GPIO %d, Relè su GPIO %d\n", startButton_pin, pressControlRelay_pin);
    printf0_FN("Premi il pulsante e rilascia al livello MEDIUM_PRESS per attivare/disattivare il relè.\n");
    waitForEnter();

}




void loop() {
    static bool first_run=true;
    static uint32_t startedMillis;
    uint32_t now;

    if (first_run) {
        first_run=false;
        startedMillis=millis();
    }
    now = millis() - startedMillis;
    activeBuzzer.update(); // Call update on the activeBuzzer object

    // Call external functions, passing pointers to the objects
    notifyCurrentButtonLevel(&pumpState, &activeBuzzer);
    notifyCurrentButtonLevel(&startButton, &activeBuzzer);

    // Call member functions directly on the objects
    if (startButton.read()) { // state is changed
        processButton(&startButton); // processButton still takes a pointer for now
        startButton.resetState(); // Reset the button state after processing
    }
    if (pumpState.read()) { // state is changed
        processButton(&pumpState); // processButton still takes a pointer for now
        pumpState.resetState(); // Reset the button state after processing
    }

    delay(10);
}
#endif // #ifdef __ln_MODULE_DEBUG_TEST__
