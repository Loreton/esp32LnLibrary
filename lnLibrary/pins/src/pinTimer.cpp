/*
// updated by ...: Loreto Notarantonio
// Date .........: 15-04-2025 18.48.52
*/


// --- others
// #include "@lnMacros.h"
#include "@pinStructures.h"
#include "@ln_time.h" // to_HHMMSS()

// ---------------------------------
// macros Aliases for LOG
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"




// #########################################
// # Ex.: setting del pulsetime;
// #########################################
void pinTimerStop(io_input_pin_struct_t *p) {
    p->timer.enabled = false;
    p->timer.cur_duration = 0;
    p->timer.msecs_start  = 0;
}


// #########################################
// # Ex.: start del pulsetime;
// #########################################
void pinTimerStart(io_input_pin_struct_t *p, int32_t msec_length) {

    // --- if temporary value overrides default
    if (msec_length > 0) {
        p->timer.cur_duration = msec_length; // in millisecondi
    }

    // --- if default value
    else if (p->timer.msecs_default != 0) { // carichiamo  il default
        p->timer.cur_duration = p->timer.msecs_default; // in millisecondi
    }
    else {
        p->timer.cur_duration = 0; // no pulseTime
    }

    if (p->timer.cur_duration != 0) {
        p->timer.enabled = true;
        printf1_NFN("%s starting timer: %d msecs\n", p->pinID, p->timer.cur_duration);
        p->timer.msecs_start = millis(); // in millisecondi l'orario di fine
    }

}

// #########################################
// # Ex.: check if pulsetime;expired
// #########################################
bool pinTimerExpired(io_input_pin_struct_t *p) {
    bool expired = false;

    if (p->timer.cur_duration != 0 ) {
        uint32_t elapsed = millis() - p->timer.msecs_start;
        if ( elapsed > p->timer.cur_duration ) {
            printf1_NFN("%s timer msecs elapsed: %ld\n", p->pinID, elapsed);
            pinTimerStop(p);
            expired = true;
        }
    }
    return expired;
}


// #########################################
// # Ex.: check if pulsetime;expired
// #########################################
uint32_t pinTimerRemaining(io_input_pin_struct_t *p) {
    PINtimer *ptr = &p->timer;
    uint32_t remaining=0;

    if (ptr->cur_duration != 0 ) {
        remaining = ptr->cur_duration - (millis() - ptr->msecs_start);
    }
    to_HHMMSS(remaining, ptr->hms_remaining); // write in HH:MM:SS format

    return remaining;
}


