//
// updated by ...: Loreto Notarantonio
// Date .........: 13-02-2025 09.17.30
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <WiFi.h>

#include "@ln_wifi.h"

// --- loreto others
#include "@lnMacros.h"
#include "@ln_MillisTimer.h"
#include "@ln_time.h"
#include "@telegram.h"

extern ESP32Time     rtc;
extern struct tm timeinfo; // capire se va bene uno per tutti i moduli oppure mantenerli separati per evitare overwrites

// ---------------------------------
// macros Aliases
// ---------------------------------
// #define print           ln_print
#define printf0          lnPrintF
#define printf0_FN        lnPrintF_FN
#define printf0_NFN        lnPrintF_NowFN

#define print_ln         lnPrintLN
#define print_ln2        /*lnPrintLN*/
#define printf2_FN        lnPrintF_FN
#define printf2_NFN        lnPrintF_NowFN




// #########################################
// # se non c'è ln_time.cpp mi seerver una dummy_Now()
// #########################################
const int8_t TIME_BUFFER_LENGTH = 20;
char  PROGMEM temp_buffer_time[TIME_BUFFER_LENGTH];
char *nowTimeDummy() {
    snprintf(temp_buffer_time, TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return temp_buffer_time;
}



// structMillisTimer_t timer1MIN;  structMillisTimer_t *timer1min = &timer1MIN;
// structMillisTimer_t timer15MIN; structMillisTimer_t *timer15min = &timer15MIN;


void setup() {
    Serial.begin(115200);
    delay(2000);
    print_ln();


    // -----------------------------------
    // ------ set Time
    // -----------------------------------
    time_setup();
    delay(4000);
    printf0_NFN("local time has been set\n");



    // -----------------------------------
    // ------ start WiFi -----------
    // -----------------------------------
    wifi_Start();
    for (int8_t i=0; i<SSID_ELEMENTS; i++) {
        printf2_FN("%d - new_ssid: %s\n", i, mySSID[i].ssid);
    }

    // -----------------------------------
    // ------ set NTP server
    // -----------------------------------
    NTPInit();
    delay(4000);
    printf2_NFN("ntp client has been started\n");
    telegramSetup();
    sendTelegramMessage(CHAT_ID, "ciao sono in piedi");

}



// ##########################################################
// #
// ##########################################################
uint32_t loop_millis;
uint32_t last_loop_time;

int8_t asyncWifiScan_started=0;
uint8_t first_run=0;

#define MINUTE_O_CLOCKn

void loop() {
    timeinfo = rtc.getTimeStruct();

    #ifdef MINUTE_O_CLOCK // allineamento al minuto ... non so se mi serve
    if (first_run == 0) {
        first_run++;
        printf0_NFN("waiting for minute o'clock\n");
        timeinfo = rtc.getTimeStruct();
        while (timeinfo.tm_sec != 0) {
            timeinfo = rtc.getTimeStruct();
            printf0_NFN("second: %d\n", timeinfo.tm_sec);
        }
        printf0_NFN("exiting...\n");
    }
    #endif


    // ---------------------------------
    // actions on 01 minutes timer
    // ---------------------------------
    if (isMinuteOClock()) {
        printf2_NFN("minute o'clock routine\n");
        // ---------------------------------
        // if wifiScan has been started check for results
        // ---------------------------------
        if (asyncWifiScan_started > 0) {
            asyncWifiScan_started++; // aumentiamo
            int16_t networksFound = wifi_scanComplete();
            printf2_NFN("networks found: %d (-1 means still_running)\n", networksFound);
            if (networksFound >= 0) {
                connectOnScanResult(networksFound);
                asyncWifiScan_started=0; // chiudiamo il flag
                wifi_scanDelete();
            }
            if (asyncWifiScan_started > 4) { // inutile dopo 4 minuti....
                asyncWifiScan_started=0; // chiudiamo il flag
                wifi_scanDelete();
            }
        }
    }


    // ---------------------------------
    // actions every 15 minutes (at any quarter)
    // ---------------------------------
    if (isQuarterOClock()) {
        printf2_NFN("it's a quarter of hour routine\n");
        wifi_asyncScan();
        asyncWifiScan_started=1;
    }




    if (! wifi_isConnected()) {
        printf0_NFN("connection lost\n");
        wifi_reconnect();
    }

    // last_loop_time = millis();
    // Serial.println(millis()%1000);
    // printf2_FN("millis() %ld - reminder: %d - delay: %d\n", millis(), reminder, 1000-reminder);

    // m_reminder = (millis()%1000);
    delay(200);
}


