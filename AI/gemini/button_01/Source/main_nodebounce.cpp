//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 09.54.21
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <Preferences.h>
// #include <fauxmoESP.h>


#define RELAY_PIN     16
#define BUTTON_PIN    21

#include <Arduino.h>


// --- Definizione delle Enumerazioni e Strutture ---

enum ButtonPressType {
  NO_PRESS = 0,
  SHORT_PRESS,
  MEDIUM_PRESS,
  LONG_PRESS,
  VERY_LONG_PRESS,
  MAX_DEFINED_PRESS_LEVELS
};

struct ButtonState {
  int pin;
  const char* name;
  int pressedLogicLevel;

  bool lastButtonState;
  bool buttonPressed;
  unsigned long pressStartTime;
  unsigned long pressDuration;

  ButtonPressType currentPressLevel;
  bool maxLevelReachedAndNotified;
};

// --- Funzioni per la Gestione del Pulsante (Senza Debounce) ---

void setupButton(ButtonState &btn, int pin, const char* name, int pressedLogicLevel) {
    btn.pin = pin;
    btn.name = name;
    btn.pressedLogicLevel = pressedLogicLevel;

    if (pressedLogicLevel == LOW) {
        pinMode(btn.pin, INPUT_PULLUP);
    } else {
        pinMode(btn.pin, INPUT);
    }

      btn.lastButtonState = digitalRead(btn.pin);
      btn.buttonPressed = (btn.lastButtonState == btn.pressedLogicLevel);
      btn.pressStartTime = 0;
      btn.pressDuration = 0;
      btn.currentPressLevel = NO_PRESS;
      btn.maxLevelReachedAndNotified = false;
}

bool readButton(ButtonState &btn, const unsigned long pressThresholds[], size_t numThresholds) {
    bool reading = digitalRead(btn.pin);

    if (reading != btn.lastButtonState) {
        btn.buttonPressed = reading;

        if (btn.buttonPressed == btn.pressedLogicLevel) {
            btn.pressStartTime = millis();
            btn.currentPressLevel = NO_PRESS;
            btn.maxLevelReachedAndNotified = false;
        } else {
            if (btn.pressStartTime != 0) {
                btn.pressDuration = millis() - btn.pressStartTime;
                btn.pressStartTime = 0;

                Serial.printf("[%s] \n",  btn.name,  btn.pressDuration);
                Serial.printf("     Rilasciato! Durata: %ld\n",  btn.pressDuration);
                Serial.printf("     current level: %ld/%d\n", btn.currentPressLevel, numThresholds);
                btn.currentPressLevel = NO_PRESS;
                btn.maxLevelReachedAndNotified = false;

                return true;
            }
        }
    }

  if (btn.buttonPressed == btn.pressedLogicLevel) {
    if (btn.pressStartTime != 0) {
      unsigned long currentPressTime = millis() - btn.pressStartTime;

      ButtonPressType newLevel = NO_PRESS;
      for (int i = numThresholds - 1; i >= 0; i--) {
        if (currentPressTime >= pressThresholds[i]) {
          newLevel = static_cast<ButtonPressType>(i + 1);
          break;
        }
      }

      if (newLevel > btn.currentPressLevel && newLevel <= numThresholds) {
          btn.currentPressLevel = newLevel;
      }
      if (btn.currentPressLevel == static_cast<ButtonPressType>(numThresholds) && !btn.maxLevelReachedAndNotified) {
          Serial.print("  [");
          Serial.print(btn.name);
          Serial.println("] MAX livello di pressione raggiunto!");
          btn.maxLevelReachedAndNotified = true;
      }
    }
  }

  btn.lastButtonState = reading;

  return false;
}

// --- Variabili Globali per il Test ---

// const int RELAY_PIN = 16;
bool relayState = false;

ButtonState testButton;

