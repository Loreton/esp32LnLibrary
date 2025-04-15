/*
// updated by ...: Loreto Notarantonio
// Date .........: 15-04-2025 18.48.48
*/


// --- others
// #include "@lnMacros.h"
#include "@pinStructures.h"
#include "@pinOperations.h"
#include "@ln_time.h"

// ---------------------------------
// macros Aliases for LOG
// ---------------------------------
#define LOG_LEVEL_0
#define LOG_LEVEL_4x
#include "@logMacros.h"



// #########################################
// # Ex.: setting del pulsetime;
// #########################################
void stopPulsetime(io_output_pin_struct_t *p) {
    printf1_NFN("%s stopping pulsetime\n", p->pinID);
    p->pulsetime.enabled = false;
    p->pulsetime.cur_duration = 0;
    p->pulsetime.msecs_start  = 0;
}


// #########################################
// # Ex.: start del pulsetime;
// #########################################
void startPulsetime(io_output_pin_struct_t *p, int32_t msec_pulseLength, bool autoOnOff) {
    printf4_NFN("%s processing pulse %ld mSec on pin - autoStart=%d\n", p->pinID, msec_pulseLength, autoOnOff);
    if (p->pulsetime.cur_duration != 0) {
        printf4_NFN("%s pulse is already running. new request discarded.\n", p->pinID);
        return;
    }

    p->pulsetime.auto_on_off = autoOnOff;

    // --- if temporary value overrides default
    if (msec_pulseLength > 0) {
        p->pulsetime.cur_duration = msec_pulseLength; // in millisecondi
    }

    // --- if default value
    else if (p->pulsetime.msecs_default != 0) { // carichiamo  il default
        p->pulsetime.cur_duration = p->pulsetime.msecs_default; // in millisecondi
    }

    else {
        p->pulsetime.cur_duration = 0; // no pulseTime
        printf4_NFN("%s no value for pulse %ld\n", p->pinID, p->pulsetime.cur_duration);
    }

    if (p->pulsetime.cur_duration != 0) {
        p->pulsetime.enabled = true;
        printf4_NFN("%s starting pulsetime: %ld msecs\n", p->pinID, p->pulsetime.cur_duration);
        p->pulsetime.msecs_start = millis(); // in millisecondi l'orario di fine
        if (p->pulsetime.auto_on_off) {
            // digitalWrite(p->pin, p->ON);
            pinON(p);
        }
    }

}

// #########################################
// # Ex.: check if pulsetime;expired
// #########################################
bool isPulsetimeExpired(io_output_pin_struct_t *p) {
    bool expired = false;
    PINpulsetime *pt = &p->pulsetime;

    if (pt->cur_duration != 0 ) {
        pt->elapsed   = millis() - pt->msecs_start;
        pt->remaining = pt->cur_duration - pt->elapsed;

        if ( pt->elapsed > pt->cur_duration ) {
            printf4_NFN("%s pulsetime expired (elapsed: %ld msec)\n", p->pinID, pt->elapsed);
            expired = true;
        }
        else {
            printf4_NFN("%s pulsetime NOT expired (remaining msec: %ld)\n", p->pinID, pt->remaining);
        }
    }

    return expired;
}


// #########################################
// # Ex.: check if pulsetime;expired
// #########################################
uint32_t remainingPulseTime(io_output_pin_struct_t *p) {
    PINpulsetime *pt = &p->pulsetime;
    // uint32_t remaining=0;

    if (pt->cur_duration != 0 ) {
        pt->remaining = pt->cur_duration - (millis() - pt->msecs_start);
    }
    to_HHMMSS(pt->remaining, pt->hms_remaining); // write in HH:MM:SS format

    return pt->remaining;
}



void pinPulseON(io_output_pin_struct_t *p, int32_t msec_pulseLength, bool autoOff) {
    // if (isPulsetimeExpired(p)) {
    //     startPulsetime(p, msec_pulseLength, true);
    // }
    startPulsetime(p, msec_pulseLength, true);

    // else {
    //     printf4_NFN("%s pulsetime NOT expired (remaining msec: %ld)\n", p->pinID, remaining);
    // }

}
