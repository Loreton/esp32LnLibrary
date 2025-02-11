/*
# updated by ...: Loreto Notarantonio
# Date .........: 19-07-2024 16.33.26
*/
#include <Arduino.h>

#ifndef __HW_TIMERS_H__
    #define __HW_TIMERS_H__

    typedef struct {
        bool                    aligned     = false;  // alineato al secondo/minuto
        bool                    count_up    = true;
        int8_t                  nr          = 0;
        unsigned long long      usec        = 999999999999ull;
        bool                    auto_reload = false;
        bool                    expired     = false;
        hw_timer_t              *timer      = NULL;
    } structHwTimer_t;


    enum timers_enum  : byte      { TIMER_0, TIMER_1,  TIMER_2,  TIMER_3, TIMERS_COUNT};


    extern structHwTimer_t my_hwtimer[];
    extern structHwTimer_t *hwtimer0, *hwtimer1, *hwtimer2, *hwtimer3;
    extern structHwTimer_t *hwtimer_1sec, *hwtimer_1min;




    // --------------------------------------------------
    // --- functions prototypes
    // --------------------------------------------------
    void hwtimer_setup(structHwTimer_t *t);
    void initialize_timer_struct(void);
    // void startHwTimer(uint8_t timer_nr, int sec=1, bool autoreload=false);
    void startHwTimer(structHwTimer_t *t, int secs=1, bool auto_reload=false);




#endif