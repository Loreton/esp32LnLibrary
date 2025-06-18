//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 12.23.01
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <@button_structures.h>


// --- Definizione dei PIN ---
const int RELAY_PIN = 16;  // Pin del relè.
const int BUTTON_PIN = 21; // Pin del pulsante.

// --- Variabili Globali ---
bool relayState = false; // Stato attuale del relè (false = spento, true = acceso).

ButtonState myButton; // Dichiarazione della struttura per il nostro pulsante.

// --- Setup e Loop Principale ---

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Avvio test pulsante con debounce e toggle relè.");

  // Inizializza il pin del relè come OUTPUT.
  pinMode(RELAY_PIN, OUTPUT);
  // Assicurati che il relè sia spento all'avvio.
  // Molti relè sono "attivi bassi" (HIGH=spento, LOW=acceso). Adatta se il tuo è "attivo alto".
  digitalWrite(RELAY_PIN, HIGH);

  // Inizializza il pulsante:
  // - PIN: BUTTON_PIN (GPIO 21)
  // - NOME: "MainButton"
  // - LIVELLO FISICO PREMUTO: LOW (per un pulsante collegato tra pin e GND con pull-up).
  setupButton(myButton, BUTTON_PIN, "MainButton", LOW);

    Serial.printf("Pulsante su GPIO %d, Relè su GPIO %d\n", BUTTON_PIN, RELAY_PIN);
    Serial.println("Premi e rilascia il pulsante per attivare/disattivare il relè.");
}

void loop() {
  // Leggi il pulsante. La funzione restituirà `true` solo al momento del rilascio (dopo debounce).
  if (readButton(myButton, 300)) {
    Serial.printf("[%s] Rilasciato!\n", myButton.name);

    // Toggle dello stato del relè.
    relayState = !relayState;

    if (relayState) {
      digitalWrite(RELAY_PIN, LOW); // Accendi il relè (adatta a HIGH se il tuo relè è "attivo alto").
      Serial.println("  --> Relè ACCESO!");
    } else {
      digitalWrite(RELAY_PIN, HIGH); // Spegni il relè (adatta a LOW se il tuo relè è "attivo alto").
      Serial.println("  --> Relè SPENTO!");
    }
  }

  // Piccolo ritardo per evitare busy-waiting e liberare la CPU per altre attività.
  delay(10);
}