/*
// updated by ...: Loreto Notarantonio
// Date .........: 24-06-2025 10.01.00
*/

#pragma once
#include <Arduino.h>

#define passiveBuzzer_pin           22  // OUTPUT
#define activeBuzzer_pin            23  // OUTPUT
#define pressControlLED_pin         25  // OUTPUT
#define pumpLED_pin                 26  // OUTPUT

#define pressControlRelay_pin       16  // OUTPUT
#define pumpHornAlarm_pin           17  // OUTPUT

#define pressControlState_pin       18  // INPUT
#define pumpState_pin               19  // INPUT
#define startButton_pin             21  // INPUT


// -----------------------------
// - function ptototypes...
// can be used as samples for other projects
// -----------------------------
    #include "@pinController_sc.h" // per l'active buzzer per inviare un beep durante la pressione del tasto

    void pumpState_Action(pinLongPress_sc *p);
    void startButton_Action(pinLongPress_sc *p);
    void processButton(pinLongPress_sc *p);
