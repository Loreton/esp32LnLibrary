//
// updated by ...: Loreto Notarantonio
// Date .........: 17-07-2025 08.35.19
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
    if (m_isRunning) {
        LOG_WARNING("[%s] already running, new start command was rejected", m_name);
    }
    else {
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
}



// Ferma il timer
void MillisTimer::stop() {
    m_isRunning = false;
    m_isCompleted = false; // Resetta lo stato di completamento quando fermato
    // LOG_NOTIFY("[%s] stopped!", m_name);
}


// Controlla se il timer è scaduto e gestisce la callback
void MillisTimer::update() {
    if (m_isRunning && !m_isCompleted) {
        m_elapsed = millis() - m_startTime;

        if (m_elapsed >= m_duration) {
            m_isCompleted = true;
            m_isRunning = false; // Il timer non è più in esecuzione una volta completato
            m_remaining = 0;
            LOG_NOTIFY("[%s] has been completed!", m_name);
            if (m_onCompleteCallback) {
                LOG_DEBUG("[%s] running callback!", m_name);
                m_onCompleteCallback(); // Esegue la callback se impostata
            }
            // return true; // Il timer si è appena completato
        }
        else {
            m_remaining = m_duration - m_elapsed;
            // LOG_DEBUG("[%s] Running, Elapsed: %lu ms, Remaining: %lu ms", m_name, m_elapsed, m_remaining);
        }

    }
}


// se è completato riturna true ed azzera m_isCompleted.
// quindi questa funzione la si chiama una volta
// bool MillisTimer::hasCompleted() {
//     if (m_isCompleted) {
//         m_isCompleted = false; // Resetta lo stato di completamento quando fermato
//         return true;
//     }
//     return false;
// }