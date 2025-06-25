//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 12.12.06
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

// #include "relay_class.h" // Include l'header della classe

// --- Definizione della Classe RelayManager ---
class RelayManager {
    public:
        // Costruttore
        // pin: Il numero del GPIO a cui è collegato il relè.
        // activationLevel: Il livello fisico (HIGH o LOW) che attiva il relè.
        RelayManager(int pin, int activationLevel);

        // --- Metodi Pubblici ---

        // Imposta lo stato del relè (true = acceso, false = spento)
        void setRelay(bool state);

        // Avvia un pulsetime per il relè
        // duration_ms: Durata del pulsetime in millisecondi.
        void startPulse(unsigned long duration_ms);

        // Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
        unsigned long getRemainingPulseTime();

        // Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
        void update();

        // Ritorna lo stato attuale del relè (true = acceso, false = spento)
        bool getRelayState();

        // Ritorna il pin del relè (utile per debug)
        int getRelayPin();

    private:
        // --- Membri Privati ---
        int relayPin;
        int pressedLevel; // Livello che attiva il relè (HIGH o LOW)

        unsigned long pulseStartTime;
        unsigned long pulseDuration;
        bool pulseActive;

        bool relayState;
};

