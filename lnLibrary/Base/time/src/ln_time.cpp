/*
// updated by ...: Loreto Notarantonio
// Date .........: 29-06-2025 17.45.03
*/

#include "Arduino.h"
#include <ESP32Time.h> // ESP32Time.cpp


// ---------------------------------
// loreto headers files
// ---------------------------------
#include "@ln_time.h"

// ---------------------------------
// macros Aliases for LOG
// ---------------------------------
#define LOG_LEVEL_1
#include "@logMacros.h"





#define TIME_BUFFER_LENGTH 10
ESP32Time     rtc;
char PROGMEM buffer_time[TIME_BUFFER_LENGTH];


#define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"


struct tm timeinfo = rtc.getTimeStruct();

// #########################################
// #
// #########################################
void time_setup() {
    rtc.setTime(30, 24, 15, 21, 1, 2025);  // 21th Jan 2025 15:24:30
    // rtc.setTime(00, 00, 15, 17, 1, 2024);  // 17th Jan 2021 15:24:30
    //rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
    // rtc.offset = 7200; // change offset value
    // rtc.offset = 3600; // change offset value
    // setenv("TZ", EUROPE_ROME_TZ, 1);    // Set environment variable with your time zone
    // tzset();
    configTzTime(EUROPE_ROME_TZ, "time.google.com", "time.windows.com", "pool.ntp.org");

    printf1_NFN("RTC local time set\n");
}


// // #########################################
// // #
// // #########################################
char *nowTime() {
    // struct tm timeinfo = get_timeinfo();
    timeinfo = rtc.getTimeStruct();
    snprintf(buffer_time, TIME_BUFFER_LENGTH, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    // snprintf(buffer_time, TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return buffer_time;
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
    // printf1_FN("\tnow: %s\n\tepoch: %ull\n\tsecondsOfDay: %d\n\tminutesOfDay: %d\n", Now(), getEpoch(), secondsOfDay(), minutesOfDay());
// }

// allineamento al minuto ...
void alignToMinute() {
    printf1_NFN("waiting for minute o'clock\n");
    timeinfo = rtc.getTimeStruct();
    while (timeinfo.tm_sec != 0) {
        timeinfo = rtc.getTimeStruct();
    }
    printf1_NFN("ready...\n");
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
    printf1_FN("sec_of_day: %d rest: %d\n", sec_of_day, rest);
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
    printf1(timeinfo, "%A, %B %d %Y %H:%M:%S\n");
    printf1_FN("Day of week: ");           printf1(timeinfo, "%A\n");
    printf1_FN("Month: ");                 printf1(timeinfo, "%B\n");
    printf1_FN("Day of Month: ");          printf1(timeinfo, "%d\n");
    printf1_FN("Year: ");                  printf1(timeinfo, "%Y\n");
    printf1_FN("Hour: ");                  printf1(timeinfo, "%H\n");
    printf1_FN("Hour (12 hour format): "); printf1(timeinfo, "%I\n");
    printf1_FN("Minute: ");                printf1(timeinfo, "%M\n");
    printf1_FN("Second: ");                printf1(timeinfo, "%S\n");
    // formating options  http://www.cplusplus.com/reference/ctime/strftime/

    printf1_FN("     Time variables");
    char timeHour[3]; strftime(timeHour, 3, "%H", timeinfo); printf1_FN(timeHour);
    char timeWeekDay[10]; strftime(timeWeekDay, 10, "%A", timeinfo); printf1_FN(timeWeekDay);
    printf1_FN("\n");
}

void print_rtc_time() {
    printf1_FN("\t%-20s: %s\n",   "getDate(short)",     ((char*)rtc.getDate(false).c_str()));          // (String) Sun, Jan 17 2021        false = short date format
    printf1_FN("\t%-20s: %s\n",   "getDate(long)",      rtc.getDate(true).c_str());                    // (String) Sunday, January 17 2021 true = Long date format
    printf1_FN("\t%-20s: %s\n",   "getTime()",          rtc.getTime());                                // (String) 15:24:38
    printf1_FN("\t%-20s: %s\n",   "getDateTime(short)", rtc.getDateTime(false).c_str());               // (String) Sun, Jan 17 2021 15:24:38
    printf1_FN("\t%-20s: %s\n",   "getDateTime(long)",  rtc.getDateTime(true).c_str());                // (String) Sunday, January 17 2021 15:24:38
    printf1_FN("\t%-20s: %s\n",   "getTimeDate(short)", rtc.getTimeDate(false).c_str());               // (String) 15:24:38 Sun, Jan 17 2021
    printf1_FN("\t%-20s: %s\n",   "getTimeDate(long)",  rtc.getTimeDate(true).c_str());                // (String) 15:24:38 Sunday, January 17 2021

    printf1_FN("\t%-20s: %ul\n",  "getEpoch()",         rtc.getEpoch());                               // (long)    1609459200
    printf1_FN("\t%-20s: %d\n",   "getHour()",          rtc.getHour());                                // (int)     3     (1-12)
    printf1_FN("\t%-20s: %d\n",   "getMinute()",        rtc.getMinute());                              // (int)     24    (0-59)
    printf1_FN("\t%-20s: %d\n",   "getSecond()",        rtc.getSecond());                              // (int)     38    (0-59)
    printf1_FN("\t%-20s: %ul\n",  "getMillis()",        rtc.getMillis());                              // (long)    723
    printf1_FN("\t%-20s: %ul\n",  "getMicros()",        rtc.getMicros());                              // (long)    723546

    printf1_FN("\t%-20s: %d\n",   "getHour(true)",      rtc.getHour(true));                            // (int)     15    (0-23)
    printf1_FN("\t%-20s: %s\n",   "getAmPm(uppercase)", rtc.getAmPm(false).c_str());                   // (String)  PM
    printf1_FN("\t%-20s: %s\n",   "getAmPm(lowcase)",   rtc.getAmPm(true).c_str());                    // (String)  pm

    printf1_FN("\t%-20s: %d\n",   "getYear()",          rtc.getYear());                                // (int)     2021
    printf1_FN("\t%-20s: %d\n",   "getMonth()",         rtc.getMonth());                               // (int)     0     (0-11)
    printf1_FN("\t%-20s: %d\n",   "getDay()",           rtc.getDay());                                 // (int)     17    (1-31)
    printf1_FN("\t%-20s: %d\n",   "getDayofWeek()",     rtc.getDayofWeek());                           // (int)     0     (0-6)
    printf1_FN("\t%-20s: %d\n",   "getDayofYear()",     rtc.getDayofYear());                           // (int)     16    (0-365)
    printf1_FN("\t%-20s: %ull\n", "getLocalEpoch()",    rtc.getLocalEpoch());                          // (long)    1609459200 epoch without offset
    printf1_FN("\t%-20s: %s\n",   "getAmPm()",          rtc.getTime("%A, %B %d %Y %H:%M:%S").c_str()); // (String) returns time with specified format
    // formating options  http://www.cplusplus.com/reference/ctime/strftime/


    struct tm timeinfo = rtc.getTimeStruct();
    printf1_FN("\t%-20s: %s\n", "time struct", (&timeinfo, "%A, %B %d %Y %H:%M:%S"));   //  (tm struct) Sunday, January 17 2021 07:24:38

}

