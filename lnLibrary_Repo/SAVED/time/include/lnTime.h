/*
// updated by ...: Loreto Notarantonio
// Date .........: 24-07-2025 13.20.28
*/

#pragma once

#include <ESP32Time.h> // per definire ESP32Time rtc;

    #define __LN_TIME_INCLUDED__


    extern ESP32Time     rtc;
    extern char  PROGMEM buffer_time[];




    // --------------------------------------------------
    // --- functions prototypes
    // --------------------------------------------------
    void            time_setup(void);
    char          * nowTime(void);
    uint32_t        getEpoch(uint32_t offset=0);
    int             secondsToMinute(void);
    int             secondsOfDay(int offset=0);
    int             minutesOfDay(int offset=0);
    struct tm       get_timeinfo(void);

    void            NTPInit(void);
    int8_t          waitForSecond(void);


    void printLocalTime(struct tm *timeinfo);
    void print_rtc_time(void);

    bool isSecondOClock(void);
    bool isMinuteOClock(void);
    bool isQuarterOClock(void);
    void alignToMinute(void);
    // void printTime(void);
    void to_HHMMSS(uint32_t mseconds, char *outStr, uint8_t maxlen=10);
    const char* mSecTo_HHMMSSms(uint32_t msec, char *buffer=nullptr, uint8_t buffer_len=0);

