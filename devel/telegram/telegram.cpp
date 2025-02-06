/*
 * Rui Santos
 * Complete Project Details http://randomnerdtutorials.com
*/

#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <WiFiClientSecure.h>


#include "@telegram.h"
#include "@lnMacros.h"


#define debug00                       ln_printf
#define debug01                  //     ln_printf
#define debug00_fn                    ln_printf_FN
#define debug00_now                   ln_printf_Now
#define debug00_nowfn                 ln_printf_NowFN




WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;



// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
    debug00_fn("received %d messages\n", numNewMessages);

    for (int i=0; i<numNewMessages; i++) {
        String chat_id = bot.messages[i].chat_id;
        debug00("\tchat_id: %s\n", chat_id);

        if ((chat_id != CHAT_ID) and (chat_id != "129190557")) {
            bot.sendMessage(chat_id, "Unauthorized user", "");
            continue;
        }

        // Print the received message
        String text = bot.messages[i].text;
        debug01("\treceived msg: %s", text);

        uint8_t index=text.indexOf("@");
        text=text.substring(0, index);
        debug00("\tcleaned received msg: %s", text);

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