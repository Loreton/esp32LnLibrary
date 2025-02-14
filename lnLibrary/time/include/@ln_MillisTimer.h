//
// updated by ...: Loreto Notarantonio
// Date .........: 14-02-2025 09.30.37
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
        uint32_t    interval = 0;
    } structMillisTimer_t;



    #define startMillisTimer restartMillisTimer


#endif

    void setMillisTimer(structMillisTimer_t *t, uint32_t ms_interval);
    void startMillisTimer(structMillisTimer_t *t, uint32_t ms_interval=0);
    void updateMillisTimer(structMillisTimer_t *t);
