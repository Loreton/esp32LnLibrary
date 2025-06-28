/*
# updated by ...: Loreto Notarantonio
# Date .........: 13-05-2024 17.52.27
*/

#include "Arduino.h"
#include "time.h"

#define __HW_TIMERS_CPP__
#include "@hw_timers.h"

// --- others
#include "@lnMacros.h"


#define debug00_fn                    ln_printf_FN
#define debug02_now                   ln_printf_Now
#define debug01_nowfn                 ln_printf_NowFN


const unsigned long long PROGMEM to_uSec = 1000000ull;
structHwTimer_t my_hwtimer[TIMERS_COUNT]; // definisci 4 array
structHwTimer_t *hwtimer0 = &my_hwtimer[0];
structHwTimer_t *hwtimer1 = &my_hwtimer[1];
structHwTimer_t *hwtimer2 = &my_hwtimer[2];
structHwTimer_t *hwtimer3 = &my_hwtimer[3];

structHwTimer_t *hwtimer_1sec, *hwtimer_1min;


// #########################################
// # NON devo leggere il tempo altrimenti va in errore ??????
// # Anzi è meglio non fare nulla in questa routine ma solo impostare un flag ed uscire
// # ...
// # ref: https://stackoverflow.com/questions/71992044/esp32-core-1-paniced-interrupt-wdt-timeout-on-cpu1
// # The code uses serial communication within an interrupt handler (buttonInput()). This does not work.
// #  When an interrupt handler is called, it blocks the entire system.
// # So it must only do minimal work and return quickly.
// # Serial communication however is slow and causes long delays.
// # Thus, your program is terminated by the watchdog ("wdt" is the abbreviation for the watchdog).
// #########################################
void hwtimer0_callBack(void) {
    hwtimer0->expired = true;

    if (!hwtimer0->auto_reload) {
        timerEnd(hwtimer0->timer);
    }

}


// #########################################
// # The code uses serial communication within an interrupt handler (buttonInput()). This does not work.
// #  When an interrupt handler is called, it blocks the entire system.
// # So it must only do minimal work and return quickly.
// # Serial communication however is slow and causes long delays.
// # Thus, your program is terminated by the watchdog ("wdt" is the abbreviation for the watchdog).
// #########################################
void hwtimer1_callBack(void) {
    hwtimer1->expired = true;

    if (!hwtimer1->auto_reload) {
        timerEnd(hwtimer1->timer);
    }
}



// #########################################
// #
// #########################################
void hwtimer2_callBack(void) {
    hwtimer2->expired = true;

    if (!hwtimer2->auto_reload) {
        timerEnd(hwtimer2->timer);
    }
}



// #########################################
// #
// #########################################
void hwtimer3_callBack(void) {
    hwtimer3->expired = true;

    if (!hwtimer3->auto_reload) {
        timerEnd(hwtimer3->timer);
    }
}




// #########################################
// # requisiti
// #    void timer_00_callBack(void);               funzione che sarà chiamata sull'IRQ
// #    #define TIMER_00_CB timer_00_CALL_BACK      define, che punta alla funzione, per evitare di modificare il presente file
// #########################################
// void startHwTimer(uint8_t timer_nr, int secs, bool auto_reload) {
void startHwTimer(structHwTimer_t *t, int secs, bool auto_reload) {
    t->usec = secs*to_uSec;
    t->auto_reload = auto_reload;
    t->expired = false;
    t->aligned = true;

    // ---- se già attivo chiudiamolo
    if (t->timer) {
        timerEnd(t->timer); // valuare se da problemi
        debug00_fn("destroyng previous timer %d\n", t->nr);
        delay(100);
    }


    // Initilise the timer and save pointer into structure
    t->timer = timerBegin(t->nr, 80, t->count_up); // Timer 0, clock divider 80
    if (t->nr == 0)
        timerAttachInterrupt(t->timer, &hwtimer0_callBack, true); // Attach the interrupt handling function
    else if (t->nr == 1)
        timerAttachInterrupt(t->timer, &hwtimer1_callBack, true); // Attach the interrupt handling function
    else if (t->nr == 2)
        timerAttachInterrupt(t->timer, &hwtimer2_callBack, true); // Attach the interrupt handling function
    else if (t->nr == 3)
        timerAttachInterrupt(t->timer, &hwtimer3_callBack, true); // Attach the interrupt handling function
    else return;

    timerAlarmWrite(t->timer, t->usec, t->auto_reload); // Interrupt every 1 second
    timerAlarmEnable(t->timer); // Enable the alarm
    debug01_nowfn("timer %d started\n", t->nr);
}

