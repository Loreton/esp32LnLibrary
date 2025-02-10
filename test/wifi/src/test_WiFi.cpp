//
// updated by ...: Loreto Notarantonio
// Date .........: 10-02-2025 19.38.30
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <WiFi.h>

#include "@ln_wifi.h"

// --- loreto others
#include "@lnMacros.h"
#include "@ln_MillisTimer.h"


// ---------------------------------
// macros Aliases
// ---------------------------------
// #define print           ln_print
#define printf0          LN_PRINTf
#define printf0_FN        LN_PRINTf_FN

#define print_ln         LN_PRINTln
#define print_ln2        /*LN_PRINTln*/
#define printf2_FN        LN_PRINTf_FN
#define printf2_NFN        /*LN_PRINTf_NowFN*/




// #########################################
// # se non c'è ln_time.cpp mi seerver una dummy_Now()
// #########################################
const int8_t TIME_BUFFER_LENGTH = 20;
char  PROGMEM temp_buffer_time[TIME_BUFFER_LENGTH];
char *nowTimeDummy() {
    snprintf(temp_buffer_time, TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return temp_buffer_time;
}



structMillisTimer timer1MIN; structMillisTimer *timer1min = &timer1MIN;
structMillisTimer timer15MIN; structMillisTimer *timer15min = &timer15MIN;


void setup() {
    Serial.begin(115200);
    delay(2000);
    print_ln();




    // -----------------------------------
    // ------ start WiFi -----------
    // -----------------------------------
    wifi_Start();
    for (int8_t i=0; i<SSID_ELEMENTS; i++) {
        printf2_FN("%d - new_ssid: %s\n", i, mySSID[i].ssid);
    }

    // delay(10000);
    // wifi_connect(true);
    startMillisTimer(timer1min,  (uint32_t)  1*60*1000);
    startMillisTimer(timer15min, (uint32_t)  3*60*1000);
    // startMillisTimer(asyncTIMER, 15*60*1000);
}



// ##########################################################
// #
// ##########################################################
uint32_t loop_millis;
uint32_t last_loop_time;

int8_t asyncWifiScan_started=0;

void loop() {
    #define LOOP_ELAPSED 120*1000
    loop_millis = millis();

    // update timers
    updateMillisTimer(timer1min);
    updateMillisTimer(timer15min);

    // ---------------------------------
    // actions on 01 minutes timer
    // ---------------------------------
    if (timer1min->expired) {
        printf2_FN("timer 01 min expired\n");
        restartMillisTimer(timer1min);
        // ---------------------------------
        // if wifiScan has been started check for results
        // ---------------------------------
        if (asyncWifiScan_started > 0) {
            asyncWifiScan_started++; // aumentiamo
            int16_t networksFound = wifi_scanComplete();
            printf2_FN("networks found: %d\n", networksFound);
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
    // actions aon 15 minutes timer
    // ---------------------------------
    if (timer15min->expired) {
        restartMillisTimer(timer15min);
        printf2_FN("timer 15 min expired\n");
        wifi_asyncScan();
        asyncWifiScan_started=1;
    }




    if (! wifi_isConnected()) {
        printf0_FN("connection lost\n");
        wifi_reconnect();
    }

    // printf2_FN("scan elapsed: %ld\n", millis() - loop_millis);
    // delay(1000);

    // printf2_FN("loop elapsed: %ld\n", millis() - loop_millis);
    last_loop_time = millis();
    delay(1000);
}


