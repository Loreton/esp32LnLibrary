//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 08.29.52
//


#pragma once
    // #include <Arduino.h>    // in testa anche per le definizioni dei type
    // #include <HTTPClient.h>


    // #define MAX_MESSAGE_SIZE 512
    #define MAX_TELEGRAM_MESSAGE_SIZE   300
    #define MAX_TELEGRAM_ENCODED_SIZE   500
    #define MAX_TELEGRAM_FULL_MSG_SIZE  600

    typedef struct {
        uint16_t pos = 0; // posizione da scrivere nel msg[]
        char msg[MAX_TELEGRAM_MESSAGE_SIZE + 1];
        char encoded[MAX_TELEGRAM_ENCODED_SIZE + 1];
        char fullMsg[MAX_TELEGRAM_FULL_MSG_SIZE + 1];
    } telegramBuffers_t;

    class TelegramBot_Class {
        public:
            // Costruttore
            TelegramBot_Class();

            // TelegramBot_Class(const char* token, const char* chatId, const char *parseMode="HTML");
            void init(const char* token, const char* chatId, const char *parseMode="HTML");

            // Metodi per la preparazione del messaggio
            void clearMessage();
            void startNewMessage(const char* format, ...);
            void addString(const char* text);
            void addFormattedString(const char* format, ...);
            void addTime(const char *prefix=nullptr, const char *suffix=nullptr); // nel centro mettiamo TIME
            void setParseMode(const char* mode);

            // Metodo per l'invio
            bool send();

        private:

            const char* m_token;
            const char* m_chatId;
            telegramBuffers_t tgMessage; // crea un'instanza di struct ed un pointer
            telegramBuffers_t *tg = &tgMessage; // crea un'instanza di struct ed un pointer
            // tg = &tgMessage;
            // char m_messageBuffer[MAX_MESSAGE_SIZE];
            char m_parseMode[10];
            char m_timeStamp[16];

            // Funzione helper per l'URL-encoding
            uint16_t urlEncode(const char* src, char* dest);

            // Funzione helper per l'invio HTTP
            bool sendHttpRequest(const char* url);
    };
