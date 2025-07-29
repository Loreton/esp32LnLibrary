//
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 12.14.46
/*
// updated by ...: Loreto Notarantonio
// Date .........: 29-07-2025 12.14.46
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
        char *nowTime();
        const char* timeStamp(char *buffer, uint8_t buffer_len, uint32_t millisec=0, bool stripHeader=false);
        void to_HHMMSS(uint32_t mseconds, char *outStr, uint8_t maxlen);
        void alignToMinute();
        bool isSecondOClock();
        bool isMinuteOClock();
        bool isQuarterOClock();
        int8_t waitForSecond();
        int8_t secondsToMinute();
        uint32_t secondsOfDay(int offset = 0);
        uint32_t minutesOfDay(int offset = 0);
        uint32_t getEpoch(unsigned long offset = 0);

        // Metodi per accedere alla struttura tm e all'oggetto rtc (nuovi o modificati per l'accesso esterno)
        struct tm getTimeStruct();
        ESP32Time& getRtcInstance(); // Aggiungi questo metodo per esporre l'istanza rtc

    private:
        ESP32Time rtc;
        // char        timeBuffer[TIME_BUFFER_LENGTH];
        struct tm   m_timeinfo;
        int8_t      m_last_minute = 255;
        int8_t      m_last_second = 255;
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