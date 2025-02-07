/*
// updated by ...: Loreto Notarantonio
// Date .........: 07-02-2025 17.57.50
*/

#ifndef __LN_WIFI__
    #define __LN_WIFI__


    extern const char * PROGMEM base_SSIDs     [];
    extern const char * PROGMEM base_PASSWORDs [];
    extern int8_t SSID_ELEMENTS;



    // senza LN_ davanti è gia definito in altri file di sistema
    #define LN_MAX_SSID_LEN        32
    #define LN_MAX_PASSPHRASE_LEN  64
    typedef struct {
        char PROGMEM ssid[LN_MAX_SSID_LEN+1];
        char PROGMEM password[LN_MAX_PASSPHRASE_LEN+1];
    } ssid_t;

    extern ssid_t mySSID[];
    // ssid_t *ssidPtr;

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


    extern structNET bestSSID;
    // extern structNET *bestssid;





    // --------------------------------------------------
    // --- functions prototypes
    // --------------------------------------------------

    void wifi_Start(void);
    bool processScannedNetworks(int16_t networksFound);
    void wifi_connect(void);
    // void wifi_connect(const char *ssid, const char *pass);
    int ssidInList(const char *ssid);



#endif