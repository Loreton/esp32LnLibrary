//
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 09.23.07
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include "Arduino.h" // in testa anche per le definizioni dei type
#include <ArduinoJson.h>
#include "FS.h"
#include <LittleFS.h>


// #define __LN_SSID_DEBUG__NOO
#define __LOAD_SSIDS_CPP__
#include "@loadSsid.h"


// --- others
#include "@lnMacros.h"
#include "@fileSystem.h"


// --------- definizioni modulo
#define MAX_SSIDs_ARRAY_SIZE    10
uint8_t MAX_SSID_ARRAY_SIZE=MAX_SSIDs_ARRAY_SIZE;
uint8_t SSID_ARRAY_COUNT;
ssid_t mySSID[MAX_SSIDs_ARRAY_SIZE]; // max ssid previsti

structNET bestSSID = {false, "", "", NULL, "", 0, -127, 0};
structNET *bestssid = &bestSSID;

// ---- jsonSsidToArray
#define ssid_dbg1                   ln_printf
#define ssid_dbg1_fn                ln_printf_FN




// ###########################################################
// #  Deserialize json to Array
// #  Expected format:
// #    {
// #        "ssids": {
// #            "Casetta":         "psw_Casetta",
// #            "cpe210":          "psw_cpe210",
// #            ".....":            "xxxxx",
// #        }
// #    }
// ###########################################################
uint8_t jsonSsidToArray(const char *json_data, ssid_t *ptr, uint8_t MAX_SSID_ARRAY_SIZE) {

    // --- deserialize json data
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json_data);
    if (error) {
        ssid_dbg1("json deserialization failed: "); Serial.println(error.c_str());
        return 0;
    }
    ssid_dbg1_fn("ssid json data deserialization OK\n");
    JsonObject items = doc["ssids"].as<JsonObject>();

    // --- check real array size
    uint8_t n_elements = items.size();
    ssid_dbg1("n_elements: %d\n", n_elements);

    // --- fillup array up to MAX_SSID_ARRAY_SIZE
    uint8_t index = 0;
    uint8_t lun = 0;
    for (JsonPair kv : items) {
        if (index < MAX_SSID_ARRAY_SIZE) {
            lun = snprintf(ptr->ssid,  MAX_SSID_CHARS, "%s",kv.key().c_str());  // https://cplusplus.com/reference/cstdio/snprintf/
            lun = snprintf(ptr->passw, MAX_SSID_CHARS, "%s",kv.value().as<const char *>()); // https://cplusplus.com/reference/cstdio/snprintf/
            ptr++;
            index++;
        } else {
            ssid_dbg1("\n\n\n\n");
            ssid_dbg1("n_elements: %d EXCEEDed array[%d] \n", index, MAX_SSID_ARRAY_SIZE);
            ssid_dbg1("\n\n\n\n");
        }
    }

    ssid_dbg1_fn("array filled-up with: %d elements\n", index);
    return index;

}


//##############################################################
//#
//##############################################################
uint8_t load_ssidArray(fs::FS &fs, const char *jsonFile) {
    const char * ssid_json_string = readTextFile(fs, jsonFile);

    // --- legge json_file, se non lo trova lo crea con i dati base degli ssid definiti in "ln_ssid.h"
    if (ssid_json_string == "") {
        if (writeTextFile(fs, jsonFile, SSID_json_base_data)) { // SSID_json_base_data defined in ln_ssid.h
            ssid_json_string = readTextFile(fs, jsonFile);
        }
        else {
            return 0;
        }
    }
    lnprintf("%s\n", ssid_json_string);

    // --- carica i dati degli SSID nell'array mySSID[]
    SSID_ARRAY_COUNT = jsonSsidToArray(ssid_json_string, &mySSID[0], MAX_SSID_ARRAY_SIZE); // ritorna il numero di SSID presenti



    #ifdef __LN_SSID_DEBUG__
        for (uint8_t j=0; j<SSID_ARRAY_COUNT; j++) {
            lnprintf("%d - %s: %s\n", j, mySSID[j].ssid, mySSID[j].passw);
        }
        lnprintf("\n");
        for (uint8_t j=0; j<SSID_ARRAY_COUNT; j++) {
            Serial.print("var_type: ");Serial.print(typeStr(mySSID[j].ssid)); Serial.println(mySSID[j].ssid);
            Serial.print("var_type: ");Serial.print(typeStr(mySSID[j].passw)); Serial.println(mySSID[j].passw);
            Serial.println();
        }
        lnprintf("\n");
    #endif




    return SSID_ARRAY_COUNT;
}