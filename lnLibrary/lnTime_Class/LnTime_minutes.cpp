/*
// updated by ...: Loreto Notarantonio
// Date .........: 10-09-2025 15.44.28
*/

#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "LnTime_Class.h"


// ######################################################################
// # Controlla se è iniziato un nuovo minute
// ######################################################################
bool LnTime_Class::onMinute() {
    if (m_timeinfo.tm_min != m_last_minute) {
        m_last_minute = m_timeinfo.tm_min;

        /* Reset della mappa dei minuti per riattivare i flag
        altrimenti funziona solo per la prima ora */
        m_at_last_second_map.clear();
        #ifdef LN_TIME_CLASS_SECONDS_VECTOR
            m_at_last_second_vector_map.clear();
        #endif

        return true;
    }
    return false;
}




// ######################################################################
// # std::map<uint8_t, int8_t> m_at_last_minute_map;
// # true se ci troviamo al minuteo x
// # Questa soluzione funziona solo se lo si chiama una volta perché
// # la volta successiva ritorna false
// #
// # Se chiami atMinute(30), la funzione aggiornerà solo
// #    l'entry m_at_last_minute_map[30], senza influenzare un'eventuale
// #    successiva chiamata a atMinute(45).
// ######################################################################
bool LnTime_Class::atMinute(uint8_t minute) {
    uint8_t current_minute = m_timeinfo.tm_min;

    // Se non esiste, prendiamo il valore dallo stato
    if (m_at_last_minute_map.count(minute)) {
        m_at_last_minute = m_at_last_minute_map[minute];
    }

    if (current_minute == minute && current_minute != m_at_last_minute) {
        m_at_last_minute_map[minute] = current_minute; // aggiorniamo il valore
        return true;
    }
    return false;
}





#if 0
// FORSE NON SERVE - TUTTO DIROTTATO AL onModulo() nei Seconds
// ######################################################################
// # modulo non è limitato a 60
// ######################################################################
bool LnTime_Class::onMinuteModulo(uint32_t modulo, bool trueOnFirstRun) {
    bool firstRun = true;
    bool isTime = false;
    if (modulo == 0) {modulo = 60; }

    uint32_t last_epoch_minutes = 0;
    if (m_last_epoch_minutes_map.count(modulo)) { // se esiste la entry
        last_epoch_minutes = m_last_epoch_minutes_map[modulo]; // prelevane il valore
        firstRun = false;
    }


    // uint32_t current_minute = rtc.getEpoch();
    // invece di prendere l'epoch() forse è più rapido questo...
    // uint32_t current_minute = m_timeinfo.tm_hour*60*60 + m_timeinfo.tm_min*60 + m_timeinfo.tm_sec;
    uint32_t current_minute = m_timeinfo.tm_hour*60*60 + m_timeinfo.tm_min;

    if (current_minute != last_epoch_minutes && current_minute % modulo == 0) {
        m_last_epoch_minutes_map[modulo] = current_minute; // Aggiorna la mappa
        isTime = true;
    }

    if (!trueOnFirstRun) {firstRun=false; } // se non è stato richiesto il trueOnFirstRun

    return (isTime || firstRun) ? true : false;
}
#endif



// ===============================================================
// =   V E C T O R   - V E C T O R   - V E C T O R   - V E C T O R   -
// ===============================================================

#ifdef LN_TIME_CLASS_MINUTES_VECTOR

// ######################################################################
// # identia a atMinute(uint8_t min) ma dedicata alla parte con i vector
// ######################################################################
bool LnTime_Class::atMinuteFlag(uint8_t minute) {
    uint8_t current_minute = m_timeinfo.tm_min;

    // Se non esiste, prendiamo il valore dallo stato
    if (m_at_last_minute_vector_map.count(minute)) {
        m_at_last_minute_vector = m_at_last_minute_vector_map[minute];
    }

    if (current_minute == minute && current_minute != m_at_last_minute_vector) {
        m_at_last_minute_vector_map[minute] = current_minute; // aggiorniamo il valore
        return true;
    }
    return false;
}


// ######################################################################
// # possiamo fare uso dei vector.
// # quindi impostiamo tutti i valori che vogliamo controllare
// # e la classe provvede a salvare lo stato in una map
// #
// #   const std::vector<uint8_t> minutes = {0, 1, 15, 30, 45, 120};
// #
// ######################################################################
void LnTime_Class::setAtMinuteFlag(const std::vector<uint8_t>& minutes) {
    m_at_minutes_to_monitor = minutes;
    for (uint8_t _minute : m_at_minutes_to_monitor) {
        LOG_INFO("_minute: %d", _minute);
    }

}


// ######################################################################
// #
// ######################################################################
bool LnTime_Class::getAtMinuteFlag(uint8_t minute) {
    if (m_at_minute_flags.count(minute)) {  // se lo troviamo
        return m_at_minute_flags[minute];   // ritorna lo stato
    }
    return false; // Restituisce false se il minuteo non è stato registrato
}



// ######################################################################
// # Implement the new method to update the flags
// # must be called in loop or in update()
// ######################################################################
void LnTime_Class::updateAtMinuteFlags() {
    for (uint8_t _minute : m_at_minutes_to_monitor) {
        m_at_minute_flags[_minute] = atMinuteFlag(_minute) ? true : false;
    }
}




#endif