/*
# updated by ...: Loreto Notarantonio
# Date .........: 19-07-2024 16.33.13
*/

#ifndef WIFI_H
#define WIFI_H


    // --------------------------------------------------
    // --- functions prototypes
    // --------------------------------------------------

    void wifi_Start(void);
    bool processScannedNetworks(int16_t networksFound);
    void wifi_connect(void);
    // void wifi_connect(const char *ssid, const char *pass);
    int ssidInList(const char *ssid);



    // void wifi_asyncScan(void);
    // void wifi_disconnect(void);
    // bool wifi_isConnected(void);

    // int16_t wifi_scanComplete(void);

        // --- prototypes ----
    // void wifi_setup();

    // void wifi_rotate_setup();
    // int wifi_getBestSSID(const char *next_ssid);

#endif