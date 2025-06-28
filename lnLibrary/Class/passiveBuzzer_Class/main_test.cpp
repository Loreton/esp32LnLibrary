//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 17.15.57
// ref:
// https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__
// file: main.cpp
#include <Arduino.h> // Include Arduino core functionalities

// Define logging levels (ensure these are properly defined in @globalVars.h or elsewhere)
#define LOG_LEVEL_2
#define LOG_LEVEL_99x
#include "@globalVars.h" // Assuming this contains printf_XFN() definitions

#include "ledc_buzzer_Class.h" // Now including the new class header

#define passiveBuzzer_pin 22 // Define the output pin for the buzzer

// --- Example Usage ---

// Frequencies for an approximate C Major scale
int C_major_scale[] = {
    262, // C4
    294, // D4
    330, // E4
    349, // F4
    392, // G4
    440, // A4
    494, // B4
    523  // C5
};
int num_notes_C_major = sizeof(C_major_scale) / sizeof(C_major_scale[0]);

// Create an instance of the buzzer class
#define LEDC_CHANNEL 0
// Changed 'passiveBuzzer_sc' to 'passiveBuzzer_Class'
passiveBuzzer_Class myBuzzer("passiveBz", passiveBuzzer_pin, LEDC_CHANNEL, 10); // Buzzer connected to GPIO 22, LEDC channel 0, 10-bit resolution

void setup() {
    Serial.begin(115200);
    delay(1000);
    printf99_FN("ESP32 Buzzer Class with Scale Example (Fixed ledcSetFreq error)\n");

    myBuzzer.begin(); // Initialize the buzzer
}

void loop() {
    // Call handle() for each buzzer instance in the main loop
    myBuzzer.handle();

    static unsigned long lastActionTime = 0;
    static int step = 0;
    const unsigned long PAUSE_BETWEEN_ACTIONS = 2000; // 2 seconds pause between actions

    // Execute an action only if the buzzer is not playing (neither single tone nor scale)
    // and the pause time has elapsed
    if (!myBuzzer.isPlayingSomething() && (millis() - lastActionTime >= PAUSE_BETWEEN_ACTIONS)) {
        switch (step) {
        case 0:
            myBuzzer.playToneFixed(784, 500); // Single tone G for 500ms
            step = 1;
            break;
        case 1:
            myBuzzer.playScale(C_major_scale, num_notes_C_major, 150, true); // Ascending scale, 150ms per note
            step = 2;
            break;
        case 2:
            myBuzzer.playScale(C_major_scale, num_notes_C_major, 200, false); // Descending scale, 200ms per note
            step = 3;
            break;
        case 3:
            myBuzzer.playToneDutyCycle(1000, 30.0, 600); // Tone with 30% DC for 600ms
            step = 4;
            break;
        default:
            myBuzzer.noTone();
            step = 0; // Restart the sequence
            printf99_FN("Demonstration sequence completed, restarting.\n");
            // Assuming waitForEnter() is defined in @globalVars.h or elsewhere
            // It typically waits for user input on Serial to proceed.
            waitForEnter();
            break;
        }
        lastActionTime = millis(); // Update the time of the last started action
    }
}

#endif