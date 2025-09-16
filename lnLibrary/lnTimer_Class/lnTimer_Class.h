//
// updated by ...: Loreto Notarantonio
// Date .........: 17-08-2025 20.22.16
//

#pragma once

#include <Arduino.h> // Necessario per millis() e per i tipi come uint32_t

// Definisce un tipo per un puntatore a funzione senza argomenti e senza ritorno
// Dichiarazione forward della classe per i callback
class MillisTimer;
// typedef void (*TimerCallback)();
typedef void (*TimerCallback)(MillisTimer *t);

// Classe per gestire un timer basato su millis()
class MillisTimer {
    private:
        const char* m_name;
        uint32_t        m_startTime     = 0;
        uint32_t        m_duration      = 0;
        uint32_t        m_elapsed       = 0;
        uint32_t        m_remaining     = 0;
        bool            m_isRunning     = false;
        bool            m_hasExpired   = false;

        TimerCallback   m_onCompleteCallback = nullptr; // Inizializza la callback a null



    public:
        // Costruttore
        MillisTimer();

        void init(const char *name, uint32_t duration, TimerCallback callback = nullptr);

        // Inizializza il timer (potrebbe non essere strettamente necessario in una classe con costruttore)
        // void begin();

        // Avvia/riavvia il timer
        // void start(uint32_t msDuration, TimerCallback callback = nullptr);
        void start(uint32_t msDuration=0);
        void restart(uint32_t msDuration=0);

        // Ferma il timer
        void stop();

        // Controlla lo stato del timer e gestisce la callback
        void update(void);
        void showStatus(void);

        // Query: il timer è in esecuzione?
        // inline bool isRunning(void) const { return m_isRunning; }
        bool isRunning(void);

        // Query: il timer è completato?
        // inline bool hasExpired(void) const { return m_isExpired; }
        bool hasExpired(void);
        const char *name(void) const { return m_name; };
        bool hasCallBack(void) const { return (m_onCompleteCallback) ? true : false; }; // return true se ha la CallBack attiva

        // Restituisce il tempo trascorso
        inline uint32_t getElapsedTime(void) const { return m_elapsed; }

        // Restituisce il tempo rimanente
        inline uint32_t getRemainingTime(void) const { return m_remaining; }
    };