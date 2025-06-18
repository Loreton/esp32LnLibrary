#include "RelayManager.h"

#define RELAY_PIN 16

Relay relay;

void setup() {
    Serial.begin(115200);
    relayInit(&relay, RELAY_PIN, "RELAY_PIN", HIGH);
    relayStartPulse(&relay, 3000); // 3 secondi
}

void loop() {
    relayUpdate(&relay, millis());
    Serial.print("Relay stato: ");
    Serial.print(relayGetState(&relay));
    Serial.print(", tempo rimanente: ");
    Serial.println(relay.pulseRemainMs);
    delay(100);
}