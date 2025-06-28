/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-05-2025 08.05.37
*/

#include <ESP32Time.h> // per definire ESP32Time rtc;


// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0x
#include "@logMacros.h"
#include "@mainStructures.h"
#include "@ln_time.h" // per rtc



extern io_output_pin_struct_t *passiveBuzzer;
// io_output_pin_struct_t *buzzer = passiveBuzzer;



//##########################################################
//#  buzzer OFF
//##########################################################
void passiveBuzzerPulse(int16_t frequency, uint32_t duration) {
    tone(passiveBuzzer->pin, frequency, duration);
    noTone(passiveBuzzer->pin);
}




//##########################################################
//# Suona un passive buzzer
//##########################################################
void passiveBuzzerScaleUp() {
    printf1_NFN("%s increasing buzzer tone\n", passiveBuzzer->pinID);
    int _duration=500;
    int base_frequency=400;
    for (int i=1; i<=5; i++) {
        tone(passiveBuzzer->pin, base_frequency*i, _duration);
        delay(_duration*1.1);
    }
    noTone(passiveBuzzer->pin);
}




//##########################################################
//# Suona un passive buzzer
//##########################################################
void passiveBuzzerScaleDown() {
    printf1_NFN("%s decreasing buzzer tone\n", passiveBuzzer->pinID);
    int _duration=500;
    int base_frequency=400;
    for (int i=5; i>0; i--) {
        tone(passiveBuzzer->pin, base_frequency*i, _duration);
        delay(_duration*1.1);
    }
    noTone(passiveBuzzer->pin);
}




