//
// updated by ...: Loreto Notarantonio
// Date .........: 09-08-2025 19.16.48
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <WiFi.h>

#include "lnLogger_Class.h"
#include "lnSerialRead.h" // waitForEnter()
#include "lnTime_Class.h"
#include "WiFiManager_Class.h"


#define __LOAD_SSIDS_CPP__
    #include "wifiManager_ssid_credentials.h"
#undef __LOAD_SSIDS_CPP__

// ----------------------------------------------------
// Codice principale del tuo sketch
// ----------------------------------------------------

bool isElapsed() {
    static uint32_t lastElapsed, elapsed;
    elapsed = millis() - lastElapsed;
    if (elapsed%2000 >= 10) { // ogni 2 secondi
        lastElapsed = millis();
        return true;
    }
    return false;
}


// Crea un'istanza della classe WiFiManager
// WiFiManager_Class myWiFiManager(myNetworks, sizeof(myNetworks) / sizeof(myNetworks[0]));
WiFiManager_Class myWiFiManager;
// LnTime_Class lnTime;

void setup() {
    Serial.begin(115200);
    delay(1000);
    lnLog.init();
    // Inizializza la classe
    myWiFiManager.init(myNetworks, sizeof(myNetworks) / sizeof(myNetworks[0]));
    delay(5000); // attendiamo un po...
    LOG_INFO("connessione time");
    lnTime.setup();
    LOG_INFO("fine connessione time");
}


struct tm timeinfo;
void loop() {
    static bool first_run=true;
    static uint8_t last_second=99;
    uint8_t curr_second;
    // static uint32_t startedMillis;
    char nowTimeBUFFER[16];
    // uint32_t now;
    timeinfo = lnTime.getTimeStruct();
    curr_second = timeinfo.tm_sec;
    // LOG_SPEC("curr_second: %d", curr_second);

    if (first_run) {
        first_run=false;
        // startedMillis=millis();
    }


    // Chiama il metodo loop() della classe per la gestione continua
    myWiFiManager.update();

    if (myWiFiManager.isConnected()) {
        if (! lnTime.ntpActive()) {
            lnTime.setup();
        }
    }



    // now = millis() - startedMillis;
    // if (curr_second%5 == 0 && curr_second != last_second) { // ogni 5 secondi
    if (lnTime.everyXseconds(5)) { // ogni 5 secondi
        LOG_SPEC("curr_second: %d", curr_second);
        // last_second = curr_second;
        LOG_NOTIFY("-----------");
        LOG_INFO("nowTime:                %s",         lnTime.nowTime());
        LOG_INFO("nowTime:         0      %s",         lnTime.timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER)) );
        LOG_INFO("nowTime:         0      %s",         lnTime.timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER), 0, true) );

        LOG_INFO("msecToTimeStamp: millis %s",         lnTime.timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER), millis(), true));

        LOG_INFO("msecToTimeStamp: mymsec %s",        lnTime.timeStamp(nowTimeBUFFER, sizeof(nowTimeBUFFER), lnTime.millisecOfDay(), true));
        LOG_NOTIFY("------------");
        // uint32_t epoch = lnTime.getEpoch();
        // LOG_SPEC(" epoch ms  %lu",         epoch);
        // uint32_t seconds = lnTime.getSecond();
        // LOG_SPEC(" seconds  %lu",         epoch);
        // LOG_NOTIFY("------------");

    }

    delay(50);
    // waitForEnter();

}
