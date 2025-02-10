/*
// updated by ...: Loreto Notarantonio
// Date .........: 10-02-2025 18.44.26
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
        bool        available = false;                         /** data in struct was changed */
        const char  *ssid;                     /**< SSID of AP */
        const char  *password;
        uint8_t     *bssid = NULL;                     /**< MAC address of AP */
        const char  *bssidStr;                     /**< MAC address of AP */
        uint8_t     channel = 0;                      /**< channel of AP */
        int8_t      rssi = -127;                         /**< signal strength of AP */
        // int8_t      diff_rssi=0;                         /**< signal strength of AP */
        int8_t      is_better=false;       // newRSSI better than curRSSI
    } structNET;



    extern structNET bestSSID;
    // extern structNET *bestssid;




    // --------------------------------------------------
    // --- functions prototypes
    // --------------------------------------------------

    void wifi_Start(void);
    bool processScannedNetworks(int16_t networksFound);
    void wifi_connect(void);
    void wifi_reconnect(void);
    // void wifi_connect(const char *ssid, const char *pass);
    int ssidInList(const char *ssid);
    bool connectOnScanResult(int16_t networksFound=-99);

    // inline bool wifi_isConnected(void);
    // inline void wifi_asyncScan(void);
    // inline void wifi_scanDelete(void);
    // inline int16_t wifi_scanComplete(void);

    bool wifi_isConnected(void);
    void wifi_asyncScan(void);
    void wifi_scanDelete(void);
    int16_t wifi_scanComplete(void);

#endif