//
// updated by ...: Loreto Notarantonio
// Date .........: 16-07-2025 17.33.59
//


#include "lnLogger.h"

#include "millisTimer_Struct.h" // Includi il tuo file header

// MillisTimer::MillisTimer(void) {}

// Costruttore: Assicura uno stato iniziale pulito
MillisTimer::MillisTimer(const char *name) {
    m_name=name;
    // LOG_DEBUG("[%s] has been created!", m_name);
}


// Avvia o riavvia il timer
void MillisTimer::start(uint32_t msDuration, TimerCallback callback) {
    m_startTime = millis();
    m_duration = msDuration;
    m_isRunning = true;
    m_isCompleted = false;
    m_onCompleteCallback = callback;
    if (m_onCompleteCallback) {
        LOG_NOTIFY("[%s] started for %lu seconds. (with callback)", m_name, m_duration);
    } else {
        LOG_NOTIFY("[%s] started for %lu seconds. (without callback)", m_name, m_duration);
    }
}

// Ferma il timer
void MillisTimer::stop() {
    m_isRunning = false;
    m_isCompleted = false; // Resetta lo stato di completamento quando fermato
    LOG_NOTIFY("[%s] stopped!", m_name);
}

// Controlla se il timer è scaduto e gestisce la callback
bool MillisTimer::check() {
    if (m_isRunning && !m_isCompleted) {
        if (millis() - m_startTime >= m_duration) {
            m_isCompleted = true;
            m_isRunning = false; // Il timer non è più in esecuzione una volta completato
            LOG_NOTIFY("[%s] has been completed!", m_name);
            if (m_onCompleteCallback) {
                LOG_DEBUG("[%s] running callback!", m_name);
                m_onCompleteCallback(); // Esegue la callback se impostata
            }
            return true; // Il timer si è appena completato
        }
    }
    return m_isCompleted; // Restituisce lo stato di completamento attuale
}

// Query: il timer è in esecuzione?
bool MillisTimer::isTimerRunning() {
    if (m_isRunning) {
        LOG_DEBUG("[%s] Running, Elapsed: %lu ms, Remaining: %lu ms", m_name, getElapsedTime(), getRemainingTime());
    }
    return m_isRunning;
}

// Query: il timer è completato?
bool MillisTimer::hasCompleted() {
    if (m_isCompleted) {
        LOG_NOTIFY("[%s] completed!", m_name);
    }
    return m_isCompleted;
}

// Restituisce il tempo trascorso dall'avvio del timer
uint32_t MillisTimer::getElapsedTime() {
    if (m_isRunning) {
        return millis() - m_startTime;
    } else if (m_isCompleted) {
        return m_duration; // Se completato, il tempo trascorso è la durata completa
    }
    return 0; // Se non in esecuzione e non completato
}

// Restituisce il tempo rimanente fino al completamento
uint32_t MillisTimer::getRemainingTime() {
    if (m_isRunning) {
        uint32_t elapsed = millis() - m_startTime;
        if (elapsed < m_duration) {
            return m_duration - elapsed;
        } else {
            return 0; // Dovrebbe essere 0 se già completato
        }
    }
    return 0; // Se non in esecuzione o già completato
}