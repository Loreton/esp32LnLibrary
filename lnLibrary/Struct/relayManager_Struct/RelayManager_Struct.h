//
// updated by ...: Loreto Notarantonio
// Date .........: 21-07-2025 20.14.19
//

#include <Arduino.h> // Necessario per funzioni come pinMode, digitalWrite, millis

// #include "relay_class.h" // Include l'header della classe


    typedef struct RelayManager_Struct {
        uint8_t         m_pin = 99;
        const char*     m_name  = nullptr;
        char            m_pinID[17];     // [pin_name.pin_nr]:

        uint8_t         m_activeLevel=LOW; //              Livello che attiva il relè (HIGH o LOW)
        uint8_t         m_On=LOW; //              Livello che attiva il relè (HIGH o LOW)
        uint8_t         m_Off=LOW; //              Livello che attiva il relè (HIGH o LOW)

        uint32_t        m_pulseStartTime=0;
        uint32_t        m_pulseDuration=0;

        bool            m_pulseActive=false;
        bool            m_relayState=false;


        // Costruttore
        RelayManager_Struct(void);

        // --- Metodi Pubblici ---
        // pin: Il numero del GPIO a cui è collegato il relè.
        // activationLevel: Il livello fisico (HIGH o LOW) che attiva il relè.
        void init(const char *name, uint8_t pin, uint8_t activationLevel);

        // Imposta lo stato del relè (true = acceso, false = spento)
        void setRelay(bool state);
        void on(void);
        void off(void);
        void toggle(void);

        // Avvia un pulsetime per il relè
        // duration_ms: Durata del pulsetime in millisecondi.
        void startPulse(uint32_t duration_ms);

        // Ottiene il tempo rimanente del pulsetime (0 se non attivo o scaduto)
        uint32_t getRemainingPulseTime();

        // Deve essere chiamata regolarmente nel loop() per aggiornare lo stato del relè
        void update();

        // Ritorna lo stato attuale del relè (true = acceso, false = spento)
        bool state();
        bool isActive(void)   {return digitalRead(m_pin) == m_activeLevel; };

        // Ritorna il pin del relè (utile per debug)
        // int pin();
        const char *pinID(void);
        // const char *pinID() const char * {return m_pinID;};



    } RelayManager_Struct;



