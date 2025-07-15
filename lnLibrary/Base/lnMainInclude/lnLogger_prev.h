/*
// updated by ...: Loreto Notarantonio
// Date .........: 14-07-2025 16.39.03
*/
#pragma once

    #include <Arduino.h>

    /*
    ####################################################
       red='\033[0;31m';    redH='\033[1;31m'
     green='\033[0;32m';  greenH='\033[1;32m'
    yellow='\033[0;33m'; yellowH='\033[1;33m'
      blue='\033[0;34m';   blueH='\033[1;34m'
    purple='\033[0;35m'; purpleH='\033[1;35m'
      cyan='\033[0;36m';   cyanH='\033[1;36m'
      gray='\033[0;37m';   white='\033[1;37m'
    colorReset='\033[0m' # No Color

    --- test ---
    Serial.println("\x1B[1;31mRosso\x1B[0m Normale");
    Serial.print("\x1B[1;31mROSSO\x1B[0m ");
    Serial.print("\x1B[1;32mVERDE\x1B[0m ");
    Serial.print("\x1B[1;33mGIALLO\x1B[0m ");
    Serial.println("FINE");
    ####################################################
    */

    #define ANSI_RESET     "\x1B[0m"
    #define ANSI_RED       "\x1B[1;31m"
    #define ANSI_GREEN     "\x1B[1;32m"
    #define ANSI_YELLOW    "\x1B[1;33m"
    #define ANSI_BLUE      "\x1B[1;34m"
    #define ANSI_PURPLE    "\x1B[1;35m"
    #define ANSI_CYAN      "\x1B[1;36m"
    #define ANSI_WHITE     "\x1B[1;37m"

    // Imposta il livello attivo (es: 4 = DEBUG, 0 = NESSUN LOG)
        // Livelli di log
        #define LOG_LEVEL_NONE   0
        #define LOG_LEVEL_ERROR  1
        #define LOG_LEVEL_WARN   2
        #define LOG_LEVEL_INFO   3
        #define LOG_LEVEL_NOTIFY 4
        #define LOG_LEVEL_DEBUG  5
        #define LOG_LEVEL_TRACE  6


    #ifndef LOG_LEVEL
        #pragma message "LOG_LEVEL not DEFINED"
        #define LOG_LEVEL LOG_LEVEL_INFO
    #else
        // #include "lnGlobalVars.h" // per #pragma message VAR_NAME_VALUE(LOG_LEVEL)
        // #pragma message VAR_NAME_VALUE(LOG_LEVEL)
    #endif


    #define __LOG_TIMESTAMP_BASE__ ({ \
        static char tbuf[16]; \
        uint32_t now = millis(); \
        uint32_t ms = now % 1000; \
        uint32_t s = (now / 1000) % 60; \
        uint32_t m = (now / 60000) % 60; \
        uint32_t h = (now / 3600000); \
        snprintf(tbuf, sizeof(tbuf), "%02lu:%02lu:%02lu.%03lu", h, m, s, ms); \
        tbuf; \
    })


    #include "esp_timer.h"  // assicurati sia incluso
    #define __LOG_TIMESTAMP_ESP_TIMER__ ({ \
        static char tbuf[16]; \
        int64_t us = esp_timer_get_time(); \
        uint32_t ms = (us / 1000) % 1000; \
        uint32_t s  = (us / 1000000) % 60; \
        uint32_t m  = (us / 60000000) % 60; \
        uint32_t h  = (us / 3600000000); \
        snprintf(tbuf, sizeof(tbuf), "%02lu:%02lu:%02lu.%03lu", h, m, s, ms); \
        tbuf; \
    })



    #define __FILE_LINE_ID__ ({ \
        static char out[128]; \
        const char *filename = strrchr(__FILE__, '/'); \
        filename = filename ? filename + 1 : __FILE__; \
        const char *sep = strrchr(filename, '_'); \
        if (!sep) sep = strrchr(filename, '.'); \
        size_t len = sep ? (size_t)(sep - filename) : strlen(filename); \
        snprintf(out, sizeof(out), "%.*s.%03d", (int)len, filename, __LINE__); \
        out; \
    })


    #define __MAX_FILENAME_LENGTH__ 15
    #define __FILENAME_PADDED_LINE_ID__ ({ \
        static char out[32]; \
        const char *filename = strrchr(__FILE__, '/'); \
        filename = filename ? filename + 1 : __FILE__; \
        const char *sep = strrchr(filename, '_'); \
        if (!sep) sep = strrchr(filename, '.'); \
        size_t len = sep ? (size_t)(sep - filename) : strlen(filename); \
        const size_t maxlen = __MAX_FILENAME_LENGTH__; \
        char name[maxlen + 1]; \
        if (len > maxlen) len = maxlen; \
        memset(name, '.', maxlen); \
        memcpy(name, filename, len); \
        name[maxlen] = '\0'; \
        snprintf(out, sizeof(out), "%s.%03d", name, __LINE__); \
        out; \
    })



    // #define LOG_OUTPUT(color, tag, fmt, ...) \
    //     Serial.printf("%s[%s][%s] " fmt "%s\n", color, __FILE_LINE_ID__, tag, ##__VA_ARGS__, ANSI_RESET)


    // #define __LOG_TIMESTAMP__ __LOG_TIMESTAMP_BASE__
    #define __LOG_TIMESTAMP__ __LOG_TIMESTAMP_ESP_TIMER__
    // #define __FILE_AND_LINE__ __FILE_LINE_ID__
    #define __FILE_AND_LINE__ __FILENAME_PADDED_LINE_ID__

    #define LOG_OUTPUT(color, tag, fmt, ...) \
        Serial.printf("%s[%s][%s][%s] " fmt "%s\n", \
        color, __LOG_TIMESTAMP__, __FILENAME_PADDED_LINE_ID__, tag, ##__VA_ARGS__, ANSI_RESET)
        // color, __LOG_TIMESTAMP__, __FILE_LINE_ID__, tag, ##__VA_ARGS__, ANSI_RESET)



    /*
        L'intero costrutto do {} while (0) è un'istruzione che il compilatore moderno
        è in grado di ottimizzare completamente via (dead code elimination)
        in quanto non ha effetti collaterali e non esegue alcuna operazione.
    */
    #if LOG_LEVEL >= LOG_LEVEL_ERROR
        #define LOG_ERROR(fmt, ...) LOG_OUTPUT(ANSI_RED,   "ERR", fmt, ##__VA_ARGS__)
    #else
        #define LOG_ERROR(fmt, ...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_WARN
        #define LOG_WARN(fmt, ...)  LOG_OUTPUT(ANSI_YELLOW,"WRN", fmt, ##__VA_ARGS__)
    #else
        #define LOG_WARN(fmt, ...)  do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_INFO
        #define LOG_INFO(fmt, ...)  LOG_OUTPUT(ANSI_GREEN, "INF", fmt, ##__VA_ARGS__)
    #else
        #define LOG_INFO(fmt, ...)  do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_NOTIFY
        #define LOG_NOTIFY(fmt, ...) LOG_OUTPUT(ANSI_PURPLE,  "NFY", fmt, ##__VA_ARGS__)
    #else
        #define LOG_NOTIFY(fmt, ...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_DEBUG
        #define LOG_DEBUG(fmt, ...) LOG_OUTPUT(ANSI_CYAN,  "DBG", fmt, ##__VA_ARGS__)
    #else
        #define LOG_DEBUG(fmt, ...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_TRACE
        #define LOG_TRACE(fmt, ...) LOG_OUTPUT(ANSI_WHITE,  "TRC", fmt, ##__VA_ARGS__)
    #else
        #define LOG_TRACE(fmt, ...) do {} while (0)
    #endif

