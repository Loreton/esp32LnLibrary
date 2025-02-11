//
// updated by ...: Loreto Notarantonio
// Date .........: 22-01-2025 11.42.35
//

#include <Arduino.h> // in testa anche per le definizioni dei type



void _checkOutputPin(uint8_t pin) {

    Serial.printf("working on pin: %d ", pin);

    delay(1000); Serial.print(" - LOW"); digitalWrite(pin, LOW); pinMode(pin, OUTPUT);
    delay(1000); Serial.print(" - HIGH");  digitalWrite(pin, HIGH);
    delay(1000); Serial.print(" - LOW"); digitalWrite(pin, LOW);
    delay(1000); pinMode(pin, INPUT);

    Serial.printf("\n");
}



void discovery_ESP32_Pins(void) {
    const uint8_t out_pins[] = {0,  2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
    uint8_t count = sizeof(out_pins);
    Serial.printf("numero di pins %d\n", count);

    for (int i = 0; i < count; ++i) {
        uint8_t pin = out_pins[i];
        _checkOutputPin(pin);

    }

}


