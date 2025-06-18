//
// updated by ...: Loreto Notarantonio
// Date .........: 28-05-2025 09.33.41
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include "Arduino.h" // in testa anche per le definizioni dei type
#include <WiFi.h>



// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
#include "@ln_wifi.h"
#include "@wifiCredentials.h"



// ---------------------------------
// module variables
// ---------------------------------
#define SCAN_DELAY 60*1000
#define WIFI_ASYNC_SCAN true


// structNET bestSSID = {false, "", "", NULL, "", 0, -127, 0};
structNET bestSSID;
structNET *bestssid = &bestSSID;
int16_t networksFound = 0;




// #########################################
// # not used... comodo dall'esterno....
// #########################################
// inline int16_t wifi_scanComplete(void) {
int16_t wifi_scanComplete(void) {
    return WiFi.scanComplete();
}

// #########################################
// # not used... comodo dall'esterno....
// #########################################
// inline bool wifi_isConnected() {
bool wifi_isConnected() {
    if ( (WiFi.status() != WL_CONNECTED) || (! WiFi.isConnected()) ) {
        return false;
    }
    return true;
}



//##############################################################
//# ref: https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiScanAsync/WiFiScanAsync.ino
//##############################################################
// inline void wifi_scanDelete() {
void wifi_scanDelete() {
    WiFi.scanDelete();
}

// inline void wifi_asyncScan() {
void wifi_asyncScan() {
    printf0_NFN("async_Scan started\n");
    WiFi.scanNetworks(true);  // 'true' turns Async Mode ON
    // return true;
}


