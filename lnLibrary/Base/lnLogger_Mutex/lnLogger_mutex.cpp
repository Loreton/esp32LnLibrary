/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-07-2025 08.09.08
*/



#include <Arduino.h>
// #include "esp_timer.h"
#include <ESP32Time.h> // ESP32Time.cpp
#include <freertos/semphr.h> // <-- AGGIUNGI QUESTO anche qui per l'implementazione del mutex

#include "lnLogger.h"


ESP32Time     this_rtc;

// Costruttore: Inizializza il mutex
ESP32LoggerMutex::ESP32LoggerMutex(void) {
    // Il costruttore fa il minimo indispensabile, o nulla.
    // Il mutex verrà creato in init().
    m_logMutex = NULL; // Inizializza a NULL per sapere che non è stato ancora creato
};

// la uso in modo che posso mandare un messaggio se non riesco ad inizializzare il mutex
void ESP32LoggerMutex::init() {
    if (!m_mutexInitialized) {
        m_logMutex = xSemaphoreCreateMutex();
        if (m_logMutex == NULL) {
            // Se la creazione del mutex fallisce, è un errore grave,
            // ma almeno ora Serial è inizializzata per provarci.
            Serial.println("\n\nERRORE: Impossibile creare mutex per il logger! Logging non protetto.\n\n");
        }
        else {
            m_mutexInitialized = true;
            Serial.println("\n\nOK: Mutex inizializzato.\n\n");
        }
    }
}



// const char* ESP32LoggerMutex::uSecTo_HHMMSS(uint32_t usec, char *outStr, uint8_t maxlen) {
// const char* ESP32LoggerMutex::mSecTo_HHMMSS(uint32_t millisec) {
//     static const uint8_t MAX_BUFFER_LEN = 16; // Buffer statico per il timestamp
//     static char timestamp[16]; // Buffer statico per il timestamp

//     if (millisec == 0) {
//         // uint32_t usec = esp_timer_get_time(); // Tempo in microsecondi dall'avvio
//         // msec = (usec / 1000) % 1000;
//         millisec = this_rtc.getMillis(); // Tempo in millisecondi dall'avvio
//     }
//     uint32_t msec  = millisec % 1000; // prendo il resto
//     uint32_t sec   = (millisec / 1000) % 60;
//     uint32_t min   = (millisec / 60000) % 60;
//     uint32_t hour  = (millisec / 3600000);
//     snprintf(timestamp, MAX_BUFFER_LEN, "%02lu:%02lu:%02lu.%03lu", hour, min, sec, msec);
//     return timestamp;
// }

