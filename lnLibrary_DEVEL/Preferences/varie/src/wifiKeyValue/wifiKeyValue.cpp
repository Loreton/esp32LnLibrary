/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>
#include "wifiKeyValue.h"


#include "main.h"


const char * wifiNameSpace = "wifiCredentials"; // namespace

// ##########################################################
// #
// ##########################################################
String readKey(const char * nameSpace, const char * key_name) {
    Preferences preferences;
    Serial.printf("\n\nReading key: %s\n", key_name);

    preferences.begin(nameSpace, RO_MODE);
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

    preferences.begin(nameSpace, RW_MODE);
    preferences.putString(key, value);
    preferences.end();

    Serial.println("    Network Credentials Saved using Preferences");

}



void readWiFi_keyValue(bool fClear) {
    // if (fClear) {
    //     wifiNameSpace.clear();                     // delete any previous keys in this namespace
    // }


    String ssid1     = readKey(wifiNameSpace, "ssid1");
    String password1 = readKey(wifiNameSpace, "password1");
    String ssid2     = readKey(wifiNameSpace, "ssid2");
    String password2 = readKey(wifiNameSpace, "password2");


    Serial.printf("\nNetwork Credentials Read using Preferences\n");
    Serial.printf("    ssid1......: %s\n", ssid1);
    Serial.printf("    password1..: %s\n", password1);
    Serial.printf("    ssid2......: %s\n", ssid2);
    Serial.printf("    password2..: %s\n", password2);

    if (ssid1 == "" or password1 == "") { // se falso ....
        writeKeyValue(wifiNameSpace, "ssid1",     "net01");
        writeKeyValue(wifiNameSpace, "password1", "net01_pass");
    }

    if (ssid2 == "" or password2 == "") { // se falso ....
        writeKeyValue(wifiNameSpace, "ssid2",      "net02");
        writeKeyValue(wifiNameSpace, "password2",  "net02_pass");
    }


}

