//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 11.49.29
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <WiFi.h>
#include <stdarg.h> // Per i parametri variabili di addFormattedString
#include <HTTPClient.h>


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include "telegramBot_Class.h"

HTTPClient http;

char tempBuffer[MAX_TELEGRAM_MESSAGE_SIZE];


TelegramBot_Class::TelegramBot_Class() {}

// #######################################################################
// # init
// #######################################################################
void TelegramBot_Class::init(const char* token, const char* chatId, const char *parseMode) {
    m_token = token;
    m_chatId = chatId;
    setParseMode(parseMode);
    clearMessage();
}


// #######################################################################
// # aggiunge una linea in formato HTML quindi aggiunge uno %0a finale
// # forse posso eliminarla mettendo lo '\n' nella addString()
// #######################################################################
void TelegramBot_Class::clearMessage(void) {
    tg->msg[0] = '\0';
    tg->pos = 0;
}

// #######################################################################
// # aggiunge una linea in formato HTML quindi aggiunge uno %0a finale
// # forse posso eliminarla mettendo lo '\n' nella addString()
// #######################################################################
void TelegramBot_Class::startNewMessage(const char* format, ...) {
    clearMessage();
    va_list args;
    va_start(args, format);
    vsnprintf(tempBuffer, sizeof(tempBuffer), format, args); // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.
    va_end(args);

    uint16_t max_size = MAX_TELEGRAM_MESSAGE_SIZE - tg->pos;
    tg->pos += snprintf(tg->msg + tg->pos, max_size, "%s", tempBuffer);
    // LOG_INFO("pos: %d msg: %s", tg->pos, tg->msg);
}




// #######################################################################
// # stringa
// #######################################################################
void TelegramBot_Class::addString(const char* text) {
    uint16_t max_size = MAX_TELEGRAM_MESSAGE_SIZE - tg->pos;
    tg->pos += snprintf(tg->msg + tg->pos, max_size, "%s", text);
    // LOG_INFO("pos: %d msg: %s", tg->pos, tg->msg);
}



// #######################################################################
// # stringa in formato printf()
// #######################################################################
void TelegramBot_Class::addFormattedString(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(tempBuffer, sizeof(tempBuffer), format, args); // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.
    va_end(args);

    // strcat(tg->msg, tempBuffer);
    uint16_t max_size = MAX_TELEGRAM_MESSAGE_SIZE - tg->pos;
    tg->pos += snprintf(tg->msg + tg->pos, max_size, "%s", tempBuffer);
    // LOG_INFO("pos: %d msg: %s", tg->pos, tg->msg);
}



// #######################################################################
// # add "HH:MM:SS"
// #######################################################################
void TelegramBot_Class::addTime(const char *prefix, const char *suffix) {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      addString("TIME_ERROR"); // Fallback in caso di errore
      return;
    }

    // prendiamo il tempo;
    snprintf(m_timeStamp, sizeof(m_timeStamp), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    addFormattedString("%s%s%s", prefix, m_timeStamp, suffix);
}






// #######################################################################
// # set parse mode
// #######################################################################
void TelegramBot_Class::setParseMode(const char* mode) {
    snprintf(m_parseMode, sizeof(m_parseMode), "%s", mode);
    // LOG_INFO("parseMode: %s", m_parseMode);
}




