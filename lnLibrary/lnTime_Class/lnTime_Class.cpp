/*
// updated by ...: Loreto Notarantonio
// Date .........: 10-08-2025 18.46.11
*/

#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp
#include "esp_sntp.h"
#include <WiFi.h>


#include "lnGlobalVars.h"
// #include "lnTime_Class.h"
// #include "lnLogger_Class.h"

#define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

// Costruttore
LnTime_Class::LnTime_Class() {
    // Il costruttore può essere vuoto o inizializzare ESP32Time con valori di default
    // o semplicemente preparare le variabili membro.
    // L'inizializzazione del tempo effettivo verrà fatta in setup().
}



// ==================   NTP functions ==========================
// ==================   NTP functions ==========================
// ==================   NTP functions ==========================


// Implementazione del metodo statico
void LnTime_Class::cbSyncTime(struct timeval *tv) {
    static const char* sntp_status[] = {
        "SNTP_SYNC_STATUS_RESET",
        "SNTP_SYNC_STATUS_COMPLETED",
        "SNTP_SYNC_STATUS_IN_PROGRESS"
    };
    uint8_t status = sntp_get_sync_status();
    LOG_NOTIFY("NTP time synched: %d [%s]", status, sntp_status[status]);
}

void LnTime_Class::initNTP(void) {
    // Controlla se il Wi-Fi è connesso prima di avviare l'NTP
    if (WiFi.status() == WL_CONNECTED && !m_ntp_active) {
        LOG_INFO("WiFi is connected, initializing NTP.");

        // Imposta la modalità di sincronizzazione
        sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);

        // Imposta la callback (ora che è un metodo statico, funziona)
        sntp_set_time_sync_notification_cb(cbSyncTime);

        // Imposta l'intervallo di sincronizzazione
        sntp_set_sync_interval(12 * 60 * 60 * 1000UL); // 12 ore
        sntp_set_sync_interval(10 * 60 * 1000UL); // 10 minuti

        // Imposta i server NTP
        // configTime(0, 0, ntpServer1, ntpServer2, ntpServer3);
        configTime(0, 0, m_ntpServer1, m_ntpServer2, m_ntpServer3);

        // Imposta il fuso orario
        setenv("TZ", EUROPE_ROME_TZ, 1);
        tzset();
        m_ntp_active = true;
        // Allinea l'RTC interno con l'ora NTP
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            rtc.setTimeStruct(timeinfo);
            LOG_INFO("RTC synchronized with NTP time.");
        }
    } else {
        LOG_WARN("WiFi not connected. Skipping NTP initialization.");
        m_ntp_active = false;
    }
}

void LnTime_Class::update(void) {
    if (WiFi.status() == WL_CONNECTED && !m_ntp_active) {
        initNTP();
    }
}



// ==================   TIME functions ==========================


// Inizializzazione del modulo tempo
void LnTime_Class::setup() {
    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi is connected. Synchronizing time with NTP server...");
        initNTP(); // Imposta il fuso orario e i server NTP
        LOG_INFO("NTP time synchronization initiated.");
    }
    else {
        rtc.setTime(0, 0, 8, 1, 6, 2025); // 1st Jun 2025 08:00:00 - Esempio di data iniziale
        configTzTime(EUROPE_ROME_TZ,  m_ntpServer1, m_ntpServer2, m_ntpServer3);
        LOG_INFO("RTC local time set");
    }
}







// Ottiene l'ora corrente formattata HH:MM:SS
char *LnTime_Class::nowTime() {
    m_timeinfo = rtc.getTimeStruct();
    snprintf(sharedTimeBUFFER, sizeof(sharedTimeBUFFER), "%02d:%02d:%02d", m_timeinfo.tm_hour, m_timeinfo.tm_min, m_timeinfo.tm_sec);
    return sharedTimeBUFFER;
}





