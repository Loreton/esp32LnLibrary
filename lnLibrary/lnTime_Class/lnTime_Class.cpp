/*
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 14.02.00
*/

#include <Arduino.h> // ESP32Time.cpp
#include <ESP32Time.h> // ESP32Time.cpp


#include "lnGlobalVars.h"
// #include "lnTime_Class.h"
// #include "lnLogger_Class.h"

// Costruttore
LnTime_Class::LnTime_Class() {
    // Il costruttore può essere vuoto o inizializzare ESP32Time con valori di default
    // o semplicemente preparare le variabili membro.
    // L'inizializzazione del tempo effettivo verrà fatta in setup().
}

// Inizializzazione del modulo tempo
void LnTime_Class::setup() {
    rtc.setTime(0, 0, 8, 1, 6, 2025); // 1st Jun 2025 08:00:00 - Esempio di data iniziale
    configTzTime(EUROPE_ROME_TZ, "time.google.com", "time.windows.com", "pool.ntp.org");
    LOG_INFO("RTC local time set");
}

// Ottiene l'ora corrente formattata HH:MM:SS
char *LnTime_Class::nowTime() {
    m_timeinfo = rtc.getTimeStruct();
    snprintf(timeBUFFER, sizeof(timeBUFFER), "%02d:%02d:%02d", m_timeinfo.tm_hour, m_timeinfo.tm_min, m_timeinfo.tm_sec);
    return timeBUFFER;
}

// Converte millisecondi in HH:MM:SS.ms
const char* LnTime_Class::timeStamp(char *buffer, uint8_t buffer_len, uint32_t millisec, bool stripHeader) {
    if (millisec == 0) {
        millisec = rtc.getMillis();
    }

    uint16_t msec    = (millisec % 1000);
    uint32_t seconds = (millisec / 1000);
    uint8_t sec      = (seconds  % 60);
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = (seconds / 3600);

    if (stripHeader) {
        if (hour > 0) {
            snprintf(buffer, buffer_len, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
        } else if (min > 0) {
            snprintf(buffer, buffer_len, "%02d:%02d.%03lu", min, sec, msec);
        } else {
            snprintf(buffer, buffer_len, "%02d.%03lu", sec, msec);
        }
    }
    else {
        snprintf(buffer, buffer_len, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
    }


    return buffer;
}

// Converte millisecondi in HH:MM:SS (utilizzando gmtime)
void LnTime_Class::to_HHMMSS(uint32_t mseconds, char *outStr, uint8_t maxlen) {
    time_t rawTime = mseconds / 1000;
    struct tm *timeInfo = gmtime(&rawTime); // Utilizza gmtime per orario UTC
    strftime(outStr, maxlen, "%H:%M:%S", timeInfo);
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
struct tm LnTime_Class::getTimeStruct() {
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

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
    char buffer[64];

    strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", timeinfo_ptr);
    LOG_DEBUG("%s", buffer);

    strftime(buffer, sizeof(buffer), "%A", timeinfo_ptr);
    LOG_DEBUG("Day of week: %s", buffer);

    strftime(buffer, sizeof(buffer), "%B", timeinfo_ptr);
    LOG_DEBUG("Month: %s", buffer);

    strftime(buffer, sizeof(buffer), "%d", timeinfo_ptr);
    LOG_DEBUG("Day of Month: %s", buffer);

    strftime(buffer, sizeof(buffer), "%Y", timeinfo_ptr);
    LOG_DEBUG("Year: %s", buffer);

    strftime(buffer, sizeof(buffer), "%H", timeinfo_ptr);
    LOG_DEBUG("Hour: %s", buffer);

    strftime(buffer, sizeof(buffer), "%I", timeinfo_ptr);
    LOG_DEBUG("Hour (12 hour format): %s", buffer);

    strftime(buffer, sizeof(buffer), "%M", timeinfo_ptr);
    LOG_DEBUG("Minute: %s", buffer);

    strftime(buffer, sizeof(buffer), "%S", timeinfo_ptr);
    LOG_DEBUG("Second: %s", buffer);

    LOG_DEBUG("      Time variables");
    char timeHour[3]; strftime(timeHour, 3, "%H", timeinfo_ptr); LOG_DEBUG("%s", timeHour);
    char timeWeekDay[10]; strftime(timeWeekDay, 10, "%A", timeinfo_ptr); LOG_DEBUG("%s", timeWeekDay);
    LOG_DEBUG("");
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