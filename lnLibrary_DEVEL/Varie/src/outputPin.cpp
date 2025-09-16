/*
// updated by ...: Loreto Notarantonio
// Date .........: 20-02-2025 13.28.28
*/

#include <ESP32Time.h> // per definire ESP32Time rtc;
#include "@pinDefinitions.h"

// --- others
#include "@lnMacros.h"
#include "@ln_time.h" // per rtc






#define printf0                  lnPrintF
#define printf0_NFN               lnPrintF_NowFN


void displayOutputPin(io_output_pin_struct_t *p) {
    printf0_NFN("%s: \n", p->name);
    printf0("\t%-18s: %2d - (%s)\n" , "pin nr (log status)" , p->pin          , str_OffOn[p->lg_state]);
    printf0("\t%-18s: %2d - %s\n"   , "active level"  , p->active_level , str_pinLevel[p->active_level]);
    printf0("\t%-18s: %2d - %s\n"   , "phisical level" , p->ph_state        , str_pinLevel[p->ph_state]);
    printf0("\t%-18s: %2d - %s\n"   , "logical level" , p->lg_state        , str_pinLevel[p->ph_state]);
    printf0("\t%-18s: %ul\n"        , "pulsetime"     , p->pulsetime);
    printf0("\t%-18s: %ul\n"        , "remaining"     , p->remaining);
}






uint8_t readOutputPin(io_output_pin_struct_t *p, bool logicalState) { //
    p->ph_state = digitalRead(p->pin);
    p->lg_state = (p->ph_state == p->active_level) ? true : false;
    if (logicalState) {
        return p->lg_state;
    }
    return p->ph_state;

}



void setOutputPinOFF(io_output_pin_struct_t *p) {
    printf0_NFN("[%s] - turning off pin: %d\n", p->name, p->pin);
    digitalWrite(p->pin, p->OFF);
    p->start_time=0;
    p->end_time=0;
}



void setOutputPinON(io_output_pin_struct_t *p, int16_t pulsetime) {
    printf0_NFN("[%s] - turning on pin: %d\n", p->name, p->pin);
    digitalWrite(p->pin, p->ON);
    p->start_time=0;
    p->end_time=0;

    if (pulsetime <= 0 && p->pulsetime > 0) {
        pulsetime = p->pulsetime;
    }

    if (pulsetime > 0) {
        p->start_time=rtc.getEpoch();
        p->end_time=rtc.getEpoch()+pulsetime;
        printf0_NFN("[%s] - started pulsetime: %d\n", p->name, pulsetime);
    }
}



//##########################################################
//#  buzzer OFF
//##########################################################
void pinHighPulse(uint8_t pin, uint16_t duration) {
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
}

//##########################################################
//#  buzzer OFF
//##########################################################
void pinLowPulse(uint8_t pin, uint16_t duration) {
    digitalWrite(pin, LOW);
    delay(duration);
    digitalWrite(pin, HIGH);
}




void pinPulse(io_output_pin_struct_t *p, uint16_t ms_duration) {
    printf0_NFN("[%s] - turning on pin: %d duration %d ms\n", p->name, p->pin, ms_duration);
    digitalWrite(p->pin, p->ON);
    delay(ms_duration);
    digitalWrite(p->pin, p->OFF);
    printf0_NFN("[%s] - turning off pin: %d\n", p->name, p->pin);
}


void pinOFF(io_output_pin_struct_t *p) {
    digitalWrite(p->pin, p->OFF);
}

void pinON(io_output_pin_struct_t *p) {
    digitalWrite(p->pin, p->ON);
}


void togglePin(io_output_pin_struct_t *p) {
    digitalWrite(p->pin, !digitalRead(p->pin));
}





