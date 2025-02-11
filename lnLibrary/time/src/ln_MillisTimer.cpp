//
// updated by ...: Loreto Notarantonio
// Date .........: 11-02-2025 20.52.58
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

void setMillisTimer(structTimer_t *t, uint32_t interval) {
    t->interval_lentgh = interval;
}

void startMillisTimer(structTimer_t *t, uint32_t interval) {
    t->start = millis();
    t->running = true;
    t->expired = false;
    if (interval != 0)
        t->interval_lentgh = interval;
}


void updateMillisTimer(structTimer_t *t) {
    if ( (millis() - t->start) > t->interval_lentgh) {
        t->start = millis();
        t->running = false;
        t->expired = true;
    }
}



// #########################################
// # con EPOCH time
// #########################################

void setTimer(structTimer_t *t, uint32_t seconds) {
    t->interval_lentgh = seconds;
}

void setTimer(structTimer_t *t, int8_t hh, int8_t mm, int8_t ss) {
    t->interval_lentgh = hh*3600 + mm*60 + ss; // da valutare cosa meglio
}


void starTimer(structTimer_t *t, uint32_t seconds) {
    t->start = rtc.getEpoch(); //timeinfo2.tm_hour, timeinfo2.tm_min, timeinfo2.tm_sec
    t->running = true;
    t->expired = false;
    if (seconds != 0)
        t->interval_lentgh = seconds;
}


void updateTimer(structTimer_t *t) {
    uint32_t epoch=rtc.getEpoch();
    if ( (epoch - t->start) > t->interval_lentgh) {
        t->start = epoch;
        t->running = false;
        t->expired = true;
    }
}

#if 0
bool isMinuteOClock() {
    static int8_t last_minute=-1;
    timeinfo2 = rtc.getTimeStruct();
    if (timeinfo2.tm_sec == 0) {
        if (timeinfo2.tm_min == last_minute) {
            return false;
        } else {
            last_minute = timeinfo2.tm_min;
            return true;
        }
    }
    return false;
}
#endif

bool isMinuteOClock() {
    static int8_t last_minute=-1;
    timeinfo2 = rtc.getTimeStruct();
    if (timeinfo2.tm_sec == 0 && timeinfo2.tm_min != last_minute) {
        last_minute = timeinfo2.tm_min;
        return true;
    }
    return false;
}

bool isQuarterOClock() {
    static int8_t last_minute=-1;
    timeinfo2 = rtc.getTimeStruct();

    if (timeinfo2.tm_min%15 == 0 && timeinfo2.tm_min != last_minute) {
        last_minute = timeinfo2.tm_min;
        return true;
    }
    return false;
}



