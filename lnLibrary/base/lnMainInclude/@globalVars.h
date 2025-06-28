/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-06-2025 17.21.26
*/


#pragma once
#include <Arduino.h>


    // ---------------------------------
    // lnLibrary headers files
    // ---------------------------------
    #include "@logMacros.h"
    // #include "SerialRead.h" // waitForEnter()
    #include "../serialRead/include/@SerialRead.h" // NON capisco perch* devo mettere il path relativo
    #include "@lnString.h" // setPinID




    #ifdef __I_AM_MAIN_CPP__
        const char PROGMEM *str_action[]     = {"released", "pressed"};
        const char PROGMEM *str_pinLevel[]   = {"LOW", "HIGH"};
        const char PROGMEM *str_TrueFalse[]  = {"FALSE", "TRUE"};
        const char PROGMEM *str_OffOn[]      = {"OFF", "ON"};
        const char PROGMEM *str_ON           = "ON";
        const char PROGMEM *str_OFF          = "OFF";
        const char PROGMEM *str_INPUT        = "INPUT";
        const char PROGMEM *str_INPUT_PULLUP = "INPUT_PULLUP";
        const char PROGMEM *str_OUTPUT       = "OUTPUT";
        // ------- in pinOperations.cpp ....devo capire perché non posso spostarli
        // const char * PROGMEM THRESHOLD_LEVEL_TYPES[] = {"NO_PRESSED_BUTTON", "PRESSED_LEVEL_01", "PRESSED_LEVEL_02", "PRESSED_LEVEL_03", "PRESSED_LEVEL_04", "PRESSED_LEVEL_05", "PRESSED_LEVEL_06", "OVERFLOW_TIME"};
        // const int8_t THRESHOLDS_LEVELS_TYPES_length = sizeof(THRESHOLD_LEVEL_TYPES)/sizeof(char *);
    #else
        extern const char PROGMEM *str_action[];
        extern const char PROGMEM *str_pinLevel[];
        extern const char PROGMEM *str_TrueFalse[];
        extern const char PROGMEM *str_OffOn[];
        extern const char PROGMEM *str_ON;
        extern const char PROGMEM *str_OFF;
        extern const char PROGMEM *str_INPUT;
        extern const char PROGMEM *str_INPUT_PULLUP;
        extern const char PROGMEM *str_OUTPUT;
        extern const char * PROGMEM THRESHOLD_LEVEL_TYPES[];
        extern const int8_t THRESHOLDS_LEVELS_TYPES_length;
    #endif

