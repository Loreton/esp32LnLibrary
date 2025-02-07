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


#define printf      Serial.printf
#define printf2     //Serial.printf
#define println     Serial.println

const char PROGMEM *ssid_NameSpace = "ssids_ns";
const char PROGMEM *ssid_NRs = "nr_ssids";

const char * PROGMEM base_SSIDs     [] = {"Casetta",      "cpe210",        "OpenWrt",      "LoretoHotSpot", "Loreto.net",   "WebPocket-4545"};
const char * PROGMEM base_PASSWORDs [] = {"CasettaPassw", "cpe210Passw", "OpenWrtPassw", "LoretohsPassw",      "LoretonetPassw",    "WebSockPassw"};
uint8_t SSID_NUMBERS = sizeof(base_SSIDs) / sizeof(base_SSIDs[0]); // calcolo del numero di elementi


// #define MAX_SSID_CHARS          20
// typedef struct  {
//     char  PROGMEM ssid[MAX_SSID_CHARS+1];
//     char  PROGMEM passw[MAX_SSID_CHARS+1];
// } ssid_t;


#define MAX_SSID_LEN 20
typedef struct {
    char PROGMEM ssid[MAX_SSID_LEN+1];
    char PROGMEM password[MAX_SSID_LEN+1];
} ssid_t;

#define MAX_SSIDS 10
ssid_t mySSID[MAX_SSIDS];
ssid_t *ssidPtr;


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
    const size_t SSID_NUMBERS = sizeof(base_SSIDs) / sizeof(base_SSIDs[0]); // calcolo del numero di elementi

    // numero degli SSID
    ns.putChar(ssid_NRs, SSID_NUMBERS);

    // buffer per numero sequenziale dell'indice
    char buf[4];
    for (int i = 0; i < SSID_NUMBERS; ++i) {
        sprintf(buf, "%02x", i);
        // write index
        printf("writing: %s, String = %s\n", buf, base_SSIDs[i]);
        ns.putString(buf, base_SSIDs[i]);
        printf("writing: %s, String = %s\n", base_SSIDs[i], base_PASSWORDs[i]);
        // write ssid-psw
        ns.putString(base_SSIDs[i], base_PASSWORDs[i]);
     }
}


// ssid into char
void readSSIDs_struct_char(Preferences ns) {
    // retrieve del numero di SSIds
    SSID_NUMBERS = ns.getChar(ssid_NRs);

    if (SSID_NUMBERS>MAX_SSIDS) {SSID_NUMBERS = MAX_SSIDS;}

    ssid_t *ptr;
    // buffer per numero sequenziale dell'indice
    char buf[4];
    for (int i = 0; i < SSID_NUMBERS; ++i) {
        // prepara l'indice per leggere l'ssid index
        ptr = &mySSID[i];
        sprintf(buf, "%02x", i);
        size_t len = ns.getString(buf, ptr->ssid, MAX_SSID_LEN);
        printf2("read: %s, value = %s (len: %d)\n", buf, ptr->ssid, len);

        len = ns.getString(buf, ptr->password, MAX_SSID_LEN);
        printf2("read: %s, value = %s (len: %d)\n", buf, ptr->password, len);
        println();
     }
}



void setSssidPreferences() {
    Preferences ssidPrefs;
    ssidPrefs.begin(ssid_NameSpace, RO_MODE);           // Open our namespace (or create it

    //  if it doesn't exist) in RO mode.
    bool isSsidList = ssidPrefs.isKey(ssid_NRs);     // Test for the existence
    if (isSsidList == false) {
        ssidPrefs.end();                             // close the namespace in RO mode and...
        ssidPrefs.begin(ssid_NameSpace, RW_MODE);        //  reopen it in RW mode.
        writeSSIDs(ssidPrefs);
        ssidPrefs.end();                             // Close the namespace in RW mode and...
        ssidPrefs.begin(ssid_NameSpace, RO_MODE);        //  reopen it in RO mode so the setup code

    }

    readSSIDs_struct_char(ssidPrefs);
    ssidPrefs.end();                                      // Close our preferences namespace.
}



void setup() {
    Serial.begin(115200);
    delay(2000);
    println();

    setSssidPreferences();
    for (int8_t i=0; i<SSID_NUMBERS; i++) {
        printf("%d - new_ssid: %s\n", i, mySSID[i].ssid);
        printf("%d - new_ssid: %s\n", i, mySSID[i].password);
        println();
    }
}



// ##########################################################
// #
// ##########################################################
void loop() {
  // put your main code here, to run repeatedly:
}


