//
// updated by ...: Loreto Notarantonio
// Date .........: 07-02-2025 18.36.00
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include "Arduino.h" // in testa anche per le definizioni dei type
#include <WiFi.h>
// #include <ArduinoJson.h>


// --- loreto
#include "@ln_wifi.h"

// --- others
#include "@lnMacros.h"






// ---------------------------------
// macros Aliases
// ---------------------------------
#define print_f          LN_PRINTf
#define print_ln         LN_PRINTln
#define print_f2         LN_PRINTf

// ssidInList
#define print_ssidInList         /*LN_PRINTf_LN*/

// processScannedNetworks
#define printf_psn_FN         LN_PRINTf_FN
#define printf_psn2_FN         LN_PRINTf_FN

#define print_f5         LN_PRINTf
#define print_f6         LN_PRINTf

#define print_f_NFN      LN_PRINTf_NowFN

// printNetwork
#define printf_pn_FN       LN_PRINTf_FN
#define printf_pn2       LN_PRINTf



// ---------------------------------
// module variables
// ---------------------------------
#define SCAN_DELAY 60*1000
unsigned long last_wifi_scan=0;
structNET bestSSID = {false, "", "", NULL, "", 0, -127, 0};
structNET *bestssid = &bestSSID;


//##############################################################
//#
//##############################################################
void printNetwork(int i, int16_t networksFound) {
    static int counter = 0;

    if (counter == 0) {
        counter++;
        printf_pn_FN("nScan done %d networks found\n", networksFound);
        printf_pn2("\tNr | SSID                             | RSSI | CH | BSSID              | Encryption\n");
    }

    printf_pn2("\t%2d", i+1);
    printf_pn2(" | %-32.32s", WiFi.SSID(i).c_str());
    printf_pn2(" | %4ld", WiFi.RSSI(i));
    printf_pn2(" | %2ld", WiFi.channel(i));
    printf_pn2(" | %s", WiFi.BSSIDstr(i).c_str());
    printf_pn2(" | ");

    switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:            printf_pn2("open"); break;
        case WIFI_AUTH_WEP:             printf_pn2("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         printf_pn2("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        printf_pn2("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    printf_pn2("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: printf_pn2("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        printf_pn2("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   printf_pn2("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        printf_pn2("WAPI"); break;
        default:                        printf_pn2("unknown");
    }
    printf_pn2("\n"); //chiude riga
}




// #########################################
// check if ssid is in the configured list
// #########################################
int8_t ssidInList(const char *ssid, ssid_t *ptr) {
    print_ssidInList("\nsearching for %s in list [SSID_ELEMENTS=%d]...\n", ssid, SSID_ELEMENTS);
    for (int i = 0; i < SSID_ELEMENTS; i++, ptr++) {
        print_ssidInList("\tcomparing [%s] with [%s] - ", ssid, ptr->ssid);
        if (strcmp(ssid, ptr->ssid) == 0) {
            print_ssidInList("Matched\n\n");
            return i;
        } else {
            print_ssidInList("Not Matched\n");
        }
    }
    print_ssidInList("...NOT FOUND\n");
    return -1;
}










// #########################################
// #
// #########################################
void wifi_disconnect(void) {
    if (WiFi.isConnected()) {

        print_f_NFN("rying Disconnecting from SSID: %s\n", WiFi.SSID().c_str());
        if (WiFi.disconnect()) {
            print_f_NFN("WIFI] Disconnected\n");
            delay(100);
        } else {
            print_f_NFN("rror Disconnecting\n");
        }
    }
}



// #########################################
// #
// #########################################
int16_t wifi_scanComplete(void) {
    return WiFi.scanComplete();
}

// #########################################
// #
// #########################################
bool wifi_isConnected() {
    return WiFi.isConnected();
}


//##############################################################
//#
//##############################################################
bool processScannedNetworks(int16_t networksFound) {
static unsigned long wifi_delay=0;

    // if (millis() - wifi_delay < SCAN_DELAY) { // non pù di uno al minuto
    //     return false;
    // }

    print_f_NFN("%d networks found\n", networksFound);
    bestssid->updated = false;
    switch (networksFound) {

        case 0:
            printf_psn_FN("WiFi Scan no networks found - status %d\n", networksFound);
            break;

        // (networksFound == WIFI_SCAN_FAILED)  //  WIFI_SCAN_FAILED = -2)
        case WIFI_SCAN_RUNNING:
            printf_psn_FN("WiFi Scan is still running %d\n", networksFound);
            break;

        case WIFI_SCAN_FAILED:
            printf_psn_FN("WiFi Scan failed %d\n", networksFound);
            break;

        default:
            printf_psn_FN("WiFi Scan found %d networks\n", networksFound);
            for (int i = 0; i < networksFound; ++i) {
                printNetwork(i, networksFound);

                int8_t ssid_index = ssidInList(WiFi.SSID(i).c_str(), &mySSID[0]);

                // print_f_NFN("hecking - [%d . %s]", ssid_index, WiFi.SSID(i).c_str());
                // printf_psn2_FN(" - checking for %s ...", WiFi.SSID(i).c_str());
                if ( (ssid_index >= 0) && (WiFi.RSSI(i) > bestssid->rssi) ) {
                    // print_f_NFN("- [FOUND %d . %s]\n", ssid_index, WiFi.SSID(i).c_str());
                    // printf_psn2_FN(" FOUND]\n");
                    bestssid->ssid           = mySSID[ssid_index].ssid;
                    bestssid->password       = mySSID[ssid_index].password;
                    bestssid->channel        = WiFi.channel(i);
                    bestssid->bssid          = WiFi.BSSID(i);
                    bestssid->rssi           = WiFi.RSSI(i);
                    bestssid->bssidStr       = WiFi.BSSIDstr(i).c_str(); // credo che si perda in seguito
                    bestssid->diff_rssi      = WiFi.RSSI() - WiFi.RSSI(i);
                    bestssid->updated        = true;
                } /*else {
                    printf_psn2_FN(" NOT FOUND]\n");
                } */
            }
            if (!WiFi.isConnected()) {
                bestssid->updated = true;
            }
            else if (bestssid->diff_rssi <= -10) { // new RSSI better than 10 points than current
                printf_psn_FN("New RSSI %d is better than current one: %d\n", bestssid->rssi, WiFi.RSSI());
                bestssid->updated = true;
            } else {
                printf_psn_FN("New RSSI %d is not enough better than current one: %d (NO reconnection will take)\n", bestssid->rssi, WiFi.RSSI());
            }

    }
    wifi_delay = millis();
    return bestssid->updated;
}





// #########################################
// #
// #########################################
void printConnection(bool print) {
    if (print) {
        print_f_NFN("connected to: %s\n", WiFi.SSID());
        print_f6("\tSSID:    %2s\n",  WiFi.SSID());
        print_f6("\tRSSI:    %4ld\n", WiFi.RSSI());
        print_f6("\tCHANNEL: %2ld\n", WiFi.channel());
        print_f6("\tBSSID:   %s\n",   WiFi.BSSIDstr().c_str());
        print_f6("\tIP:      %s\n",   WiFi.localIP().toString().c_str());
        print_f6("\n");
    }
}


// #########################################
// # ref:
// #    http://community.heltec.cn/t/solved-wifi-reconnect/1396/3
// #    https://stackoverflow.com/questions/70415075/esp32-select-one-from-multiple-wifi-ap-with-same-name-ssid
// #########################################
void wifi_connect(void) {

    bestssid->updated = false;
    if (wifi_isConnected()) {
        WiFi.disconnect();
        print_f_NFN("e-start connection\n");
    } else {
        print_f_NFN("start connection\n");
    }

    WiFi.mode(WIFI_STA); // Set WIFI module to STA mode


    print_f_NFN("connecting to SSID: %s  on channel: %d\n", bestssid->ssid, bestssid->channel);
    WiFi.begin(bestssid->ssid, bestssid->password, bestssid->channel, bestssid->bssid);
    // if (WiFi.setAutoReconnect(true)) {}

    // Wait for wifi to be connected
    uint8_t notConnectedCounter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        print_ln("");
        if(++notConnectedCounter > 150) { // Reset board if not connected after 5s
            print_f_NFN("RROR connecting to Wifi...\n");
            // ESP.restart();
            break;
        }
    }

    if (WiFi.isConnected()) {
        printConnection(true);
    }
}



//##############################################################
//# ref:
//#     https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiScanAsync/WiFiScanAsync.ino
//##############################################################
void wifi_asyncScan() {
    if (millis() - last_wifi_scan < SCAN_DELAY) { // non pù di uno al minuto
        return;
    }

    print_f_NFN("sync_Scan start\n");
    WiFi.scanNetworks(true);  // 'true' turns Async Mode ON
    last_wifi_scan=millis();

}


//##############################################################
//# ref: https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiScanAsync/WiFiScanAsync.ino
//##############################################################
void wifi_Start() {
    if (last_wifi_scan != 0) {
        if (millis() - last_wifi_scan < SCAN_DELAY) { // non pù di uno scan al minuto
            return;
        }
    }


    print_f_NFN("syncronous_Scan start\n");
    int16_t networksFound = WiFi.scanNetworks(false);  // 'false' turns Async Mode OFF
    if (processScannedNetworks(networksFound) ) { // se sono state trovate delle network
        wifi_connect();
    }
    last_wifi_scan=millis();
}
