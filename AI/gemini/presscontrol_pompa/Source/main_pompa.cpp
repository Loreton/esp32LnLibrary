//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 12.55.13
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <@button_structures.h>

// Definizione dei pin
const int passiveBuzzer_pin = 22;
const int activeBuzzer_pin = 23;
const int pressControlLED_pin = 25;
const int pumpLED_pin = 26;
const int pressControlRelay_pin = 16;
const int pressControlState_pin = 18;
const int pumpState_pin = 19;
const int startButton_pin = 21;

// Variabili per i timer e gli stati
unsigned long lastButtonPressTime = 0;
const long debounceDelay = 50; // ms

bool pressControlOn = false;
unsigned long pressControlOnTime = 0;
const unsigned long pressControlMaxOnTime = 30 * 60 * 1000; // 30 minuti in ms

unsigned long lastPumpLEDToggleTime = 0;
unsigned long lastPressControlLEDToggleTime = 0;
const unsigned long ledToggleInterval = 1000; // 1 secondo

unsigned long pumpOnStartTime = 0;
bool pumpIsOn = false;

// Variabili per i beep della pompa accesa prolungata
unsigned long lastBeepTime = 0;
int currentBeepStep = 0;
const int beepIntervals[] = {60000, 55000, 50000, 45000, 40000, 35000}; // ms
const int numBeepSteps = 6;

bool emergencyAlarmActive = false;
unsigned long lastEmergencyBeepTime = 0;
const unsigned long emergencyBeepInterval = 5000; // 5 secondi



// Funzione per suono a frequenze crescenti (accensione pompa)
void playAscendingTone() {
  int frequencies[] = {262, 294, 330, 349}; // C4, D4, E4, F4
  for (int freq : frequencies) {
    tone(passiveBuzzer_pin, freq);
    delay(100);
  }
  noTone(passiveBuzzer_pin);
}

// Funzione per suono a frequenze decrescenti (spegnimento pompa)
void playDescendingTone() {
  int frequencies[] = {349, 330, 294, 262}; // F4, E4, D4, C4
  for (int freq : frequencies) {
    tone(passiveBuzzer_pin, freq);
    delay(100);
  }
  noTone(passiveBuzzer_pin);
}



void setup() {
  Serial.begin(115200);

  // Configura i pin di output
  pinMode(passiveBuzzer_pin, OUTPUT);
  pinMode(activeBuzzer_pin, OUTPUT);
  pinMode(pressControlLED_pin, OUTPUT);
  pinMode(pumpLED_pin, OUTPUT);
  pinMode(pressControlRelay_pin, OUTPUT);

  // Configura i pin di input con pull-up interno
  pinMode(pressControlState_pin, INPUT_PULLUP);
  pinMode(pumpState_pin, INPUT_PULLUP);
  pinMode(startButton_pin, INPUT_PULLUP);

  // Assicurati che tutto sia spento all'avvio
  digitalWrite(pressControlRelay_pin, LOW);
  digitalWrite(pressControlLED_pin, LOW);
  digitalWrite(pumpLED_pin, LOW);
  noTone(passiveBuzzer_pin);
  digitalWrite(activeBuzzer_pin, LOW);
}

