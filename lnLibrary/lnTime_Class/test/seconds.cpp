//
// updated by ...: Loreto Notarantonio
// Date .........: 07-09-2025 20.35.11
//


// #define  LOG_MODULE_LEVEL LOG_DEFAULT_TRACE
#include "lnLogger_Class.h"

#include "LnTime_Class.h"


// ESP32Time rtc;
// struct tm timeinfo;
extern LnTime_Class lnTime;

// ================ Seconds ====================
void atSecond(void) {
    // LOG_WARN("atSecond:  %d", value);
    static const int8_t at_array[] = {0, 1, 5, 15, 30, 45};
    for (int i = 0; i < sizeof(at_array); i++) {
        int8_t value = at_array[i];
        if (lnTime.atSecond(value)) {
            LOG_WARN("atSecond:  %d", value);
        }
    }
}

void onSecondModulo(void) {
    static const int8_t array[] = {0, 12, 15, 30, 45, 59};
    for (int i = 0; i < sizeof(array); i++) {
        int8_t value = array[i];

        if (lnTime.onModulo(0,0,value)) {
            LOG_SPEC("onModulo: %d (%s)", value, lnTime.secToHMS(value));
        }
    }
}


#ifdef LN_TIME_CLASS_SECONDS_VECTOR
    const std::vector<uint8_t> seconds = {0, 1, 5, 15, 30, 45};
    const std::vector<uint8_t>& mySecs = seconds;
    void atSecondVector(void) {
        static bool first_run = true;
        if (first_run) {
            LOG_NOTIFY("Setting flag Seconds");
            lnTime.setAtSecondFlag(seconds);  // create map inside class
            first_run = false;
        }

        for (uint8_t sec : mySecs) {
            if (lnTime.getAtSecondFlag(sec)) {
                LOG_NOTIFY("flagSecond: %d", sec);
            }
        }
    }
#else
    #pragma message "LN_TIME_CLASS_SECONDS_VECTOR not defined"
    void atSecondVector(void) {do {} while (0);}
#endif

