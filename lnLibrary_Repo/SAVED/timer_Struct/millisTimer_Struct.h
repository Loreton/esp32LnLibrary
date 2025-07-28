//
// updated by ...: Loreto Notarantonio
// Date .........: 17-07-2025 08.26.20
//

#pragma once

    #include <Arduino.h> // Necessario per millis() e per i tipi come uint32_t

    // Definisce un tipo per un puntatore a funzione senza argomenti e senza ritorno
    typedef void (*TimerCallback)();

    // Struttura per gestire un timer basato su millis()
    struct MillisTimer {
        const char*     m_name;
        uint32_t        m_startTime          = 0;
        uint32_t        m_duration           = 0;
        uint32_t        m_elapsed            = 0;
        uint32_t        m_remaining          = 0;
        bool            m_isRunning          = false;
        bool            m_isCompleted        = false;
        TimerCallback   m_onCompleteCallback = nullptr; // Inizializza la callback a null

        // Costruttore
        // MillisTimer();
        MillisTimer(const char *name);

        // Inizializza il timer
        void begin();

        // Avvia/riavvia il timer
        void start(uint32_t msDuration, TimerCallback callback = nullptr);

        // Ferma il timer
        void stop();

        // Controlla lo stato del timer e gestisce la callback
        void update();

        // Query: il timer è in esecuzione?
        inline bool isRunning(void) {return m_isRunning;};

        // Query: il timer è completato?
        inline bool hasCompleted(void) {return m_isCompleted;};
        // bool hasCompleted(void);

        // Restituisce il tempo trascorso
        inline uint32_t getElapsedTime(void) {return m_elapsed;};

        // Restituisce il tempo rimanente
        inline uint32_t getRemainingTime(void) {return m_remaining;};
    };

