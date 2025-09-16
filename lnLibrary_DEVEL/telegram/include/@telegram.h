/*
 * Rui Santos
 * Complete Project Details http://randomnerdtutorials.com
*/

#include <Arduino.h>

#ifndef __TELEGRAM_H__
    #define __TELEGRAM_H__

    #include "@ln_esp32_telegram_definitions.h" // /home/loreto/lnProfile/envars/include/@ln_esp32_telegram_definitions.h

    #define BOTtoken LoretoEsp32Bot
    #define CHAT_ID Esp32_test_chat


    void telegramSetup(void);


    void telgramHandle(void);
    void sendTelegramMessage(String chat_id, String text_msg);

#endif


// #else