/*
// updated by ...: Loreto Notarantonio
// Date .........: 26-07-2025 10.04.35
*/

#include "Arduino.h"
#include <ESP32Time.h> // ESP32Time.cpp


// ---------------------------------
// loreto headers files
// ---------------------------------
#include "lnTime.h"

// ---------------------------------
// macros Aliases for LOG
// ---------------------------------
#include "lnLogger.h"





#define TIME_BUFFER_LENGTH 16
ESP32Time     rtc;
char PROGMEM timeBuffer[TIME_BUFFER_LENGTH];


#define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"


struct tm timeinfo = rtc.getTimeStruct();

// #########################################
// #
// #########################################
void time_setup() {
    rtc.setTime(0, 0, 8, 21, 1, 2025);  // 21th Jan 2025 08:00:00
    // rtc.setTime(00, 00, 15, 17, 1, 2024);  // 17th Jan 2021 15:24:30
    //rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
    // rtc.offset = 7200; // change offset value
    // rtc.offset = 3600; // change offset value
    // setenv("TZ", EUROPE_ROME_TZ, 1);    // Set environment variable with your time zone
    // tzset();
    configTzTime(EUROPE_ROME_TZ, "time.google.com", "time.windows.com", "pool.ntp.org");

    LOG_INFO("RTC local time set");
}


