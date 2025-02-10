//
// updated by ...: Loreto Notarantonio
// Date .........: 28-01-2025 21.05.54
//

#include <Arduino.h>

// #include "pins_Initialization.h"

#ifndef __PINS_DEFINITIONS_H__
    #define __PINS_DEFINITIONS_H__

/*
ESP32 Pinout Description https://www.teachmemicro.com/esp32-pinout-diagram-wroom-32/
The ESP32 pins are categorized into digital pins, analog pins, and power pins.
Refer to the table below for details on pins with secondary functions. Moreover, these secondary pins often serve communication purposes, such as I2C and SPI or as ADC channels.

Pin Name            Description                             Pin     Name                Description
3V3     3.3         V power supply                          GND     Ground
EN      CHIP_PU,    Reset                                   IO23    GPIO23
VP      GPIO36      ADC1_CH0  S_VP                          IO22    GPIO22
VN      GPIO39      ADC1_CH3  S_VN  TX                      GPIO1   U0TXD
IO34    GPIO34      ADC1_CH6  VDET_1                        RX      GPIO3  U0RXD
IO35    GPIO35      ADC1_CH7  VDET_2                        IO21    GPIO21
IO32    GPIO32      ADC1_CH4  TOUCH_CH9 XTAL_32K_P          GND     Ground
IO33    GPIO33      ADC1_CH5  TOUCH_CH8 XTAL_32K_N          IO19    GPIO19
IO25    GPIO25      ADC1_CH8  DAC_1                         IO18    GPIO18
IO26    GPIO26      ADC2_CH9  DAC_2                         IO5     GPIO5
IO27    GPIO27      ADC2_CH7  TOUCH_CH7                     IO17    GPIO17 3
IO14    GPIO14      ADC2_CH6  TOUCH_CH6 MTMS                IO16    GPIO16 3
IO12    GPIO12      ADC2_CH5  TOUCH_CH5 MTDI                IO4     GPIO4  ADC2_CH0 TOUCH_CH0
GND     Ground                                              IO0     GPIO0  ADC2_CH1 TOUCH_CH1 Boot
IO13    GPIO13      ADC2_CH4  TOUCH_CH4 MTCK                IO2     GPIO2  ADC2_CH2 TOUCH_CH2
D2      GPIO9       D2 2                                    IO15    GPIO15 ADC2_CH3 TOUCH_CH3 MTDO
D3      GPIO10      D3 2                                    D1      GPIO8  D1 2
CMD     GPIO11      CMD 2                                   D0      GPIO7  D0 2
5V  5   V power supply  CLK GPIO6 ,                         CLK     2
*/


