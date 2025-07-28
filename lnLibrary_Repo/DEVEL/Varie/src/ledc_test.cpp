#include <Arduino.h>
#include <driver/ledc.h> // Add this line

void setup() {
    Serial.begin(115200);
    delay(2000);
    ledcSetup(0, 1000, 10);     // canale 0, 1 kHz, risoluzione 10 bit
    ledcAttachPin(4, 0);        // GPIO 4 al canale 0
    ledcWrite(0, 512);          // 50% duty cycle
    Serial.println("Buzzer attivato");
}

void loop() {}