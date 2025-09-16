/*
// updated by ...: Loreto Notarantonio
// Date .........: 13-09-2025 16.57.57
*/


#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp
#include "esp_sntp.h"
#include <WiFi.h>


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "LnTime_Class.h"


// ----------------------------------------------------
// Classe LnTime_Class
// ----------------------------------------------------
LnTime_Class lnTime;

// ==================   TIME functions ==========================


// Costruttore
LnTime_Class::LnTime_Class() {}

// ################################################################
// # ntpIntervalTimeSync: seconds
// ################################################################
void LnTime_Class::setup(uint16_t ntpIntervalTimeSync) {
    m_NTP_SYNC_INTERVAL = ntpIntervalTimeSync*1000UL; // per la funzione sntp_set_sync_interval(millisec)

    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi is connected. Synchronizing time with NTP server...");
        initNTP(); // Imposta il fuso orario e i server NTP
        LOG_INFO("NTP time synchronization initiated.");
    }
    else {
        rtc.setTime(0, 0, 8, 1, 6, 2025); // 1st Jun 2025 08:00:00 - Esempio di data iniziale
        configTzTime(EUROPE_ROME_TZ,  m_ntpServer1, m_ntpServer2, m_ntpServer3);
        LOG_INFO("RTC local time set");
        m_ntp_active = false;
    }
}



// ################################################################
// #
// ################################################################
void LnTime_Class::set_dhmCustomUpdate(bool customUpdate) {
    m_dhmCustomUpdate = customUpdate;
}



// ################################################################
// # ritorna l'ora corrente formattata HH:MM:SS
// ################################################################
char *LnTime_Class::now() {
    m_timeinfo = rtc.getTimeStruct();
    snprintf(sharedTimeBUFFER, sizeof(sharedTimeBUFFER), "%02d:%02d:%02d", m_timeinfo.tm_hour, m_timeinfo.tm_min, m_timeinfo.tm_sec); // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.
    return sharedTimeBUFFER;
}


