//
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 09.22.08
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include "Arduino.h" // in testa anche per le definizioni dei type
// #include <ArduinoJson.h>

#include <WiFi.h>
#include "@ln_wifi.h"
#include "@loadSsid.h"

// --- others
#include "@lnMacros.h"
// #include "@ln_time.h"
// #include "@fileSystem.h"



#define SCAN_DELAY 60*1000
unsigned long last_wifi_scan=0;


// Aliases

// --- essentials
#define wifi_dbg00                 ln_printf
#define wifi_dbg0                  ln_printf_NowFN
#define wifi_dbg1       //            ln_printf


// ---- printNetwork
#define prtnet_dbg1       //            ln_printf
#define ssidinlist_dbg1       //            ln_printf

// ---- processScannedNetworks
#define scan_dbg0               ln_printf
#define scan_dbg1     //           ln_printfFN

// ---- printConnection
#define prtconn_dbg1    //               ln_printf







//##############################################################
//#
//##############################################################
void printNetwork(int i, int16_t networksFound) {
    static int counter = 0;

    if (counter == 0) {
        counter++;
        wifi_dbg0("\nScan done %d networks found\n", networksFound);
        prtnet_dbg1("Nr | SSID                             | RSSI | CH | BSSID                   | Encryption\n");
    }

    prtnet_dbg1("%2d", i+1);
    prtnet_dbg1(" | %-32.32s", WiFi.SSID(i).c_str());
    prtnet_dbg1(" | %4ld", WiFi.RSSI(i));
    prtnet_dbg1(" | %2ld", WiFi.channel(i));
    prtnet_dbg1(" | %s", WiFi.BSSIDstr(i).c_str());
    prtnet_dbg1(" | ");

    switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:            prtnet_dbg1("open"); break;
        case WIFI_AUTH_WEP:             prtnet_dbg1("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         prtnet_dbg1("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        prtnet_dbg1("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    prtnet_dbg1("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: prtnet_dbg1("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        prtnet_dbg1("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   prtnet_dbg1("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        prtnet_dbg1("WAPI"); break;
        default:                        prtnet_dbg1("unknown");
    }
    // lnprintf0("\n");
}




// #########################################
// check if ssid is in the configured list
// #########################################
int ssidInList(const char *ssid, ssid_t *ptr) {
    ssidinlist_dbg1("searching for %s in list [SSID_ARRAY_COUNT=%d]...\n", ssid, SSID_ARRAY_COUNT);
    for (int i = 0; i < SSID_ARRAY_COUNT; i++, ptr++) {
        ssidinlist_dbg1("\tcomparing [%s] with [%s] - ", ssid, ptr->ssid);
        if (strcmp(ssid, ptr->ssid) == 0) {
            ssidinlist_dbg1("Matched\n");
            return i;
        } else {
            ssidinlist_dbg1("Not Matched\n");
        }
    }
    ssidinlist_dbg1("...NOT FOUND\n");
    return -1;
}




//##############################################################
//# ref:
//#     https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiScanAsync/WiFiScanAsync.ino
//##############################################################
void wifi_asyncScan() {
    if (millis() - last_wifi_scan < SCAN_DELAY) { // non pù di uno al minuto
        return;
    }

    wifi_dbg0("Async_Scan start\n");
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


    wifi_dbg0("Syncronous_Scan start\n");
    int16_t networksFound = WiFi.scanNetworks(false);  // 'false' turns Async Mode OFF
    if (processScannedNetworks(networksFound) ) { // se sono state trovate delle network
        wifi_connect();
    }
    last_wifi_scan=millis();
}





// #########################################
// #
// #########################################
void wifi_disconnect(void) {
    if (WiFi.isConnected()) {

        wifi_dbg0("trying Disconnecting from SSID: %s\n", WiFi.SSID().c_str());
        if (WiFi.disconnect()) {
            wifi_dbg0("[WIFI] Disconnected\n");
            delay(100);
        } else {
            wifi_dbg0("Error Disconnecting\n");
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

    // lnprintf("%d networks found\n", networksFound);
    // lnprintf("[%s] - %d networks found\n", Now(), networksFound);
    bestssid->updated = false;
    switch (networksFound) {

        case 0:
            scan_dbg0("WiFi Scan no networks found - status %d\n", networksFound);

        // (networksFound == WIFI_SCAN_FAILED)  //  WIFI_SCAN_FAILED = -2)
        case WIFI_SCAN_RUNNING:
            scan_dbg0("WiFi Scan is still running %d\n", networksFound);

        case WIFI_SCAN_FAILED:
            scan_dbg0("WiFi Scan failed %d\n", networksFound);

        default:
            scan_dbg0("WiFi Scan found %d networks\n", networksFound);
            const char * this_ssid;
            for (int i = 0; i < networksFound; ++i) {
                printNetwork(i, networksFound);

                this_ssid = WiFi.SSID(i).c_str();
                int8_t ssid_index = ssidInList(WiFi.SSID(i).c_str(), &mySSID[0]);

                // wifi_dbg0("checking - [%d . %s]", ssid_index, WiFi.SSID(i).c_str());
                scan_dbg0(" - checking for %s ...", this_ssid);
                if ( (ssid_index >= 0) && (WiFi.RSSI(i) > bestssid->rssi) ) {
                    // wifi_dbg0(" - [FOUND %d . %s]\n", ssid_index, WiFi.SSID(i).c_str());
                    scan_dbg0(" FOUND]\n");
                    bestssid->ssid           = mySSID[ssid_index].ssid;
                    bestssid->password       = mySSID[ssid_index].passw;
                    bestssid->channel        = WiFi.channel(i);
                    bestssid->bssid          = WiFi.BSSID(i);
                    bestssid->rssi           = WiFi.RSSI(i);
                    bestssid->bssidStr       = WiFi.BSSIDstr(i).c_str(); // credo che si perda in seguito
                    bestssid->diff_rssi      = WiFi.RSSI() - WiFi.RSSI(i);
                    bestssid->updated        = true;
                } else {
                    scan_dbg0(" NOT FOUND]\n");
                }

            }
            if (!WiFi.isConnected()) {
                bestssid->updated = true;
            }
            else if (bestssid->diff_rssi <= -10) { // new RSSI better than 10 points than current
                scan_dbg1("New RSSI %d is better than current one: %d\n", bestssid->rssi, WiFi.RSSI());
                bestssid->updated = true;
            } else {
                scan_dbg1("New RSSI %d is not enough better than current one: %d (NO reconnection will take)\n", bestssid->rssi, WiFi.RSSI());
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
        wifi_dbg0("Connected to: %s\n", WiFi.SSID());
        prtconn_dbg1("\tSSID:    %2s\n",  WiFi.SSID());
        prtconn_dbg1("\tRSSI:    %4ld\n", WiFi.RSSI());
        prtconn_dbg1("\tCHANNEL: %2ld\n", WiFi.channel());
        prtconn_dbg1("\tBSSID:   %s\n",   WiFi.BSSIDstr().c_str());
        prtconn_dbg1("\tIP:      %s\n",   WiFi.localIP().toString().c_str());
        prtconn_dbg1("\n");
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
        wifi_dbg0("re-start connection\n");
    } else {
        wifi_dbg0("start connection\n");
    }

    WiFi.mode(WIFI_STA); // Set WIFI module to STA mode


    wifi_dbg0("Connecting to SSID: %s  on channel: %d\n", bestssid->ssid, bestssid->channel);
    WiFi.begin(bestssid->ssid, bestssid->password, bestssid->channel, bestssid->bssid);
    // if (WiFi.setAutoReconnect(true)) {}

    // Wait for wifi to be connected
    uint8_t notConnectedCounter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        wifi_dbg00(".");
        if(++notConnectedCounter > 150) { // Reset board if not connected after 5s
            wifi_dbg0("ERROR connecting to Wifi...\n");
            // ESP.restart();
            break;
        }
    }

    wifi_dbg00("\n");
    if (WiFi.isConnected()) {
        printConnection(true);
    }
}

