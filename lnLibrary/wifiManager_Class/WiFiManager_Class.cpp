//
// updated by ...: Loreto Notarantonio
// Date .........: 11-08-2025 18.24.37
//


#include <WiFi.h>
#include "WiFiManager_Class.h"
#include "lnLogger_Class.h"


// ----------------------------------------------------
// Classe WiFiManager_Class
// ----------------------------------------------------
WiFiManager_Class* WiFiManager_Class::s_instance = nullptr;

// Costruttore
WiFiManager_Class::WiFiManager_Class() {
    s_instance = this;
};


// Inizializza il WiFi in modalità Station e si connette
void WiFiManager_Class::init(Network* creds, int8_t count) {
    m_networks       = creds;
    m_networkCount = count;
    LOG_INFO("Inizializzazione WiFi...");
    WiFi.mode(WIFI_STA);

    // Registra la funzione di gestione degli eventi WiFi
    WiFi.onEvent(handleEvent);

    connectToBestNetwork();
}




// Controlla se è trascorso troppo tempo senza connessione e riavvia l'ESP32
void WiFiManager_Class::checkOutOfService() {
    if (millis() - m_wifiOutTime > m_maxWifiTimeout) {
        LOG_ERROR("Nessuna connessione da più di 1 ora. Riavvio forzato.");
        ESP.restart();
    }
}



// Funzione da chiamare nel loop principale per monitorare la connessione
void WiFiManager_Class::update() {
    // Se non siamo connessi o è il momento di scansionare nuovamente, avvia la scansione
    // if ((WiFi.status() != WL_CONNECTED || (millis() - m_lastScanTime > m_scanInterval)) && !m_scanning) {
    //     LOG_NOTIFY("Tempo di scansione periodica o connessione persa. Avvio scansione...");
    //     connectToBestNetwork();
    //     m_lastScanTime = millis();
    // }

    if (WiFi.status() != WL_CONNECTED ) {
        LOG_ERROR("connessione persa. Avvio scansione...");
        connectToBestNetwork();
        m_lastScanTime = millis();
    }

    if ( (millis() - m_lastScanTime > m_scanInterval) && !m_scanning ) {
        LOG_NOTIFY("Tempo di scansione periodica scaduto (%lu ms). Avvio scansione...", m_scanInterval);
        connectToBestNetwork();
        m_lastScanTime = millis();
    }


    // Se la scansione è in corso, controlla se è terminata
    if (m_scanning) {
        int scanResult = WiFi.scanComplete();
        if (scanResult >= 0) { // La scansione è completata
            m_scanning = false;
            processScanResults(scanResult);
        }
    }
    checkOutOfService();
}

// Avvia una scansione non bloccante
void WiFiManager_Class::connectToBestNetwork() {
    if (!m_scanning) {
        LOG_NOTIFY("Avvio scansione reti (asincrona)...");
        WiFi.scanNetworks(true); // Scansione non bloccante
        m_scanning = true;
    }
}

// Elabora i risultati della scansione e si connette alla rete migliore
void WiFiManager_Class::processScanResults(int n) {
    int bestRSSI = -127; // Valore RSSI minimo
    int bestNetworkIndex = -1;

    if (n == 0) {
        LOG_WARN("Nessuna rete trovata.");
        return;
    }

    LOG_INFO("reti trovate: %d", n);

    // Cerca la rete migliore tra quelle configurate
    for (int i = 0; i < n; ++i) {
        // LOG_INFO("  %d: %s (%d dBm)", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
        LOG_INFO("  %d: SSID: %-12s BSSID: %s (%d dBm)", i + 1, WiFi.SSID(i).c_str(), WiFi.BSSIDstr(i).c_str(), WiFi.RSSI(i));

        for (int j = 0; j < m_networkCount; ++j) {
            if (String(WiFi.SSID(i)) == String(m_networks[j].ssid)) {
                if (WiFi.RSSI(i) > bestRSSI) {
                    bestRSSI = WiFi.RSSI(i);
                    bestNetworkIndex = j;
                }
                break;
            }
        }
    }

    // Connessione alla rete scelta
    if (bestNetworkIndex != -1) {
        // Controlla se siamo già connessi alla rete migliore
        if (WiFi.status() == WL_CONNECTED && String(WiFi.SSID()) == String(m_networks[bestNetworkIndex].ssid)) {
            LOG_NOTIFY("Già connesso alla rete migliore: %s - %s.", WiFi.SSID(), WiFi.BSSIDstr().c_str());
            LOG_NOTIFY("...non è necessario cambiare.");
        } else {
            LOG_INFO("Connessione a: %s", m_networks[bestNetworkIndex].ssid);
            WiFi.begin(m_networks[bestNetworkIndex].ssid, m_networks[bestNetworkIndex].password);
        }
    } else {
        LOG_ERROR("Nessuna delle reti configurate è stata trovata.");
    }
}


