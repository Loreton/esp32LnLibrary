//
// updated by ...: Loreto Notarantonio
// Date .........: 07-02-2025 17.55.00
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <WiFi.h>

#include "@ln_wifi.h"

// --- loreto others
#include "@lnMacros.h"
#include "@wifiCredentials.h"


// ---------------------------------
// macros Aliases
// ---------------------------------
// #define print           ln_print
#define print_f          LN_PRINTf
#define print_ln         LN_PRINTln
#define print_ln2        /*LN_PRINTln*/
#define print_f2        /*LN_PRINTf_FN*/

// #########################################
// # se non c'è ln_time.cpp mi seerver una dummy_Now()
// #########################################
const int8_t TIME_BUFFER_LENGTH = 20;
char  PROGMEM temp_buffer_time[TIME_BUFFER_LENGTH];
char *nowTimeDummy() {
    snprintf(temp_buffer_time, TIME_BUFFER_LENGTH, "%s", "01:02:03");
    return temp_buffer_time;
}





void setup() {
    Serial.begin(115200);
    delay(2000);
    print_ln();

    setWifiCredentials();

    for (int8_t i=0; i<SSID_ELEMENTS; i++) {
        print_f2("%d - new_ssid: %s\n", i, mySSID[i].ssid);
        print_f2("%d - new_ssid: %s\n\n", i, mySSID[i].password);
    }


    // -----------------------------------
    // ------ start WiFi -----------
    // -----------------------------------
    wifi_Start();
}



// ##########################################################
// #
// ##########################################################
void loop() {
  // put your main code here, to run repeatedly:
}