// #######################################################################
// # prima dell'invio passa il messaggio alla urlencode per convertice caratteri speciali
// # comunque ripulisce il messaggio     tg->msg[0] = '\0';
// #######################################################################
bool TelegramBot_Class::send() {
    bool fStatus=false;
    if (WiFi.status() != WL_CONNECTED) {
        LOG_ERROR("WiFi non connected. Message non sent.");
        tg->msg[0] = '\0'; // clear message
        return false;
    }
    // LOG_INFO("pos: %d msg: %s", tg->pos, tg->msg);


    // HTTPClient http;

    // ---messsage
    uint16_t msg_len = strlen(tg->msg);
    LOG_DEBUG("tg->msg [%ld]: %s", msg_len, tg->msg);
    if (msg_len > MAX_TELEGRAM_MESSAGE_SIZE) {
        LOG_ERROR("tg->msg [len: %ld] is greather than MAX_TELEGRAM_MESSAGE_SIZE (%ld)", msg_len, MAX_TELEGRAM_MESSAGE_SIZE);
    }

    // ---encoded
    urlEncode(tg->msg, tg->encoded);

    // Costruisce l'URL completo con tutti i parametri

    // snprintf() scrive al massimo n-1 caratteri più il terminatore nul (\0) in dest.
    const char *urlFormat = "https://api.telegram.org/bot%s/sendMessage?chat_id=%s&parse_mode=%s&text=%s";
    snprintf(tg->fullMsg, sizeof(tg->fullMsg), urlFormat, m_token, m_chatId, m_parseMode, tg->encoded);

    LOG_DEBUG("Sending msg: [%ld]: %s", strlen(tg->fullMsg), tg->fullMsg);
    http.begin(tg->fullMsg);
    int httpResponseCode = http.GET();
    http.end();
    tg->msg[0] = '\0'; // clear message

    LOG_DEBUG("[http code: %d]", httpResponseCode);

    switch (httpResponseCode) {
        // Informational responses (100 – 199)
        // Successful responses (200 – 299)
        // Redirection messages (300 – 399)
        // Client error responses (400 – 499)
        // Server error responses (500 – 599)

        case 200 ...299:
            LOG_INFO("[%d] - Send OK", httpResponseCode);
            fStatus=true;
            break;

        case 300 ...399:
            LOG_INFO("[%d] - Redirect....????", httpResponseCode);
            fStatus=true;
            break;

        case 400 ...499:
            LOG_ERROR("[%d] - Bad Request! Client Error URL: %s (size: %ld)", httpResponseCode, tg->fullMsg, strlen(tg->fullMsg));
            break;

        case 500 ...599:
            LOG_ERROR("[%d] - Bad Request! Server Error URL: %s (size: %ld)", httpResponseCode, tg->fullMsg, strlen(tg->fullMsg));
            break;

        default:
            LOG_ERROR("[%d] - Send ERROR! URL: %s (size: %ld)", httpResponseCode, tg->fullMsg, strlen(tg->fullMsg));
            break;
    }


    return fStatus;
}



// | Carattere                   | Escape C/C++ | Decimale | Esadecimale | URL encoding                                                     |
// | --------------------------- | ------------ | -------- | ----------- | ---------------------------------------------------------------- |
// | **NUL**                     | `\0`         | 0        | 00          | `%00`                                                            |
// | **TAB**                     | `\t`         | 9        | 09          | `%09`                                                            |
// | **LF** (line feed, newline) | `\n`         | 10       | 0A          | `%0A`                                                            |
// | **CR** (carriage return)    | `\r`         | 13       | 0D          | `%0D`                                                            |
// | **SPACE**                   | `' '`        | 32       | 20          | `%20` *(oppure `+` solo in `application/x-www-form-urlencoded`)* |
// | **"** (doppio apice)        | `\"`         | 34       | 22          | `%22`                                                            |
// | **'** (apice singolo)       | `'`          | 39       | 27          | `%27`                                                            |
// | **/** (slash)               | `/`          | 47       | 2F          | `%2F`                                                            |
// | **\\** (backslash)          | `\\`         | 92       | 5C          | `%5C`                                                            |


uint16_t TelegramBot_Class::urlEncode(const char* src, char* dest) {
    uint16_t urlEncode_len;
    const char *p = src;
    char *q = dest;
    while (*p) {
        if (isalnum((unsigned char)*p) || *p == '-' || *p == '_' || *p == '.' || *p == '~') {
            *q++ = *p;
        } else if (*p == ' ') {
            *q++ = '+';
        } else if (*p == '\n') { // <--- Aggiungi questa condizione
            *q++ = '%';
            *q++ = '0';
            *q++ = 'A';
        } else if (*p == '\t') { // <--- Aggiungi questa condizione
            *q++ = '%';
            *q++ = '0';
            *q++ = '9';
        } else {
            // Codifica i caratteri speciali in formato esadecimale %XX
            sprintf(q, "%%%02X", (unsigned char)*p);
            q += 3;
        }
        p++;
        urlEncode_len = q-dest;
        if (urlEncode_len >= MAX_TELEGRAM_ENCODED_SIZE) {
            LOG_ERROR("urlEncode [len: %ld] is greather than MAX_TELEGRAM_ENCODED_SIZE (%ld)", urlEncode_len, MAX_TELEGRAM_ENCODED_SIZE);
            break;
        }
    }
    *q = '\0';
    LOG_DEBUG("urlEncode [len: %ld] - %s", urlEncode_len, dest);
    return urlEncode_len;
}