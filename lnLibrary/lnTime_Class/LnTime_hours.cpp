/*
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 18.05.01
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
// # Controlla se è iniziato un nuovo hour
// ######################################################################
bool LnTime_Class::onHour() {
    if (m_timeinfo.tm_hour != m_last_hour) {
        m_last_hour = m_timeinfo.tm_hour;

        /* Reset della mappa dei minuti per riattivare i flag
        altrimenti funziona solo per la prima ora */
        m_at_last_minute_map.clear();
        #ifdef LN_TIME_CLASS_MINUTES_VECTOR
            m_at_last_minute_vector_map.clear();
        #endif

        return true;
    }
    return false;
}


// ######################################################################
// # std::map<uint8_t, int8_t> m_at_last_hour_map;
// # true se ci troviamo al houro x
// # Questa soluzione funziona solo se lo si chiama una volta perché
// # la volta successiva ritorna false
// #
// # Se chiami atHour(30), la funzione aggiornerà solo
// #    l'entry m_at_last_hour_map[30], senza influenzare un'eventuale
// #    successiva chiamata a atHour(45).
// ######################################################################
bool LnTime_Class::atHour(uint8_t hour) {
    uint8_t current_hour = m_timeinfo.tm_hour;

    // Se non esiste, prendiamo il valore dallo stato
    if (m_at_last_hour_map.count(hour)) {
        m_at_last_hour = m_at_last_hour_map[hour];
    }

    if (current_hour == hour && current_hour != m_at_last_hour) {
        m_at_last_hour_map[hour] = current_hour; // aggiorniamo il valore
        return true;
    }
    return false;
}






// ===============================================================
// =   V E C T O R   - V E C T O R   - V E C T O R   - V E C T O R   -
// ===============================================================

#ifdef LN_TIME_CLASS_HOURS_VECTOR

// ######################################################################
// # identia a atHour(uint8_t min) ma dedicata alla parte con i vector
// ######################################################################
bool LnTime_Class::atHourFlag(uint8_t hour) {
    uint8_t current_hour = m_timeinfo.tm_min;

    // Se non esiste, prendiamo il valore dallo stato
    if (m_at_last_hour_vector_map.count(hour)) {
        m_at_last_hour_vector = m_at_last_hour_vector_map[hour];
    }

    if (current_hour == hour && current_hour != m_at_last_hour_vector) {
        m_at_last_hour_vector_map[hour] = current_hour; // aggiorniamo il valore
        return true;
    }
    return false;
}


// ######################################################################
// # possiamo fare uso dei vector.
// # quindi impostiamo tutti i valori che vogliamo controllare
// # e la classe provvede a salvare lo stato in una map
// #
// #   const std::vector<uint8_t> hours = {0, 1, 15, 30, 45, 120};
// #
// ######################################################################
void LnTime_Class::setAtHourFlag(const std::vector<uint8_t>& hours) {
    m_at_hours_to_monitor = hours;
    for (uint8_t _hour : m_at_hours_to_monitor) {
        LOG_INFO("_hour: %d", _hour);
    }

}


// ######################################################################
// #
// ######################################################################
bool LnTime_Class::getAtHourFlag(uint8_t hour) {
    if (m_at_hour_flags.count(hour)) {  // se lo troviamo
        return m_at_hour_flags[hour];   // ritorna lo stato
    }
    return false; // Restituisce false se il houro non è stato registrato
}



// ######################################################################
// # Implement the new method to update the flags
// # must be called in loop or in update()
// ######################################################################
void LnTime_Class::updateAtHourFlags() {
    for (uint8_t _hour : m_at_hours_to_monitor) {
        m_at_hour_flags[_hour] = atHourFlag(_hour) ? true : false;
    }
}




#endif