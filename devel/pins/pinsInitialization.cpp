/*
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 08.57.40
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type


#include "@pinDefinitions.h"

// --- others
#include "@lnMacros.h"
#include "@lnString.h"

// #include "@lnFunctions.h"

#define pininit_dbg0       ln_printf_FN

// local prototype

const char PROGMEM *str_action[]     = {"released", "pressed"};
const char PROGMEM *str_pinLevel[]   = {"LOW", "HIGH"};
const char PROGMEM *str_ON           = "ON";
const char PROGMEM *str_OFF          = "OFF";
const char PROGMEM *str_INPUT        = "INPUT";
const char PROGMEM *str_INPUT_PULLUP = "INPUT_PULLUP";
const char PROGMEM *str_OUTPUT       = "OUTPUT";


/*
    valori di soglia per pressione tasti
*/
const char * PROGMEM THRESHOLD_LEVEL_TYPES[]    = {"SHORT_PRESS", "NORMAL_PRESS", "MEDIUM_PRESS", "LONG_PRESS", "LONG_LONG_PRESS"};
const int8_t THRESHOLDS_LEVELS_TYPES_length = sizeof(THRESHOLD_LEVEL_TYPES)/sizeof(char *);

// ---------------------------------------------------------- short medium normal long   longlong  endOfArray
const int16_t PROGMEM startButton_thresholds_level_values[] = {0,   400,   1500,   2500,  3500,     -1}; // l'ultimo valore deve essere negativo and indicare fine array
const int16_t PROGMEM base_thresholds_level_values[]        = {0,   400, -1}; // l'ultimo valore deve essere negativo and indicare fine array


const int8_t INPUT_PINS  = 3;
const int8_t OUTPUT_PINS = 6;

io_input_pin_struct_t inpPINs[INPUT_PINS+1];
io_input_pin_struct_t *pumpState           = &inpPINs[0]; // input
io_input_pin_struct_t *startButton         = &inpPINs[1]; // input
io_input_pin_struct_t *pressControlState   = &inpPINs[2]; // input


io_output_pin_struct_t outPINs[OUTPUT_PINS+1];
io_output_pin_struct_t *pressControlRelay  = &outPINs[0];
io_output_pin_struct_t *loadSuperioreRelay = &outPINs[1];
io_output_pin_struct_t *activeBuzzer       = &outPINs[2];
io_output_pin_struct_t *passiveBuzzer      = &outPINs[3];
io_output_pin_struct_t *LED                = &outPINs[5];
io_output_pin_struct_t *led_internal       = &outPINs[4];

char msg[] = "Loreto   ciao    300     4000     50000";
char *my_word=getWord(msg, " ", 1); // just for testing

/*
    #########################################
    # array ending with negative number
    # iint16_t array[] = {400, 1500, 3000, 4000, -1};
    #########################################
*/
int8_t intArrayLength(const int16_t *p) {
    uint8_t lun=0;
    while (*p >= 0) {
        // pininit_dbg0("lun: %d - value: %d\n", lun, *p);
        lun++;
        p++;
    }
    return lun;
}

/*
    #########################################
    # initializzazione pin
    #########################################
*/
void inputPinInit(const char *name, uint8_t nr, io_input_pin_struct_t *p, uint8_t mode, bool active_level, const int16_t *thresholds) {
    // --- non verranno più modificati
    p->name           = name;
    p->pin            = nr;
    p->mode           = mode;
    p->active_level   = active_level;
    p->OFF            = !active_level;
    p->ON             = active_level;
    // p->strActiveLevel = str_pinLevel[p->active_level]; // text visualization

    p->thresholds     = thresholds;
    p->thresholds_len = intArrayLength(thresholds); // --- calculate thresholds array length

    //  verifica per sicurezza
    if (p->thresholds_len > THRESHOLDS_LEVELS_TYPES_length) {
        pininit_dbg0("p->thresholds_len (%d) > THRESHOLDS_LEVELS_TYPES_length (%d)... changing to: (%d)", p->thresholds_len, THRESHOLDS_LEVELS_TYPES_length, THRESHOLDS_LEVELS_TYPES_length);
        p->thresholds_len = THRESHOLDS_LEVELS_TYPES_length; // per sicurezza
    }


    pinMode(p->pin, p->mode);

    p->isPressed = false;
    p->isReleased = false;
    p->changedState = false;
    p->state        = digitalRead(p->pin);
    p->lastState    = p->state;

    delay(50);

}


/*
    #########################################
    # initializzazione pin
    #########################################
*/
void outputPinInit(const char *name, uint8_t nr, io_output_pin_struct_t *p, uint8_t mode, bool active_level) {
    // --- non verranno più modificati
    // --- non verranno più modificati
    p->name           = name;
    p->pin            = nr;
    p->mode           = mode;
    p->active_level   = active_level;
    p->OFF            = !active_level;
    p->ON             = active_level;
    // p->strActiveLevel = str_pinLevel[p->active_level]; // text visualization

    // --- process pin MODE
    if ( (p->mode == OUTPUT) || (p->mode == OUTPUT_OPEN_DRAIN) ) {
        // p->strMode = str_OUTPUT; // text visualization
        digitalWrite(p->pin, p->OFF);
        delay(50);
    }

    pinMode(p->pin, p->mode);

    p->pulsetime = 0l;
    p->remaining = 0l;


    delay(50);
    // displayOutputPin(p);
}







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
void pinsInitialization(void) {
    //====================================================
    //= set input pins
    //====================================================
    // ------  name,                 pin_nr                 , pin_struct       , mode (inp/out), active_level);
    inputPinInit("Pump"              , pumpState_pin         , pumpState         , INPUT_PULLUP , LOW , &base_thresholds_level_values[0]);
    inputPinInit("PressControlState" , pressControlState_pin , pressControlState , INPUT_PULLUP , LOW , &base_thresholds_level_values[0]);
    inputPinInit("startButton"       , startButton_pin       , startButton       , INPUT_PULLUP , LOW , &startButton_thresholds_level_values[0]);



    //====================================================
    //= set output pins
    //====================================================
    // ------  name,                 pin_nr                 , pin_struct       , mode (inp/out), active_level);
    // inputPinInit("relay02"            , relay02_pin           , relay02           , OUTPUT    , LOW, &base_thresholds_level_values[0] );
    outputPinInit("PressControlRelay" , pressControlRelay_pin , pressControlRelay  , OUTPUT , LOW);
    outputPinInit("loadSuperiore"     , loadSuperiore_pin     , loadSuperioreRelay , OUTPUT , LOW);
    outputPinInit("LED"               , LED_pin               , LED                , OUTPUT , LOW);
    outputPinInit("led_internal_pin"  , led_internal_pin      , led_internal       , OUTPUT , LOW);
    outputPinInit("activeBuzzer"      , activeBuzzer_pin      , activeBuzzer       , OUTPUT , HIGH);
    outputPinInit("passiveBuzzer"     , passiveBuzzer_pin     , passiveBuzzer      , OUTPUT , HIGH);
}