// // #########################################
// // #
// // #########################################
char *nowTime() {
    // struct tm timeinfo = get_timeinfo();
    timeinfo = rtc.getTimeStruct();
    snprintf(timeBuffer, TIME_BUFFER_LENGTH, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    // snprintf(timeBuffer, TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return timeBuffer;
}


const char* mSecTo_HHMMSSms(uint32_t millisec, char *buffer, uint8_t buffer_len) {
    if (millisec == 0) {
        millisec = rtc.getMillis(); // Tempo in millisecondi
    }

    if (buffer && buffer_len != 0) {
    }
    else {
        buffer = timeBuffer; // definito in testa
        buffer_len = TIME_BUFFER_LENGTH;
    }
    uint16_t msec    = (millisec % 1000); // valode da 0-1000
    uint32_t seconds = (millisec / 1000); // potrebbe essere lungo
    uint8_t sec      = (seconds  % 60);
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = (seconds / 3600);
    snprintf(buffer, buffer_len, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
    return buffer;
}



// converts to (HH:MM:SS)
void to_HHMMSS(uint32_t mseconds, char *outStr, uint8_t maxlen) {
    // time_t rawTime = millis() / 1000;
    // struct tm *timeInfo = gmtime(&rawTime);
    // char timeStr[32];
    // strftime(timeStr, sizeof(timeStr), "ESP32-CAM run since: %H:%M:%S", timeInfo);
    // Serial.println(timeStr);
    // myBot.sendMessage(msg, timeStr);

    time_t rawTime = mseconds / 1000;
    struct tm *timeInfo = gmtime(&rawTime);
    strftime(outStr, maxlen, "%H:%M:%S", timeInfo);
}

// #########################################
// #
// #########################################
// void printTime(void) {
    // LOG_NOTIFY("\tnow: %s\tepoch: %ull\tsecondsOfDay: %d\tminutesOfDay: %d", Now(), getEpoch(), secondsOfDay(), minutesOfDay());
// }

// allineamento al minuto ...
void alignToMinute() {
    LOG_INFO("waiting for minute o'clock");
    timeinfo = rtc.getTimeStruct();
    while (timeinfo.tm_sec != 0) {
        timeinfo = rtc.getTimeStruct();
    }
    LOG_INFO("ready...");
}


bool isSecondOClock() {
    static int8_t last_second=-1;
    timeinfo = rtc.getTimeStruct();
    if (timeinfo.tm_sec != last_second) {
        last_second = timeinfo.tm_sec;
        return true;
    }
    return false;
}



bool isMinuteOClock() {
    static int8_t last_minute=-1;
    timeinfo = rtc.getTimeStruct();
    if (timeinfo.tm_sec == 0 && timeinfo.tm_min != last_minute) {
        last_minute = timeinfo.tm_min;
        return true;
    }
    return false;
}

bool isQuarterOClock() {
    static int8_t last_minute=-1;
    timeinfo = rtc.getTimeStruct();

    if (timeinfo.tm_min%15 == 0 && timeinfo.tm_min != last_minute) {
        last_minute = timeinfo.tm_min;
        return true;
    }
    return false;
}


// #########################################
// #
// #########################################
int8_t waitForSecond(void) {
    int8_t last_second = timeinfo.tm_sec;
    while (timeinfo.tm_sec == last_second) {
        delay(50);
        timeinfo = rtc.getTimeStruct(); // re-get time
    }
    return timeinfo.tm_sec - last_second;
}

// #########################################
// # return mssing seconds to minutes (60)
// #########################################
int secondsToMinute() {
    timeinfo = rtc.getTimeStruct();
    int sec_of_day=(timeinfo.tm_hour*3600) + (timeinfo.tm_min*60)  + timeinfo.tm_sec;
    int rest = sec_of_day % 60;
    LOG_NOTIFY("sec_of_day: %d rest: %d", sec_of_day, rest);
    return rest;
}




// #########################################
// #
// #########################################
// struct tm get_timeinfo(void) {
//     struct tm timeinfo;
//     getLocalTime(&timeinfo);
//     return timeinfo;
// }

// #########################################
// # return seconds
// #########################################
// uint8_t getSeconds(void) {
//     timeinfo = rtc.getTimeStruct();
//     struct tm timeinfo;
//     getLocalTime(&timeinfo);
//     return timeinfo.tm_sec;
// }


// #########################################
// # return seconds of day
// #########################################
int secondsOfDay(int offset) {
    timeinfo = rtc.getTimeStruct();
    return (timeinfo.tm_hour*3600) + (timeinfo.tm_min*60) + timeinfo.tm_sec + offset;
}

// #########################################
// # return minutes of day
// #########################################
int minutesOfDay(int offset) {
    timeinfo = rtc.getTimeStruct();
    return (timeinfo.tm_hour*3600) + timeinfo.tm_min  + offset;
}




// #########################################
// # return seconds of day
// #########################################
unsigned long getEpoch(unsigned long offset) {
    return rtc.getEpoch() + offset;
}


// #########################################
// #
// #########################################
void printLocalTime(struct tm *timeinfo) {
    LOG_NOTIFY("%s", (timeinfo, "%A, %B %d %Y %H:%M:%S"));
    LOG_NOTIFY("Day of week: %s",           (timeinfo, "%A"));
    LOG_NOTIFY("Month: %s",                 (timeinfo, "%B"));
    LOG_NOTIFY("Day of Month: %s",          (timeinfo, "%d"));
    LOG_NOTIFY("Year: %s",                  (timeinfo, "%Y"));
    LOG_NOTIFY("Hour: %s",                  (timeinfo, "%H"));
    LOG_NOTIFY("Hour (12 hour format): %s", (timeinfo, "%I"));
    LOG_NOTIFY("Minute: %s",                (timeinfo, "%M"));
    LOG_NOTIFY("Second: %s",                (timeinfo, "%S"));
    // formating options  http://www.cplusplus.com/reference/ctime/strftime/

    LOG_NOTIFY("     Time variables");
    char timeHour[3]; strftime(timeHour, 3, "%H", timeinfo); LOG_NOTIFY("%s", timeHour);
    char timeWeekDay[10]; strftime(timeWeekDay, 10, "%A", timeinfo); LOG_NOTIFY("%s", timeWeekDay);
    LOG_NOTIFY("");
}

void print_rtc_time() {
    LOG_NOTIFY("\t%-20s: %s",   "getDate(short)",     ((char*)rtc.getDate(false).c_str()));          // (String) Sun, Jan 17 2021        false = short date format
    LOG_NOTIFY("\t%-20s: %s",   "getDate(long)",      rtc.getDate(true).c_str());                    // (String) Sunday, January 17 2021 true = Long date format
    LOG_NOTIFY("\t%-20s: %s",   "getTime()",          rtc.getTime());                                // (String) 15:24:38
    LOG_NOTIFY("\t%-20s: %s",   "getDateTime(short)", rtc.getDateTime(false).c_str());               // (String) Sun, Jan 17 2021 15:24:38
    LOG_NOTIFY("\t%-20s: %s",   "getDateTime(long)",  rtc.getDateTime(true).c_str());                // (String) Sunday, January 17 2021 15:24:38
    LOG_NOTIFY("\t%-20s: %s",   "getTimeDate(short)", rtc.getTimeDate(false).c_str());               // (String) 15:24:38 Sun, Jan 17 2021
    LOG_NOTIFY("\t%-20s: %s",   "getTimeDate(long)",  rtc.getTimeDate(true).c_str());                // (String) 15:24:38 Sunday, January 17 2021

    LOG_NOTIFY("\t%-20s: %lu",  "getEpoch()",         rtc.getEpoch());                               // (long)    1609459200
    LOG_NOTIFY("\t%-20s: %d",   "getHour()",          rtc.getHour());                                // (int)     3     (1-12)
    LOG_NOTIFY("\t%-20s: %d",   "getMinute()",        rtc.getMinute());                              // (int)     24    (0-59)
    LOG_NOTIFY("\t%-20s: %d",   "getSecond()",        rtc.getSecond());                              // (int)     38    (0-59)
    LOG_NOTIFY("\t%-20s: %lu",  "getMillis()",        rtc.getMillis());                              // (long)    723
    LOG_NOTIFY("\t%-20s: %lu",  "getMicros()",        rtc.getMicros());                              // (long)    723546

    LOG_NOTIFY("\t%-20s: %d",   "getHour(true)",      rtc.getHour(true));                            // (int)     15    (0-23)
    LOG_NOTIFY("\t%-20s: %s",   "getAmPm(uppercase)", rtc.getAmPm(false).c_str());                   // (String)  PM
    LOG_NOTIFY("\t%-20s: %s",   "getAmPm(lowcase)",   rtc.getAmPm(true).c_str());                    // (String)  pm

    LOG_NOTIFY("\t%-20s: %d",   "getYear()",          rtc.getYear());                                // (int)     2021
    LOG_NOTIFY("\t%-20s: %d",   "getMonth()",         rtc.getMonth());                               // (int)     0     (0-11)
    LOG_NOTIFY("\t%-20s: %d",   "getDay()",           rtc.getDay());                                 // (int)     17    (1-31)
    LOG_NOTIFY("\t%-20s: %d",   "getDayofWeek()",     rtc.getDayofWeek());                           // (int)     0     (0-6)
    LOG_NOTIFY("\t%-20s: %d",   "getDayofYear()",     rtc.getDayofYear());                           // (int)     16    (0-365)
    LOG_NOTIFY("\t%-20s: %lu",  "getLocalEpoch()",    rtc.getLocalEpoch());                          // (long)    1609459200 epoch without offset
    LOG_NOTIFY("\t%-20s: %s",   "getAmPm()",          rtc.getTime("%A, %B %d %Y %H:%M:%S").c_str()); // (String) returns time with specified format
    // formating options  http://www.cplusplus.com/reference/ctime/strftime/


    struct tm timeinfo = rtc.getTimeStruct();
    LOG_NOTIFY("\t%-20s: %s", "time struct", (&timeinfo, "%A, %B %d %Y %H:%M:%S"));   //  (tm struct) Sunday, January 17 2021 07:24:38

}






