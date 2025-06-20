//
// updated by ...: Loreto Notarantonio
// Date .........: 19-06-2025 15.34.52
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
enum Mode { AUTO, MANUAL };

struct BlinkLed {
      int pin;
      unsigned long period;
      float dutyCycle; // 0.0 - 1.0
      bool activeLow;
      bool state; // stato logico (acceso/spento)
      unsigned long previousMillis;
      Mode mode;
      bool manualState;
};

// ========== FUNZIONI ESTERNE ==========

void blinkLedBegin(BlinkLed &led) {
      pinMode(led.pin, OUTPUT);
      led.state = false;
      led.previousMillis = millis();
      led.mode = AUTO;
      led.manualState = false;
      digitalWrite(led.pin, getPhysicalState(led, false));
}

void blinkLedUpdate(BlinkLed &led) {
      if (led.mode == MANUAL) return;

      unsigned long currentMillis = millis();
      unsigned long onTime = led.period * led.dutyCycle;
      unsigned long offTime = led.period - onTime;

      if (led.state && currentMillis - led.previousMillis >= onTime) {
            led.state = false;
            led.previousMillis = currentMillis;
            digitalWrite(led.pin, getPhysicalState(led, false));
      } else if (!led.state && currentMillis - led.previousMillis >= offTime) {
            led.state = true;
            led.previousMillis = currentMillis;
            digitalWrite(led.pin, getPhysicalState(led, true));
      }
}

void blinkLedSetManual(BlinkLed &led, bool on) {
      led.mode = MANUAL;
      led.manualState = on;
      digitalWrite(led.pin, getPhysicalState(led, on));
}

void blinkLedSetAuto(BlinkLed &led) {
      led.mode = AUTO;
      led.previousMillis = millis();
}

void blinkLedSetDutyCycle(BlinkLed &led, float dc) {
    led.dutyCycle = constrain(dc, 0.0, 1.0);
}

void blinkLedSetPeriod(BlinkLed &led, unsigned long p) {
    led.period = p;
}

void blinkLedSetActiveLow(BlinkLed &led, bool enable) {
      led.activeLow = enable;
      digitalWrite(led.pin, getPhysicalState(led, led.state));
}

// Funzione helper
int getPhysicalState(const BlinkLed &led, bool logicalOn) {
    return (logicalOn ^ led.activeLow) ? HIGH : LOW;
}
