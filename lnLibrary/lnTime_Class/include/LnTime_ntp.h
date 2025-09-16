//
// updated by ...: Loreto Notarantonio
// Date .........: 13-09-2025 16.08.29
//

#pragma once




private:
    bool           m_ntp_active        = false;
    uint32_t       m_lastNtpAttempt    = 0;
    uint32_t       m_NTP_SYNC_INTERVAL = 2*60*1000UL;
    bool           m_NTP_synched       = false;    // comodo da utilizzare esternamente

    uint8_t         m_lastNtpStatus = 0xFF;
    // uint8_t         m_last_status = 0xFF;
    unsigned long   m_ntpStartTime = 0;
    // uint32_t        m_ntp_start_time = 0;

    const char*    m_ntpServer1        = "pool.ntp.org";
    const char*    m_ntpServer2        = "time.google.com";
    const char*    m_ntpServer3        = "br.pool.ntp.org";
    const char*    m_ntpServer4        = "time.nist.gov";
    const char*    m_ntpServer5        = "2.br.pool.ntp.org";
    const char*    m_ntpServer6        = "time.windows.com";


    // --- methods
    static  void cbSyncTime(struct timeval *tv) ;




public:
    void initNTP(); // Nuovo metodo pubblico per la sincronizzazione
    bool isNtpSynched(void) const { return m_NTP_synched; }
    const char * ntpSyncStatus(void);
    // bool checkNtpSynched(bool force=false);
    bool updateNtpSyncStatus(void);

