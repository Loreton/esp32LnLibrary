//
// updated by ...: Loreto Notarantonio
// Date .........: 01-07-2025 07.59.39
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
enum Mode { AUTO, MANUAL };

struct BlinkLed {
  int pin;
  unsigned long period;
  float dutyCycle; // 0.0 - 1.0
  bool activeLow = false; // logica diretta (HIGH = acceso) o inversa (LOW = acceso)
  bool state = false; // stato attuale logico (acceso/spento)
  unsigned long previousMillis = 0;
  Mode mode = AUTO;
  bool manualState = false; // stato forzato in modalità manuale

  void begin() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, getPhysicalState(false)); // spento inizialmente
  }

  void setManual(bool on) {
    mode = MANUAL;
    manualState = on;
    digitalWrite(pin, getPhysicalState(on));
  }

  void setAuto() {
    mode = AUTO;
    previousMillis = millis(); // reset timing per evitare glitch
  }

  void setDutyCycle(float dc) {
    dutyCycle = constrain(dc, 0.0, 1.0);
  }

  void setPeriod(unsigned long p) {
    period = p;
  }

  void setActiveLow(bool enable) {
    activeLow = enable;
    // Aggiorna lo stato corrente
    digitalWrite(pin, getPhysicalState(state));
  }

  void update() {
    if (mode == MANUAL) return;

    unsigned long currentMillis = millis();
    unsigned long onTime = period * dutyCycle;
    unsigned long offTime = period - onTime;

    if (state && currentMillis - previousMillis >= onTime) {
      state = false;
      previousMillis = currentMillis;
      digitalWrite(pin, getPhysicalState(state));
    } else if (!state && currentMillis - previousMillis >= offTime) {
      state = true;
      previousMillis = currentMillis;
      digitalWrite(pin, getPhysicalState(state));
    }
  }

private:
  int getPhysicalState(bool logicalOn) {
    return (logicalOn ^ activeLow) ? HIGH : LOW;
  }
};



BlinkLed led1 = {2, 1000, 0.5, true}; // LED su GPIO 2, logica inversa

void setup() {
  led1.begin();
  led1.setAuto(); // lampeggio automatico
}

void loop() {
  led1.update();

  // Simulazione di comando manuale dopo 5 secondi
  if (millis() > 5000 && millis() < 10000) {
    led1.setManual(true); // forza acceso
  } else if (millis() >= 10000) {
    led1.setAuto(); // torna a lampeggiare
  }
}