// // Converte millisecondi in HH:MM:SS.ms
// const char* LnTime_Class::timeStamp(char *buffer, uint8_t buffer_len, uint32_t millisec, bool stripHeader) {
//     if (millisec == 0) {
//         int64_t time_since_boot = esp_timer_get_time(); // Time in microseconds from boot
//         millisec = time_since_boot / 1000; // Time in microseconds from boot
//     }

//     uint16_t msec    = (millisec % 1000);
//     uint32_t seconds = (millisec / 1000);
//     uint8_t sec      = (seconds  % 60);
//     uint8_t min      = (seconds / 60) % 60;
//     uint8_t hour     = (seconds / 3600);

//     snprintf(buffer, buffer_len, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
//     if (stripHeader) {
//         if (hour > 0) {
//             return buffer;
//         } else if (min > 0) {
//             return buffer+3;
//         } else {
//             return buffer+6;
//         }
//     }
//     return buffer;
// }

/** ------------------ Sample
    if (millisec == 0) {
        struct tm timeInfo = rtc.getTimeStruct(); // ora corrente
        millisec = rtc.getMillis();  // current mSeconds (0-999)
        strftime(buffer, buffer_len, "%H:%M:%S", &timeInfo);
    }
    else {
        time_t rawTime = millisec / 1000;
        struct tm *timeInfo = gmtime(&rawTime); // Utilizza gmtime per orario UTC - vedi getEpoch()
        strftime(buffer, buffer_len, "%H:%M:%S", timeInfo);
    }
----------------- */

// ################################################################
// Converte millisecondi in HH:MM:SS.ms
// ritorna il timestamp del giorno
//    se millisec != 0 allora converte i millisec in timestamp
//    addMilliSec = true: aggiunge .xxx alla fine della stringa
//    stripHeader = true: rimuove hour o minutes se == 0
// ################################################################
const char* LnTime_Class::timeStamp(char *buffer, uint8_t buffer_len, uint32_t millisec, bool addMilliSec, bool stripHeader) {
    uint16_t msec;
    uint32_t seconds;

    if (millisec == 0) {
        m_timeinfo = rtc.getTimeStruct();
        seconds = (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec;
        msec = rtc.getMillis();  // current mSeconds (0-999)
    }
    else {
        msec    = (millisec % 1000);
        seconds = (millisec / 1000);
    }

    uint8_t sec      = (seconds  % 60);
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = (seconds / 3600);

    if (addMilliSec) {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
    }
    else {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d", hour, min, sec);
    }

    if (stripHeader) {
        if (hour > 0) {
            return buffer;
        } else if (min > 0) {
            return buffer+3;
        } else {
            return buffer+6;
        }
    }
    return buffer;
}


#ifdef LN_SCARTATA
// ################################################################
// Converte millisecondi in HH:MM:SS (utilizzando gmtime)
// ################################################################
const char *LnTime_Class::to_HHMMSS_discarded(uint32_t millisec, char *buffer, uint8_t buffer_len, bool addMilliSec) {
    uint16_t msec, seconds;

    if (millisec == 0) {
        msec = rtc.getMillis();  // current mSeconds (0-999)
        seconds = rtc.getEpoch();
    }
    else {
        msec    = (millisec % 1000);
        seconds = (millisec / 1000);
    }


    time_t rawTime = seconds;
    struct tm *timeInfo = gmtime(&rawTime); // Utilizza gmtime per orario UTC
    strftime(buffer, buffer_len, "%H:%M:%S", timeInfo);
    if (addMilliSec) {
        snprintf(buffer, buffer_len, "%0s:%03lu", buffer, msec);
    }
    return buffer;
}
#endif




// true: se ci troviamo nel modulo de secondo richiestso (Sec%reqSec)
bool LnTime_Class::everyXseconds(uint8_t seconds) {
    uint8_t curr_second;

    m_timeinfo = rtc.getTimeStruct();
    curr_second = m_timeinfo.tm_sec;

    if (curr_second%seconds == 0 && curr_second != m_last_second) { // ogni 5 secondi
        m_last_second = curr_second;
        return true;
    }
    return false;
}



// true: se ci troviamo nel modulo de secondo richiestso (Sec%reqSec)
bool LnTime_Class::everyXminutes(uint8_t minutes) {
    uint8_t curr_minute;

    m_timeinfo = rtc.getTimeStruct();
    curr_minute = m_timeinfo.tm_min;

    if (curr_minute%minutes == 0 && curr_minute != m_last_minute) { // ogni 5 minutei
        m_last_minute = curr_minute;
        return true;
    }
    return false;
}



// Allinea l'esecuzione all'inizio del minuto
void LnTime_Class::alignToMinute() {
    LOG_DEBUG("waiting for minute o'clock");
    m_timeinfo = rtc.getTimeStruct();
    while (m_timeinfo.tm_sec != 0) {
        delay(50); // Piccolo ritardo per evitare busy-waiting eccessivo
        m_timeinfo = rtc.getTimeStruct();
    }
    // LOG_INFO("ready...");
}

// Controlla se è iniziato un nuovo secondo
bool LnTime_Class::isSecondOClock() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_sec != m_last_second) {
        m_last_second = m_timeinfo.tm_sec;
        return true;
    }
    return false;
}

