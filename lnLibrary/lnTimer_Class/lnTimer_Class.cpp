//
// updated by ...: Loreto Notarantonio
// Date .........: 30-08-2025 10.58.13
//


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h> // Assicurati che questo sia disponibile nel tuo ambiente
#include "lnTimer_Class.h" // Includi il tuo nuovo file header della classe

// Costruttore vuoto....
MillisTimer::MillisTimer(void ){} ;

// Inizializza il timer
void MillisTimer::init(const char *name, uint32_t duration, TimerCallback callback) {
    m_name = name;
    m_duration = duration;
    m_onCompleteCallback = callback;
    if (m_onCompleteCallback) {
        LOG_INFO("[TIMER: %s] created for %lu ms. (with callback)", m_name, m_duration);
    } else {
        LOG_INFO("[TIMER: %s] created for %lu ms. (without callback)", m_name, m_duration);
    }
}

// Avvia o riavvia il timer
// void MillisTimer::start(uint32_t duration, TimerCallback callback) {
void MillisTimer::start(uint32_t duration) {
    if (m_isRunning) {
        LOG_WARN("[TIMER: %s] already running, new start command was rejected", m_name);
    } else {
        m_duration = (duration==0) ? m_duration : duration;
        m_isRunning = true;
        m_hasExpired = false;
        m_startTime = millis();
    }
    LOG_NOTIFY("[TIMER: %s] - start....%lu", m_name, m_duration);
}

// Avvia o riavvia il timer
void MillisTimer::restart(uint32_t duration) {
    m_isRunning = false;
    start(duration);
}

// Ferma il timer
void MillisTimer::stop() {
    m_isRunning = false;
    m_hasExpired = false; // Resetta lo stato di completamento quando fermato
    LOG_NOTIFY("[TIMER: %s] stopped!", m_name);
}

// Controlla se il timer è scaduto e gestisce la callback
void MillisTimer::update() {
    if (m_isRunning && !m_hasExpired) {
        m_elapsed = millis() - m_startTime;

        if (m_elapsed >= m_duration) {
            m_hasExpired = true;
            m_isRunning = false; // Il timer non è più in esecuzione una volta completato
            m_remaining = 0;
            LOG_NOTIFY("[TIMER: %s] has been completed!", m_name);
            if (m_onCompleteCallback) {
                LOG_DEBUG("[TIMER: %s] running callback!", m_name);
                m_onCompleteCallback(this); // Esegue la callback se impostata
            }
        } else {
            m_remaining = m_duration - m_elapsed;
            // LOG_DEBUG("[%s] Running, Elapsed: %lu ms, Remaining: %lu ms", m_name, m_elapsed, m_remaining);
        }
    }
}



// #############################################################
// # Se uso questo ovviamente posso fare a meno di fare l'update()
// #############################################################
bool MillisTimer::hasExpired(void) {
    update();
    return m_hasExpired;
}

// #############################################################
// # Se uso questo ovviamente posso fare a meno di fare l'update()
// #############################################################
bool MillisTimer::isRunning(void) {
    update();
    return m_isRunning;
}


// #############################################################
// # Se uso questo ovviamente posso fare a meno di fare l'update()
// #############################################################
void MillisTimer::showStatus(void) {
    LOG_NOTIFY("[TIMER: %s] - duration: %lu - remaining: %lu - hasExpired: %d - isRunning: %d", m_name, m_duration, m_remaining, m_hasExpired, m_isRunning);
}