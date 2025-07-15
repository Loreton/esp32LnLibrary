/*
// updated by ...: Loreto Notarantonio
// Date .........: 15-07-2025 19.19.17
*/


#include <Arduino.h>
#include "esp_timer.h" // Per esp_timer_get_time()

#include "lnLogger.h"

ESP32Logger::ESP32Logger(void) {};



/**
 * @brief Genera un timestamp formattato (HH:MM:SS.mmm) usando esp_timer_get_time().
 * @return Una stringa costante contenente il timestamp.
 */

const char* ESP32Logger::getTimestamp() {
    m_us = esp_timer_get_time(); // Tempo in microsecondi dall'avvio
    m_ms = (m_us / 1000) % 1000;
    m_s  = (m_us / 1000000) % 60;
    m_m  = (m_us / 60000000) % 60;
    m_h  = (m_us / 3600000000);

    snprintf(m_tbuf, sizeof(m_tbuf), "%02lu:%02lu:%02lu.%03lu", m_h, m_m, m_s, m_ms);
    return m_tbuf;
}


/**
 * @brief Formatta il nome del file e il numero di linea.
 * Il nome del file viene troncato a una lunghezza massima e riempito con punti se più corto.
 * @param file Il path completo del file (solitamente __FILE__).
 * @param line Il numero di linea (solitamente __LINE__).
 * @return Una stringa costante contenente il nome del file formattato e il numero di linea.
 */
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
    memset(name_buffer, paddingChar, m_maxlen); // Riempie il buffer con punti per il padding
    memcpy(name_buffer, m_filename, len); // Copia il nome effettivo
    name_buffer[m_maxlen] = '\0'; // Termina la stringa

    snprintf(m_out, sizeof(m_out), "%s.%03d", name_buffer, line);
    return m_out;
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
// void ESP32Logger::info(const char* file, int line, const char* format, ...) {
//      write(LogColors::GREEN, "INF", file, line, format,  ##__VA_ARGS__);
// }

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
    char buffer[256]; // Buffer per il messaggio formattato
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
