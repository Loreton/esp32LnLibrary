/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 18.06.28
*/



#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>

#include "@pinController_Class.h" // Changed to new class header

// --- Main Functions (setup and loop) for demonstration ---

// Each 'myLedX' variable is a specific instance of PinController_Class
#define activeBuzzer_pin 23
#define LED2_pin 26

// Now these are instances of the PinController_Class class
PinController_Class activeBuzzer("Buzzer", activeBuzzer_pin, HIGH);
PinController_Class myLed2("Led", LED2_pin, HIGH);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting LED module demonstration with classes...");

    // No need to call 'init' explicitly as it's handled by the constructor
    // activeBuzzer.init("Buzzer", activeBuzzer_pin, HIGH); // REMOVE THIS LINE
    // myLed2.init("Led", LED2_pin, HIGH); // REMOVE THIS LINE

    Serial.println("Initialization complete. Starting loop...");
}

void loop() {
    static bool first_run = true;
    static uint32_t startedMillis;
    uint32_t now;

    if (first_run) {
        first_run = false;
        // The blinking_dc function now takes float for duty cycle
        activeBuzzer.blinking_dc(1000, 0.50, 4); // Period, Duty Cycle, Cycles
        myLed2.pulse(3000);
        startedMillis = millis();
    }

    now = millis() - startedMillis;
    // Call update() for each PinController_Class instance you want to manage
    // You MUST call this function for EACH controller individually
    activeBuzzer.update();
    myLed2.update();

    // examples of commands:

    if (now > 20000 && now < 20100) {
        myLed2.blinking_dc(1000, 0.10); // Period, Duty Cycle
        activeBuzzer.pulse(3000);
    }

    if (now > 24000 && now < 24100) {
        activeBuzzer.blinking_dc(1000, 0.70, 5); // Period, Duty Cycle, Cycles
    }

    if (now > 30000 && now < 30100) {
        activeBuzzer.off();
    }

    delay(50);
}
#endif