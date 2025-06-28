/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-04-2025 12.46.49
*/

#include <ESP32Time.h> // per definire ESP32Time rtc;

// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_x
#include "@logMacros.h"
#include "@mainStructures.h"




extern io_output_pin_struct_t *activeBuzzer;

//##########################################################
//#  buzzer pulse ma blocca il programma per tutto il tempo
//#  verificare l'utilizzo di un timer
//##########################################################
void activeBuzzerPulse(uint16_t ms_duration) {
    printf1_NFN("[%s.%d] turning ON - duration: %d ms\n", activeBuzzer->name, activeBuzzer->pin, ms_duration);
    digitalWrite(activeBuzzer->pin, activeBuzzer->ON);
    delay(ms_duration);
    printf1_NFN("[%s.%d] turning OFF\n", activeBuzzer->name, activeBuzzer->pin);
    digitalWrite(activeBuzzer->pin, activeBuzzer->OFF);
}


void activeBuzzerON(void) {
    digitalWrite(activeBuzzer->pin, activeBuzzer->ON);
}

void activeBuzzerOFF(void) {
    digitalWrite(activeBuzzer->pin, activeBuzzer->OFF);
}


void activeBuzzerToggle(void) {
    digitalWrite(activeBuzzer->pin, !digitalRead(activeBuzzer->pin));
}





