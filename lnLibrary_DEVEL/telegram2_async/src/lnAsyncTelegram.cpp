/*
    ref: https://github.com/cotestatnt/AsyncTelegram2/tree/master

    Name:        lightBot.ino
    Created:     26/03/2021
    Author:      Tolentino Cotesta <cotestatnt@yahoo.com>
    Description: a simple example that do:
                1) parse incoming messages
                2) if "LIGHT ON" message is received, turn on the onboard LED
                3) if "LIGHT OFF" message is received, turn off the onboard LED
                4) otherwise, reply to sender with a welcome message

*/


/* 
    Set true if you want use external library for SSL connection instead ESP32@WiFiClientSecure
    For example https://github.com/OPEnSLab-OSU/SSLClient/ is very efficient BearSSL library.
    You can use AsyncTelegram2 even with other MCUs or transport layer (ex. Ethernet)
    With SSLClient, be sure "certificates.h" file is present in sketch folder
*/ 

#define USE_CLIENTSSL false

#include <AsyncTelegram2.h>
// Timezone definition
#include <time.h>
#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"

#include "@ln_esp32_telegram_definitions.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
WiFiClientSecure client;


AsyncTelegram2 myBot(client);
// const char* ssid  =  "Casetta";     // SSID WiFi network
// const char* pass  =  "password";     // Password  WiFi network
// const char* token =  "xxxxxxxxxxx:xxxxxxxxxxxxxxxxxxxxxxxxxxxxx";  // Telegram token

const uint8_t LED = LED_BUILTIN;




// local variable to store telegram message data
TBMessage msg;

void chkTelegram() {

    // if there is an incoming message...
    if (myBot.getNewMessage(msg)) {
        String msgText = msg.text;

        if (msgText.equals("/light_on")) {                 // if the received message is "LIGHT ON"...
            digitalWrite(LED, LOW);                          // turn on the LED (inverted logic!)
            myBot.sendMessage(msg, "Light is now ON");       // notify the sender
        }
        else if (msgText.equals("/light_off")) {           // if the received message is "LIGHT OFF"...
            digitalWrite(LED, HIGH);                          // turn off the led (inverted logic!)
            myBot.sendMessage(msg, "Light is now OFF");       // notify the sender
        }
        else {                                              // otherwise...
            // generate the message for the sender
            String reply;
            reply = "Welcome " ;
            reply += msg.sender.username;
            reply += ".\nTry /light_on or /light_off ";
            myBot.sendMessage(msg, reply);                    // and send it
        }
    }

}



void setupTelegram() {
    // Set the Telegram bot properies
    myBot.setUpdateTime(1000);
    myBot.setTelegramToken(token);

    // Check if all things are ok
    Serial.print("\nTest Telegram connection... ");
    myBot.begin() ? Serial.println("OK") : Serial.println("NOK");

    Serial.print("Bot name: @");
    Serial.println(myBot.getBotName());
}











void main() {
    // initialize the Serial
    Serial.begin(115200);
    Serial.println("\nStarting TelegramBot...");

    // set the pin connected to the LED to act as output pin
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH); // turn off the led (inverted logic!)

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    delay(500);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }

    // Sync time with NTP
    configTzTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
    #if USE_CLIENTSSL == false
        client.setCACert(telegram_cert);
    #endif
    setupTelegram();
}


void setup__() {
    main();
}

void loop__() {
    uint32_t ms = millis();
    chkTelegram();
    Serial.printf("elapsed: %ld\n", millis()-ms);
}

