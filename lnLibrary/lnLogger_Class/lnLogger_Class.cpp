/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-07-2025 19.52.35
*/

#include <Arduino.h>
#include <ESP32Time.h> // ESP32Time.cpp
#include <freertos/semphr.h> // Required here for mutex implementation

#include "lnLogger_Class.h" // Changed to new header file name

// ESP32Time       this_rtc;
// extern ESP32Time      rtc;


// char elapsedTimeBUFFER[16];
// char nextTimeBUFFER[16];
// char nowTimeBUFFER[16];


// Constructor: Initializes the mutex
ESP32Logger::ESP32Logger(void) { // Changed class name
    // The constructor does the bare minimum, or nothing.
    // The mutex will be created in init().
    m_logMutex = NULL; // Initialize to NULL to indicate it hasn't been created yet
};

// Used so I can send a message if I can't initialize the mutex
void ESP32Logger::init() { // Changed class name
    if (!m_mutexInitialized) {
        m_logMutex = xSemaphoreCreateMutex();
        if (m_logMutex == NULL) {
            // If mutex creation fails, it's a serious error,
            // but at least Serial is now initialized to try.
            Serial.println("\n\nERRORE: Impossibile creare mutex per il logger! Logging non protetto.\n\n");
        }
        else {
            m_mutexInitialized = true;
            Serial.println("\n\nOK: Mutex inizializzato.\n\n");
        }
    }
}



/**
 * Se timeBuffer è statica e globale.
 * Ogni chiamata a timeStamp sovrascrive il contenuto di timeBuffer,
 * quindi quando stampi più valori nello stesso printf,
 * entrambe le chiamate restituiscono il valore dell'ultima chiamata
 * per tale ragione il buffer deve essere allocato estrnamente
*/
const char* ESP32Logger::timeStamp(char *buffer, uint8_t maxBufferLen, uint32_t millisec, bool trimHeader) { // Changed class name

    if (millisec == 0) {
        millisec = esp_timer_get_time() / 1000; // Time in microseconds from boot
        // millisec = rtc.getMillis(); // Time in milliseconds
        // millisec = this_rtc.getMicros() / 1000; // Time in milliseconds
    }

    uint16_t msec    = (millisec % 1000); // value from 0-999
    uint32_t seconds = (millisec / 1000); // could be long
    uint8_t sec      = (seconds % 60);
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = (seconds / 3600);
    if (trimHeader) {
        if (hour > 0) {
            snprintf(buffer, maxBufferLen, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
        } else if (min > 0) {
            snprintf(buffer, maxBufferLen, "%02d:%02d.%03lu", min, sec, msec);
        } else {
            snprintf(buffer, maxBufferLen, "%02d.%03lu", sec, msec);
        }
    }
    else {
        snprintf(buffer, maxBufferLen, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
    }
    return buffer;
}



/**
 * @brief Formats the file name and line number.
 * The file name is truncated to a maximum length and padded with dots if shorter.
 * @param file The full path of the file (usually __FILE__).
 * @param line The line number (usually __LINE__).
 * @return A constant string containing the formatted file name and line number.
 */
const char* ESP32Logger::getFileLineInfo(char *outBUFFER, const uint8_t OutBUFFER_maxLen, const char* file, int line) { // Changed class name
    uint8_t filename_maxLen = OutBUFFER_maxLen-5;                    // Temporary buffer for truncated/padded file name (5 for .lineNo with 3 digits)
    const char    paddingChar  = '.';                             // padding char for the file

    const char *filename = strrchr(file, '/');                           // Find the last '/' to get only the file name
    filename = filename ? filename + 1 : file;                           // If found, move the pointer, otherwise use the entire path

    /*
    const char *sep = strrchr(filename, '_');                            // Find the separator to cut a suffix (e.g. _H)
    if (!sep) sep = strrchr(filename, '.');                              // Find the separator to cut the extension .cpp)
    */

    const char *sep = strrchr(filename, '.');                            // Find the separator to cut a suffix (e.g. .xxx)
    size_t len = sep ? (size_t)(sep - filename) : strlen(filename);      // Length of the name without extension

    if (len > filename_maxLen) len = filename_maxLen;                                  // Truncate the name if longer than maxlen

    // -- copiamo nel fileBuffer
    char filenameBUFFER[filename_maxLen+1];                                // space for the filename
    memset(filenameBUFFER, paddingChar, filename_maxLen);                         // Fill the buffer with dots for padding
    memcpy(filenameBUFFER, filename, len);                                 // Copy the actual name
    filenameBUFFER[filename_maxLen] = '\0';                                       // Terminate the string

    snprintf(outBUFFER, OutBUFFER_maxLen, "%s.%03d", filenameBUFFER, line);
    return outBUFFER;
}


/**
 * @brief Internal function for actual log output.
 * This function is called by the logging macros.
 * @param color ANSI color code for output.
 * @param tag Log level label (e.g. "ERR", "INF").
 * @param file The source file path.
 * @param line The line number in the source file.
 * @param format Printf-like format string.
 * @param ... Variable arguments for the format string.
 */
void ESP32Logger::write(const char* color, const char* tag, const char* file, int line, const char* format, ...) { // Changed class name
    if (!m_mutexInitialized) {
        // If not initialized, we cannot use the mutex or Serial.
        // You might print a crude warning message or discard the log.
        // This is the most critical situation: a log before Serial.init() and myLog.init().
        // For now, we print it to Serial but without guarantees of visibility.
        // For very early logs, you might consider a FIFO buffer or JTAG/SWD.
        Serial.printf("AVVISO: Logging prima dell'inizializzazione: (lanciare il init())");
        // Serial.printf(format, ##__VA_ARGS__);
        Serial.println();
        return; // Exit to avoid issues
    }

    // Try to acquire the mutex. Wait indefinitely (portMAX_DELAY) if it's already locked.
    if (m_logMutex != NULL && xSemaphoreTake(m_logMutex, portMAX_DELAY) == pdTRUE) {
        char nowTimeBUFFER[16];
        char fnameBUFFER[25];
        char logLineBUFFER[256];

        va_list args;
        va_start(args, format);
        int len = vsnprintf(logLineBUFFER, sizeof(logLineBUFFER), format, args);
        va_end(args);

        if (len >= sizeof(logLineBUFFER)) {
            logLineBUFFER[sizeof(logLineBUFFER) - 1] = '\0';
        }

        Serial.printf("%s[%s][%s][%s] %s%s\n",
                      color,
                      this->timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER), 0, false),
                      this->getFileLineInfo(fnameBUFFER, sizeof(fnameBUFFER), file, line),
                      tag,
                      logLineBUFFER,
                      LogColors::RESET);

        xSemaphoreGive(m_logMutex); // Release the mutex, allowing other tasks to log
    } else {
        // This part of the code is executed only if the mutex was not created
        // or if mutex acquisition fails (extremely rare with portMAX_DELAY).
        // You might still print a basic message to not lose the log,
        // but it won't be thread-safe.
        Serial.printf("!!! Failed to acquire log mutex or mutex not initialized: ");
        // Serial.printf(format, ##__VA_ARGS__); // Print the original unformatted message
        Serial.println();
    }
}

ESP32Logger lnLog; // Changed class name