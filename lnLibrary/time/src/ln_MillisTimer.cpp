//
// updated by ...: Loreto Notarantonio
// Date .........: 14-02-2025 09.30.49
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

void setMillisTimer(structMillisTimer_t *t, uint32_t interval) {
    t->interval = interval;
}

void startMillisTimer(structMillisTimer_t *t, uint32_t interval) {
    t->start = millis();
    t->running = true;
    t->expired = false;
    if (interval != 0)
        t->interval = interval;
}


void updateMillisTimer(structMillisTimer_t *t) {
    if ( (millis() - t->start) > t->interval) {
        t->start = millis();
        t->running = false;
        t->expired = true;
    }
}


