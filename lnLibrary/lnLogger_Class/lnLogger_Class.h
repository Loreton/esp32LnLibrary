/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 11.31.31
*/

#pragma once

#include <Arduino.h>
#include <freertos/semphr.h> // Required for FreeRTOS types (SemaphoreHandle_t)
#include "esp_timer.h" // For esp_timer_get_time()

// #########################################################################
// #     Logger CLASS     Logger CLASS     Logger CLASS     Logger CLASS
// #########################################################################

// Class for ESP32 logger
class ESP32Logger { // Renamed from ESP32LoggerMutex for simplicity and clarity
    public:
        ESP32Logger(void);
        void init(void);
        void write(const char* color, const char* tag, const char* file, int line, const char* format, ...);

    private:
        bool m_mutexInitialized = false;
        SemaphoreHandle_t m_logMutex = NULL; // The mutex to protect log operations
        const char* getTimeStamp(uint32_t millisec=0, bool fStrip=false);
        const char* getFileLineInfo(const char* file, int line);
}; // class ESP32Logger


extern ESP32Logger lnLog; // defined in lnLogger.cpp



// #########################################################################
// #     Logger MACROS      Logger MACROS      Logger MACROS      Logger MACROS
// #########################################################################

    #define VALUE_TO_STRING(x) #x
    #define VALUE(x) VALUE_TO_STRING(x)
    #define VAR_NAME_VALUE(var) #var "="  VALUE(var)

    // ANSI Color Definitions
    namespace LogColors {
        const char* const RESET  = "\x1B[0m";
        const char* const RED    = "\x1B[1;31m";
        const char* const GREEN  = "\x1B[1;32m";
        const char* const YELLOW = "\x1B[1;33m";
        const char* const BLUE   = "\x1B[1;34m";
        const char* const PURPLE = "\x1B[1;35m";
        const char* const CYAN   = "\x1B[1;36m";
        const char* const WHITE  = "\x1B[1;37m";
    } // namespace LogColors


    // Log Levels
    #define    LOG_LEVEL_NONE    0
    #define    LOG_LEVEL_ERROR    1
    #define    LOG_LEVEL_WARN    2
    #define    LOG_LEVEL_INFO    3
    #define    LOG_LEVEL_NOTIFY    4
    #define    LOG_LEVEL_DEBUG    5
    #define    LOG_LEVEL_TRACE    6

    // Set the global log level
    #ifndef LOG_LEVEL
        #pragma message "LOG_LEVEL not DEFINED. Defaulting to LOG_LEVEL_WARN."
        #define LOG_LEVEL LOG_LEVEL_WARN
    #else
        // #pragma message VAR_NAME_VALUE(LOG_LEVEL)
    #endif

    // ---

    // Convenience macros for logging.
    // These macros check LOG_LEVEL at pre-compilation time
    // and call write only if the level is enabled, otherwise
    // they expand to `do {} while(0)` to generate no code.

    #if LOG_LEVEL >= LOG_LEVEL_ERROR
        #define LOG_ERROR(fmt, ...)    lnLog.write(LogColors::RED, "ERR", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_ERROR(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_WARN
        #define LOG_WARNING(fmt, ...)     lnLog.write(LogColors::YELLOW, "WRN", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_WARNING(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_INFO
        #define LOG_INFO(fmt, ...)     lnLog.write(LogColors::GREEN, "INF", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_INFO(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_NOTIFY
        #define LOG_NOTIFY(fmt, ...)  lnLog.write(LogColors::PURPLE, "NFY", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_NOTIFY(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_DEBUG
        #define LOG_DEBUG(fmt, ...)    lnLog.write(LogColors::CYAN, "DBG", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_DEBUG(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_TRACE
        #define LOG_TRACE(fmt, ...)    lnLog.write(LogColors::WHITE, "TRC", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_TRACE(...) do {} while (0)
    #endif