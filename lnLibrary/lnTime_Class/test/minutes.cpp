//
// updated by ...: Loreto Notarantonio
// Date .........: 08-09-2025 17.42.21
//


// #define  LOG_MODULE_LEVEL LOG_DEFAULT_TRACE
#include "lnLogger_Class.h"
#include "LnTime_Class.h"



extern LnTime_Class lnTime;

// ================ Minutes ====================
void atMinute(void) {
    // LOG_WARN("atMinute:  %d", value);
    static const int8_t at_array[] = {0, 1, 5, 15, 30, 45};
    for (int i = 0; i < sizeof(at_array); i++) {
        int8_t value = at_array[i];
        if (lnTime.atMinute(value)) {
            LOG_WARN("atMinute:  %d", value);
        }
    }
}

void onMinuteModulo(void) {
    static const int8_t array[] = {0, 12, 15, 30, 45, 59};
    for (int i = 0; i < sizeof(array); i++) {
        int8_t value = array[i];

        if (lnTime.onModulo(0,0,value)) {
            LOG_SPEC("onModulo: %d (%s)", value, lnTime.secToHMS(value*60));
        }
    }
}


#ifdef LN_TIME_CLASS_MINUTES_VECTOR
    const std::vector<uint8_t> minutes = {0, 1, 5, 15, 30, 45};
    const std::vector<uint8_t>& myMinutes = minutes;
    void atMinuteVector(void) {
        static bool first_run = true;
        if (first_run) {
            LOG_NOTIFY("Setting flag Minutes");
            lnTime.setAtMinuteFlag(minutes);  // create map inside class
            first_run = false;
        }

        for (uint8_t min : myMinutes) {
            if (lnTime.getAtMinuteFlag(min)) {
                LOG_NOTIFY("flagMinute: %d", min);
            }
        }
    }
#else
    #pragma message "LN_TIME_CLASS_MINUTES_VECTOR not defined"
    void atMinuteVector(void) {do {} while (0);}
#endif