//##############################################################
//#
//##############################################################
void displayNetwork(int i, int16_t networksFound) {
    static int counter = 0;

    if (counter == 0) {
        counter++;
        printf3("\tNr | SSID                             | RSSI | CH | BSSID              | Encryption\n");
    }

    printf3("\t%2d", i+1);
    printf3(" | %-32.32s", WiFi.SSID(i).c_str());
    printf3(" | %4ld", WiFi.RSSI(i));
    printf3(" | %2ld", WiFi.channel(i));
    printf3(" | %s", WiFi.BSSIDstr(i).c_str());
    printf3(" | ");

    switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:            printf3("open"); break;
        case WIFI_AUTH_WEP:             printf3("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         printf3("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        printf3("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    printf3("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: printf3("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        printf3("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   printf3("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        printf3("WAPI"); break;
        default:                        printf3("unknown");
    }
    printf3("\n"); //chiude riga
}















// #########################################
// #
// #########################################
void printConnection() {
    printf0_NFN("\tSSID:    %2s\n",  WiFi.SSID());
    printf0_NFN("\tRSSI:    %4ld\n", WiFi.RSSI());
    printf0_NFN("\tCHANNEL: %2ld\n", WiFi.channel());
    printf0_NFN("\tBSSID:   %s\n",   WiFi.BSSIDstr().c_str());
    printf0_NFN("\tIP:      %s\n",   WiFi.localIP().toString().c_str());
    printf0_NFN("\n");
}






// #########################################
// # not used... comodo dall'esterno....
// #########################################
void wifi_disconnect(bool force) {
    printf0_NFN("Disconnecting wifi %s\n", WiFi.SSID());
    bestssid->is_better = false; // remove flag
    bestssid->rssi=-127; //forziamo un valore basso per permettere l'analisi deele reti
    WiFi.scanDelete();
    if (WiFi.disconnect()) {
        printf2_NFN("[WIFI] Disconnected\n");
        delay(100);
    } else {
        printf2_NFN("Error Disconnecting\n");
    }

}



// #########################################
// # ref:
// #    https://stackoverflow.com/questions/70415075/esp32-select-one-from-multiple-wifi-ap-with-same-name-ssid
// #########################################
void wifi_connect() {
    bestssid->is_better = false; // reset flag

    WiFi.mode(WIFI_STA); // Set WIFI module to STA mode

    printf0_NFN("connecting to SSID: %s  on channel: %d\n", bestssid->ssid, bestssid->channel);
    WiFi.begin(bestssid->ssid, bestssid->password, bestssid->channel, bestssid->bssid);
    // if (WiFi.setAutoReconnect(true)) {}

    // Wait for wifi to be connected
    uint8_t notConnectedCounter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        if(++notConnectedCounter > 150) { // Reset board if not connected after 5s
            printf0_NFN("ERROR connecting to Wifi...\n");
            // delay(10000);
            // ESP.restart();
            break;
        }
    }

    if (WiFi.isConnected()) {
        printf0_NFN("connected to: %s\n", WiFi.SSID());
        printConnection();
    }

}



// #########################################
// # ref:
// #    http://commun ity.heltec.cn/t/solved-wifi-reconnect/1396/3
// #    https://stackoverflow.com/questions/70415075/esp32-select-one-from-multiple-wifi-ap-with-same-name-ssid
// #########################################
void wifi_reconnect() {
    wifi_disconnect(true);

    printf0_NFN("syncronous_Scan start\n");
    // int16_t networksFound = WiFi.scanNetworks(false);  // 'false' turns Async Mode OFF
    connectOnScanResult(); // verifica le  network
}





//##############################################################
// check if ssid is in the configured list
//##############################################################
int8_t isInMyList(const char *ssid, ssid_t *ptr)  {
    for (int i = 0; i < SSID_ELEMENTS; i++, ptr++) {
        if (strcmp(ssid, ptr->ssid) == 0) {
            return i; // matched
        }
    }
    return -1;
}


//##############################################################
// check rssi difference if new>curr+5
//##############################################################
bool isNewRssiBetter(int8_t curr_rssi, int8_t best_rssi)  {
    int8_t thr_level=curr_rssi+5;
    if (best_rssi > thr_level ) {
        return true;  //printf("best_rssi: %d is better than curr_rssi %d", best_rssi, thr_level);
    }
    return false;
}


//##############################################################
// verifica che nelle net trovate ci sia qualcuna predefinita nella configurazione
//##############################################################
bool checkForBestSSID(int16_t networksFound) {
    bestssid->is_better = false;
    int8_t curr_rssi = WiFi.RSSI(); // set current RSSI
    bestssid->rssi = -127; // set current RSSI

    for (int i = 0; i < networksFound; ++i) {
        // displayNetwork(i, networksFound);

        int8_t ssid_index = isInMyList(WiFi.SSID(i).c_str(), &mySSID[0]);
        printf1_FN("[%d] %-15s - RSSI: %d (mylist_inx: %d)\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i), ssid_index);

        // - cerchiamo SSID con il miglior RSSI (valori negativi fino a -100)
        if ( (ssid_index >= 0) && (WiFi.RSSI(i) > bestssid->rssi) ) {
            bestssid->ssid           = mySSID[ssid_index].ssid;
            bestssid->password       = mySSID[ssid_index].password;
            bestssid->channel        = WiFi.channel(i);
            bestssid->bssid          = WiFi.BSSID(i);
            bestssid->rssi           = WiFi.RSSI(i);
            bestssid->bssidStr       = WiFi.BSSIDstr(i).c_str(); // credo che si perda in seguito
            bestssid->rssi_diff      = curr_rssi - bestssid->rssi;
        }
    }

    bestssid->is_better = isNewRssiBetter(curr_rssi, bestssid->rssi);


    if (!WiFi.isConnected()) {
        bestssid->is_better = true; // necessary to connect
    }

    return bestssid->is_better;
}



//##############################################################
//#
//##############################################################
bool connectOnScanResult(int16_t networksFound) {
    if (networksFound < 0)
        networksFound = WiFi.scanNetworks(false);  // 'false' turns Async Mode OFF


    bool scanCompleted=true;
    switch (networksFound) {

        case 0:
            printf1_FN("WiFi Scan no networks found - status %d\n", networksFound);
            break;

        case WIFI_SCAN_RUNNING: // WIFI_SCAN_RUNNING   (-1)
            printf1_FN("WiFi Scan is still running %d\n", networksFound);
            scanCompleted=false;
            break;

        case WIFI_SCAN_FAILED:  // WIFI_SCAN_FAILED    (-2)
            printf1_FN("WiFi Scan failed %d\n", networksFound);
            break;

        default:
            int8_t curr_rssi = WiFi.RSSI(); // set current RSSI
            printf1_FN("checking for best SSID (networks found: %d)\n", networksFound);
            bool is_better = checkForBestSSID(networksFound); // verifica le  network

            if (!WiFi.isConnected()) {
                wifi_connect(); // connetti se trovata rete valida

            } else if (is_better) { // new RSSI better than 10 points than current
                printf0_FN("new_SSID: %s.RSSI: %d > cur_SSID %s (RSSI %d) (diff: %d) start reconnection\n", bestssid->ssid,
                                                                                                            bestssid->rssi,
                                                                                                            WiFi.SSID().c_str(),
                                                                                                            curr_rssi,
                                                                                                            bestssid->rssi_diff);
                wifi_connect(); // connetti se trovata rete valida

            } else {
                printf1_FN("new_SSID: %s.RSSI: %d <= cur_SSID %s (RSSI %d) (diff: %d) NO reconnection\n", bestssid->ssid,
                                                                                                            bestssid->rssi,
                                                                                                            WiFi.SSID().c_str(),
                                                                                                            curr_rssi,
                                                                                                            bestssid->rssi_diff);
            }


    }
    if (scanCompleted)
        WiFi.scanDelete();
    return scanCompleted; // per capire se ci sono modifiche
}






//##############################################################
//# ref: https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiScanAsync/WiFiScanAsync.ino
//##############################################################
void wifi_Start() {

    #ifdef LN_USE_LITTLEFS
        #include "fileSystem.h"
        // ---- open "fileSystem.cpp"
        if (! openLittleFS() ) {return; }

        // ---- read ssid json file "loadSsid.cpp"
        SSID_ARRAY_COUNT = load_ssidArray(LittleFS, "/ssids.json");
        printf0_NFN("loaded  %d ssid network definitions\n", SSID_ARRAY_COUNT);
        if (SSID_ARRAY_COUNT < 0) {return;}
    #else
        setWifiCredentials();
        #endif
    #if 0
    #endif


    printf0_NFN("syncronous_Scan start\n");
    // int16_t networksFound = WiFi.scanNetworks(false);  // 'false' turns Async Mode OFF
    connectOnScanResult();
}
