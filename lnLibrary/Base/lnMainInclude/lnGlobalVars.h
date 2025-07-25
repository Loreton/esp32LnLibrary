/*
// updated by ...: Loreto Notarantonio
// Date .........: 23-07-2025 14.36.34
*/


#pragma once
#include <Arduino.h>



    // ---------------------------------
    // lnLibrary headers files
    // ---------------------------------
    // #include "@logMacros.h"
    // #include "@serialRead.h" // waitForEnter()
    // #include "../serialRead/include/@SerialRead.h" // NON capisco perch* devo mettere il path relativo
    // #include "lnString.h" // setPinID
    /* definition to expand macro then apply to pragma message */
    #define VALUE_TO_STRING(x) #x
    #define VALUE(x) VALUE_TO_STRING(x)
    #define VAR_NAME_VALUE(var) #var "="  VALUE(var)
    #define PRINT_VAR(var) #var "="  VALUE(var)

    /* Some example here */
    // #pragma message(VAR_NAME_VALUE(NOT_DEFINED))
    // #pragma message(VAR_NAME_VALUE(DEFINED_BUT_NO_VALUE))
    // #pragma message(VAR_NAME_VALUE(DEFINED_INT))
    // #pragma message(VAR_NAME_VALUE(DEFINED_STR))
    // #pragma message(VAR_NAME_VALUE(LORETO))
    // #pragma message(VAR_NAME_VALUE(LORETO2))

    #define ON  true
    #define OFF false

    #ifdef __I_AM_MAIN_CPP__
        const char PROGMEM *str_action[]     = {"released", "pressed"};
        const char PROGMEM *str_pinLevel[]   = {"LOW", "HIGH"};
        const char PROGMEM *str_TrueFalse[]  = {"FALSE", "TRUE"};
        // const char PROGMEM *str_OffOn[]      = {"OFF", "ON"};
        const char PROGMEM *str_OnOff[]      = {"OFF", "ON"};
        const char PROGMEM *str_ON           = "ON";
        const char PROGMEM *str_OFF          = "OFF";
        const char PROGMEM *str_INPUT        = "INPUT";
        const char PROGMEM *str_INPUT_PULLUP = "INPUT_PULLUP";
        const char PROGMEM *str_OUTPUT       = "OUTPUT";


        // #ifdef __INCLUDE_LN_TIME__
        //     #warning  "__INCLUDE_LN_TIME__ defines"
        //     #include "@ln_time.h"
        //     ESP32Time     rtc;
        //     struct tm timeinfo; // capire se va bene uno per tutti i moduli oppure mantenerli separati per evitare overwrites
        //     // time_setup();
        // #endif


    #else
        extern const char PROGMEM *str_action[];
        extern const char PROGMEM *str_pinLevel[];
        extern const char PROGMEM *str_TrueFalse[];
        // extern const char PROGMEM *str_OffOn[];
        extern const char PROGMEM *str_OnOff[];
        extern const char PROGMEM *str_ON;
        extern const char PROGMEM *str_OFF;
        extern const char PROGMEM *str_INPUT;
        extern const char PROGMEM *str_INPUT_PULLUP;
        extern const char PROGMEM *str_OUTPUT;
        extern const char * PROGMEM THRESHOLD_LEVEL_TYPES[];
        extern const int8_t THRESHOLDS_LEVELS_TYPES_length;


        // #ifdef __INCLUDE_LN_TIME__
        //     #warning  "extern __INCLUDE_LN_TIME__ defines"
        //     #include "@ln_time.h"
        //     extern ESP32Time     rtc;
        //     extern struct tm timeinfo; // capire se va bene uno per tutti i moduli oppure mantenerli separati per evitare overwrites
        //     // time_setup();
        // #endif

    #endif



    #ifdef __I_AM_MAIN_CPP__
        #include "lnLogger.h"
        void printSizeOf(void) {
            // Serial.printf("byte:            %d\n", sizeof(byte));
            // Serial.printf("char:            %d\n", sizeof(char));
            // Serial.printf("bool:            %d\n", sizeof(bool));
            // Serial.printf("int:             %d\n", sizeof(int));
            // Serial.printf("long:            %d\n", sizeof(long));
            // Serial.printf("unsigned long:   %lu\n", sizeof(unsigned));
            // Serial.printf("uint8_t:         %d\n", sizeof(uint8_t));
            // Serial.printf("uint16_t:        %lu\n", sizeof(uint16_t));
            // Serial.printf("uint32_t:        %lu\n", sizeof(uint32_t));
            LOG_TRACE("byte:            %d", sizeof(byte));
            LOG_TRACE("char:            %d", sizeof(char));
            LOG_TRACE("bool:            %d", sizeof(bool));
            LOG_TRACE("int:             %d", sizeof(int));
            LOG_TRACE("long:            %d", sizeof(long));
            LOG_TRACE("unsigned long:   %lu", sizeof(unsigned));
            LOG_TRACE("uint8_t:         %d", sizeof(uint8_t));
            LOG_TRACE("uint16_t:        %lu", sizeof(uint16_t));
            LOG_TRACE("uint32_t:        %lu", sizeof(uint32_t));
        }
    #else
        void printSizeOf(void);
    #endif