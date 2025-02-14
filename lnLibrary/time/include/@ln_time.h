/*
// updated by ...: Loreto Notarantonio
// Date .........: 14-02-2025 10.06.06
*/

#include <ESP32Time.h> // per definire ESP32Time rtc;

#ifndef __LN_TIME_H__
    #define __LN_TIME_H__



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


#endif