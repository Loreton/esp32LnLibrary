//
// updated by ...: Loreto Notarantonio
// Date .........: 27-05-2025 20.03.15
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include "Arduino.h" // in testa anche per le definizioni dei type
#include <ESP32Ping.h>

const char* remote_host_ = "www.google.com";


// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_1
#include "@logMacros.h"



bool pingHost(const char *remote_host) {
    bool retStatus = false;
    if (Ping.ping(remote_host, 1)) {
        printf1_NFN("Pinging host %s success!!\n", remote_host);
        retStatus = true;
    }

    else {
        printf1_NFN("Pinging host %s Error :( !!\n", remote_host);
    }
    return retStatus;
}