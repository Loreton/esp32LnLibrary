

#ifndef _MYSTRUCT_H
    #define MYSTRUCT_H

    #define MAX_WEEKLY_SCHEDULER 16
    #define MAX_DAYS 7
    typedef struct {
        uint8_t             nr                  = 0;
        bool                enable              = false;
        bool                recursive           = false;
        uint8_t             relay_nr            = 0;
        uint8_t             hour                = 0;
        int                 minute              = 0;
        int                 secs_day            = 0; // secondi del giorno
        uint8_t             bool_day [7]        = {false, false, false, false, false, false, false};
        char                day_map[MAX_DAYS+1] = "0123456";
        // sched_callback      callback  = 0; // if the callback pointer is zero, the slot is free, i.e. doesn't "contain" any timer

    } structScheduler;


    #ifdef _MYSTRUCT_CPP
        structScheduler  weeklySched[MAX_WEEKLY_SCHEDULER];
        structScheduler  ee_weeklySched[MAX_WEEKLY_SCHEDULER];
    #else
        extern structScheduler weeklySched[];
        extern structScheduler  buffer[MAX_WEEKLY_SCHEDULER];

    #endif




    // void myStructSetup(bool fClear=false);
    structScheduler *myStructSetup(bool fClear=false);


#endif