//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 09.27.27
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <Preferences.h>
// #include <fauxmoESP.h>


#define RELAY_PIN     16
#define BUTTON_PIN    21

#include <Arduino.h>

// --- Definizione delle Enumerazioni e Strutture ---

// Definisce i possibili tipi di pressione del pulsante
// Puoi aggiungere altri livelli qui, assicurandoti che l'array di soglie abbia abbastanza elementi.
enum ButtonPressType {
  NO_PRESS = 0,        // Nessuna pressione rilevata o ancora in corso
  SHORT_PRESS,         // Pressione breve (il primo livello valido dopo il debounce)
  MEDIUM_PRESS,        // Pressione media
  LONG_PRESS,          // Pressione lunga
  VERY_LONG_PRESS,     // Pressione molto lunga
  // ... aggiungi altri livelli se necessario
  MAX_DEFINED_PRESS_LEVELS // Utile per scopi interni, non un vero "tipo di pressione"
};

// Struttura per mantenere lo stato di ogni pulsante
struct ButtonState {
  int pin;
  const char* name;                   // Nome del pulsante per identificazione
  int pressedLogicLevel;              // Livello logico che indica il pulsante premuto (LOW o HIGH)

  bool lastButtonState;               // Ultima lettura RAW del pin
  unsigned long lastDebounceTime;     // Ultimo momento in cui il pin ha cambiato stato RAW

  bool buttonPressed;                 // Stato debounced: true se premuto, false se rilasciato
  unsigned long pressStartTime;       // Timestamp quando il pulsante è stato premuto
  unsigned long pressDuration;        // Durata dell'ultima pressione (in ms)

  ButtonPressType currentPressLevel;  // Livello di pressione attualmente raggiunto (aggiornato durante la pressione)
  bool maxLevelReachedAndNotified;    // Flag per indicare se il massimo livello è stato già notificato
};

// --- Funzioni per la Gestione del Pulsante ---

