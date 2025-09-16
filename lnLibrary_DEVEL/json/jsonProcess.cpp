/*
// updated by ...: Loreto Notarantonio
// Date .........: 16-01-2025 08.31.26
* ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
*/

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include "ArduinoJson.h"


#include "@ln_ssid.h"



typedef struct {
    const char *hostname;
    int time;
    int data[10];
} myStruct;
const char *filename = "/test.json";


typedef struct {
    const char *ssid;
    const char *passwowd;
} ssid_t;





// #########################################
// # Deserialize json data to array
// #########################################
int loadJsonToSsidArrayx(const char *jsonData, ssid_t *s) {

}

// #########################################
// # Deserialize json data to array
// #########################################
int serializeJson(const char *jsonData, ssid_t *s) {

    // const char* input;
    // size_t inputLength; (optional)

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, jsonData, inputLength);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    const char* ssid = doc["ssid"];
    const char* password = doc["passwowd"];

    float data_0 = doc["data"][0]; // 48.75608
    float data_1 = doc["data"][1]; // 2.302038
}



// #########################################
// DeSerialize array to json data
// #########################################
int serializeJson(const char *jsonData) {
    typedef {
        const char *ssid;
        const char *passwowd;
    } ssid_t;

    JsonDocument jsonDoc2;
    DeserializationError error = deserializeJson(doc, jsonData);
    // Check for errors in deserialization
    if (error) {
        lnprintln("Failed to parse JSON\n");
        return;
    }
    return 1;
}




