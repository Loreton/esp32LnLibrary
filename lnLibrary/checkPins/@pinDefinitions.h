//
// updated by ...: Loreto Notarantonio
// Date .........: 05-08-2025 18.35.59
//

#include <Arduino.h>

// #include "pins_Initialization.h"
#pragma once

/*
ESP32 Pinout Description https://www.teachmemicro.com/esp32-pinout-diagram-wroom-32/
The ESP32 pins are categorized into digital pins, analog pins, and power pins.
Refer to the table below for details on pins with secondary functions.
Moreover, these secondary pins often serve communication purposes, such as I2C and SPI or as ADC channels.

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
5V  5   V power supply                                      CLK     GPIO6  CLK     2
*/


//                       GPIO         Name  Descr                           Input       Output   Notes
    #define GPIO00        0        // avoid - IO0   ADC2_CH1 TOUCH_CH1 Boot         pulled up   OK       outputs PWM signal at boot, must be LOW to enter flashing mode
    #define GPIO01        1        // avoid - U0TXD -                               --          OK       TX pin debug output at boot
    #define GPIO02        2        // avoid - IO2   ADC2_CH2 TOUCH_CH2              OK          OK       GPIO1, TXD connected to on-board LED, must be left floating or LOW to enter flashing mode
    #define GPIO03        3        // avoid - RX    U0RXD                           OK          --       GPIO3, RXD HIGH at boot
    #define GPIO04        4        // good  - IO4   ADC2_CH0 TOUCH_CH0              OK          OK       ADC2_CH0 TOUCH_CH0
    #define GPIO05        5        // avoid - IO5                                   OK          OK       outputs PWM signal at boot, strapping pin

    #define GPIO06        6        // avoid - CLK   CLK 2                           connected to the integrated SPI flash
    #define GPIO07        7        // avoid - D0    D0 2                            connected to the integrated SPI flash
    #define GPIO08        8        // avoid - D1    D1 2                            connected to the integrated SPI flash
    #define GPIO09        9        // avoid - D2    D2 2                            connected to the integrated SPI flash
    #define GPIO10       10        // avoid - D3    D3 2                            connected to the integrated SPI flash
    #define GPIO11       11        // avoid - CMD   CMD 2                           connected to the integrated SPI flash

    #define GPIO12       12        // good  - IO12  ADC2_CH5  TOUCH_CH5 MTDI        OK          OK       boot fails if pulled high, strapping pin
    #define GPIO13       13        // good  - IO13  ADC2_CH4  TOUCH_CH4 MTCK        OK          OK       ADC2_CH4  TOUCH_CH4 MTCK
    #define GPIO14       14        // good  - IO14  ADC2_CH6  TOUCH_CH6 MTMS        OK          OK       outputs PWM signal at boot
    #define GPIO15       15        // good  - IO15  ADC2_CH3 TOUCH_CH3 MTDO         OK          OK       outputs PWM signal at boot, strapping pin
    #define GPIO16       16        // good  - IO16  3                               OK          OK       RELAY1
    #define GPIO17       17        // good  - IO17  3                               OK          OK       RELAY2
    #define GPIO18       18        // good  - IO18                                  OK          OK
    #define GPIO19       19        // good  - IO19                                  OK          OK
    #define GPIO21       21        // good  - IO21                                  OK          OK
    #define GPIO22       22        // good  - IO22                                  OK          OK
    #define GPIO23       23        // good  - IO23                                  OK          OK       LED_ON_BOARD
    #define GPIO25       25        // good  - IO25 ADC1_CH8  DAC_1                  OK          OK
    #define GPIO26       26        // good  - IO26 ADC2_CH9  DAC_2                  OK          OK
    #define GPIO27       27        // good  - IO27 ADC2_CH7  TOUCH_CH7OK            OK
    #define GPIO32       32        // good  - IO32 ADC1_CH4  TOUCH_CH9 XTAL_32K_P   OK          OK
    #define GPIO33       33        // good  - IO33 ADC1_CH5  TOUCH_CH8 XTAL_32K_N   OK          OK
    #define GPIO34       34        // good  - IO34 ADC1_CH6  VDET_1                 OK          --   input only (PULL_DOWN)
    #define GPIO35       35        // good  - IO35 ADC1_CH7  VDET_2                 OK          --   input only (PULL_DOWN)

    #define GPIO36       36        // avoid - VP   ADC1_CH0  S_VP                   OK          --   input only (NOT found)
    #define GPIO39       39        // avoid - VN   ADC1_CH3  S_VN TX                OK          --   input only (NOT found)