// Controlla se è iniziato un nuovo minuto
bool LnTime_Class::isMinuteOClock() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_sec == 0 && m_timeinfo.tm_min != m_last_minute) {
        m_last_minute = m_timeinfo.tm_min;
        return true;
    }
    return false;
}

// Controlla se è iniziato un nuovo quarto d'ora
bool LnTime_Class::isQuarterOClock() {
    m_timeinfo = rtc.getTimeStruct();
    if (m_timeinfo.tm_min % 15 == 0 && m_timeinfo.tm_min != m_last_minute) {
        m_last_minute = m_timeinfo.tm_min;
        return true;
    }
    return false;
}

// Attende il cambio di secondo
int8_t LnTime_Class::waitForSecond() {
    m_last_second = m_timeinfo.tm_sec; // Usa m_timeinfo che dovrebbe essere aggiornato dall'ultima chiamata a getTimeStruct
    while (rtc.getTimeStruct().tm_sec == m_last_second) { // Rileggi direttamente da rtc per l'attesa
        delay(50);
    }
    return rtc.getTimeStruct().tm_sec - m_last_second;
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
    return ( (m_timeinfo.tm_hour * 3600) + (m_timeinfo.tm_min * 60) + m_timeinfo.tm_sec + offset ) * 1000 + rtc.getMillis();
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

// Nuovo metodo per esporre l'istanza di ESP32Time
ESP32Time& LnTime_Class::getRtcInstance() {
    return rtc;
}


// Crea un'istanza della classe LnTime
// visibile da tutti. Se ne serve una dedicata la posso creare nel modulo
LnTime_Class lnTime;



// Implementazioni delle funzioni di stampa (possono essere in un nuovo file .cpp, es. LnTime_ClassUtils.cpp)
void printLocalTime(const struct tm *timeinfo_ptr) {

#if LOG_LEVEL >= LOG_LEVEL_TRACE
    char buffer[64];

    strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", timeinfo_ptr);
    LOG_SPEC("%s", buffer);

    strftime(buffer, sizeof(buffer), "%A", timeinfo_ptr);
    LOG_SPEC("Day of week: %s", buffer);

    strftime(buffer, sizeof(buffer), "%B", timeinfo_ptr);
    LOG_SPEC("Month: %s", buffer);

    strftime(buffer, sizeof(buffer), "%d", timeinfo_ptr);
    LOG_SPEC("Day of Month: %s", buffer);

    strftime(buffer, sizeof(buffer), "%Y", timeinfo_ptr);
    LOG_SPEC("Year: %s", buffer);

    strftime(buffer, sizeof(buffer), "%H", timeinfo_ptr);
    LOG_SPEC("Hour: %s", buffer);

    strftime(buffer, sizeof(buffer), "%I", timeinfo_ptr);
    LOG_SPEC("Hour (12 hour format): %s", buffer);

    strftime(buffer, sizeof(buffer), "%M", timeinfo_ptr);
    LOG_SPEC("Minute: %s", buffer);

    strftime(buffer, sizeof(buffer), "%S", timeinfo_ptr);
    LOG_SPEC("Second: %s", buffer);

    LOG_SPEC("      Time variables");
    char timeHour[3]; strftime(timeHour, 3, "%H", timeinfo_ptr); LOG_SPEC("%s", timeHour);
    char timeWeekDay[10]; strftime(timeWeekDay, 10, "%A", timeinfo_ptr); LOG_SPEC("%s", timeWeekDay);
    LOG_SPEC("");
#endif
}

void print_rtc_time(LnTime_Class *p) {
    if (!p) return; // Controllo per puntatore nullo

#if LOG_LEVEL >= LOG_LEVEL_DEBUG

    ESP32Time& rtc_instance = p->getRtcInstance(); // Ottieni il riferimento all'istanza rtc

    LOG_NOTIFY("\t%-20s: %s", "getDate(short)",      rtc_instance.getDate(false).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getDate(long)",       rtc_instance.getDate(true).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getTime()",           rtc_instance.getTime());
    LOG_NOTIFY("\t%-20s: %s", "getDateTime(short)",  rtc_instance.getDateTime(false).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getDateTime(long)",   rtc_instance.getDateTime(true).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getTimeDate(short)",  rtc_instance.getTimeDate(false).c_str());
    LOG_NOTIFY("\t%-20s: %s", "getTimeDate(long)",   rtc_instance.getTimeDate(true).c_str());

    LOG_NOTIFY("\t%-20s: %lu", "getEpoch()",          rtc_instance.getEpoch());
    LOG_NOTIFY("\t%-20s: %d",  "getHour()",           rtc_instance.getHour());
    LOG_NOTIFY("\t%-20s: %d",  "getMinute()",         rtc_instance.getMinute());
    LOG_NOTIFY("\t%-20s: %d",  "getSecond()",         rtc_instance.getSecond());
    LOG_NOTIFY("\t%-20s: %lu", "getMillis()",         rtc_instance.getMillis());
    LOG_NOTIFY("\t%-20s: %lu", "getMicros()",         rtc_instance.getMicros());

    LOG_NOTIFY("\t%-20s: %d",  "getHour(true)",       rtc_instance.getHour(true));
    LOG_NOTIFY("\t%-20s: %s",  "getAmPm(uppercase)",  rtc_instance.getAmPm(false).c_str());
    LOG_NOTIFY("\t%-20s: %s",  "getAmPm(lowcase)",    rtc_instance.getAmPm(true).c_str());

    LOG_NOTIFY("\t%-20s: %d",  "getYear()",           rtc_instance.getYear());
    LOG_NOTIFY("\t%-20s: %d",  "getMonth()",          rtc_instance.getMonth());
    LOG_NOTIFY("\t%-20s: %d",  "getDay()",            rtc_instance.getDay());
    LOG_NOTIFY("\t%-20s: %d",  "getDayofWeek()",      rtc_instance.getDayofWeek());
    LOG_NOTIFY("\t%-20s: %d",  "getDayofYear()",      rtc_instance.getDayofYear());
    LOG_NOTIFY("\t%-20s: %lu", "getLocalEpoch()",     rtc_instance.getLocalEpoch());
    LOG_NOTIFY("\t%-20s: %s",  "getAmPm()",           rtc_instance.getTime("%A, %B %d %Y %H:%M:%S").c_str());

    struct tm timeinfo_local = rtc_instance.getTimeStruct();
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &timeinfo_local);
    LOG_NOTIFY("\t%-20s: %s", "time struct", buffer);
#endif
}