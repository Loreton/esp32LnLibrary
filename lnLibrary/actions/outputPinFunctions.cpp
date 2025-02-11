/*
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 08.55.29
*/

#include <ESP32Time.h> // per definire ESP32Time rtc;
#include "@pinDefinitions.h"

// --- others
#include "@lnMacros.h"
#include "@ln_time.h" // per rtc

// #define dstat_dbg        //           ln_printf
// #define displpin_dbg0               ln_printf
#define debug_fn               ln_printf_FN
#define debug_f                 ln_printf
#define fVerbose true
#define debug00_fn                    ln_printf_FN
#define debug01_fn    //                ln_printfFN
#define debug02_fn   //                ln_printfFN
#define debug_fn_now   //                ln_printfFN
#define pulsepin_dbg   //                ln_printfFN

// extern ESP32Time rtc;
static struct tm timeinfo = rtc.getTimeStruct();

#define BUZZER_FREQ 400

void displayOutputPin(io_output_pin_struct_t *p) {
    debug_fn("%s: \n", p->name);
    debug_f("\t%-18s: %2d - (%s)\n" , "pin nr"        , p->pin          , str_pinLevel[p->state]);
    debug_f("\t%-18s: %2d - %s\n"   , "active level"  , p->active_level , str_pinLevel[p->active_level]);
    debug_f("\t%-18s: %2d - %s\n"   , "current level" , p->state        , str_pinLevel[p->state]);
    debug_f("\t%-18s: %ul\n"        , "pulsetime"     , p->pulsetime);
    debug_f("\t%-18s: %ul\n"        , "remaining"     , p->remaining);
}





bool isTimerExpired(io_output_pin_struct_t *p) {
    int32_t epochTime = rtc.getEpoch();
    if (p->end_time > 0 && epochTime >= p->end_time) {
        return true;
    }
    return false;
}

// io_output_pin_struct_t *buzz = passiveBuzzer;

//##########################################################
//#  buzzer OFF
//##########################################################
void pulseSound(uint8_t pin, int16_t frequency, uint32_t duration) {
    tone(pin, frequency, duration);
    noTone(pin);
}




//##########################################################
//# Suona un passive buzzer
//##########################################################
void buzzerScaleUp(io_output_pin_struct_t *p) {
    uint8_t pin = p->pin;
    debug_fn("%s.%dincreasing buzzer tone\n", p->name, pin);
    int _duration=500;
    int _frequency=BUZZER_FREQ;
    for (int i=1; i<=5; i++) {
        tone(pin, _frequency*i, _duration);
        delay(_duration*1.1);
    }
    noTone(pin);
}




//##########################################################
//# Suona un passive buzzer
//##########################################################
void buzzerScaleDown(io_output_pin_struct_t *p) {
    uint8_t pin = p->pin;
    lnprintf("decreasing buzzer tone on pin: %d\n",  pin);
    int _duration=500;
    int _frequency=BUZZER_FREQ;
    for (int i=5; i>0; i--) {
        tone(pin, _frequency*i, _duration);
        delay(_duration*1.1);
    }
    noTone(pin);
}



// uint8_t digitalReadOutputPin(uint8_t pin) {
//   uint8_t bit = digitalPinToBitMask(pin);
//   uint8_t port = digitalPinToPort(pin);
//   if (port == NOT_A_PIN)
//     return LOW;

//   return (*portOutputRegister(port) & bit) ? HIGH : LOW;
// }


uint8_t readOutputPin(io_output_pin_struct_t *p) { //
    uint8_t state = digitalRead(p->pin);
    p->state        = state;
    // p->lastState    = state;
    return state;
}


