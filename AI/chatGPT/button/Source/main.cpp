//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 09.18.01
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

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


// #define RELAY_PIN     16
// #define BUTTON_PIN    21

const int buttonPin = 21;
const int relayPin = 16;
const int pressedLevel = LOW; // considera LOW come "premuto"

// Definiamo 3 livelli di pressione: breve, medio, lungo
const unsigned long pressLevels[] = {300, 1000, 2000}; // in ms
const int numLevels = sizeof(pressLevels) / sizeof(pressLevels[0]);

ButtonState buttonState;
bool relayState = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // inizialmente spento
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  if (readButtonWithLevels(buttonPin, pressLevels, numLevels, "GPIO21", pressedLevel, buttonState)) {
    if (buttonState.pressLevel >= 2) { // livello MEDIUM_PRESS (index 1 + 1)
      relayState = !relayState;
      digitalWrite(relayPin, relayState ? HIGH : LOW);
      Serial.printf("Pulsante %s rilasciato con livello %d -> toggle relè: %s\n",
                    "GPIO21", buttonState.pressLevel, relayState ? "ON" : "OFF");
    } else {
      Serial.printf("Pulsante %s rilasciato con livello %d -> nessuna azione\n", "GPIO21", buttonState.pressLevel);
    }
  }
}
