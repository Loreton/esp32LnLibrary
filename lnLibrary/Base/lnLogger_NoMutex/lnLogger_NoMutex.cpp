/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-07-2025 16.21.32
*/


#include <Arduino.h>
#include "esp_timer.h" // Per esp_timer_get_time()

#include "lnLogger.h"

ESP32Logger::ESP32Logger(void) {};

void ESP32Logger::begin() {
    Serial.println("\n\nOK: Logger NO-MUTEX inizializzato.\n\n");
}




/**
 * @brief Genera un timestamp formattato (HH:MM:SS.mmm) usando esp_timer_get_time().
 * @return Una stringa costante contenente il timestamp.
 */

const char* ESP32Logger::getTimestamp() {
    static const char tbuf[16]; // Buffer statico per il timestamp
    static int64_t  usec = esp_timer_get_time(); // Tempo in microsecondi dall'avvio
    static uint32_t msec = (usec / 1000) % 1000;
    static uint32_t sec  = (usec / 1000000) % 60;
    static uint32_t min  = (usec / 60000000) % 60;
    static uint32_t hour  = (usec / 3600000000);

    snprintf(tbuf, sizeof(tbuf), "%02lu:%02lu:%02lu.%03lu", hour, min, sec, msec);
    return tbuf;
}


/**
 * @brief Formatta il nome del file e il numero di linea.
 * Il nome del file viene troncato a una lunghezza massima e riempito con punti se più corto.
 * @param file Il path completo del file (solitamente __FILE__).
 * @param line Il numero di linea (solitamente __LINE__).
 * @return Una stringa costante contenente il nome del file formattato e il numero di linea.
const char* ESP32Logger::getFileLineInfo(const char* file, int line) {
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

 */

/**
 * @brief Formatta il nome del file e il numero di linea.
 * Il nome del file viene troncato a una lunghezza massima e riempito con punti se più corto.
 * @param file Il path completo del file (solitamente __FILE__).
 * @param line Il numero di linea (solitamente __LINE__).
 * @return Una stringa costante contenente il nome del file formattato e il numero di linea.
 */
const char* ESP32Logger::getFileLineInfo(const char* file, int line) {
    char buffer[32];                    // spazio per il [filename.linNo]
    static const uint8_t MAX_BUFFER_LEN = sizeof(buffer);
    static const uint8_t FILE_LEN = MAX_BUFFER_LEN-4; // Buffer temporaneo per il nome del file troncato/paddato 4 per .lineNo
    static const char   paddingChar  = '.';    // padding char per il file
    static char fname_buffer[FILE_LEN+1];                           // spazio per il filename
    const char *filename = strrchr(file, '/');                      // Trova l'ultimo '/' per ottenere solo il nome del file


    filename = filename ? filename + 1 : file;                      // Se trovato, sposta il puntatore, altrimenti usa l'intero path

    // Trova il separatore per tagliare l'estensione o un suffisso (es. _H o .cpp)
    const char *sep = strrchr(filename, '_');
    if (!sep) sep = strrchr(filename, '.');

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
void ESP32Logger::write(const char* color, const char* tag, const char* file, int line, const char* format, ...) {
    static char buffer[256]; // Buffer per il messaggio formattato
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args); // Formatta il messaggio
    va_end(args);

    // Assicurati che il buffer sia terminato correttamente in caso di overflow
    if (len >= sizeof(buffer)) {
        buffer[sizeof(buffer) - 1] = '\0';
    }

    // Stampa il messaggio formattato sulla Serial
    Serial.printf("%s[%s][%s][%s] %s%s\n",
                  color,
                  getTimestamp(),
                  getFileLineInfo(file, line),
                  tag,
                  buffer,
                  LogColors::RESET);
}

ESP32Logger myLog;


