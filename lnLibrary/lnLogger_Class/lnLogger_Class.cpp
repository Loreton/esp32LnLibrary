/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 11.36.32
*/

#include <Arduino.h>
#include <ESP32Time.h> // ESP32Time.cpp
#include <freertos/semphr.h> // Required here for mutex implementation

#include "lnLogger_Class.h" // Changed to new header file name

// ESP32Time       this_rtc;
// extern ESP32Time      rtc;

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

const char* ESP32Logger::getTimeStamp(uint32_t millisec, bool fStrip) { // Changed class name
    static const uint8_t BUFFER_LEN = 16;
    static char timeBuffer[BUFFER_LEN]; // Static buffer for the timestamp

    if (millisec == 0) {
        // Serial.printf("millisec - RTC: %lu   this_RTC: %lu   get_time: %lu\n", rtc.getMicros()/1000,   this_rtc.getMicros()/1000, esp_timer_get_time()/1000 );
        millisec = esp_timer_get_time() / 1000; // Time in microseconds from boot
        // millisec = rtc.getMillis(); // Time in milliseconds
        // millisec = this_rtc.getMicros() / 1000; // Time in milliseconds
    }

    uint16_t msec    = (millisec % 1000); // value from 0-999
    uint32_t seconds = (millisec / 1000); // could be long
    uint8_t sec      = (seconds % 60);
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = (seconds / 3600);
    if (fStrip) {
        if (hour > 0) {
            snprintf(timeBuffer, BUFFER_LEN, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
        } else if (min > 0) {
            snprintf(timeBuffer, BUFFER_LEN, "%02d:%02d.%03lu", min, sec, msec);
        } else {
            snprintf(timeBuffer, BUFFER_LEN, "%02d.%03lu", sec, msec);
        }
    }
    else {
        snprintf(timeBuffer, BUFFER_LEN, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
    }
    return timeBuffer;
}

/**
 * @brief Formats the file name and line number.
 * The file name is truncated to a maximum length and padded with dots if shorter.
 * @param file The full path of the file (usually __FILE__).
 * @param line The line number (usually __LINE__).
 * @return A constant string containing the formatted file name and line number.
 */
const char* ESP32Logger::getFileLineInfo(const char* file, int line) { // Changed class name
    static char buffer[25];                                              // space for [filename.linNo]
    static const uint8_t MAX_BUFFER_LEN = sizeof(buffer);
    static const uint8_t FILE_LEN = MAX_BUFFER_LEN-5;                    // Temporary buffer for truncated/padded file name (5 for .lineNo with 3 digits)
    static const char    paddingChar  = '.';                             // padding char for the file

    static char fname_buffer[FILE_LEN+1];                                // space for the filename
    const char *filename = strrchr(file, '/');                           // Find the last '/' to get only the file name
    filename = filename ? filename + 1 : file;                           // If found, move the pointer, otherwise use the entire path

    const char *sep = strrchr(filename, '_');                            // Find the separator to cut a suffix (e.g. _H)
    if (!sep) sep = strrchr(filename, '.');                              // Find the separator to cut the extension .cpp)

    size_t len = sep ? (size_t)(sep - filename) : strlen(filename);      // Length of the name without extension

    if (len > FILE_LEN) len = FILE_LEN;                                  // Truncate the name if longer than maxlen

    memset(fname_buffer, paddingChar, FILE_LEN);                         // Fill the buffer with dots for padding
    memcpy(fname_buffer, filename, len);                                 // Copy the actual name
    fname_buffer[FILE_LEN] = '\0';                                       // Terminate the string

    snprintf(buffer, MAX_BUFFER_LEN, "%s.%03d", fname_buffer, line);
    return buffer;
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
        static char buffer[256];
        va_list args;
        va_start(args, format);
        int len = vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        if (len >= sizeof(buffer)) {
            buffer[sizeof(buffer) - 1] = '\0';
        }

        Serial.printf("%s[%s][%s][%s] %s%s\n",
                      color,
                      this->getTimeStamp(0),
                      this->getFileLineInfo(file, line),
                      tag,
                      buffer,
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