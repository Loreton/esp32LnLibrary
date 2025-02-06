/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>

bool RO_MODE = true;
bool RW_MODE = false;


#define printf Serial.printf
#define println Serial.println

const char PROGMEM *ssid_NS = "my-ssids";
const char PROGMEM *ssidList_key = "ssids_list";

const char * PROGMEM mySSIDs [] = {"Casetta",      "cpe210",        "OpenWrt",      "LoretoHotSpot", "Loreto.net",   "WebPocket-4545"};
const char * PROGMEM myPASSs [] = {"CasettaPassw", "cpe210Passw", "OpenWrtPassw", "LoretohsPassw",      "LoretonetPassw",    "WebSockPassw"};




void setSssidPreferences() {
    Preferences ssidPrefs;
    ssidPrefs.begin(ssid_NS, RO_MODE);           // Open our namespace (or create it
                                                   //  if it doesn't exist) in RO mode.
    bool isSsidList = ssidPrefs.isKey(ssidList_key);     // Test for the existence
                                                   // of the "already initialized" key.

    if (isSsidList == false) {
       // If isSsidList is 'false', the key "nvsInit" does not yet exist therefore this
       //  must be our first-time run. We need to set up our Preferences namespace keys. So...
       ssidPrefs.end();                             // close the namespace in RO mode and...
       ssidPrefs.begin(ssid_NS, RW_MODE);        //  reopen it in RW mode.


       // non potendo capire il sizeof di un char * converto tutto in string
       const size_t nr_strings = sizeof(mySSIDs) / sizeof(mySSIDs[0]); // calcolo del numero di elementi
       String ss;
       for (int i = 0; i < nr_strings; ++i) {
           ss += mySSIDs[i];
           ss += " ";
       }
       printf("len: %d, String = %s\n", ss.length(), ss.c_str());
       ssidPrefs.putString(ssidList_key, ss);
       ssidPrefs.end();                             // Close the namespace in RW mode and...

       ssidPrefs.begin(ssid_NS, RO_MODE);        //  reopen it in RO mode so the setup code
    }

    // Retrieve the operational parameters from the namespace
    //  and save them into their run-time variables.
    String data = ssidPrefs.getString(ssidList_key);
    const char *cdata = ssidPrefs.getString(ssidList_key);
    ssidPrefs.end();                                      // Close our preferences namespace.


    printf("ssid list keys: %s", data.c_str());

}



void setup() {
    Serial.begin(115200);
    delay(2000);
    println();

    setSssidPreferences();
}



// ##########################################################
// #
// ##########################################################
void loop() {
  // put your main code here, to run repeatedly:
}


