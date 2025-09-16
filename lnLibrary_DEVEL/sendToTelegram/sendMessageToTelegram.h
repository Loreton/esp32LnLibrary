//
// updated by ...: Loreto Notarantonio
// Date .........: 16-08-2025 11.36.25
//


#pragma once

    // Telegram offre tre modalità di parsing per i messaggi dei bot, che puoi specificare con il parametro parse_mode:
    // HTML: Permette di usare tag HTML standard come:
    //  Riepilogo dei tag supportati
    //      Quando usi parse_mode=HTML, Telegram supporta solo un subset specifico di tag.
    //      <b> o <strong> per il grassetto
    //      <i> o <em> per il corsivo
    //      <u> o <ins> per il <u>sottolineato</u>
    //      <s> o <strike> o <del> per il <s>barrato</s>
    //      <code> per il codice inline
    //      <pre> per un blocco di codice preformattato
    //      <a> per i link
    //      <span class="tg-spoiler"> o <tg-spoiler> per il testo spoiler
    //      %%0a (doppio percente) per sostituire il <br>
    //
    // MarkdownV2: Un sistema di formattazione più ricco del vecchio Markdown, che supporta:
    //      grassetto, corsivo, sottolineatura, barrato, spoiler, link e codice.
    //      Richiede l'escaping di alcuni caratteri speciali (_, *, [, ], (, ), ~, \ , >,#,+,-,=,|,{,} etc.)
    //      per evitare che vengano interpretati come markup.
    //
    // Nessuna modalità:
    //      Se il parametro parse_mode non viene specificato, o viene lasciato vuoto,
    //      il messaggio viene inviato come testo semplice, senza alcuna formattazione.

    typedef struct telegramMessage {
        //       char        urlBuffer[200];    // Buffer per l'URL completo
        // const uint16_t    urlBuffer_LEN=200;
        //       char        dataBuffer[300];    // Buffer per l'URL completo
        // const uint16_t    dataBuffer_LEN=300;

              char        timestamp[16];    // Buffer per l'URL completo
              char        buffer[512];    // Buffer per l'URL completo
        const uint16_t    MAX_BUFFER_LEN=512;
        // uint16_t          startData=0;
        uint16_t          MAX_DATA_LEN=0;
        const char      * bufferData; // puntatore da cui andremo a scrivere i dati del messaggio (subito dopo la URL) a partire da startData



        const char      * telegramApiUrl;
        const char      * token;
        uint64_t          chatID;
    } telegramMessage_t;


    #ifdef   __I_AM_SEND_MESSAGE_TO_TELEGRAM_CPP__
        telegramMessage_t tgMsg;
        const char *modeHTML     = "HTML";
        const char *modeMARKDOWN = "MarkdownV2";
        const char *modeTEXT     = "";
    #else
        extern telegramMessage_t tgMsg;
        extern const char *modeHTML;
        extern const char *modeMARKDOWN;
        extern const char *modeTEXT;
    #endif


    void sendMessageToTelegram(String message, const char* parseMode);
    // void sendMessageToTelegramS(const char* message, const char* parseMode);
    // void sendMessageToTelegram(const char* message, const char* parseMode);
    // void sendMessageToTelegram(const char* parseMode);
    void sendMessageToTelegram_setup(const char *token, int64_t chatID);
    void sendMessageToTelegram(void);


    void sendMessageToTelegram(const char* title, const char* message, const char* parseMode);
