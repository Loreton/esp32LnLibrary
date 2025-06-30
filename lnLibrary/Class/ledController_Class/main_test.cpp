/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 12.39.20
*/



#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>
#define LOG_LEVEL_99
#include "@logMacros.h" // Changed to new class header
#include "@ledController_Class.h" // Changed to new class header

// --- Main Functions (setup and loop) for demonstration ---

// Each 'myLedX' variable is a specific instance of LedController_Class
#define activeBuzzer_pin 23
#define LED2_pin 26

// Now these are instances of the LedController_Class class
// LedController_Class activeBuzzer("Buzzer", activeBuzzer_pin, HIGH);
// LedController_Class myLed2("Led", LED2_pin, HIGH);
LedController_Class activeBuzzer;
LedController_Class myLed2;

void setup() {
    Serial.begin(115200);
    delay(1000);
    printf99_FN("Starting LED module demonstration with classes...\n");

    activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH);
    myLed2.init("Led", LED2_pin, HIGH);
    // No need to call 'init' explicitly as it's handled by the constructor
    // activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH); // REMOVE THIS LINE
    // myLed2.init("Led", LED2_pin, HIGH); // REMOVE THIS LINE

}

void loop() {
    static bool first_run = true;
    static uint32_t startedMillis;
    uint32_t elapsed;

    if (first_run) {
        printf99_FN("Initialization complete. Starting loop...\n");
        first_run = false;
        // The blinking_dc function now takes float for duty cycle
        activeBuzzer.blinking_dc(1000, 0.50, 4); // Period, Duty Cycle, Cycles
        myLed2.pulse(3000);
        startedMillis = millis();
    }

    elapsed = millis() - startedMillis;
    // printf99_FN("sono qui...%lu millis\n", elapsed);
    // Call update() for each LedController_Class instance you want to manage
    // You MUST call this function for EACH controller individually
    activeBuzzer.update();
    myLed2.update();

    // examples of commands:

    if (elapsed > 20000 && elapsed < 20100) {
        printf99_FN("superato 20000...%lu millis\n", elapsed);
        myLed2.blinking_dc(1000, 0.10); // Period, Duty Cycle
        activeBuzzer.pulse(3000);
    }

    if (elapsed > 24000 && elapsed < 24100) {
        printf99_FN("superato 24000...%lu millis\n", elapsed);
        activeBuzzer.blinking_dc(1000, 0.70, 5); // Period, Duty Cycle, Cycles
    }

    if (elapsed > 30000 && elapsed < 30100) {
        printf99_FN("superato 30000...%lu millis\n", elapsed);
        activeBuzzer.off();
    }

    delay(50);
}
#endif