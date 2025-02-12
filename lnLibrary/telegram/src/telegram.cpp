/*
 * Rui Santos
 * Complete Project Details http://randomnerdtutorials.com
*/

#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <WiFiClientSecure.h>


#include "@telegram.h"
#include "@lnMacros.h"





// ---------------------------------
// macros Aliases
// ---------------------------------
// #define printf0                       LN_PRINTf
#define printf0_NFN                    LN_PRINTf_NowFN




WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;



// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
    printf0_NFN("received %d messages\n", numNewMessages);

    for (int i=0; i<numNewMessages; i++) {
        String chat_id = bot.messages[i].chat_id;
        printf0_NFN("chat_id: %s\n", chat_id);

        if ((chat_id != CHAT_ID) and (chat_id != "129190557")) {
            bot.sendMessage(chat_id, "Unauthorized user", "");
            continue;
        }

        // Print the received message
        String text = bot.messages[i].text;
        printf0_NFN("received msg: %s\n", text);

        uint8_t index=text.indexOf("@");
        text=text.substring(0, index);
        printf0_NFN("cleaned received msg: %s\n", text);

        String from_name = bot.messages[i].from_name;

        if (text == "/help" || text == "/h" ) {
            String welcome = "Welcome, " + from_name + ".\n";
            welcome += "Use the following commands to control your outputs.\n\n";
            welcome += "/pump_on [pulsetime (sec)]\n";
            welcome += "/pump_off\n";
            welcome += "/loadSuperiore_on [pulsetime (sec)]\n";
            welcome += "/loadSuperiore_off\n";
            welcome += "/led_state to request current LED state \n";
            bot.sendMessage(chat_id, welcome, "");
        } else if (text.startsWith("/pump_on")) {
            bot.sendMessage(chat_id, "LED state set to ON", "");
            // digitalWrite(LED, HIGH);
        } else if (text == "/pump_off") {
            bot.sendMessage(chat_id, "LED state set to OFF", "");
            // digitalWrite(LED, LOW);
        } else if (text == "/led_state") {
            // if (digitalRead(LED)){
            //     bot.sendMessage(chat_id, "LED is ON", "");
            // } else {
            //     bot.sendMessage(chat_id, "LED is OFF", "");
            // }
        }

    }
}



void telegramSetup() {
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
}


void telgramHandle() {
    if (millis() > lastTimeBotRan + botRequestDelay)  {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while(numNewMessages) {
            Serial.println("got response");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }

        lastTimeBotRan = millis();
    }
}


void sendTelegramMessage(String chat_id, String text_msg) {
     bot.sendMessage(chat_id, text_msg, "");
}