void loop() {
  unsigned long currentTime = millis();

  // Gestione del pulsante Start/Stop
  int buttonState = digitalRead(startButton_pin);
  if (buttonState == LOW && (currentTime - lastButtonPressTime) > debounceDelay) {
    lastButtonPressTime = currentTime;
    pressControlOn = !pressControlOn; // Inverti lo stato del press-control

    if (pressControlOn) {
      digitalWrite(pressControlRelay_pin, HIGH); // Accendi il press-control
      pressControlOnTime = currentTime; // Reset timer accensione
      currentBeepStep = 0; // Reset beep prolungati
      emergencyAlarmActive = false; // Disattiva allarme emergenza
      Serial.println("relay acceso");
    } else {
      digitalWrite(pressControlRelay_pin, LOW); // Spegni il press-control
      Serial.println("relay spento");
      // Resetta stato pompa e allarmi
      pumpIsOn = false;
      noTone(passiveBuzzer_pin);
      digitalWrite(activeBuzzer_pin, LOW);
      currentBeepStep = 0;
      emergencyAlarmActive = false;
    }
  }

// Monitoraggio e controllo del press-control
  int currentPressControlState = digitalRead(pressControlState_pin);
  // *** MODIFICA QUI: da HIGH a LOW ***
  if (currentPressControlState == LOW) { // Ora LOW = acceso
    digitalWrite(pressControlLED_pin, HIGH); // Accendi fisso (LED acceso)
  } else {
    // LED lampeggiante se press-control spento (cioè currentPressControlState è HIGH)
    if ((currentTime - lastPressControlLEDToggleTime) >= ledToggleInterval) {
      lastPressControlLEDToggleTime = currentTime;
      digitalWrite(pressControlLED_pin, !digitalRead(pressControlLED_pin));
    }
  }

// Monitoraggio e controllo della pompa
  int currentPumpState = digitalRead(pumpState_pin);
  // *** MODIFICA QUI: da HIGH a LOW ***
  if (currentPumpState == LOW) { // Ora LOW = acceso
    if (!pumpIsOn) {
      pumpIsOn = true;
      pumpOnStartTime = currentTime;
      playAscendingTone(); // Suono accensione pompa
      Serial.println("Pompa accesa");
    }
    digitalWrite(pumpLED_pin, HIGH); // Accendi fisso (LED acceso)
  } else {
    if (pumpIsOn) {
      pumpIsOn = false;
      playDescendingTone(); // Suono spegnimento pompa
      Serial.println("Pompa spenta");
    }
    // LED lampeggiante se pompa spenta (cioè currentPumpState è HIGH)
    if ((currentTime - lastPumpLEDToggleTime) >= ledToggleInterval) {
      lastPumpLEDToggleTime = currentTime;
      digitalWrite(pumpLED_pin, !digitalRead(pumpLED_pin));
    }
    // Resetta beep prolungati e allarme se la pompa si spegne
    currentBeepStep = 0;
    digitalWrite(activeBuzzer_pin, LOW);
    emergencyAlarmActive = false;
  }



  // Beep per pompa accesa prolungata
  if (pumpIsOn && currentBeepStep < numBeepSteps) {
    if ((currentTime - pumpOnStartTime) >= beepIntervals[currentBeepStep]) {
      digitalWrite(activeBuzzer_pin, HIGH);
      delay(50); // Beep breve
      digitalWrite(activeBuzzer_pin, LOW);
      currentBeepStep++;
      lastBeepTime = currentTime; // Aggiorna il tempo dell'ultimo beep
      Serial.print("Beep pompa prolungata, step: ");
      Serial.println(currentBeepStep);

      if (currentBeepStep == numBeepSteps) {
        // Dopo l'ultimo step, spegni il press-control
        digitalWrite(pressControlRelay_pin, LOW);
        pressControlOn = false;
        Serial.println("Press-control spento per pompa prolungata");
        // Verifica se press-control o pompa sono ancora accesi per allarme
        if (digitalRead(pressControlState_pin) == HIGH || digitalRead(pumpState_pin) == HIGH) {
            emergencyAlarmActive = true;
        }
      }
    }
  }

  // Timer spegnimento press-control dopo 30 minuti
  if (pressControlOn && (currentTime - pressControlOnTime) >= pressControlMaxOnTime) {
    digitalWrite(pressControlRelay_pin, LOW); // Spegni il press-control
    pressControlOn = false;
    Serial.println("Press-control spento per timer 30 minuti");
    // Attiva allarme emergenza se press-control o pompa rimangono accesi
    // *** MODIFICA QUI: da HIGH a LOW per la verifica dello stato ***
    if (digitalRead(pressControlState_pin) == LOW || digitalRead(pumpState_pin) == LOW) {
        emergencyAlarmActive = true;
    }
  }

  // Beep per pompa accesa prolungata
  if (pumpIsOn && currentBeepStep < numBeepSteps) {
    // ... (codice precedente) ...
      if (currentBeepStep == numBeepSteps) {
        // ... (codice precedente) ...
        // Verifica se press-control o pompa sono ancora accesi per allarme
        // *** MODIFICA QUI: da HIGH a LOW per la verifica dello stato ***
        if (digitalRead(pressControlState_pin) == LOW || digitalRead(pumpState_pin) == LOW) {
            emergencyAlarmActive = true;
        }
      }
    }
  // Allarme emergenza (beep ogni 5 sec)
  if (emergencyAlarmActive) {
    if ((currentTime - lastEmergencyBeepTime) >= emergencyBeepInterval) {
      digitalWrite(activeBuzzer_pin, HIGH);
      delay(100); // Beep più lungo per allarme
      digitalWrite(activeBuzzer_pin, LOW);
      lastEmergencyBeepTime = currentTime;
      Serial.println("Allarme emergenza: beep!");
    }
  }


}