// Funzione per inizializzare il pin del pulsante
void setupButton(ButtonState &btn, int pin, const char* name, int pressedLogicLevel) {
  btn.pin = pin;
  btn.name = name;
  btn.pressedLogicLevel = pressedLogicLevel;

  // Configura il pin in base al pressedLogicLevel
  if (pressedLogicLevel == LOW) {
    pinMode(btn.pin, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno
  } else {
    pinMode(btn.pin, INPUT);        // Se premuto porta a HIGH, non usare pull-up interno (richiede pull-down esterno se flottante)
  }

  btn.lastButtonState = !pressedLogicLevel; // Stato iniziale: non premuto
  btn.lastDebounceTime = 0;
  btn.buttonPressed = false;
  btn.pressStartTime = 0;
  btn.pressDuration = 0;
  btn.currentPressLevel = NO_PRESS;
  btn.maxLevelReachedAndNotified = false;
}

// Funzione per leggere un pulsante con debounce e rilevamento dei livelli di pressione
// Restituisce TRUE solo quando il pulsante viene RILASCIATO.
// I livelli di pressione raggiunti durante la pressione e la durata sono disponibili nella struttura ButtonState.
//
// Parametri:
//   btn: Riferimento alla struttura ButtonState del pulsante.
//   debounceDelay: Il tempo di debounce in millisecondi.
//   pressThresholds: Un array di unsigned long contenente le soglie di tempo per i livelli di pressione.
//                    L'indice 0 per il primo livello (es. SHORT_PRESS). Le soglie devono essere ordinate in modo crescente.
//                    Assicurati che `numThresholds` sia coerente con l'enum (es. se hai 4 livelli, `numThresholds` = 4).
//   numThresholds: Il numero di elementi nell'array pressThresholds.
bool readButton(ButtonState &btn, unsigned long debounceDelay, const unsigned long pressThresholds[], size_t numThresholds) {

  // Legge lo stato attuale del pulsante (RAW)
  bool reading = digitalRead(btn.pin);

  // Controlla se lo stato RAW del pulsante è cambiato
  if (reading != btn.lastButtonState) {
    // Resetta il timer di debounce
    btn.lastDebounceTime = millis();
  }

  // Se è trascorso il tempo di debounce
  if ((millis() - btn.lastDebounceTime) > debounceDelay) {
    // Se lo stato RAW è stabile e diverso dallo stato debounced
    if (reading != btn.buttonPressed) {
      btn.buttonPressed = reading; // Aggiorna lo stato debounced

      // Se il pulsante è stato premuto
      if (btn.buttonPressed == btn.pressedLogicLevel) {
        btn.pressStartTime = millis(); // Registra il momento della pressione
        btn.currentPressLevel = NO_PRESS; // Reset del livello di pressione
        btn.maxLevelReachedAndNotified = false; // Reset del flag di notifica max livello
      }
      // Se il pulsante è stato rilasciato
      else {
        // Calcola la durata della pressione
        if (btn.pressStartTime != 0) { // Assicurati che fosse stato premuto
          btn.pressDuration = millis() - btn.pressStartTime;
          btn.pressStartTime = 0; // Resetta per la prossima pressione

          // Reset del livello di pressione e flag max level
          btn.currentPressLevel = NO_PRESS;
          btn.maxLevelReachedAndNotified = false;

          return true; // Il pulsante è stato rilasciato
        }
      }
    }
  }

  // Se il pulsante è attualmente premuto e stabile, aggiorna il livello di pressione
  if (btn.buttonPressed == btn.pressedLogicLevel) {
    if (btn.pressStartTime != 0) { // Assicurati che il timer sia partito
      unsigned long currentPressTime = millis() - btn.pressStartTime;

      ButtonPressType newLevel = NO_PRESS;
      // Itera le soglie dall'ultima (più grande) alla prima (più piccola)
      // per assegnare correttamente il livello più alto raggiunto.
      for (int i = numThresholds - 1; i >= 0; i--) {
        if (currentPressTime >= pressThresholds[i]) {
          newLevel = static_cast<ButtonPressType>(i + 1); // +1 perché SHORT_PRESS è 1, indice 0
          break; // Trovato il livello più alto raggiunto
        }
      }

      // Aggiorna il livello solo se è cambiato e non è stato raggiunto il massimo possibile (o se è la prima assegnazione)
      if (newLevel > btn.currentPressLevel && newLevel <= numThresholds) { // Assicurati di non superare i livelli definiti dall'array
          btn.currentPressLevel = newLevel;
      }
      // Se il livello attuale è già il massimo definito e non abbiamo ancora notificato
      if (btn.currentPressLevel == static_cast<ButtonPressType>(numThresholds) && !btn.maxLevelReachedAndNotified) {
          // Questa è una "notifica" interna che il massimo livello è stato raggiunto
          Serial.print("  [");
          Serial.print(btn.name);
          Serial.println("] MAX livello di pressione raggiunto!");
          btn.maxLevelReachedAndNotified = true; // Imposta il flag per non notificare più volte
      }
    }
  }

  // Salva la lettura attuale per la prossima iterazione
  btn.lastButtonState = reading;

  return false; // Il pulsante non è stato rilasciato in questo ciclo
}

// --- Variabili Globali per il Test ---

// Definizione del PIN del relè
// const int RELAY_PIN = 16;
// Stato del relè (true = acceso, false = spento)
bool relayState = false;

// Dichiarazione della variabile ButtonState per il nostro pulsante di test
ButtonState testButton;

// Definisci le soglie di pressione per il pulsante di test
// Queste devono essere in ordine CRESCENTE.
const unsigned long TEST_BUTTON_THRESHOLDS[] = {
  50,   // Livello 1: SHORT_PRESS
  800,  // Livello 2: MEDIUM_PRESS
  2000, // Livello 3: LONG_PRESS
  5000  // Livello 4: VERY_LONG_PRESS
};
const size_t NUM_TEST_BUTTON_THRESHOLDS = sizeof(TEST_BUTTON_THRESHOLDS) / sizeof(TEST_BUTTON_THRESHOLDS[0]);

// --- Setup e Loop Principale ---

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Avvio test pulsante con livelli di pressione e relè.");

  // Inizializza il pin del relè come OUTPUT
  pinMode(RELAY_PIN, OUTPUT);
  // Assicurati che il relè sia spento all'avvio (adatta HIGH/LOW in base al tuo modulo)
  digitalWrite(RELAY_PIN, HIGH); // Assumiamo un relè che si attiva con LOW

  // Inizializza il pulsante di test
  // GPIO 21, nome "TestButton", premuto quando LOW (connessione a GND con INPUT_PULLUP)
  setupButton(testButton, BUTTON_PIN, "TestButton", LOW);

  Serial.println("Pulsante su GPIO 21, Relè su GPIO 16.");
  Serial.println("Premi il pulsante e rilascia al livello MEDIUM_PRESS per attivare/disattivare il relè.");
}

