//
// updated by ...: Loreto Notarantonio
// Date .........: 21-07-2025 12.15.23
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__


#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__
#include "lnLogger.h"
#include "lnGlobalVars.h" // printf:XFN()
#include "lnSerialRead.h" // waitForEnter()

#include "../ButtonLongPress_Struct.h"



ButtonLongPress_Struct startButton;

// void pinsInitialization(void) ;

// void pinsInitialization(void) {
//     const  uint32_t START_BUTTON_THRESHOLDS[] = {
//                                                     400,
//                                                     800,
//                                                     2000,
//                                                     5000,
//                                                 };

//     const  uint32_t PUMP_STATE_THRESHOLDS[] = {
//                                                 400,
//                                                 5000,
//                                                 15000,
//                                                 20000,
//                                                 25000,
//                                             };

//     const uint8_t NUM_START_BUTTON_THRESHOLDS = sizeof(START_BUTTON_THRESHOLDS) / sizeof(START_BUTTON_THRESHOLDS[0]);
//     const uint8_t NUM_PUMP_STATE_THRESHOLDS   = sizeof(PUMP_STATE_THRESHOLDS)   / sizeof(PUMP_STATE_THRESHOLDS[0]);



//     //====================================================
//     //= set input pins
//     //====================================================
//     // ------  name,                 pin_nr          active_level   );
//     startButton.init("startButton", startButton_pin, LOW,           START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS); // Now an object, not a struct
//     startButton.showStatus();


// }



void setup() {
    Serial.begin(115200);
    while (!Serial) {};
    delay(2000);
    myLog.begin();

    const  uint32_t START_BUTTON_THRESHOLDS[] = {
                                                    400,
                                                    800,
                                                    2000,
                                                    5000,
                                                };

    const  uint32_t PUMP_STATE_THRESHOLDS[] = {
                                                400,
                                                5000,
                                                15000,
                                                20000,
                                                25000,
                                            };

    const uint8_t NUM_START_BUTTON_THRESHOLDS = sizeof(START_BUTTON_THRESHOLDS) / sizeof(START_BUTTON_THRESHOLDS[0]);
    const uint8_t NUM_PUMP_STATE_THRESHOLDS   = sizeof(PUMP_STATE_THRESHOLDS)   / sizeof(PUMP_STATE_THRESHOLDS[0]);



    //====================================================
    //= set input pins
    //====================================================
    // ------  name,                 pin_nr          active_level   );
    startButton.init("startButton", startButton_pin, LOW,           START_BUTTON_THRESHOLDS, NUM_START_BUTTON_THRESHOLDS); // Now an object, not a struct
    startButton.showStatus();


    // pinsInitialization();
    // startButton.showStatus();

}


bool first_run=true;
void loop() {
    if (first_run) {
        first_run=false;
        LOG_INFO("processing started....");
    }

    if (startButton.read()) {
        startButton.showStatus();
    }
    delay(10);

}


#endif // #ifdef __ln_MODULE_DEBUG_TEST__
