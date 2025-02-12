//
// updated by ...: Loreto Notarantonio
// Date .........: 12-02-2025 14.49.55
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <ESP32Time.h> // ESP32Time.cpp



#include "@ln_MillisTimer.h"

// struct tm timeinfo2;
extern ESP32Time     rtc;
struct tm timeinfo2;



// #########################################
// # con millis() time
// #########################################

void setMillisTimer(structMillisTimer_t *t, uint32_t pulsetime) {
    t->pulsetime = pulsetime;
}

void startMillisTimer(structMillisTimer_t *t, uint32_t pulsetime) {
    t->start = millis();
    t->running = true;
    t->expired = false;
    if (pulsetime != 0)
        t->pulsetime = pulsetime;
}


void updateMillisTimer(structMillisTimer_t *t) {
    if ( (millis() - t->start) > t->pulsetime) {
        t->start = millis();
        t->running = false;
        t->expired = true;
    }
}


