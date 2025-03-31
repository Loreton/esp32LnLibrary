//
// updated by ...: Loreto Notarantonio
// Date .........: 31-03-2025 09.16.06
//

#include <Arduino.h> // in testa anche per le definizioni dei type

void copyA(int* src, int* dst, int len) {
    Serial.printf("src0: %d", *src);
    memcpy(dst, src, sizeof(src[0])*len);
}

void copyB(int* src, int* dst, int len) {
    for (int i = 0; i < len; i++) {
        *dst++ = *src++;
    }
}

void copyC(int* src, int* dst, int len) {
    for (int i = 0; i < len; i++) {
        Serial.printf("counter: %d", i);
        // dst[i] = src[i];
    }
}

void setup() {
    int start, finish, elements = 1000;
    int source[elements], destination[elements];

    for (int i = 0; i < elements; i++) {
        source[i] = i;
    }

    Serial.begin(115200);
    delay(2200);

    Serial.print("memcpy() time in µs: ");
    // start = micros();
    copyA(source, destination, elements);
    // finish = micros() - start;
    Serial.println(finish);

    // Serial.print("pointers time in µs: ");
    // start = micros();
    // copyB(source, destination, elements);
    // finish = micros() - start;
    // Serial.println(finish);

    // Serial.print("indexing time in µs: ");
    // start = micros();
    // copyC(&source[0], &destination[0], elements);
    // finish = micros() - start;
    // Serial.println(finish);
}

void loop() {
    // Nothing to do here
}