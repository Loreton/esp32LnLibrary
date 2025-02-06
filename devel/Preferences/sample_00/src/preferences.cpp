/*
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 17.39.19

ref: https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
*/

#include <Arduino.h>
#include <Preferences.h>

#define printf Serial.printf
#define println Serial.println

bool RO_MODE = true;
bool RW_MODE = false;


/*
    ESP32 startup counter example with Preferences library.

    This simple example demonstrates using the Preferences library to store how many times the ESP32 module has booted.
    The Preferences library is a wrapper around the Non-volatile storage on ESP32 processor.

    created for arduino-esp32 09 Feb 2017 by Martin Sloup (Arcao)

    Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/


*/

void sample_01() {
    Preferences preferences;
    // Open Preferences with my-app namespace. Each application module, library, etc
    // has to use a namespace name to prevent key name collisions. We will open storage in
    // RW-mode (second parameter has to be false).
    // Note: Namespace name is limited to 15 chars.
    preferences.begin("my-app", RW_MODE); // false

    // Remove all preferences under the opened namespace
    //preferences.clear();

    // Or remove the counter key only
    //preferences.remove("counter");

    // Get the counter value, if the key does not exist, return a default value of 0
    // Note: Key name is limited to 15 chars.
    unsigned int counter = preferences.getUInt("counter", 0);

    // Increase counter by 1
    counter++;

    // Print the counter to Serial Monitor
    printf("Current counter value: %u\n", counter);

    // Store the counter to the Preferences
    preferences.putUInt("counter", counter);

    // Close the Preferences
    preferences.end();
    #if 0
    // Wait 10 seconds
    printf("Restarting in 10 seconds...\n");
    delay(10000);

    // Restart ESP
    ESP.restart();
    #endif
}




void setup() {
    Serial.begin(115200);
    delay(2000);
    println();
    sample_01();
    println();

}

void loop() {}