//
// updated by ...: Loreto Notarantonio
// Date .........: 13-09-2025 16.55.36
//

// LnTime_Class.h
#pragma once

    #include "Arduino.h"
    #include <ESP32Time.h> // ESP32Time.cpp
    #include <map>
    #include <vector> // Use std::vector for flexible arrays

    #define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3"

    class LnTime_Class {


        private:
            ESP32Time rtc;
            struct tm      m_timeinfo;
            bool m_dhmCustomUpdate = false;
            uint8_t m_last_ntp_update = 0;


        public:
            // Costruttore
            LnTime_Class();

            // Metodi pubblici
            void setup(uint16_t ntpIntervalTimeSync=2*60); // 2 minuti
            void update(void);
            char *now(void);
            void set_dhmCustomUpdate(bool customUpdate);



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


        #include "LnTime_ntp.h"
        #include "LnTime_timeMethods.h"
        #include "LnTime_seconds.h"
        #include "LnTime_minutes.h"
        #include "LnTime_hours.h"
        #include "LnTime_days.h"

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