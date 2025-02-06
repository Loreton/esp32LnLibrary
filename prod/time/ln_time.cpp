/*
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 08.47.40
*/

#include "Arduino.h"
#include <ESP32Time.h> // ESP32Time.cpp

#define __LN_TIME_CPP__
#include "@ln_time.h"

// --- others
#include "@lnMacros.h"

#define TIME_BUFFER_LENGTH 10


ESP32Time     rtc;
char PROGMEM buffer_time[TIME_BUFFER_LENGTH];


#define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

#define debug0                    Serial.print
#define debug0_ln                 Serial.println
#define debug0_f                  ln_printf
#define debug00_now               ln_printf_Now
#define debug01_nowfn             ln_printf_NowFN



// #########################################
// #
// #########################################
void time_setup() {
    rtc.setTime(30, 24, 15, 21, 1, 2025);  // 21th Jan 2025 15:24:30
    // rtc.setTime(00, 00, 15, 17, 1, 2024);  // 17th Jan 2021 15:24:30
    //rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
    // rtc.offset = 7200; // change offset value
    // rtc.offset = 3600; // change offset value
    setenv("TZ", EUROPE_ROME_TZ, 1);    // Set environment variable with your time zone
    tzset();

    debug00_now("RTC local time set\n");
}


// // #########################################
// // #
// // #########################################
char *nowTime() {
    // struct tm timeinfo = get_timeinfo();
    struct tm timeinfo = rtc.getTimeStruct();
    snprintf(buffer_time, TIME_BUFFER_LENGTH, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    // snprintf(buffer_time, TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return buffer_time;
}



// #########################################
// #
// #########################################
void printTime(void) {
    debug0_f("\tnow: %s\n\tepoch: %ull\n\tsecondsOfDay: %d\n\tminutesOfDay: %d\n", Now(), getEpoch(), secondsOfDay(), minutesOfDay());
}



// #########################################
// #
// #########################################
int8_t waitForSecond(void) {
    struct tm timeinfo = rtc.getTimeStruct();
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
    struct tm timeinfo = rtc.getTimeStruct();
    int sec_of_day=(timeinfo.tm_hour*3600) + (timeinfo.tm_min*60)  + timeinfo.tm_sec;
    int rest = sec_of_day % 60;
    debug0_f("sec_of_day: %d rest: %d\n", sec_of_day, rest);
    return rest;
}




// #########################################
// #
// #########################################
struct tm get_timeinfo(void) {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo;
}

// #########################################
// # return seconds
// #########################################
uint8_t getSeconds(void) {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return timeinfo.tm_sec;
}


// #########################################
// # return seconds of day
// #########################################
int secondsOfDay(int offset) {
    struct tm timeinfo = rtc.getTimeStruct();
    return (timeinfo.tm_hour*3600) + (timeinfo.tm_min*60) + timeinfo.tm_sec + offset;
}

// #########################################
// # return minutes of day
// #########################################
int minutesOfDay(int offset) {
    struct tm timeinfo = rtc.getTimeStruct();
    return (timeinfo.tm_hour*3600) + timeinfo.tm_min  + offset;
}




// #########################################
// # return seconds of day
// #########################################
unsigned long getEpoch(unsigned long offset) {
    return rtc.getEpoch() + offset;
}


// https://stackoverflow.com/questions/61826937/get-ntp-time-in-milliseconds
// static int64_t getNowUs() {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);

//     return (int64_t)tv.tv_usec + tv.tv_sec * 1000000ll;
// }

// uint64_t getNowUs2() {
//     uint64_t start = esp_timer_get_time();
//     debug01_nowfn("time %lld\n", start);
//     return start;
// }

// #########################################
// #
// #########################################
void printLocalTime(struct tm *timeinfo) {
    // struct tm timeinfo = getLocalTime(&timeinfo);
    // struct tm timeinfo;
    // getLocalTime(&timeinfo);
    // getNowUs();
    // getNowUs2();
    // debug0_f("getNowUs: %lld\n", getNowUs);
    // debug0_f("getNowUs2: %lld\n", getNowUs2);
    debug0_ln(timeinfo, "%A, %B %d %Y %H:%M:%S");
    debug0_f("Day of week: ");           ln_println(timeinfo, "%A");
    debug0_f("Month: ");                 ln_println(timeinfo, "%B");
    debug0_f("Day of Month: ");          ln_println(timeinfo, "%d");
    debug0_f("Year: ");                  ln_println(timeinfo, "%Y");
    debug0_f("Hour: ");                  ln_println(timeinfo, "%H");
    debug0_f("Hour (12 hour format): "); ln_println(timeinfo, "%I");
    debug0_f("Minute: ");                ln_println(timeinfo, "%M");
    debug0_f("Second: ");                ln_println(timeinfo, "%S");
    // formating options  http://www.cplusplus.com/reference/ctime/strftime/

    debug0_f("     Time variables");
    char timeHour[3]; strftime(timeHour, 3, "%H", timeinfo); debug0_f(timeHour);
    char timeWeekDay[10]; strftime(timeWeekDay, 10, "%A", timeinfo); debug0_f(timeWeekDay);
    debug0_f("\n");
}

void print_rtc_time() {
    debug0_f("\t%-20s: %s\n",   "getDate(short)",     ((char*)rtc.getDate(false).c_str()));          // (String) Sun, Jan 17 2021        false = short date format
    debug0_f("\t%-20s: %s\n",   "getDate(long)",      rtc.getDate(true).c_str());                    // (String) Sunday, January 17 2021 true = Long date format
    debug0_f("\t%-20s: %s\n",   "getTime()",          rtc.getTime());                                // (String) 15:24:38
    debug0_f("\t%-20s: %s\n",   "getDateTime(short)", rtc.getDateTime(false).c_str());               // (String) Sun, Jan 17 2021 15:24:38
    debug0_f("\t%-20s: %s\n",   "getDateTime(long)",  rtc.getDateTime(true).c_str());                // (String) Sunday, January 17 2021 15:24:38
    debug0_f("\t%-20s: %s\n",   "getTimeDate(short)", rtc.getTimeDate(false).c_str());               // (String) 15:24:38 Sun, Jan 17 2021
    debug0_f("\t%-20s: %s\n",   "getTimeDate(long)",  rtc.getTimeDate(true).c_str());                // (String) 15:24:38 Sunday, January 17 2021

    debug0_f("\t%-20s: %ul\n",  "getEpoch()",         rtc.getEpoch());                               // (long)    1609459200
    debug0_f("\t%-20s: %d\n",   "getHour()",          rtc.getHour());                                // (int)     3     (1-12)
    debug0_f("\t%-20s: %d\n",   "getMinute()",        rtc.getMinute());                              // (int)     24    (0-59)
    debug0_f("\t%-20s: %d\n",   "getSecond()",        rtc.getSecond());                              // (int)     38    (0-59)
    debug0_f("\t%-20s: %ul\n",  "getMillis()",        rtc.getMillis());                              // (long)    723
    debug0_f("\t%-20s: %ul\n",  "getMicros()",        rtc.getMicros());                              // (long)    723546

    debug0_f("\t%-20s: %d\n",   "getHour(true)",      rtc.getHour(true));                            // (int)     15    (0-23)
    debug0_f("\t%-20s: %s\n",   "getAmPm(uppercase)", rtc.getAmPm(false).c_str());                   // (String)  PM
    debug0_f("\t%-20s: %s\n",   "getAmPm(lowcase)",   rtc.getAmPm(true).c_str());                    // (String)  pm

    debug0_f("\t%-20s: %d\n",   "getYear()",          rtc.getYear());                                // (int)     2021
    debug0_f("\t%-20s: %d\n",   "getMonth()",         rtc.getMonth());                               // (int)     0     (0-11)
    debug0_f("\t%-20s: %d\n",   "getDay()",           rtc.getDay());                                 // (int)     17    (1-31)
    debug0_f("\t%-20s: %d\n",   "getDayofWeek()",     rtc.getDayofWeek());                           // (int)     0     (0-6)
    debug0_f("\t%-20s: %d\n",   "getDayofYear()",     rtc.getDayofYear());                           // (int)     16    (0-365)
    debug0_f("\t%-20s: %ull\n", "getLocalEpoch()",    rtc.getLocalEpoch());                          // (long)    1609459200 epoch without offset
    debug0_f("\t%-20s: %s\n",   "getAmPm()",          rtc.getTime("%A, %B %d %Y %H:%M:%S").c_str()); // (String) returns time with specified format
    // formating options  http://www.cplusplus.com/reference/ctime/strftime/


    struct tm timeinfo = rtc.getTimeStruct();
    debug0_f("\t%-20s: %s\n", "time struct", (&timeinfo, "%A, %B %d %Y %H:%M:%S"));   //  (tm struct) Sunday, January 17 2021 07:24:38

}


#ifdef PPPPPP
#endif