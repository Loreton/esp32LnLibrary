//
// updated by ...: Loreto Notarantonio
// Date .........: 10-02-2025 19.12.57
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <WiFi.h>



#ifndef __LN_MILLIS_TIMER_H__
    #define __LN_MILLIS_TIMER_H__


    typedef struct {
        uint32_t start = 0;
        uint32_t interval_lentgh = 0;
        bool     running = false;
        bool     expired = false;
    } structMillisTimer;

    #define restartMillisTimer startMillisTimer


#endif

    // void startMillisTimer(structMillisTimer *t, uint32_t interval=0);
    void restartMillisTimer(structMillisTimer *t, uint32_t interval=0);
    void updateMillisTimer(structMillisTimer *t);