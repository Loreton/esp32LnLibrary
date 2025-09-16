//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 08.24.20
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <WiFi.h>

// #include "lnLogger_Class.h"
// #include "lnSerialRead.h" // waitForEnter()
// #include "lnTime_Class.h"
// #include "WiFiManager_Class.h"


#include <lnLogger_Class.h>
// #include "../telegramBot_Class.h"
#include "telegramBot_Class.h"



#define __I_AM_MAIN_CPP__   // in testa a tutto
#include "wifiManager_ssid_credentials.h"
#include "@ln_esp32_telegram_credentials.h"



const char* ssid = casetta_ssid;
const char* password = casetta_password;

const char* telegramBotToken = Loreto_Esp32_BotToken;
const char* telegramChatId   = pressControl_ChatID_str;

TelegramBot_Class myBot;

void setup() {
    Serial.begin(115200);
    lnLog.init();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connessione WiFi...");
    }
    Serial.println("Connesso al WiFi");

    // Per addTime() è necessario impostare il time
    configTime(3600, 3600, "pool.ntp.org");
    myBot.init(telegramBotToken, telegramChatId);
}

void loop() {
    myBot.clearMessage();
    myBot.setParseMode("HTML");

    // myBot.addFormattedString("<b>ESP32</b> - ");
    myBot.addTime("<b>ESP32</b> - ", "\n");
    myBot.addString("PC relay: <b>OFF</b>\n");
    myBot.addString("PC status: <b>OFF</b>\n");
    myBot.addString("PUMP status: <b>OFF</b>\n");
    myBot.addFormattedString("<b>ESP32</b> - %s %d", "ciao", 54);

    if (myBot.send()) {
        Serial.println("Messaggio inviato con successo!");
    } else {
        Serial.println("Invio messaggio fallito.");
    }

    delay(60000); // Invia un messaggio ogni minuto
}