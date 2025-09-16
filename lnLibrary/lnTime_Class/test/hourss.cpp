//
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 17.43.51
//


// #define  LOG_MODULE_LEVEL LOG_DEFAULT_TRACE
#include "lnLogger_Class.h"
#include "LnTime_Class.h"


extern LnTime_Class lnTime;

// ================ Hours ====================
void atHour(void) {
    // LOG_WARN("atHour:  %d", value);
    static const int8_t at_array[] = {0, 1, 5, 15, 30, 45};
    for (int i = 0; i < sizeof(at_array); i++) {
        int8_t value = at_array[i];
        if (lnTime.atHour(value)) {
            LOG_WARN("atHour:  %d", value);
        }
    }
}

void onHourModulo(void) {
    static const int8_t array[] = {0, 12, 15, 30, 45, 59};
    for (int i = 0; i < sizeof(array); i++) {
        int8_t value = array[i];

        if (lnTime.onModulo(0,0,value)) {
            LOG_SPEC("onModulo: %d (%s)", value, lnTime.secToHMS(value));
        }
    }
}


#ifdef LN_TIME_CLASS_HOURS_VECTOR
    const std::vector<uint8_t> hours = {0, 1, 5, 15, 30, 45};
    const std::vector<uint8_t>& myHours = hours;
    void atHourVector(void) {
        static bool first_run = true;
        if (first_run) {
            LOG_NOTIFY("Setting flag Hours");
            lnTime.setAtHourFlag(hours);  // create map inside class
            first_run = false;
        }

        for (uint8_t hour : myHours) {
            if (lnTime.getAtHourFlag(hour)) {
                LOG_NOTIFY("flagHour: %d", hour);
            }
        }
    }
#else
    #pragma message "LN_TIME_CLASS_HOURS_VECTOR not defined"
    void atHourVector(void) {do {} while (0);}
#endif

