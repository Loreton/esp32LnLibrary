/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>
#include "WiFi.h"

// #define _LN_SSID_CPP
// #include "ln_ssid.h"






// ##########################################################
// #
// ##########################################################
String readKey(const char * nameSpace, const char * key_name) {
    Preferences preferences;
    Serial.printf("\n\nReading key: %s\n", key_name);

    preferences.begin(nameSpace, true);
    String value = preferences.getString(key_name); // prototype String getString(const char* key, String defaultValue = String());
    preferences.end();

    if (value == "") {
        Serial.printf("     No values found for %s\n", key_name);
    } else {
        Serial.printf("     value: %s for key: %s has been read from flash memory\n", value, key_name);
    }

    return value;
}


// ##########################################################
// #
// ##########################################################
void writeKeyValue(const char * nameSpace, const char * key, const char * value) {
    Preferences preferences;

    Serial.printf("\nWriting key: %s = %s\n", key, value);

    preferences.begin(nameSpace, false);
    preferences.putString(key, value);
    preferences.end();

    Serial.println("    Network Credentials Saved using Preferences");

}


// ##########################################################
// #
// ##########################################################
bool connectWiFi(String ssid, String password) {
    uint32_t notConnectedCounter = 0;
    bool fConnected = true;
    const int  DELAY = 200;
    const uint8_t  TIMEOUT = 15; // seconds

    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.printf("Connecting to SSID: %s ", ssid);

    // Wait for wifi to be connected
    while (WiFi.status() != WL_CONNECTED) {
        delay(DELAY);
        Serial.print(".");
        if ( ++notConnectedCounter > (TIMEOUT * (1000/DELAY)) ) { // Reset board if not connected after 15s
            fConnected = false;
            Serial.printf("\nerror connecting to SSID: %s\n", ssid);
            // ESP.restart();
            break;
        }
    }

    return fConnected;
}






const char * nsWiFiCredentials = "wifiCredentials";

// ##########################################################
// #
// ##########################################################
void setup() {
    Serial.begin(115200);
    Serial.println("waiting 2 seconds ..");
    delay(2000);

    // String ssid1 = preferences.getString("ssid", "");

    String ssid1     = readKey(nsWiFiCredentials, "ssid1");
    String password1 = readKey(nsWiFiCredentials, "password1");
    String ssid2     = readKey(nsWiFiCredentials, "ssid2");
    String password2 = readKey(nsWiFiCredentials, "password2");



    Serial.printf("\nNetwork Credentials Read using Preferences\n");
    Serial.printf("    ssid1......: %s\n", ssid1);
    Serial.printf("    password1..: %s\n", password1);
    Serial.printf("    ssid2......: %s\n", ssid2);
    Serial.printf("    password2..: %s\n", password2);

    if (ssid1 == "" or password1 == "") { // se falso ....
        writeKeyValue(nsWiFiCredentials, "ssid1",     WIFI_SSID1);
        writeKeyValue(nsWiFiCredentials, "password1", WIFI_PASS1);
    }

    if (ssid2 == "" or password2 == "") { // se falso ....
        writeKeyValue(nsWiFiCredentials, "ssid2",      WIFI_SSID2);
        writeKeyValue(nsWiFiCredentials, "password2",  WIFI_PASS2);
    }

    bool fConnected=false;
    fConnected = connectWiFi(ssid2, password2);
    if (!fConnected) {
        fConnected = connectWiFi(ssid1, password1);
    }

    Serial.printf("\nconnected to WIFI\n");


    if (fConnected) {
        Serial.println("scanning wifi networks");
        int n = WiFi.scanNetworks();
        Serial.println("scan done");
        if (n == 0) {
            Serial.println("no networks found");
        } else {
            Serial.printf("%d networks found\n", n);
            for (int i = 0; i < n; ++i) {
                // Print SSID and RSSI for each network found
                // Serial.printf("STATION Mode, IP address: %s, SSID: %s, RSSI: %d\n", WiFi.localIP().toString().c_str(), WiFi.SSID().c_str(), WiFi.RSSI());
                Serial.printf("     [%02d] - SSID: %-20s - RSSI: %d\n", i + 1, WiFi.SSID(i), WiFi.RSSI(i) );
                delay(10);
            }
            Serial.printf("Local IP: %s", WiFi.localIP().toString().c_str());
        }
    }

}



// ##########################################################
// #
// ##########################################################
void loop() {
  // put your main code here, to run repeatedly:
}


