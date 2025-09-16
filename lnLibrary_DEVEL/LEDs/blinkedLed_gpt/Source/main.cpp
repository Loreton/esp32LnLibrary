//
// updated by ...: Loreto Notarantonio
// Date .........: 19-06-2025 15.42.20
//


#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__
#include "@led_structures.h"

BlinkLed led1 = {2, 1000, 0.5, true, false, 0, AUTO, false};
BlinkLed led2 = {4, 500, 0.8, false, false, 0, AUTO, false};

void setup() {
      blinkLedBegin(led1);
      blinkLedBegin(led2);
}

void loop() {
      blinkLedUpdate(led1);
      blinkLedUpdate(led2);

      // Controllo manuale dimostrativo
      if (millis() > 5000 && millis() < 10000) {
            blinkLedSetManual(led1, true); // forzato acceso
      } else if (millis() > 10000) {
            blinkLedSetAuto(led1);         // torna in automatico
      }
}
