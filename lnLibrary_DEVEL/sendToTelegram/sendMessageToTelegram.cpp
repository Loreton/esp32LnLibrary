//
// updated by ...: Loreto Notarantonio
// Date .........: 16-08-2025 11.33.27
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <HTTPClient.h>

#include <lnLogger_Class.h>


#define     __I_AM_SEND_MESSAGE_TO_TELEGRAM_CPP__
    #include <sendMessageToTelegram.h>
#undef      __I_AM_SEND_MESSAGE_TO_TELEGRAM_CPP__






// #######################################################################################
// #
// #######################################################################################
void sendMessageToTelegram_setup(const char *token, int64_t chatID) {
    tgMsg.telegramApiUrl = "https://api.telegram.org";
    tgMsg.token = token;
    tgMsg.chatID = chatID;
    // uint16_t MAX_BUFFER_LEN = sizeof(tgMsg.buffer);

    // !                      MAX_BUFFER_LEN                                    !
    // !...................!....................................................!
    // !    ApiUrl         !                       message data                 !
    // !    len            !                  MAX_DATA_LEN                      !

    uint16_t len = snprintf(tgMsg.buffer, tgMsg.MAX_BUFFER_LEN, "%s/bot%s/sendMessage?chat_id=%lld", tgMsg.telegramApiUrl, tgMsg.token, tgMsg.chatID);


    tgMsg.bufferData = tgMsg.buffer + len; // creiamo il pointer alla parte dati della URL
    tgMsg.MAX_DATA_LEN = tgMsg.MAX_BUFFER_LEN - len; // calcolo data_len

    LOG_DEBUG("buffer initialized with URL: %s", tgMsg.buffer);
    LOG_DEBUG("\tbuffer occupied: %lu"tgMsg.startData);

}



// #######################################################################################
// #
// #######################################################################################
// void prepareMessage(const char *message, const char *parseMode) {
//     // uint16_t len = snprintf(tgMsg.buffer, tgMsg.MAX_BUFFER_LEN, "%s/bot%s/sendMessage?chat_id=%lld", tgMsg.telegramApiUrl, tgMsg.token, tgMsg.chatID);

//     // calcolo puntamento area consecutiva
//     // uint16_t MAX_DATA_LEN=sizeof(tgMsg.buffer)
//     const char *ptr=tgMsg.buffer+len;



//     len = snprintf(ptr, MAX_DATA_LEN, "&parse_mode=%s&text=pressControl - %s", parseMode, tgMsg.timestamp, );
//     len = snprintf(tgMsg.buffer+len, tgMsg.MAX_BUFFER_LEN-len, "%s/bot%s/sendMessage?chat_id=%lld", tgMsg.telegramApiUrl, tgMsg.token, tgMsg.chatID);

//     tgMsg.bufferData = tgMsg.buffer + tgMsg.startData; // creiamo il pointer alla parte dati della URL
//     tgMsg.MAX_DATA_LEN = tgMsg.MAX_BUFFER_LEN + tgMsg.startData; // len


//     // tgMsg.startData = snprintf(tgMsg.buffer, tgMsg.MAX_BUFFER_LEN, "%s/bot%s/sendMessage?chat_id=%lld", tgMsg.telegramApiUrl, tgMsg.token, tgMsg.chatID);
//     // tgMsg.bufferData = tgMsg.buffer + tgMsg.startData;

//     LOG_DEBUG("buffer initialized with URL: %s", tgMsg.buffer);
//     LOG_DEBUG("\tbuffer occupied: %lu"tgMsg.startData);

// }





// #######################################################################################
// #
// // #######################################################################################
// void sendMessageToTelegram(const char* parseMode) {
//     LOG_NOTIFY("%s", message);

//     if (WiFi.status() == WL_CONNECTED) {
//         uint16_t len;
//         uint16_t MAX_LEN = tgMsg.MAX_DATA_LEN;
//         HTTPClient http;
//         char *ptr = tgMsg.bufferData;

//         // calcolo time
//         char timeStr[16];
//         lnTime.timeStamp(timeStr, sizeof(timeStr)); // timeStamp

//         // inserisci parse_mode il titolo e d il messaggion
//         len = snprintf(ptr, tgMsg.MAX_DATA_LEN, "&parse_mode=%s&text=pressControl - %s&text=", parseMode, timeStr, message);

//         // agguingiamo il messaggio
//         len += snprintf(ptr+len, MAX_DATA_LEN-len, "&parse_mode=%s&text=%s", parseMode, message);

//         LOG_DEBUG("telegramUrl: %s", tgMsg.buffer);
//         http.begin(tgMsg.buffer);
//         int httpResponseCode = http.GET();

//         if (httpResponseCode > 0) {
//             LOG_INFO("Codice di risposta HTTP: %d", httpResponseCode);
//         } else {
//             LOG_ERROR("Errore nella richiesta HTTP: %d", httpResponseCode);
//         }

//         http.end();
//     }

//     else {
//         LOG_ERROR("msg non sent: WiFi not active.");
//     }
// }






// #######################################################################################
// # Messaggio già salvato in tgMsg.buffer
// #######################################################################################
void sendMessageToTelegram() {
    LOG_NOTIFY("%s", message);
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        LOG_DEBUG("telegramUrl: %s", tgMsg.buffer);  // puntare all'inizio del buffer
        http.begin(tgMsg.buffer);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            LOG_INFO("Codice di risposta HTTP: %d", httpResponseCode);
        } else {
            LOG_ERROR("Errore nella richiesta HTTP: %d", httpResponseCode);
        }

        http.end();
    }

    else {
        LOG_ERROR("msg non sent: WiFi not active.");
    }
}






// #######################################################################################
// #
// #######################################################################################
// void sendMessageToTelegram(const char* title, const char* message, const char* parseMode) {
void sendMessageToTelegram(const char* message, const char* parseMode) {
    LOG_NOTIFY("%s", message);
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // preparazione messaggio
        // uint16_t len;
        // uint16_t MAX_LEN = tgMsg.MAX_DATA_LEN;
        // HTTPClient http;
        // char *ptr = tgMsg.bufferData;

        // calcolo time
        // char timeStr[16];
        // lnTime.timeStamp(timeStr, sizeof(timeStr)); // timeStamp

        // inserisci parse_mode il titolo e d il messaggion
        len = snprintf(tgMsg.bufferData, tgMsg.MAX_DATA_LEN, "&parse_mode=%s&text=%s", parseMode, message);


        LOG_DEBUG("telegramUrl: %s", tgMsg.buffer);  // puntare all'inizio del buffer
        http.begin(tgMsg.buffer);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            LOG_INFO("Codice di risposta HTTP: %d", httpResponseCode);
        } else {
            LOG_ERROR("Errore nella richiesta HTTP: %d", httpResponseCode);
        }

        http.end();
    }

    else {
        LOG_ERROR("msg non sent: WiFi not active.");
    }
}







// #######################################################################################
// #
// #######################################################################################
void sendMessageToTelegram(String message, const char *parseMode) {
    LOG_NOTIFY("%s", message);

    // Length (with one extra character for the null terminator)
    int str_len = message.length() + 1;

    // Prepare the character array (the buffer)
    char charMessage[str_len];

    // Copy it over
    message.toCharArray(charMessage, str_len);
    // oppure
    // sprintf(charMessage, "%s", sString.c_str());
    sendMessageToTelegram(charMessage, parseMode);
}