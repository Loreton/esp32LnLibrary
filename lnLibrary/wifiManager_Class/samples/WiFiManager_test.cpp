//
// updated by ...: Loreto Notarantonio
// Date .........: 08-08-2025 18.29.10
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include <WiFi.h>

#include "lnLogger_Class.h"
#include "WiFiManager_Class.h"


#define __LOAD_SSIDS_CPP__
    #include "wifiManager_ssid_credentials.h"
#undef __LOAD_SSIDS_CPP__

// ----------------------------------------------------
// Codice principale del tuo sketch
// ----------------------------------------------------
// Array di reti con SSID e password
// Network myNetworks1[] = {
//     {"Casetta",         "Casetta.Adsl"},
//     {"WebPocket-4545",  "BBBA6CAC"},
//     {"cpe210",          "Loreto.CPE210"},
//     {"LoretoHotSpot",   "Loretohs"}
// };



// Crea un'istanza della classe WiFiManager
// WiFiManager_Class myWiFiManager(myNetworks, sizeof(myNetworks) / sizeof(myNetworks[0]));
WiFiManager_Class myWiFiManager;

void setup() {
    Serial.begin(115200);
    delay(1000);
    lnLog.init();
    // Inizializza la classe
    myWiFiManager.init(myNetworks, sizeof(myNetworks) / sizeof(myNetworks[0]));
    // myWiFiManager.begin();
}

void loop() {
    // Chiama il metodo loop() della classe per la gestione continua
    myWiFiManager.update();

    // Puoi aggiungere il resto del tuo codice qui, ad esempio per leggere sensori o inviare dati
    // Fai attenzione a non usare delay troppo lunghi che bloccherebbero il loop
    // per un periodo prolungato, impedendo la scansione WiFi.
}
