//
// updated by ...: Loreto Notarantonio
// Date .........: 09-08-2025 19.11.34
/*
// updated by ...: Loreto Notarantonio
// Date .........: 09-08-2025 19.11.34
*/

#pragma once

    #include "Arduino.h"
    #include <ESP32Time.h> // ESP32Time.cpp
    // #include "lnLogger_Class.h" // Assicurati che lnLogger.h sia disponibile nel tuo progetto

    // #define TIME_BUFFER_LENGTH 16
    #define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3"

    class LnTime_Class {
    public:
        // Costruttore
        LnTime_Class();

        // Metodi pubblici
        void setup();
        void initNTP(); // Nuovo metodo pubblico per la sincronizzazione

        char *nowTime();
        // const char* timeStamp(char *buffer, uint8_t buffer_len, uint32_t millisec=0, bool stripHeader=false);
        const char* timeStamp(char *buffer, uint8_t buffer_len, uint32_t millisec=0, bool addMilliSec=false, bool stripHeader=false); // msec from boot (or millisec) HH:MM:SS.msec
        // const char *to_HHMMSS(uint32_t mseconds, char *buffer, uint8_t buffer_len, bool addMilliSec=false); // un po dupllicato di msecToTimeStamp()
        void alignToMinute();
        bool isSecondOClock();
        bool isMinuteOClock();
        bool isQuarterOClock();
        int8_t waitForSecond();
        int8_t secondsToMinute();
        uint32_t millisecOfDay(int offset = 0);
        uint32_t secondsOfDay(int offset = 0);
        uint32_t minutesOfDay(int offset = 0);
        uint32_t getEpoch(unsigned long offset = 0);
        bool     everyXseconds(uint8_t seconds);



        bool ntpActive(void) const {return m_ntp_active;};



        // Metodi per accedere alla struttura tm e all'oggetto rtc (nuovi o modificati per l'accesso esterno)
        struct tm getTimeStruct();
        ESP32Time& getRtcInstance(); // Aggiungi questo metodo per esporre l'istanza rtc
        /**
         * Se timeBuffer è statica e globale.
         * Ogni chiamata a timeStamp sovrascrive il contenuto di timeBuffer,
         * quindi quando stampi più valori nello stesso printf,
         * entrambe le chiamate restituiscono il valore dell'ultima chiamata
         * per tale ragione il buffer deve essere allocato estrnamente
         * ....oppure prestare molta attenzione
        */
        char sharedTimeBUFFER[16];

    private:
        ESP32Time rtc;
        struct tm   m_timeinfo;
        int8_t      m_last_minute = 99;
        int8_t      m_last_second = 99;
        bool        m_ntp_active = false;
        // uint8_t     m_last_second=99;

        static void cbSyncTime(struct timeval *tv) ;
        // const char* m_ntpServer [] = {"pool.ntp.org", "time.google.com", "br.pool.ntp.org", "time.nist.gov", "2.br.pool.ntp.org"};

        const char* m_ntpServer1 = "pool.ntp.org";
        const char* m_ntpServer2 = "time.google.com";
        const char* m_ntpServer3 = "br.pool.ntp.org";
        const char* m_ntpServer4 = "time.nist.gov";
        const char* m_ntpServer5 = "2.br.pool.ntp.org";
        const char* m_ntpServer6 = "time.windows.com";

    };

    // Dichiarazioni delle funzioni di stampa esterne
    void printLocalTime(const struct tm *timeinfo_ptr); // Prende un const tm* per chiarezza
    void print_rtc_time(LnTime_Class *p); // Prende un puntatore a LnTime_Class

/**  Spiegazione delle Modifiche in LnTime_Class.h:
    Abbiamo aggiunto un metodo pubblico ESP32Time& getRtcInstance();
    nella classe LnTime_Class. Questo è fondamentale!
    Ti permette di accedere all'istanza privata rtc della classe LnTime_Class dall'esterno,
    in modo che le funzioni di stampa possano chiamare i metodi di ESP32Time (come getDate(), getTime(), ecc.).
    Restituire un riferimento (&) evita di copiare l'oggetto, che è efficiente.

    Le dichiarazioni di printLocalTime e print_rtc_time sono state spostate fuori dalla classe,
    ma all'interno del file .h per renderle visibili ad altri file che includono LnTime_Class.h.
*/
extern LnTime_Class lnTime;