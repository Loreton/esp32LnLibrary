//
// updated by ...: Loreto Notarantonio
// Date .........: 10-02-2025 19.11.33
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@ln_MillisTimer.h"




// void startMillisTimer(structMillisTimer *t, uint32_t interval) {
//     t->start = millis();
//     t->running = false;
//     t->expired = false;
//     t->interval_lentgh = interval;
// }



void restartMillisTimer(structMillisTimer *t, uint32_t interval) {
    t->start = millis();
    t->running = false;
    t->expired = false;
    if (interval != 0)
        t->interval_lentgh = interval;
//     Serial.printf("interval: %ld", interval)
//     Serial.printf("interval_lentgh: %ld", t->interval_lentgh)
}


void updateMillisTimer(structMillisTimer *t) {
    // Serial.printf("millis - start: %ld", millis() - t->start)
    // Serial.printf("millis: %ld", t->interval_lentgh)
    if ( (millis() - t->start) > t->interval_lentgh) { // non pù di uno al minuto
        t->start = millis();
        t->running = false;
        t->expired = true;
    }
}



