//
// updated by ...: Loreto Notarantonio
// Date .........: 11-02-2025 20.17.03
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <WiFi.h>



#ifndef __LN_MILLIS_TIMER_H__
    #define __LN_MILLIS_TIMER_H__


    // typedef struct {
    //     uint32_t start = 0;
    //     uint32_t interval_lentgh = 0;
    //     bool     running = false;
    //     bool     expired = false;
    // } structTimer_t;

    typedef struct {
        uint32_t start = 0;
        uint32_t interval_lentgh = 0;
        bool     running = false;
        bool     expired = false;
    } structTimer_t;

    #define startMillisTimer restartMillisTimer
    #define startTimer restartTimer


#endif

    // void startTimer(structTimer *t, uint32_t interval=0);
    void setMillisTimer(structTimer_t *t, uint32_t ms_interval);
    void startMillisTimer(structTimer_t *t, uint32_t ms_interval=0);
    void updateMillisTimer(structTimer_t *t);

    void setTimer(structTimer_t *t, uint32_t secondsl);
    void setTimer(structTimer_t *t, int8_t hh, int8_t mm=0, int8_t ss=0);
    void startTimer(structTimer_t *t, uint32_t seconds=0);
    void updateTimer(structTimer_t *t);

    bool isMinuteOClock(void);
    bool isQuarterOClock(void);

