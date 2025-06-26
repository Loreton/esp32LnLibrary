/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.58.24
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
    #include "@pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto

    void pumpState_Action(pinLongPress_Struct *p);
    void startButton_Action(pinLongPress_Struct *p);
    void processButton(pinLongPress_Struct *p);
