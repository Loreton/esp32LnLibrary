/*
// updated by ...: Loreto Notarantonio
// Date .........: 17-07-2025 08.33.23
*/

#pragma once

#include <Arduino.h>
#include "esp_timer.h" // Per esp_timer_get_time()

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
enum LogLevel {
    LOG_LEVEL_NONE   = 0,
    LOG_LEVEL_ERROR  = 1,
    LOG_LEVEL_WARN   = 2,
    LOG_LEVEL_INFO   = 3,
    LOG_LEVEL_NOTIFY = 4,
    LOG_LEVEL_DEBUG  = 5,
    LOG_LEVEL_TRACE  = 6
};

// Imposta il livello di log globale
#ifndef LOG_LEVEL
    #pragma message "LOG_LEVEL not DEFINED. Defaulting to LOG_LEVEL_INFO."
    #define LOG_LEVEL LOG_LEVEL_INFO
#endif

// Classe per il logger ESP32
class ESP32Logger {
    public:
        ESP32Logger(void);
        void begin(void);
        void write(const char* color, const char* tag, const char* file, int line, const char* format, ...);
        // void info(const char* file, int line, const char* format, ...);
        char m_out[32];
        char m_tbuf[16]; // Buffer statico per il timestamp

    private:
        int64_t  m_usec = 0;
        uint32_t m_msec = 0;
        uint32_t m_sec  = 0;
        uint32_t m_min  = 0;
        uint32_t m_hour  = 0;

        const size_t m_maxlen = 10;     // Massima lunghezza desiderata per il nome del file
        const char m_paddingChar  = '.';   //padding char per il file
        const char *m_filename;
        const char *m_sep;

        const char* getTimestamp();
        const char* getFileLineInfo(const char* file, int line);
}; // class ESP32Logger


extern ESP32Logger myLog; // defined in lnLogger.cpp


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
    // #define LOG_INFO(fmt, ...)    myLog.info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
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
