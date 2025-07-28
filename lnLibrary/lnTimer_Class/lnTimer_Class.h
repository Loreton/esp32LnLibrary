//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 14.24.05
//

#pragma once

#include <Arduino.h> // Necessario per millis() e per i tipi come uint32_t

// Definisce un tipo per un puntatore a funzione senza argomenti e senza ritorno
typedef void (*TimerCallback)();

// Classe per gestire un timer basato su millis()
class MillisTimer {
private:
    const char* m_name;
    uint32_t        m_startTime     = 0;
    uint32_t        m_duration      = 0;
    uint32_t        m_elapsed       = 0;
    uint32_t        m_remaining     = 0;
    bool            m_isRunning     = false;
    bool            m_isCompleted   = false;
    TimerCallback   m_onCompleteCallback = nullptr; // Inizializza la callback a null

public:
    // Costruttore
    MillisTimer(const char *name);

    // Inizializza il timer (potrebbe non essere strettamente necessario in una classe con costruttore)
    void begin();

    // Avvia/riavvia il timer
    void start(uint32_t msDuration, TimerCallback callback = nullptr);

    // Ferma il timer
    void stop();

    // Controlla lo stato del timer e gestisce la callback
    void update();

    // Query: il timer è in esecuzione?
    inline bool isRunning(void) const { return m_isRunning; }

    // Query: il timer è completato?
    inline bool hasCompleted(void) const { return m_isCompleted; }

    // Restituisce il tempo trascorso
    inline uint32_t getElapsedTime(void) const { return m_elapsed; }

    // Restituisce il tempo rimanente
    inline uint32_t getRemainingTime(void) const { return m_remaining; }
};