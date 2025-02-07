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

const char PROGMEM *ssid_NameSpace = "ssids_ns";
// const char PROGMEM *ssidList_key = "ssids_list";
// const char PROGMEM *ssid_keyIndex = "ssids_list";
const char PROGMEM *ssid_NRs = "nr_ssids";

const char * PROGMEM base_SSIDs     [] = {"Casetta",      "cpe210",        "OpenWrt",      "LoretoHotSpot", "Loreto.net",   "WebPocket-4545"};
const char * PROGMEM base_PASSWORDs [] = {"CasettaPassw", "cpe210Passw", "OpenWrtPassw", "LoretohsPassw",      "LoretonetPassw",    "WebSockPassw"};
uint8_t SSID_NUMBERS = sizeof(base_SSIDs) / sizeof(base_SSIDs[0]); // calcolo del numero di elementi

char  *cSSID[20];
String sSSID[20];

// typedef struct {
//     String ssid;
//     String password;
// } ssid_String_t;

#define MAX_SSID_LEN 20
typedef struct {
    const char ssid[MAX_SSID_LEN];
    const char password[MAX_SSID_LEN];
} ssid_char_t;

ssid_char_t *ssidStruct;

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


// ssid into char using malloc()
void readSSIDs_malloc(Preferences ns) {
    String password;
    String ssid;
    // retrieve del numero di SSIds
    // int8_t nr_ssids = ns.getChar(ssid_NRs);
    SSID_NUMBERS = ns.getChar(ssid_NRs);

    // buffer per numero sequenziale dell'indice
    char buf[4];
    for (int i = 0; i < SSID_NUMBERS; ++i) {
        sprintf(buf, "%02x", i);
        ssid = ns.getString(buf);
        printf("read: %s, value = %s\n", buf, ssid.c_str());

        password = ns.getString(ssid.c_str());
        printf("ssid: %s, password = %s\n", ssid.c_str(), password.c_str());

        // convert co char* using malloc ... ma non mi fido
        int str_len = ssid.length() + 1;
        cSSID[i] = (char *)malloc((str_len+1) * sizeof(char ));;
        ssid.toCharArray(cSSID[i], str_len);
        printf("%d - s_ssid: %s\n", i, ssid.c_str());
        printf("%d - new_ssid: %s\n", i, cSSID[i]);
        println();
     }
}

// ssid into char
void readSSIDs_struct_char(Preferences ns) {
    // retrieve del numero di SSIds
    SSID_NUMBERS = ns.getChar(ssid_NRs);
    ssid_char_t xxx[22];
    ssidStruct = &xxx;
    // buffer per numero sequenziale dell'indice
    char buf[4];
    for (int i = 0; i < SSID_NUMBERS; ++i) {
        // prepara l'indice per leggere l'ssid index
        sprintf(buf, "%02x", i);
        size_t len = ns.getString(buf, ssidStruct->ssid, MAX_SSID_LEN);
        printf("read: %s, value = %s (len: %d)\n", buf, ssidStruct->ssid, len);

        // password = ns.getString(ssid.c_str());
        // printf("ssid: %s, password = %s\n", ssid.c_str(), password.c_str());

        // // convert co char* using malloc ... ma non mi fido
        // int str_len = ssid.length() + 1;
        // cSSID[i] = (char *)malloc((str_len+1) * sizeof(char ));;
        // ssid.toCharArray(cSSID[i], str_len);
        // printf("%d - s_ssid: %s\n", i, ssid.c_str());
        // printf("%d - new_ssid: %s\n", i, cSSID[i]);
        // println();
     }
}

// ssid into String
void readSSIDs_String(Preferences ns) {
    String password;
    // retrieve del numero di SSIds
    // int8_t nr_ssids = ns.getChar(ssid_NRs);
    SSID_NUMBERS = ns.getChar(ssid_NRs);

    // buffer per numero sequenziale dell'indice
    char buf[4];
    for (int i = 0; i < SSID_NUMBERS; ++i) {
        sprintf(buf, "%02x", i);
        sSSID[i] = ns.getString(buf);
        printf("read: %s, value = %s\n", buf, sSSID[i].c_str());

        password = ns.getString(sSSID[i].c_str());
        printf("ssid: %s, password = %s\n", sSSID[i].c_str(), password.c_str());

     }
}

// ssid into String
void readSSIDs_String(Preferences ns) {
    String password;
    String ssid;
    // retrieve del numero di SSIds
    // int8_t nr_ssids = ns.getChar(ssid_NRs);
    SSID_NUMBERS = ns.getChar(ssid_NRs);

    // buffer per numero sequenziale dell'indice
    char buf[4];
    for (int i = 0; i < SSID_NUMBERS; ++i) {
        sprintf(buf, "%02x", i);
        sSSID[i] = ns.getString(buf);
        printf("read: %s, value = %s\n", buf, sSSID[i].c_str());

        password = ns.getString(sSSID[i].c_str());
        printf("ssid: %s, password = %s\n", sSSID[i].c_str(), password.c_str());

     }
}


// ssid into struct
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

    // Retrieve the operational parameters from the namespace
    //  and save them into their run-time variables.
    // String data = ssidPrefs.getString(ssid_NRs);
    // const char *cdata = ssidPrefs.getString(ssid_NRs);
    // readSSIDs_String(ssidPrefs);
    readSSIDs_struct_char(ssidPrefs);
    ssidPrefs.end();                                      // Close our preferences namespace.

    // printf("xxxssid: %s\n", xxxSSID);

    // printf("ssid list keys: %s", data.c_str());

}



void setup() {
    Serial.begin(115200);
    delay(2000);
    println();

    setSssidPreferences();
    // for (int8_t i=0; i<SSID_NUMBERS; i++) {
    //     printf("%d - new_ssid: %s\n", i, sSSID[i].c_str());
    // }
}



// ##########################################################
// #
// ##########################################################
void loop() {
  // put your main code here, to run repeatedly:
}


