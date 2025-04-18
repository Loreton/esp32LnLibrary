/*
// updated by ...: Loreto Notarantonio
// Date .........: 09-04-2025 17.24.41
*/

#include <Arduino.h>

#ifndef __LOG_MACROS_H__
    #define __LOG_MACROS_H__

    #ifndef  __FILENAME__
        // per Arduino
        #include <string.h>
        #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) // remove path
        //#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
    #endif


    // -------------------------------------------------
    char * nowTime(void);
    char * nowTimeDummy(void);
    char *extractFileName(char *source, char *dest);

    #define m_FNAME1                     Serial.printf(PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)
    // - insert one of these if defined otherwise BLANK...,
    #define m_NOW1                       Serial.printf("[%s]: ", nowTime())
    // -------------------------------------------------




    #define lnSERIAL Serial

    #ifdef lnSERIAL
        #if defined(ARDUINO_ARCH_ESP32)
            #define m_FNAME                                         lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)
            #define m_Func                                          lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __func__, __LINE__)
            #define m_FUNCTION                                      lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __FUNCTION__, __LINE__)
            #define m_FILE_FUNC                                     lnSERIAL.printf((PGM_P) PSTR("[%s.%-10s:%04d] "), __FILENAME__, __func__, __LINE__)


            // #define m_FULLNAME                                      lnSERIAL.printf((PGM_P) "[%s]: ", extractFileName((char*)__FILE__, __appo__))
            #define m_NOW                                           lnSERIAL.printf((PGM_P) "[%s]: ", nowTime())
            #define lnPrint(...)                                    lnSERIAL.print(__VA_ARGS__)
            #define lnPrintLN(...)                                  lnSERIAL.println(__VA_ARGS__)
            #define lnPrintF(fmt, ...)       {                      lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_FN(fmt, ...)    {        m_FNAME;   lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_now(fmt, ...)   {        m_NOW;        lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_NowFN(fmt, ...) { m_NOW; m_FNAME;   lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
        #else
            #error "Non sono in ARDUINO_ARCH_ESP32"
            #define lnPrint(...)                                    lnSERIAL.print(__VA_ARGS__)
            #define lnPrintLN(...)                                  lnSERIAL.println(__VA_ARGS__)
            #define lnPrintF(fmt, ...)       {                      lnSERIAL.printf(fmt, ## __VA_ARGS__); }
            #define lnPrintF_FN(fmt, ...)    {          m_FNAME;    lnSERIAL.printf(fmt, ## __VA_ARGS__); }
            #define lnPrintF_now(fmt, ...)   {          m_NOW;      lnSERIAL.printf(fmt, ## __VA_ARGS__); }
            #define lnPrintF_NowFN(fmt, ...) { m_NOW;   m_FNAME;    lnSERIAL.printf(fmt, ## __VA_ARGS__); }
        #endif
    #else
        #warning "Serial NOT be used."
        #define lnPrint(...)
        #define lnPrintLN(...)
        #define lnPrintF(fmt, ...)
        #define lnPrintF_FN(fmt, ...)
        #define lnPrintF_now(fmt, ...)
        #define lnPrintF_NowFN(fmt, ...)
    #endif



    #ifdef  LOG_LEVEL_1
        #define printf1             lnPrintF
        #define printf1_FN          lnPrintF_FN
        #define printf1_NFN         lnPrintF_NowFN
    #else
        #define printf1
        #define printf1_FN
        #define printf1_NFN
    #endif

    #ifdef  LOG_LEVEL_2
        #define printf2             lnPrintF
        #define printf2_FN          lnPrintF_FN
        #define printf2_NFN         lnPrintF_NowFN
    #else
        #define printf2
        #define printf2_FN
        #define printf2_NFN
    #endif


    #ifdef  LOG_LEVEL_3
        #define printf3             lnPrintF
        #define printf3_FN          lnPrintF_FN
        #define printf3_NFN         lnPrintF_NowFN
    #else
        #define printf3
        #define printf3_FN
        #define printf3_NFN
    #endif


    #ifdef  LOG_LEVEL_4
        #define printf4             lnPrintF
        #define printf4_FN          lnPrintF_FN
        #define printf4_NFN         lnPrintF_NowFN
    #else
        #define printf4
        #define printf4_FN
        #define printf4_NFN
    #endif







#endif