void loop() {
  // Leggi il pulsante di test. La funzione restituirà true solo al rilascio.
  if (readButton(testButton, 300, TEST_BUTTON_THRESHOLDS, NUM_TEST_BUTTON_THRESHOLDS)) {
    Serial.print("[");
    Serial.print(testButton.name);
    Serial.print("] Rilasciato! Durata: ");
    Serial.print(testButton.pressDuration);
    Serial.print(" ms. Livello finale raggiunto: ");

    switch (testButton.currentPressLevel) {
      case SHORT_PRESS: Serial.println("BREVE"); break;
      case MEDIUM_PRESS: Serial.println("MEDIA"); break;
      case LONG_PRESS: Serial.println("LUNGA"); break;
      case VERY_LONG_PRESS: Serial.println("MOLTO LUNGA"); break;
      default: Serial.println("Sconosciuto/Non Qualificato"); break;
    }

    // --- LOGICA DI TEST PER IL RELÈ ---
    // Se il livello raggiunto al rilascio è MEDIUM_PRESS
    if (testButton.currentPressLevel == MEDIUM_PRESS) {
      relayState = !relayState; // Inverti lo stato del relè
      if (relayState) {
        digitalWrite(RELAY_PIN, LOW); // Accendi il relè (adatta a HIGH se il tuo relè si attiva con HIGH)
        Serial.println("  --> Relè ACCESO!");
      } else {
        digitalWrite(RELAY_PIN, HIGH); // Spegni il relè (adatta a LOW se il tuo relè si attiva con HIGH)
        Serial.println("  --> Relè SPENTO!");
      }
    } else {
      Serial.println("  --> Livello di pressione non MEDIUM_PRESS. Nessuna azione sul relè.");
    }
  }

  // Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
  // Questo blocco ti darà un feedback visivo sulla Serial Monitor mentre tieni premuto il pulsante.
  if (testButton.buttonPressed == testButton.pressedLogicLevel) {
    if (testButton.currentPressLevel != NO_PRESS) {
      // Usiamo una variabile static per stampare il livello solo quando cambia
      static ButtonPressType lastPrintedLevel = NO_PRESS;
      if (testButton.currentPressLevel != lastPrintedLevel) {
        Serial.print("[");
        Serial.print(testButton.name);
        Serial.print("] Pressione in corso, livello: ");
        switch (testButton.currentPressLevel) {
          case SHORT_PRESS: Serial.println("BREVE"); break;
          case MEDIUM_PRESS: Serial.println("MEDIA"); break;
          case LONG_PRESS: Serial.println("LUNGA"); break;
          case VERY_LONG_PRESS: Serial.println("MOLTO LUNGA"); break;
          default: break; // Non stampare NO_PRESS
        }
        lastPrintedLevel = testButton.currentPressLevel;
      }
    }
  } else { // Pulsante rilasciato
    // Reset della variabile static per il prossimo ciclo di pressione
    static ButtonPressType lastPrintedLevel = NO_PRESS;
    lastPrintedLevel = NO_PRESS;
  }

  delay(10); // Piccolo delay per evitare busy-waiting e liberare la CPU
}