/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 15.22.33
*/

#pragma once
#include <Arduino.h>

#define passiveBuzzer_pin            22  // OUTPUT
#define activeBuzzer_pin             23  // OUTPUT
#define pressControlLED_pin          25  // OUTPUT
#define pumpLED_pin                  26  // OUTPUT

#define pressControlRelay_pin        16  // OUTPUT
#define pumpHornAlarm_pin            17  // OUTPUT

#define pressControlState_pin        18  // INPUT
#define pumpState_pin                19  // INPUT
#define startButton_pin              21  // INPUT


// -----------------------------
// - function prototypes...
// -----------------------------
#include "@ledController_Class.h" // Include the new LedController_Class header
#include "@buttonLongPress_Class.h"  // Include the new ButtonLongPress_Class_Class header


// These functions will likely remain external as they operate on pointers
// to your ButtonLongPress_Class objects, similar to how they did with the struct.
// You might consider making them static members of the ButtonLongPress_Class class
// if they truly only relate to that class and don't need external access
// or involve other classes.
void pumpState_Action(ButtonLongPress_Class *p);
void startButton_Action(ButtonLongPress_Class *p);
void processButton(ButtonLongPress_Class *p);