const char* ESP32LoggerMutex::mSecTo_HHMMSS(uint32_t millisec) {
    static const uint8_t BUFFER_LEN = 16;
    static char timeBuffer[BUFFER_LEN]; // Buffer statico per il timestamp
    if (millisec == 0) {
        millisec = this_rtc.getMillis(); // Tempo in millisecondi
    }

    uint16_t msec    = (millisec % 1000); // valode da 0-1000
    uint32_t seconds = (millisec / 1000); // potrebbe essere lungo
    uint8_t sec      = (seconds % 60);
    uint8_t min      = (seconds / 60) % 60;
    uint8_t hour     = (seconds / 3600);
    if (hour > 0) {
        snprintf(timeBuffer, BUFFER_LEN, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
    } else if (min > 0) {
        snprintf(timeBuffer, BUFFER_LEN, "%02d:%02d.%03lu", min, sec, msec);
    } else {
        snprintf(timeBuffer, BUFFER_LEN, "%02d.%03lu", sec, msec);
    }
    // snprintf(timeBuffer, BUFFER_LEN, "%02d:%02d:%02d.%03lu", hour, min, sec, msec);
    return timeBuffer;
}



/**
 * @brief Genera un timestamp formattato (HH:MM:SS.mmm) usando esp_timer_get_time().
 * @return Una stringa costante contenente il timestamp.
 */
const char* ESP32LoggerMutex::getTimestampFromBoot() {
    return this->mSecTo_HHMMSS(esp_timer_get_time());
}


const char* ESP32LoggerMutex::getTimestamp() {
    static char timestamp[16]; // Buffer statico per il timestamp
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm tm_info;

    if (localtime_r(&tv.tv_sec, &tm_info)) {
        snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d.%03ld", tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec, tv.tv_usec / 1000);
    } else {
        uint64_t us = esp_timer_get_time(); // Tempo in microsecondi dall'avvio
        snprintf(timestamp, sizeof(timestamp), "%02llu.%03llu", us / 1000000ULL, (us / 1000ULL) % 1000);
        // snprintf(timestamp, sizeof(timestamp), "%02llu.%02llu.%03llu", (usec / 60000000) % 60), us / 1000000ULL, (us / 1000ULL) % 1000);
    }
    return timestamp;
}




/**
 * @brief Formatta il nome del file e il numero di linea.
 * Il nome del file viene troncato a una lunghezza massima e riempito con punti se più corto.
 * @param file Il path completo del file (solitamente __FILE__).
 * @param line Il numero di linea (solitamente __LINE__).
 * @return Una stringa costante contenente il nome del file formattato e il numero di linea.
 */
const char* ESP32LoggerMutex::getFileLineInfo(const char* file, int line) {
    static char buffer[19];                                         // spazio per il [filename.linNo]
    static const uint8_t MAX_BUFFER_LEN = sizeof(buffer);
    static const uint8_t FILE_LEN = MAX_BUFFER_LEN-5;               // Buffer temporaneo per il nome del file troncato/paddato 5 per .lineNo da 3 digit
    static const char   paddingChar  = '.';                         // padding char per il file

    static char fname_buffer[FILE_LEN+1];                           // spazio per il filename
    const char *filename = strrchr(file, '/');                      // Trova l'ultimo '/' per ottenere solo il nome del file
    filename = filename ? filename + 1 : file;                      // Se trovato, sposta il puntatore, altrimenti usa l'intero path

    const char *sep = strrchr(filename, '_');                       // Trova il separatore per tagliare un suffisso (es. _H)
    if (!sep) sep = strrchr(filename, '.');                         // Trova il separatore per tagliare l'estensione .cpp)

    size_t len = sep ? (size_t)(sep - filename) : strlen(filename); // Lunghezza del nome senza estensione

    if (len > FILE_LEN) len = FILE_LEN;                             // Tronca il nome se più lungo di maxlen

    memset(fname_buffer, paddingChar, FILE_LEN);                    // Riempie il buffer con punti per il padding
    memcpy(fname_buffer, filename, len);                            // Copia il nome effettivo
    fname_buffer[FILE_LEN] = '\0';                                  // Termina la stringa

    snprintf(buffer, sizeof(buffer), "%s.%03d", fname_buffer, line);
    return buffer;
}




/**
 * @brief Funzione interna per l'output del log effettivo.
 * Questa funzione è chiamata dalle macro di logging.
 * @param color Codice colore ANSI per l'output.
 * @param tag Etichetta del livello di log (es. "ERR", "INF").
 * @param file Il path del file di origine.
 * @param line Il numero di linea nel file di origine.
 * @param format Stringa di formato come in printf.
 * @param ... Argomenti variabili per la stringa di formato.
 */
void ESP32LoggerMutex::write(const char* color, const char* tag, const char* file, int line, const char* format, ...) {
    if (!m_mutexInitialized) {
        // Se non è inizializzato, non possiamo usare il mutex o la Serial.
        // Potresti stampare un messaggio di avviso rudimentale o scartare il log.
        // Questa è la situazione più critica: un log prima di Serial.init() e myLog.init().
        // Per ora, lo stampiamo su Serial ma senza garanzie di visibilità.
        // Per log molto precoci, potresti considerare un buffer FIFO o JTAG/SWD.
        Serial.printf("AVVISO: Logging prima dell'inizializzazione: (lanciare il init())");
        // Serial.printf(format, ##__VA_ARGS__);
        Serial.println();
        return; // Esci per evitare problemi
    }


    // Tenta di acquisire il mutex. Aspetta indefinitamente (portMAX_DELAY) se è già bloccato.
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
                      // this.getTimestamp(),       // Questi ora operano sui membri di myLog
                      this->mSecTo_HHMMSS(0),         // get elapsed time from boot
                      this->getFileLineInfo(file, line), // Questi ora operano sui membri di myLog
                      tag,
                      buffer,
                      LogColors::RESET);

        xSemaphoreGive(m_logMutex); // Rilascia il mutex, permettendo ad altri task di loggare
    } else {
        // Questa parte del codice viene eseguita solo se il mutex non è stato creato
        // o se l'acquisizione del mutex fallisce (estremamente raro con portMAX_DELAY).
        // Potresti comunque stampare un messaggio base per non perdere il log,
        // ma non sarà thread-safe.
        Serial.printf("!!! Failed to acquire log mutex or mutex not initialized: ");
        // Serial.printf(format, ##__VA_ARGS__); // Stampa il messaggio originale non formattato
        Serial.println();
    }
}

ESP32LoggerMutex myLog;




// EXTERNAL utiliies
// EXTERNAL utiliies
// EXTERNAL utiliies
// EXTERNAL utiliies
// EXTERNAL utiliies
// EXTERNAL utiliies

// converts to (HH:MM:SS)
// const char* to_HHMMSS2(uint32_t usec, char *outStr, uint8_t maxlen) {
//     // time_t rawTime = mseconds / 1000;
//     // struct tm *timeInfo = gmtime(&rawTime);
//     // strftime(outStr, maxlen, "%H:%M:%S", timeInfo);

//     // uint64_t usec = esp_timer_get_time(); // Tempo in microsecondi dall'avvio
//     uint32_t msec = (usec / 1000) % 1000;
//     uint32_t sec  = (usec / 1000000) % 60;
//     uint32_t min  = (usec / 60000000) % 60;
//     uint32_t hour  = (usec / 3600000000);
//     snprintf(outStr, maxlen, "%02lu:%02lu:%02lu.%03lu", hour, min, sec, msec);
//     return outStr;

// }



