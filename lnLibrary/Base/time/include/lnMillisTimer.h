//
// updated by ...: Loreto Notarantonio
// Date .........: 15-02-2025 11.30.28
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <WiFi.h>



#ifndef __LN_MILLIS_TIMER_H__
    #define __LN_MILLIS_TIMER_H__


    typedef struct {
        uint32_t    start = 0;
        bool        running = false;
        bool        expired = false;
        uint32_t    duration = 0;
        bool        enabled = false;
    } structMillisTimer_t;



    #define startMillisTimer restartMillisTimer
    // example :
    // structMillisTimer_t xxxx;
    // structMillisTimer_t *p = &xxxx;
    // setMillisTimer(p, 500);
    // startMillisTimer(p);
    // startMillisTimer(p, 2000);


#endif

    void setMillisTimer(structMillisTimer_t *t, uint32_t ms_interval);
    void startMillisTimer(structMillisTimer_t *t, uint32_t ms_interval=0);
    void stopMillisTimer(structMillisTimer_t *t);
    void checkMillisTimer(structMillisTimer_t *t);
