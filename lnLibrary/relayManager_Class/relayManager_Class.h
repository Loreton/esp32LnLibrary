//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 15.48.21
//

#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

class RelayManager {
private:
    uint8_t             m_pin = 99;
    const char* m_name  = nullptr;
    char                m_pinID[21];        // [pin_name.pin_nr]:

    uint8_t             m_activeLevel = LOW; // Livello che attiva il relè (HIGH o LOW)
    uint8_t             m_On = LOW;         // Livello per lo stato ON (dipende da m_activeLevel)
    uint8_t             m_Off = LOW;        // Livello per lo stato OFF (dipende da m_activeLevel)

    uint32_t            m_pulseStartTime = 0;
    uint32_t            m_pulseDuration = 0;

    bool                m_pulseActive = false;
    bool                m_relayState = false; // Stato logico del relè (true = acceso, false = spento)

    // Imposta lo stato fisico del relè sul pin
    void setRelay(bool state);

public:
    // Costruttore
    RelayManager();

    // --- Metodi Pubblici ---
    // Inizializza il relè.
    // name: Nome descrittivo del relè.
    // pin: Il numero del GPIO a cui è collegato il relè.
    // activationLevel: Il livello fisico (HIGH o LOW) che attiva il relè.
    void init(const char *name, uint8_t pin, uint8_t activationLevel);

    // Accende il relè
    void on();
    // Spegne il relè
    void off();
    // Inverte lo stato del relè
    void toggle();

    // Avvia un pulsetime per il relè
    // duration_ms: Durata del pulsetime in millisecondi.
    void startPulse(uint32_t duration_ms);

    // Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
    uint32_t getRemainingPulseTime();

    // Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
    void update();

    // Ritorna lo stato logico attuale del relè (true = acceso, false = spento)
    bool state() const;
    // Ritorna vero se il relè è fisicamente attivo (a livello di pin)
    bool isActive() const;

    // Ritorna l'ID del pin (utile per debug)
    const char *pinID() const;
};

#endif // RELAY_MANAGER_H