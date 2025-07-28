//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 14.36.57
//

#include "lnLogger_Class.h" // Assicurati che questo sia disponibile nel tuo ambiente
#include "lnTimer_Class.h" // Includi il tuo nuovo file header della classe

// Costruttore: Assicura uno stato iniziale pulito
MillisTimer::MillisTimer(const char *name) :
    m_name(name) {
    // I membri sono già inizializzati nell'header con = 0/false/nullptr
    // LOG_DEBUG("[%s] has been created!", m_name);
}

// Inizializza il timer (metodo mantenuto per compatibilità, ma il costruttore fa già molto)
void MillisTimer::begin() {
    // Attualmente non fa nulla di specifico che non sia già gestito dal costruttore
    // e dai successivi metodi start/stop.
}

// Avvia o riavvia il timer
void MillisTimer::start(uint32_t msDuration, TimerCallback callback) {
    if (m_isRunning) {
        LOG_WARNING("[%s] already running, new start command was rejected", m_name);
    } else {
        m_startTime = millis();
        m_duration = msDuration;
        m_isRunning = true;
        m_isCompleted = false;
        m_onCompleteCallback = callback;
        if (m_onCompleteCallback) {
            LOG_NOTIFY("[%s] started for %lu ms. (with callback)", m_name, m_duration);
        } else {
            LOG_NOTIFY("[%s] started for %lu ms. (without callback)", m_name, m_duration);
        }
    }
}

// Ferma il timer
void MillisTimer::stop() {
    m_isRunning = false;
    m_isCompleted = false; // Resetta lo stato di completamento quando fermato
    LOG_NOTIFY("[%s] stopped!", m_name);
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
        } else {
            m_remaining = m_duration - m_elapsed;
            // LOG_DEBUG("[%s] Running, Elapsed: %lu ms, Remaining: %lu ms", m_name, m_elapsed, m_remaining);
        }
    }
}