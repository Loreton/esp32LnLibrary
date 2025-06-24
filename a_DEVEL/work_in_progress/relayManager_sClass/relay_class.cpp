//
// updated by ...: Loreto Notarantonio
// Date .........: 24-06-2025 19.41.35
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

// #include "relay_class.h" // Include l'header della classe

// --- Definizione della Classe RelayManager ---
class RelayManager {
public:
    // Costruttore
    // pin: Il numero del GPIO a cui è collegato il relè.
    // activationLevel: Il livello fisico (HIGH o LOW) che attiva il relè.
    RelayManager(int pin, int activationLevel) :
        relayPin(pin),
        pressedLevel(activationLevel),
        pulseActive(false),
        relayState(false) // Inizialmente spento
    {
        pinMode(relayPin, OUTPUT);
        // Imposta il relè allo stato iniziale (spento)
        digitalWrite(relayPin, !pressedLevel);
    }

    // --- Metodi Pubblici ---

    // Imposta lo stato del relè (true = acceso, false = spento)
    void setRelay(bool state) {
        relayState = state;
        digitalWrite(relayPin, relayState ? pressedLevel : !pressedLevel);
    }

    // Avvia un pulsetime per il relè
    // duration_ms: Durata del pulsetime in millisecondi.
    void startPulse(unsigned long duration_ms) {
        pulseStartTime = millis();
        pulseDuration = duration_ms;
        pulseActive = true;
        setRelay(true); // Attiva il relè per il pulsetime
    }

    // Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
    unsigned long getRemainingPulseTime() {
        if (pulseActive) {
            unsigned long elapsed = millis() - pulseStartTime;
            if (elapsed < pulseDuration) {
                return pulseDuration - elapsed;
            } else {
                return 0; // Pulsetime scaduto
            }
        }
        return 0;
    }

    // Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
    void update() {
        // Gestione del pulsetime
        if (pulseActive) {
            if (millis() - pulseStartTime >= pulseDuration) {
                pulseActive = false;
                setRelay(false); // Disattiva il relè al termine del pulsetime
                // Puoi aggiungere qui una callback o un flag per notificare la fine del pulsetime
            }
        }
    }

    // Ritorna lo stato attuale del relè (true = acceso, false = spento)
    bool getRelayState() {
        return relayState;
    }

    // Ritorna il pin del relè (utile per debug)
    int getRelayPin() {
        return relayPin;
    }

private:
    // --- Membri Privati ---
    int relayPin;
    int pressedLevel; // Livello che attiva il relè (HIGH o LOW)

    unsigned long pulseStartTime;
    unsigned long pulseDuration;
    bool pulseActive;

    bool relayState;
};

// --- Esempio di utilizzo nel file .ino ---

// Crea istanze della classe RelayManager
// Relay 1: Pin 16, si attiva con HIGH
RelayManager relay1(16, HIGH);
// Relay 2: Pin 17, si attiva con LOW (esempio di relè a logica inversa)
RelayManager relay2(17, HIGH);
// Puoi aggiungere altri relè qui, ad esempio RelayManager relay3(18, HIGH);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Gestione Multi-Relè (solo Pulsetime) Avviata!");

    // Esempi di avvio per relay1
    relay1.startPulse(30000); // Pulsetime di 5 secondi
    Serial.println("Relay 1: Pulsetime avviato.");

    // Esempi di avvio per relay2
    relay2.setRelay(true); // Accende il relè 2
    Serial.println("Relay 2: Acceso.");
}

void loop() {
    // È FONDAMENTALE chiamare update() per ogni istanza di RelayManager
    // affinché la logica del pulsetime venga elaborata.
    relay1.update();
    relay2.update();
    // Se hai altri relè, aggiungi le loro chiamate update() qui:
    // relay3.update();

    // --- Debugging e Stampa Stato (Non bloccante) ---
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime >= 2000) { // Stampa ogni 2 secondi
        lastPrintTime = millis();
        Serial.println("--- Stato Relè ---");
        Serial.printf("Relay 1 (Pin %d): Stato: %s, Pulsetime Rimanente: %lu ms\n",
                      relay1.getRelayPin(), relay1.getRelayState() ? "ACCESO" : "SPENTO",
                      relay1.getRemainingPulseTime());
        Serial.printf("Relay 2 (Pin %d): Stato: %s, Pulsetime Rimanente: %lu ms\n",
                      relay2.getRelayPin(), relay2.getRelayState() ? "ACCESO" : "SPENTO",
                      relay2.getRemainingPulseTime());
    }

    // Altre operazioni non bloccanti del tuo programma principale qui
    // ...
}