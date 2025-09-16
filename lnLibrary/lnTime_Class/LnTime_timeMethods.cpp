/*
// updated by ...: Loreto Notarantonio
// Date .........: 13-09-2025 17.02.15
*/


#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "LnTime_Class.h"



// ################################################################
// Converte millisecondi in HH:MM:SS.ms
// ritorna il timestamp del giorno
//    withMilliSec = true: aggiunge .xxx alla fine della stringa
//    stripHeader = true: rimuove hour se == 0
// ################################################################
const char* LnTime_Class::msecToHMS(char *buffer, uint8_t buffer_len, uint32_t millisec, bool withMilliSec, bool stripHours) {

    uint16_t msec    = (millisec % 1000UL);
    uint32_t seconds = (millisec / 1000UL);

    uint8_t sec      = seconds  % 60;
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = seconds / 3600;



    if (withMilliSec) {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d.%03lu", hour, min, sec, msec); // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.
    }
    else {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d", hour, min, sec);  // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.
    }


    if (stripHours && hour == 0)  {
        return buffer+3;
    }

    return buffer;
}


const char* LnTime_Class::msecToHMS(uint32_t millisec, bool withMilliSec, bool stripHours) {
    return msecToHMS(sharedTimeBUFFER, sizeof(sharedTimeBUFFER), millisec, withMilliSec, stripHours);
}


const char* LnTime_Class::secToHMS(uint32_t seconds, bool stripHours) {
    return msecToHMS(sharedTimeBUFFER, sizeof(sharedTimeBUFFER), seconds*1000UL, false, stripHours);
}



// Restituisce i secondi mancanti al prossimo minuto completo
int8_t LnTime_Class::secondsToMinute() {
    m_timeinfo = rtc.getTimeStruct();
    uint32_t sec_of_day = (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec;
    int8_t rest = sec_of_day % 60;
    LOG_NOTIFY("sec_of_day: %d rest: %d", sec_of_day, rest);
    return rest;
}

// Restituisce i secondi del giorno
uint32_t LnTime_Class::millisecOfDay(int offset) {
    m_timeinfo = rtc.getTimeStruct();
    return ( (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec + offset ) * 1000UL + rtc.getMillis();
}
// Restituisce i secondi del giorno
uint32_t LnTime_Class::secondsOfDay(int offset) {
    m_timeinfo = rtc.getTimeStruct();
    return (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec + offset;
}

// Restituisce i minuti del giorno
uint32_t LnTime_Class::minutesOfDay(int offset) {
    m_timeinfo = rtc.getTimeStruct();
    return (m_timeinfo.tm_hour * 60) + m_timeinfo.tm_min + offset;
}

// Restituisce l'epoca (epoch time)
uint32_t LnTime_Class::getEpoch(unsigned long offset) {
    return rtc.getEpoch() + offset;
}

// Restituisce la struttura tm
struct tm LnTime_Class::getTimeStruct(void) {
    return rtc.getTimeStruct();
}

// Nuovo metodo per esportare l'istanza di ESP32Time
ESP32Time& LnTime_Class::getRtcInstance() {
    return rtc;
}

