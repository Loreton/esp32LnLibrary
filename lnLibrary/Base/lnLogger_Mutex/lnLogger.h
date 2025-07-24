/*
// updated by ...: Loreto Notarantonio
// Date .........: 24-07-2025 13.16.05
*/

#pragma once

#include <Arduino.h>
#include <freertos/semphr.h> // <-- AGGIUNGI QUESTO per i tipi FreeRTOS (SemaphoreHandle_t)
#include "esp_timer.h" // Per esp_timer_get_time()
// #include "lnGlobalVars.h" // PRINT_VAR()



// #########################################################################
// #   Logger CLASS   Logger CLASS   Logger CLASS   Logger CLASS
// #   Logger CLASS   Logger CLASS   Logger CLASS   Logger CLASS
// #   Logger CLASS   Logger CLASS   Logger CLASS   Logger CLASS
// #   Logger CLASS   Logger CLASS   Logger CLASS   Logger CLASS
// #########################################################################

// Classe per il logger ESP32
class ESP32LoggerMutex {
    public:
        ESP32LoggerMutex(void);
        void begin(void);
        void write(const char* color, const char* tag, const char* file, int line, const char* format, ...);

    private:
        bool m_mutexInitialized = false;
        SemaphoreHandle_t m_logMutex = NULL; // <-- AGGIUNGI QUESTO: il mutex per proteggere le operazioni di log
        // char timestamp[32];
        const char* mSecTo_HHMMSS(uint32_t millisec=0);
        const char* getTimestamp();
        const char* getTimestampFromBoot();
        const char* getFileLineInfo(const char* file, int line);
}; // class ESP32LoggerMutex


extern ESP32LoggerMutex myLog; // defined in lnLogger.cpp
#define lnLOGGER ESP32LoggerMutex; // per accedere ai metodi statici

// void to_HHMMSS2(uint32_t mseconds, char *outStr, uint8_t maxlen);


const char* xxx_mSecTo_HHMMSS(uint32_t msec);




// #########################################################################
// #   Logger MACROS    Logger MACROS    Logger MACROS    Logger MACROS
// #   Logger MACROS    Logger MACROS    Logger MACROS    Logger MACROS
// #   Logger MACROS    Logger MACROS    Logger MACROS    Logger MACROS
// #   Logger MACROS    Logger MACROS    Logger MACROS    Logger MACROS
// #########################################################################


    // #include "lnGlobalVars.h" // Per VAR_NAME_VALUE
    #define VALUE_TO_STRING(x) #x
    #define VALUE(x) VALUE_TO_STRING(x)
    #define VAR_NAME_VALUE(var) #var "="  VALUE(var)

    // Definizioni dei colori ANSI
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


    // Livelli di log
    #define    LOG_LEVEL_NONE    0
    #define    LOG_LEVEL_ERROR   1
    #define    LOG_LEVEL_WARN    2
    #define    LOG_LEVEL_INFO    3
    #define    LOG_LEVEL_NOTIFY  4
    #define    LOG_LEVEL_DEBUG   5
    #define    LOG_LEVEL_TRACE   6

    // Imposta il livello di log globale
    #ifndef LOG_LEVEL
        #pragma message "LOG_LEVEL not DEFINED. Defaulting to LOG_LEVEL_WARN."
        #define LOG_LEVEL LOG_LEVEL_WARN
    #else
        // #pragma message VAR_NAME_VALUE(LOG_LEVEL)
    #endif

    // #pragma message PRINT_VAR(LOG_LEVEL)

    // ---

    // Macro di convenienza per il logging.
    // Queste macro controllano il LOG_LEVEL in fase di pre-compilazione
    // e chiamano write solo se il livello è abilitato, altrimenti
    // si espandono a `do {} while(0)` per non generare codice.

    #if LOG_LEVEL >= LOG_LEVEL_ERROR
        #define LOG_ERROR(fmt, ...)   myLog.write(LogColors::RED, "ERR", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_ERROR(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_WARN
        #define LOG_WARNING(fmt, ...)    myLog.write(LogColors::YELLOW, "WRN", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_WARNING(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_INFO
        #define LOG_INFO(fmt, ...)    myLog.write(LogColors::GREEN, "INF", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_INFO(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_NOTIFY
        #define LOG_NOTIFY(fmt, ...)  myLog.write(LogColors::PURPLE, "NFY", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_NOTIFY(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_DEBUG
        #define LOG_DEBUG(fmt, ...)   myLog.write(LogColors::CYAN, "DBG", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_DEBUG(...) do {} while (0)
    #endif

    #if LOG_LEVEL >= LOG_LEVEL_TRACE
        #define LOG_TRACE(fmt, ...)   myLog.write(LogColors::WHITE, "TRC", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #else
        #define LOG_TRACE(...) do {} while (0)
    #endif
