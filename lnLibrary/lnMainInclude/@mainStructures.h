//
// updated by ...: Loreto Notarantonio
// Date .........: 19-04-2025 20.53.05
//

#include <Arduino.h>

#ifndef __LN_MAIN_STRUCTURES_H__
    #define __LN_MAIN_STRUCTURES_H__

    struct PINtimer {
        bool       enabled    = false;
        uint32_t   msecs_default = 0; // millis
        uint32_t   msecs_start   = 0; // millis()
        uint32_t   cur_duration = 0; // millis()
        char       hms_remaining[12]; // formato HH:MM:SS
    };


    struct PINpulsetime {
        bool       enabled    = false; // solo in caso di msecs_default
        bool       auto_on_off    = false; // OFF as soon pulsetime expire
        uint32_t   msecs_default = 0; // millis
        uint32_t   msecs_start   = 0; // millis()
        uint32_t   cur_duration = 0; // millis()
        char       hms_remaining[12]; // formato HH:MM:SS
        uint32_t   remaining=0;
        uint32_t   elapsed=0;
    };



    typedef struct  {

        const   char             *name;
                // una volta impostati non si toccano più
                uint8_t          pin                 = 0;
                char             pinID[21];  // contiene [pin:%02d.%-15s] p->pin, p->name,
                uint8_t          mode                = INPUT_PULLUP; // ${HOME}/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-gpio.h
                bool             ON                  = LOW;     // ON_level
                bool             OFF                 = HIGH;        // OFF_level
                bool             active_level        = LOW;

                // si modificano ad ogni cambio di stato
                bool             changedState        = false;
                bool             ph_state            = HIGH;

                bool             lastState           = HIGH;
                bool             isPressed           = false;
                bool             is_ON               = false;
                bool             is_OFF              = false;
                bool             isReleased          = false;


                // vale per i button e la durata della pressione
                uint8_t          pressedLevel        = 0;     // SHORT_PRESS, LONG_PRESS
                uint32_t         startedMillis       = 0ul; //
        const   int32_t         *thresholds;                // pointer to array {400, 1500, 3000, ...} for button pressed length
                int8_t           n_thresholds = 0;   // numero di entries  sizeof(thresholds)/sizeof(uint16_t);

                PINtimer        timer;
    } io_input_pin_struct_t;



    typedef struct  {
                // una volta impostati non si toccano più
        const   char             *name;
                uint8_t          pin          = 0;
                char             pinID[20];  // contiene [pin:%02d.%-15s] p->pin, p->name,
                uint8_t          mode         = OUTPUT; // ${HOME}/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-gpio.h
                bool             active_level = LOW;
                bool             ON           = LOW;    // ON_level
                bool             OFF          = HIGH;   // OFF_level

                // si modificano ad ogni cambio di stato
                bool             changedState = false;
                bool             lastState    = HIGH;
                bool             ph_state     = HIGH;
                bool             is_acted      = HIGH;
                bool             is_ON        = false;
                bool             is_OFF       = false;
                // bool             isReleased   = HIGH;

                bool             alexa_request   = false;
                bool             alexa_status  = false;

                PINpulsetime     pulsetime;
    } io_output_pin_struct_t;




    enum PressType   : uint8_t { NO_BUTTON_PRESSED = 0,
                                PRESSED_TIME_01,
                                PRESSED_TIME_02,
                                PRESSED_TIME_03,
                                PRESSED_TIME_04,
                                PRESSED_TIME_05,
                                PRESSED_TIME_06,
                                OVERFLOW_TIME};

    enum outo_on     : bool { fAUTO_ON_OFF = true, fNO_AUTO_ON_OFF = false};





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


#endif
