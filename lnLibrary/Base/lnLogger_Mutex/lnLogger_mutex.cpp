/*
// updated by ...: Loreto Notarantonio
// Date .........: 16-07-2025 18.08.19
*/



#include <Arduino.h>
#include "esp_timer.h"
#include <freertos/semphr.h> // <-- AGGIUNGI QUESTO anche qui per l'implementazione del mutex

#include "lnLogger.h"


// Costruttore: Inizializza il mutex
ESP32LoggerMutex::ESP32LoggerMutex(void) {
    // Il costruttore fa il minimo indispensabile, o nulla.
    // Il mutex verrà creato in begin().
    m_logMutex = NULL; // Inizializza a NULL per sapere che non è stato ancora creato
};

// la uso in modo che posso mandare un messaggio se non riesco ad inizializzare il mutex
void ESP32LoggerMutex::begin() {
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

/**
 * @brief Genera un timestamp formattato (HH:MM:SS.mmm) usando esp_timer_get_time().
 * @return Una stringa costante contenente il timestamp.
 */

const char* ESP32LoggerMutex::getTimestamp() {
    m_usec = esp_timer_get_time(); // Tempo in microsecondi dall'avvio
    m_msec = (m_usec / 1000) % 1000;
    m_sec  = (m_usec / 1000000) % 60;
    m_min  = (m_usec / 60000000) % 60;
    m_hour  = (m_usec / 3600000000);

    snprintf(m_tbuf, sizeof(m_tbuf), "%02lu:%02lu:%02lu.%03lu", m_hour, m_min, m_sec, m_msec);
    return m_tbuf;
}


/**
 * @brief Formatta il nome del file e il numero di linea.
 * Il nome del file viene troncato a una lunghezza massima e riempito con punti se più corto.
 * @param file Il path completo del file (solitamente __FILE__).
 * @param line Il numero di linea (solitamente __LINE__).
 * @return Una stringa costante contenente il nome del file formattato e il numero di linea.
 */
const char* ESP32LoggerMutex::getFileLineInfo(const char* file, int line) {
    m_filename = strrchr(file, '/'); // Trova l'ultimo '/' per ottenere solo il nome del file
    m_filename = m_filename ? m_filename + 1 : file; // Se trovato, sposta il puntatore, altrimenti usa l'intero path

    // Trova il separatore per tagliare l'estensione o un suffisso (es. _H o .cpp)
    m_sep = strrchr(m_filename, '_');
    if (!m_sep) m_sep = strrchr(m_filename, '.');

    size_t len = m_sep ? (size_t)(m_sep - m_filename) : strlen(m_filename); // Lunghezza del nome senza estensione
    // const size_t m_maxlen = 15; // Massima lunghezza desiderata per il nome del file

    char name_buffer[m_maxlen + 1]; // Buffer temporaneo per il nome del file troncato/paddato
    if (len > m_maxlen) len = m_maxlen; // Tronca il nome se più lungo di maxlen

    // memset(name_buffer, '.', m_maxlen); // Riempie il buffer con punti per il padding
    memset(name_buffer, m_paddingChar, m_maxlen); // Riempie il buffer con punti per il padding
    memcpy(name_buffer, m_filename, len); // Copia il nome effettivo
    name_buffer[m_maxlen] = '\0'; // Termina la stringa

    snprintf(m_out, sizeof(m_out), "%s.%03d", name_buffer, line);
    return m_out;
}












/**
 * @brief Questa ora è un semplice wrapper che cattura gli argomenti e li passa a write().
 * @brief Esempio nel caso volessi creare delle funzioni per ogni livello di log

void ESP32LoggerMutex::info(const char* file, int line, const char* format, ...) {
    va_list args;
    va_start(args, format);
    write(LogColors::GREEN, "INF", file, line, format,  args);
    va_end(args);
}
*/

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
        // Questa è la situazione più critica: un log prima di Serial.begin() e myLog.begin().
        // Per ora, lo stampiamo su Serial ma senza garanzie di visibilità.
        // Per log molto precoci, potresti considerare un buffer FIFO o JTAG/SWD.
        Serial.printf("AVVISO: Logging prima dell'inizializzazione: ");
        // Serial.printf(format, ##__VA_ARGS__);
        Serial.println();
        return; // Esci per evitare problemi
    }


    // Tenta di acquisire il mutex. Aspetta indefinitamente (portMAX_DELAY) se è già bloccato.
    if (m_logMutex != NULL && xSemaphoreTake(m_logMutex, portMAX_DELAY) == pdTRUE) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        int len = vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        if (len >= sizeof(buffer)) {
            buffer[sizeof(buffer) - 1] = '\0';
        }

        Serial.printf("%s[%s][%s][%s] %s%s\n",
                      color,
                      getTimestamp(),       // Questi ora operano sui membri di myLog
                      getFileLineInfo(file, line), // Questi ora operano sui membri di myLog
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

