/*
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 17.56.04
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
// # true se ci troviamo al nuovo minuto)
// ######################################################################
bool LnTime_Class::onDay() {
    // m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_yday != m_last_yday) {
        m_last_yday = m_timeinfo.tm_yday;

        /* Reset della mappa dei minuti per riattivare i flag
        altrimenti funziona solo per la prima ora */
        m_at_last_hour_map.clear();
        #ifdef LN_TIME_CLASS_HOURS_VECTOR
            m_at_last_hour_vector_map.clear();
        #endif

        return true;
    }
    return false;
}
