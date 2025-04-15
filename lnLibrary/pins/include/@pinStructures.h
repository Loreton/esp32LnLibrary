//
// updated by ...: Loreto Notarantonio
// Date .........: 15-04-2025 16.43.50
//

#include <Arduino.h>

#ifndef __LN_PIN_STRUCTURES_H__
    #define __LN_PIN_STRUCTURES_H__




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
                bool             ON                  = LOW;
                bool             OFF                 = HIGH;
                bool             active_level        = LOW;

                // si modificano ad ogni cambio di stato
                bool             changedState        = false;
                bool             ph_state            = HIGH;

                bool             lastState           = HIGH;
                bool             isPressed           = false;
                // bool             is_acted            = false; // per compatibilità con output pin (sinonimo di isPressed )
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
                bool             ON           = LOW;
                bool             OFF          = HIGH;

                // si modificano ad ogni cambio di stato
                bool             changedState = false;
                bool             lastState    = HIGH;
                bool             ph_state     = HIGH;
                bool             is_acted      = HIGH;
                // bool             is_ON         = false;
                // bool             is_OFF        = false;
                // bool             isON         = HIGH;
                bool             is_ON        = false;
                bool             is_OFF       = false;
                bool             isReleased   = HIGH;

                bool             alexa_request   = false;
                bool             alexa_status  = false;

                PINpulsetime     pulsetime;
    } io_output_pin_struct_t;





#endif
