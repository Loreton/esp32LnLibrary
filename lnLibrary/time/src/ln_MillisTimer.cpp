//
// updated by ...: Loreto Notarantonio
// Date .........: 15-02-2025 11.06.05
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
void stopMillisTimer(structMillisTimer_t *t) {
    t->start = 0;
    t->running = false;
    t->expired = false;
    t->enabled = false;
}

void setMillisTimer(structMillisTimer_t *t, uint32_t duration) {
    if (duration != 0)
        t->duration = duration;
    stopMillisTimer(t);
}

void startMillisTimer(structMillisTimer_t *t, uint32_t duration) {
    t->start = millis();
    t->enabled = true;
    t->running = true;
    t->expired = false;
    if (duration != 0)
        t->duration = duration;
}


void checkMillisTimer(structMillisTimer_t *t) {
    if (t->enabled) {
        if ( (millis() - t->start) > t->duration) {
            t->running = false;
            t->expired = true;
            t->enabled = true;
        }
    }
}



