/*
// updated by ...: Loreto Notarantonio
// Date .........: 15-04-2025 18.48.41
*/

#include <ESP32Time.h> // per definire ESP32Time rtc;

// --- others
// #include "@lnMacros.h"
#include "@pinStructures.h"
#include "@ln_time.h" // per rtc

// ---------------------------------
// macros Aliases for LOG
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"



extern io_output_pin_struct_t *passiveBuzzer;
io_output_pin_struct_t *buzzer = passiveBuzzer;



//##########################################################
//#  buzzer OFF
//##########################################################
void passiveBuzzerPulse(int16_t frequency, uint32_t duration) {
    tone(buzzer->pin, frequency, duration);
    noTone(buzzer->pin);
}




//##########################################################
//# Suona un passive buzzer
//##########################################################
void passiveBuzzerScaleUp() {
    printf1_NFN("%s increasing buzzer tone\n", buzzer->pinID);
    int _duration=500;
    int base_frequency=400;
    for (int i=1; i<=5; i++) {
        tone(buzzer->pin, base_frequency*i, _duration);
        delay(_duration*1.1);
    }
    noTone(buzzer->pin);
}




//##########################################################
//# Suona un passive buzzer
//##########################################################
void passiveBuzzerScaleDown() {
    printf1_NFN("%s decreasing buzzer tone\n", buzzer->pinID);
    int _duration=500;
    int base_frequency=400;
    for (int i=5; i>0; i--) {
        tone(buzzer->pin, base_frequency*i, _duration);
        delay(_duration*1.1);
    }
    noTone(buzzer->pin);
}




