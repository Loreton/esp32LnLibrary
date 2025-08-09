//
// updated by ...: Loreto Notarantonio
// Date .........: 09-08-2025 07.51.16
//

#pragma once
    #include <WiFi.h>

    // Struttura per memorizzare le credenziali di rete
    struct Network {
        const char* ssid;
        const char* password;
    };

    // ----------------------------------------------------
    // Classe WiFiManager_Class
    // ----------------------------------------------------
    class WiFiManager_Class {
    private:
        Network*            m_networks;
        int8_t              m_networkCount;
        bool                m_scanning;
        uint32_t            m_lastScanTime = 0;
        const uint32_t      m_scanInterval = 5*60*1000; // Intervallo di scansione (in ms), qui 1 minuto

    public:
        // Costruttore
        WiFiManager_Class(void);

        // Inizializza il WiFi in modalità Station e si connette
        void init(Network* creds, int8_t count);

        // Funzione da chiamare nel loop principale per monitorare la connessione
        void update();
        void processScanResults(int n);
        bool isConnected(void) {return WiFi.status() == WL_CONNECTED; };

    private:
        // Scansiona le reti e si connette a quella migliore (RSSI più alto)
        void connectToBestNetwork();
    };

