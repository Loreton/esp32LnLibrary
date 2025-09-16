//
// updated by ...: Loreto Notarantonio
// Date .........: 11-09-2025 17.41.06
//

#pragma once

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

class RelayManager_Class {
    #define __DUMMY_STATE__   0xF0
    #define __30_MINUTES__    30*60*1000UL
    private:
        uint8_t             m_pin = 99;
        const char* m_name  = nullptr;
        char                m_pinID[21];        // [pin_name.pin_nr]:

        uint8_t             m_activeLevel = LOW; // Livello che attiva il relè (HIGH o LOW)
        uint8_t             m_On = LOW;         // Livello per lo stato ON (dipende da m_activeLevel)
        uint8_t             m_Off = LOW;        // Livello per lo stato OFF (dipende da m_activeLevel)

        uint32_t            m_pulseStartTime = 0;
        uint32_t            m_pulseDuration = 0;
        uint32_t            m_defaultPulseTime = __30_MINUTES__; // default 30 minutes

        bool                m_pulseActive = false;
        bool                m_relayState = false; // Stato logico del relè (true = acceso, false = spento)
        // const uint8_t       m_dummyState = DUMMY_STATE; // valore dummy per updateState()

        /**
         * Se timeBuffer è statica e globale.
         * Ogni chiamata a timeStamp sovrascrive il contenuto di timeBuffer,
         * quindi quando stampi più valori nello stesso printf,
         * entrambe le chiamate restituiscono il valore dell'ultima chiamata
         * per tale ragione il buffer deve essere allocato estrnamente
         * ....oppure prestare molta attenzione
        */
        // char                m_timeBUFFER[12];
        // uint8_t             m_timeBUFFER_Len = sizeof(m_timeBUFFER);
        // Imposta lo stato fisico del relè sul pin
        // void setRelay(bool state);
        void updateState(uint8_t state = __DUMMY_STATE__);

    public:
        // Costruttore
        RelayManager_Class();

        // --- Metodi Pubblici ---
        void init(const char *name, uint8_t pin, uint8_t activationLevel);

        // Accende il relè
        void on(uint32_t duration_ms=0);
        // Spegne il relè
        void off();
        // Inverte lo stato del relè
        void toggle(uint32_t duration_ms=0);
        bool getState(void) const {return m_activeLevel;};
        // void updateState(void);

        void startPulse(uint32_t duration_ms = 0); // Avvia un pulsetime per il relè
        void setPulseTime(uint32_t duration_ms) {m_defaultPulseTime = duration_ms;};

        // Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
        uint32_t remainingPulseTime(void);

        // Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
        void update();

        // Ritorna lo stato logico attuale del relè (true = acceso, false = spento)
        // bool state() const;
        // Ritorna vero se il relè è fisicamente attivo (a livello di pin)
        bool isActive() const;

        // Ritorna l'ID del pin (utile per debug)
        // const char *pinID() const;
        inline const char* pinID(void) const { return m_pinID; }

    };

