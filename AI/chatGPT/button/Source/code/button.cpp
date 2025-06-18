
#include <Arduino.h>    // in testa anche per le definizioni dei type

#define MAX_PRESS_LEVELS 5

struct ButtonState {
  bool wasPressed = false;
  bool released = false;
  unsigned long pressStartTime = 0;
  int pressLevel = 0;
  bool actionTaken = false;
};

bool readButtonWithLevels(int pin, const unsigned long levels[], int numLevels, const String& pinName, int pressedLevel, ButtonState &state) {
  int rawState = digitalRead(pin);
  unsigned long now = millis();

  if (rawState == pressedLevel) {
    if (!state.wasPressed) {
      // inizio pressione
      state.wasPressed = true;
      state.pressStartTime = now;
      state.pressLevel = 0;
      state.actionTaken = false;
    } else {
      // calcolo livello pressione
      unsigned long duration = now - state.pressStartTime;
      for (int i = 0; i < numLevels; i++) {
        if (duration >= levels[i]) {
          state.pressLevel = i + 1;
        }
      }
      if (state.pressLevel > numLevels) {
        state.pressLevel = numLevels;
      }
    }
    state.released = false;
  } else {
    if (state.wasPressed) {
      // rilascio del pulsante
      state.wasPressed = false;
      state.released = true;
      return true;  // solo al rilascio
    }
  }

  return false;
}
