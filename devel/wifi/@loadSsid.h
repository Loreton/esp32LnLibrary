/*
# updated by ...: Loreto Notarantonio
# Date .........: 19-07-2024 16.33.13
*/
#include "FS.h"
#include <LittleFS.h>

#ifndef __LOAD_SSIDS_H__
    #define __LOAD_SSIDS_H__
    #include "@ln_esp32_ssid_definitions.h"



    typedef struct {
        bool        updated;                         /** data in struct was changed */
        const char  *ssid;                     /**< SSID of AP */
        const char  *password;
        uint8_t     *bssid;                     /**< MAC address of AP */
        const char  *bssidStr;                     /**< MAC address of AP */
        uint8_t     channel;                      /**< channel of AP */
        int8_t      rssi;                         /**< signal strength of AP */
        int8_t      diff_rssi;                         /**< signal strength of AP */
    } structNET;


    #define MAX_SSID_CHARS          20
    typedef struct  {
        char  PROGMEM ssid[MAX_SSID_CHARS+1];
        char  PROGMEM passw[MAX_SSID_CHARS+1];
    } ssid_t;



    extern structNET bestSSID;
    extern structNET *bestssid;

    extern ssid_t mySSID[];
    extern uint8_t SSID_ARRAY_COUNT;
    extern uint8_t MAX_SSID_ARRAY_SIZE;






    // --------------------------------------------------
    // --- functions prototypes
    // --------------------------------------------------
    uint8_t load_ssidArray(fs::FS &fs, const char *jsonFile);

#endif