uint8_t toggleOutputPin(io_output_pin_struct_t *p, int16_t pulsetime) { // pulsetime in secondi 32767 (9h 6m 11s [32400 + 367] )
    debug_fn_now("toggling pin: %s.%d\n", p->name, p->pin);
    digitalWrite(p->pin, !digitalRead(p->pin));

    if (pulsetime <= 0 && p->pulsetime > 0) {
        pulsetime = p->pulsetime;
    }


    uint8_t state = digitalRead(p->pin);
    if (pulsetime > 0) {
        debug_fn("\tstate: %d %s\n", state, str_pinLevel[state]);
        if (state == p->ON) { // se è attivo...statrt pulsetime
            p->start_time=rtc.getEpoch();
            p->end_time=rtc.getEpoch()+pulsetime;
            debug_fn_now("\tstarted pulsetime: %d\n", pulsetime);
        }
        else { // set timer...
            p->start_time=0;
            p->end_time=0;
            debug_fn("\tcleared timer\n");
        }
    }
    else {
        p->start_time=0;
        p->end_time=0;
        // debug_fn("\tcleared timer (no pulsetime)\n");
    }

    return state;

}


void setOutputPinOFF(io_output_pin_struct_t *p) {
    debug_fn("turning off pin: %s.%d\n", p->name, p->pin);
    digitalWrite(p->pin, p->OFF);
    p->start_time=0;
    p->end_time=0;
}



void setOutputPinON(io_output_pin_struct_t *p, int16_t pulsetime) {
    debug_fn("turning on pin: %s.%d\n", p->name, p->pin);
    digitalWrite(p->pin, p->ON);
    p->start_time=0;
    p->end_time=0;

    if (pulsetime <= 0 && p->pulsetime > 0) {
        pulsetime = p->pulsetime;
    }

    if (pulsetime > 0) {
        p->start_time=rtc.getEpoch();
        p->end_time=rtc.getEpoch()+pulsetime;
        debug_fn("\tstarted pulsetime: %d\n", pulsetime);
    }
}



//##########################################################
//#  buzzer OFF
//##########################################################
void pinHighPulse(uint8_t pin, uint16_t delaymS, uint8_t times) {
    // for (uint8_t j=1; j<=times; j++) {
        digitalWrite(pin, HIGH);
        delay(delaymS);
        digitalWrite(pin, LOW);
        // delay(delaymS/2);
    // }
}

//##########################################################
//#  buzzer OFF
//##########################################################
void pinLowPulse(uint8_t pin, uint16_t delaymS, uint8_t times) {
    digitalWrite(pin, LOW);
    delay(delaymS);
    digitalWrite(pin, HIGH);
}

// #########################################
// #
// #########################################
void pinPulse(uint8_t pin, uint16_t delaymS, const char *name) {
    uint8_t orig_state = digitalRead(pin);
    pulsepin_dbg("pulse on pin: %s.%d\n", name, pin);
    pulsepin_dbg("\tcurrent state:  %s\n", str_pinLevel[orig_state]);

    // changing state
    digitalWrite(pin, !orig_state);
    pulsepin_dbg("\tnew state:      %s\n", str_pinLevel[digitalRead(pin)]);

    // --- wait for delay
    delay(delaymS);

    // reset originaty state
    digitalWrite(pin, orig_state);
    pulsepin_dbg("\tleaving state: %s\n", str_pinLevel[digitalRead(pin)]);
}

// #########################################
// #
// #########################################
void pinPulse(uint8_t pin, uint16_t delaymS, const char *name, uint8_t level) {
    uint8_t orig_state = digitalRead(pin);
    pulsepin_dbg("pulse on pin: %s.%d\n", name, pin);
    pulsepin_dbg("\tcurrent state:  %s\n", str_pinLevel[orig_state]);

    // changing state
    digitalWrite(pin, level);
    pulsepin_dbg("\tnew state:      %s\n", str_pinLevel[digitalRead(pin)]);

    // --- wait for delay
    delay(delaymS);

    // reset originaty state
    digitalWrite(pin, !level);
    pulsepin_dbg("\tleaving state: %s\n", str_pinLevel[digitalRead(pin)]);
}


void pinPulse(io_output_pin_struct_t *p, uint16_t delaymS, uint8_t times) {
    pinPulse(p->pin, delaymS, p->name, p->active_level);
}


