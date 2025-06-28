//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 17.01.24
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__


#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__
#include "@globalVars.h" // printf:XFN()

// #include "@logMacros.h"
// #include "@SerialRead.h"
#include "@pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "@pinLongPress_Struct.h"


#include "@main_test.h"
/*
    test di due pin come input con tempi diversi
*/

pinLongPress_Struct startButton;
pinLongPress_Struct pumpState;
pinController_Struct activeBuzzer;


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


void myButtonHandler(pinLongPress_Struct* btn) {
    Serial.printf("pinLongPress Callback: %s\n", btn->pinID_);
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
    static uint32_t startedMillis;
    uint32_t elapsed;
    uint32_t now;

    if (first_run) {
        first_run=false;
        // activeBuzzer.blinking_dc(400, .75, 4);
        startedMillis=millis();
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
    notifyCurrentButtonLevel(&pumpState, &activeBuzzer);
    notifyCurrentButtonLevel(&startButton, &activeBuzzer);
    if (startButton.read()) { // state is changed
        processButton(&startButton);
    }
    if (pumpState.read()) { // state is changed
        processButton(&pumpState);
    }
/*
    // Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
    // notifyCurrentButtonLevel(&startButton);

*/

    delay(10);
}

#endif // #ifdef __ln_MODULE_DEBUG_TEST__