// Funzione statica per la gestione degli eventi Wi-Fi
void WiFiManager_Class::handleEvent(arduino_event_id_t event) {
    if (s_instance) {
        switch (event) {
            case ARDUINO_EVENT_WIFI_STA_GOT_IP:
                LOG_INFO("Connesso! %s - %s - %s", WiFi.SSID(), WiFi.BSSIDstr().c_str(), WiFi.localIP().toString().c_str());
                s_instance->m_wifiOutTime = millis(); // Resetta il timer di timeout
                if (s_instance->m_onConnectCallback) {
                    s_instance->m_onConnectCallback(event);
                }
                break;
            case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                LOG_ERROR("WiFi - Connessione persa.");
                break;
            // Aggiungi altri eventi se necessario...
        }
    }
}





#if 0
// ----------------------------------------------------
// Classe WiFiManager_Class
// ----------------------------------------------------
// Costruttore
WiFiManager_Class::WiFiManager_Class() {};


// Inizializza il WiFi in modalità Station e si connette
void WiFiManager_Class::init(Network* creds, int8_t count) {
    m_networks       = creds;
    m_networkCount = count;
    LOG_INFO("Inizializzazione WiFi...");
    WiFi.mode(WIFI_STA);
    connectToBestNetwork();
}



void WiFiManager_Class::checkOutOfService() {
    #define MAX_WIFI_TIMEOUT  1*60*1000  // 1 ora....
    if (WiFi.status() == WL_CONNECTED) {
        m_wifiOutTime = millis();  // azzeriamo counter
    }
    if (millis() - m_wifiOutTime > MAX_WIFI_TIMEOUT) {
        ESP.restart();
    }
}

// Funzione da chiamare nel loop principale per monitorare la connessione
void WiFiManager_Class::update() {
    // Se non siamo connessi o è il momento di scansionare nuovamente, avvia la scansione
    if ((WiFi.status() != WL_CONNECTED || (millis() - m_lastScanTime > m_scanInterval)) && !m_scanning) {
        LOG_NOTIFY("Tempo di scansione periodica o connessione persa. Avvio scansione...");
        connectToBestNetwork();
        m_lastScanTime = millis();
    }

    // Se la scansione è in corso, controlla se è terminata
    if (m_scanning) {
        int scanResult = WiFi.scanComplete();
        if (scanResult >= 0) { // La scansione è completata
            m_scanning = false;
            processScanResults(scanResult);
        }
    }
    checkOutOfService();
}

// Avvia una scansione non bloccante
void WiFiManager_Class::connectToBestNetwork() {
    if (!m_scanning) {
        LOG_NOTIFY("Avvio scansione reti (asincrona)...");
        WiFi.scanNetworks(true); // Scansione non bloccante
        m_scanning = true;
    }
}

// Elabora i risultati della scansione e si connette alla rete migliore
void WiFiManager_Class::processScanResults(int n) {
    int bestRSSI = -127; // Valore RSSI minimo
    int bestNetworkIndex = -1;

    if (n == 0) {
        LOG_WARN("Nessuna rete trovata.");
        return;
    }

    LOG_INFO("reti trovate: %d", n);

    // Cerca la rete migliore tra quelle configurate
    for (int i = 0; i < n; ++i) {
        // LOG_INFO("  %d: %s (%d dBm)", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
        LOG_INFO("  %d: SSID: %-12s BSSID: %s (%d dBm)", i + 1, WiFi.SSID(i).c_str(), WiFi.BSSIDstr(i).c_str(), WiFi.RSSI(i));

        for (int j = 0; j < m_networkCount; ++j) {
            if (String(WiFi.SSID(i)) == String(m_networks[j].ssid)) {
                if (WiFi.RSSI(i) > bestRSSI) {
                    bestRSSI = WiFi.RSSI(i);
                    bestNetworkIndex = j;
                }
                break;
            }
        }
    }

    // Connessione alla rete scelta
    if (bestNetworkIndex != -1) {
        // Controlla se siamo già connessi alla rete migliore
        if (WiFi.status() == WL_CONNECTED && String(WiFi.SSID()) == String(m_networks[bestNetworkIndex].ssid)) {
            LOG_NOTIFY("Già connesso alla rete migliore: %s - %s.", WiFi.SSID(), WiFi.BSSIDstr().c_str());
            LOG_NOTIFY("...non è necessario cambiare.");
        } else {
            LOG_INFO("Connessione a: %s", m_networks[bestNetworkIndex].ssid);
            WiFi.begin(m_networks[bestNetworkIndex].ssid, m_networks[bestNetworkIndex].password);

            // Attendi la connessione
            int attempts = 0;
            while (WiFi.status() != WL_CONNECTED && attempts < 20) { // 10 secondi di timeout
                delay(500);
                LOG_NOTIFY("connecting to ...%s", m_networks[bestNetworkIndex].ssid);
                attempts++;
            }

            if (WiFi.status() == WL_CONNECTED) {
                LOG_INFO("Connesso! %s - %s - %s", WiFi.SSID(), WiFi.BSSIDstr().c_str(), WiFi.localIP().toString().c_str());
                // m_wifiOutTime = millis();
            } else {
                LOG_ERROR("WiFi - Connessione fallita.");
            }
        }
    } else {
        LOG_ERROR("Nessuna delle reti configurate è stata trovata.");
    }
}

#endif