//                   GPIO          Input       Output   Notes
    #define D00        0        // pulled up   OK       outputs PWM signal at boot, must be LOW to enter flashing mode
    #define D01        1        // --          OK       TX pin debug output at boot
    #define D02        2        // OK          OK       GPIO1, TXD connected to on-board LED, must be left floating or LOW to enter flashing mode
    #define D03        3        // OK          --       GPIO3, RXD HIGH at boot
    #define D04        4        // OK          OK       ADC2_CH0 TOUCH_CH0
    #define D05        5        // OK          OK       outputs PWM signal at boot, strapping pin

    #define D06        6        // avoid - connected to the integrated SPI flash
    #define D07        7        // avoid - connected to the integrated SPI flash
    #define D08        8        // avoid - connected to the integrated SPI flash
    #define D09        9        // avoid - connected to the integrated SPI flash
    #define D10       10        // avoid - connected to the integrated SPI flash
    #define D11       11        // avoid - connected to the integrated SPI flash

    #define D12       12        // OK          OK       boot fails if pulled high, strapping pin
    #define D13       13        // OK          OK       ADC2_CH4  TOUCH_CH4 MTCK
    #define D14       14        // OK          OK       outputs PWM signal at boot
    #define D15       15        // OK          OK       outputs PWM signal at boot, strapping pin
    #define D16       16        // OK          OK       RELAY1
    #define D17       17        // OK          OK       RELAY2
    #define D18       18        // OK          OK
    #define D19       19        // OK          OK
    #define D21       21        // OK          OK
    #define D22       22        // OK          OK
    #define D23       23        // OK          OK       LED_ON_BOARD
    #define D25       25        // OK          OK
    #define D26       26        // OK          OK
    #define D27       27        // OK          OK
    #define D32       32        // OK          OK
    #define D33       33        // OK          OK
    #define D34       34        // OK          --   input only
    #define D35       35        // OK          --   input only
    #define D36       36        // OK          --   input only
    #define D39       39        // OK          --   input only


    #define pumpState_pin               D02  // INPUT
    #define startButton_pin             D03  // INPUT
    #define pressControlState_pin       D04  // INPUT

    #define activeBuzzer_pin            D12  // OUTPUT
    #define passiveBuzzer_pin           D13  // OUTPUT
    #define LED_pin                     D22  // OUTPUT

    #define pressControlRelay_pin       D16  // OUTPUT  mandatory on Esp32_X2_realy_board
    #define loadSuperiore_pin           D17  // OUTPUT  mandatory on Esp32_X2_realy_board
    #define led_internal_pin            D23  // OUTPUT  mandatory on Esp32_X2_realy_board


    // #define BUTTON_NO_PRESS         0
    // #define BUTTON_NORMAL_PRESS     1
    // #define BUTTON_MEDIUM_PRESS     2
    // #define BUTTON_LONG_PRESS       3
    // #define BUTTON_LONG_LONG_PRESS  4

    enum ErrorCode { ERROR_01 = 1, ERROR_10 = 10 };
    enum PressType { BUTTON_NO_PRESS = 0, BUTTON_NORMAL_PRESS, BUTTON_MEDIUM_PRESS, BUTTON_LONG_PRESS, BUTTON_LONG_LONG_PRESS  };




    typedef struct  {

        const   char             *name;
        const   char             *strMode;                  // "INPUT" or.....
                uint8_t          pin                = 0;
                uint8_t          mode               = INPUT_PULLUP; // ${HOME}/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-gpio.h
                bool             ON                 = LOW;
                bool             OFF                = HIGH;
                bool             changedState       = false;
                bool             state              = HIGH;
                bool             lastState          = HIGH;
                bool             active_level       = LOW;
                bool             isPressed          = false;
                bool             isReleased         = false;
                uint8_t          lastPressedLevel   = 0;
                uint8_t          pressedLevel       = 0;     // SHORT_PRESS, LONG_PRESS
                unsigned long    startedMillis       = 0ul;
        const   int16_t         *thresholds;                // pointer to array {400, 1500, 3000, ...} for button pressed length
                int8_t           thresholds_len      = 0;   // numero di entries  sizeof(thresholds)/sizeof(uint16_t);
    } io_input_pin_struct_t;



    typedef struct  {

        const   char             *name;
        const   char             *strMode;        // "INPUT" or.....
                uint8_t          pin                = 0;
                uint8_t          mode               = OUTPUT; // ${HOME}/.platformio/packages/framework-arduinoespressif32/cores/esp32/esp32-hal-gpio.h
                bool             active_level       = LOW;
                bool             ON                 = LOW;
                bool             OFF                = HIGH;
                bool             state              = HIGH;
                // bool             lastState          = HIGH;

                int32_t          pulsetime       = 0;
                int32_t          remaining       = 0;
                int32_t          start_time       = 0; // epoch time
                int32_t          end_time       = 0; // epoch time
    } io_output_pin_struct_t;






    extern const char *str_action[];
    extern const char *str_pinLevel[];
    extern const char *str_INPUT;
    extern const char *str_INPUT_PULLUP;
    extern const char *str_OUTPUT;
    extern const int8_t INPUT_PINS;
    extern const int8_t OUTPUT_PINS;



    extern io_input_pin_struct_t PINs[];
    extern io_input_pin_struct_t *pumpState; // input
    extern io_input_pin_struct_t *startButton; // input
    extern io_input_pin_struct_t *pressControlState; // input

    extern io_output_pin_struct_t *pressControlRelay;
    extern io_output_pin_struct_t *loadSuperioreRelay;
    extern io_output_pin_struct_t *activeBuzzer;
    extern io_output_pin_struct_t *passiveBuzzer;
    extern io_output_pin_struct_t *LED;
    extern io_output_pin_struct_t *led_internals;
    // extern io_output_pin_struct_t *relay02;

    extern const char * PROGMEM THRESHOLD_LEVEL_TYPES[];


    // --------------------------------------------------
    // --- functions prototypes
    // --------------------------------------------------
    // ---- readPin.cpp








    // ---- pins_Initialize.cpp
    // void pinInitialize(void);
    void pinsInitialization(void);





#endif
