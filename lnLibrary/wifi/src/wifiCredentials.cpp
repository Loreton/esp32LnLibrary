/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include <Preferences.h>

// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
#include "@ln_wifi.h"
#include "@ssidsTypeChar.h" // contiene le password degli SSIDs /home/loreto/lnProfile/envars/include/@ssidsTypeChar.h





bool RO_MODE = true;
bool RW_MODE = false;


const char PROGMEM *ssid_NameSpace = "ssids_ns";
const char PROGMEM *ssid_NRs = "nr_ssids";


// #define MAX_SSIDS 10
ssid_t mySSID[MAX_SSIDS]; // MAX_SSIDS defined in @ssidsTypeChars.h





/* ----------------------------------
    Siccome non è possibile avere una lista delle keyValue nel namespace
    creiamo una key_index_list per sopperire
    nr_ssids -> numero degli SSIDs
    01 -> ssid1
    02 -> ssid2
    ...
    che ci aiuterà per fare il retrieve della password
    ... e poi:
    ssid1 -> password1
    ...
    convivono nello stesso namespace ma non credo sia un problema
---------------------------------------*/

void writeSSIDs(Preferences ns) {
    // non potendo avere il sizeof di un char * calcolo il numero di elementi
    // size_t n_elements = sizeof(base_SSIDs) / sizeof(base_SSIDs[0]); // calcolo del numero di elementi

    // numero degli SSID
    ns.putChar(ssid_NRs, SSID_ELEMENTS);

    // buffer per numero sequenziale dell'indice
    char buf[4];
    for (int i = 0; i < SSID_ELEMENTS; ++i) {
        sprintf(buf, "%02x", i);
        // write index
        printf1_NFN("writing: %s, String = %s\n", buf, base_SSIDs[i]);
        ns.putString(buf, base_SSIDs[i]);
        printf1_NFN("writing: %s, String = %s\n", base_SSIDs[i], base_PASSWORDs[i]);
        // write ssid-psw
        ns.putString(base_SSIDs[i], base_PASSWORDs[i]);
     }
}


// ssid into char
void readSSIDs_struct_char(Preferences ns) {
    // retrieve del numero di SSIds
    SSID_ELEMENTS = ns.getChar(ssid_NRs);

    if (SSID_ELEMENTS>MAX_SSIDS) {SSID_ELEMENTS = MAX_SSIDS;}

    ssid_t *ptr;
    // buffer per numero sequenziale dell'indice
    char buf[4];
    for (int i = 0; i < SSID_ELEMENTS; ++i) {
        // prepara l'indice per leggere l'ssid index
        ptr = &mySSID[i];
        sprintf(buf, "%02x", i);
        size_t len = ns.getString(buf, ptr->ssid, LN_MAX_SSID_LEN);
        printf1_NFN("read: %s, value = %s (len: %d)\n", buf, ptr->ssid, len);

        len = ns.getString(ptr->ssid, ptr->password, LN_MAX_PASSPHRASE_LEN);
        printf1_NFN("read: %s, value = %s (len: %d)\n", buf, ptr->password, len);
        printf1_NFN("\n");
     }
}



void setWifiCredentials(bool force_write) {
    Preferences ssidPrefs;
    ssidPrefs.begin(ssid_NameSpace, RO_MODE);           // Open our namespace (or create it

    //  if it doesn't exist) in RO mode.
    bool isSsidList = ssidPrefs.isKey(ssid_NRs);     // Test for the existence
    if (isSsidList == false || force_write) {
        ssidPrefs.end();                             // close the namespace in RO mode and...
        ssidPrefs.begin(ssid_NameSpace, RW_MODE);        //  reopen it in RW mode.

        writeSSIDs(ssidPrefs);
        ssidPrefs.end();                             // Close the namespace in RW mode and...
        ssidPrefs.begin(ssid_NameSpace, RO_MODE);        //  reopen it in RO mode so the setup code

    }

    readSSIDs_struct_char(ssidPrefs);
    ssidPrefs.end();                                      // Close our preferences namespace.
}


