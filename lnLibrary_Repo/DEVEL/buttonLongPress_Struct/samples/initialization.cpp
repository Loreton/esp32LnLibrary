/*
// updated by ...: Loreto Notarantonio
// Date .........: 21-07-2025 12.03.48
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type

// ---------------------------------
// --- lnLibrary headers files
// ---------------------------------
#include "lnLogger.h"
#include "../ButtonLongPress_Struct.h"


/* #########################################
ref: /home/loreto/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-gpio.h
   #define LOW               0x0
   #define HIGH              0x1

   //GPIO FUNCTIONS
   #define INPUT             0x01
   #define OUTPUT            0x03
   #define PULLUP            0x04
   #define INPUT_PULLUP      0x05
   #define PULLDOWN          0x08
   #define INPUT_PULLDOWN    0x09
   #define OPEN_DRAIN        0x10
   #define OUTPUT_OPEN_DRAIN 0x13
   #define ANALOG            0xC0
######################################### */



extern ButtonLongPress_Struct startButton;
extern ButtonLongPress_Struct pumpState;

void pinsInitialization(void) {
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


}