const unsigned long TEST_BUTTON_THRESHOLDS[] = {
  300,   // Livello 1: SHORT_PRESS
  800,  // Livello 2: MEDIUM_PRESS
  2000, // Livello 3: LONG_PRESS
  5000  // Livello 4: VERY_LONG_PRESS
};
const size_t NUM_TEST_BUTTON_THRESHOLDS = sizeof(TEST_BUTTON_THRESHOLDS) / sizeof(TEST_BUTTON_THRESHOLDS[0]);

// --- Setup e Loop Principale ---

void setup() {
  Serial.begin(115200);
  Serial.println("Avvio test pulsante con livelli di pressione e relè (SENZA DEBOUNCE).");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Assumiamo un relè che si attiva con LOW

  setupButton(testButton, BUTTON_PIN, "TestButton", LOW);

  Serial.println("Pulsante su GPIO 21, Relè su GPIO 16.");
  Serial.println("Premi il pulsante e rilascia al livello MEDIUM_PRESS per attivare/disattivare il relè.");
  Serial.println("ATTENZIONE: Senza debounce software, potresti vedere più attivazioni/disattivazioni a causa dei rimbalzi del pulsante.");
}

void loop() {
  // readButton ora non prende il parametro debounceDelay
  if (readButton(testButton, TEST_BUTTON_THRESHOLDS, NUM_TEST_BUTTON_THRESHOLDS)) {
    Serial.print("[");
    Serial.print(testButton.name);
    Serial.print("] Rilasciato! Durata: ");
    Serial.print(testButton.pressDuration);
    Serial.print(" ms. Livello finale raggiunto: ");

    switch (testButton.currentPressLevel) {
      case SHORT_PRESS: Serial.println("SHORT_PRESS"); break;
      case MEDIUM_PRESS: Serial.println("MEDIUM_PRESS"); break;
      case LONG_PRESS: Serial.println("LONG_PRESS"); break;
      case VERY_LONG_PRESS: Serial.println("VERY_LONG_PRESS"); break;
      default: Serial.println("Sconosciuto/Non Qualificato"); break;
    }

    // if (testButton.currentPressLevel == MEDIUM_PRESS) {
    //   relayState = !relayState;
    //   if (relayState) {
    //     digitalWrite(RELAY_PIN, LOW);
    //     Serial.println("  --> Relè ACCESO!");
    //   } else {
    //     digitalWrite(RELAY_PIN, HIGH);
    //     Serial.println("  --> Relè SPENTO!");
    //   }
    // } else {
    //   Serial.println("  --> Livello di pressione non MEDIUM_PRESS. Nessuna azione sul relè.");
    // }
  }

  // Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
  if (testButton.buttonPressed == testButton.pressedLogicLevel) {
    if (testButton.currentPressLevel != NO_PRESS) {
      static ButtonPressType lastPrintedLevel = NO_PRESS;
      if (testButton.currentPressLevel != lastPrintedLevel) {
        Serial.print("[");
        Serial.print(testButton.name);
        Serial.print("] Pressione in corso, livello: ");
        switch (testButton.currentPressLevel) {
          case SHORT_PRESS: Serial.println("BREVE"); break;
          case MEDIUM_PRESS:
              Serial.println("MEDIA");
              relayState = !relayState;
              if (relayState) {
                digitalWrite(RELAY_PIN, LOW);
                Serial.println("  --> Relè ACCESO!");
              } else {
                digitalWrite(RELAY_PIN, HIGH);
                Serial.println("  --> Relè SPENTO!");
              }

              break;
          case LONG_PRESS: Serial.println("LUNGA"); break;
          case VERY_LONG_PRESS: Serial.println("MOLTO LUNGA"); break;
          default: break;
        }
        lastPrintedLevel = testButton.currentPressLevel;
      }
    }
  } else {
    static ButtonPressType lastPrintedLevel = NO_PRESS;
    lastPrintedLevel = NO_PRESS;
  }

  delay(10);
}