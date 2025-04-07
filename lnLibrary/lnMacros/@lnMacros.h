/*
// updated by ...: Loreto Notarantonio
// Date .........: 07-04-2025 19.07.42
*/

#include <Arduino.h>

#ifndef __LN_MACROS_H__
    #define __LN_MACROS_H__

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
    // #define m_NOW                       Serial.printf("[%s]: ", nowTimeDummy())
    // #define m_NOW
    // -------------------------------------------------


    // #define DEBUG_MSG_FAUXMO(fmt, ...) { DEBUG_FAUXMO.printf(fmt, ## __VA_ARGS__); }

    // #define SERIAL_DEBUG
    // #ifdef SERIAL_DEBUG
    //     #define SERIAL_BEGIN(...)           Serial.begin(__VA_ARGS__)
    //     #define LN_PRINT(...)               Serial.print(__VA_ARGS__)
    //     #define lnPrintLN(...)             Serial.println(__VA_ARGS__)
    //     #define lnPrintF(...)              Serial.printf(__VA_ARGS__, __VA_ARGS__)
    //     #define lnPrintF_FN(...)           m_FNAME; Serial.printf( __VA_ARGS__, __VA_ARGS__)
    //     #define lnPrintF_Now(...)          m_NOW;   Serial.printf( __VA_ARGS__, __VA_ARGS__)
    //     // #define lnPrintF_NowFN(...)        m_NOW;   m_FNAME; Serial.printf( __VA_ARGS__, __VA_ARGS__)
    // #else
    //     #define SERIAL_BEGIN(...)
    //     #define LN_PRINT(...)
    //     #define lnPrintLN(...)
    //     #define lnPrintF(...)
    //     #define lnPrintF_FN(...)
    //     #define lnPrintF_Now(...)
    //     #define lnPrintF_NowFN(...)
    // #endif



    #define lnSERIAL Serial

    #ifdef lnSERIAL
        extern char __appo__[];
        #if defined(ARDUINO_ARCH_ESP32)
            // #warning "Sono in ARDUINO_ARCH_ESP32"
            // #define m_FullName                                    snprintf(__appo__, 100, "%-10s.%-10s:%04d", __FILENAME__, __func__, __LINE__)
            // #define __FILENAME_NO_EXT__ (strchr(__FILE__, '.') ? strchr(__FILE__, '.')  : __FILE__) // remove path
            // #define __FILENAME_NO_EXT__ (__FILENAME__.substr(0, __FILENAME__.find_last_of("."));) // remove path
            // #define __FILENAME_NO_EXT__ (__FILE__.substr(0, __FILE__.find_last_of("."));) // remove path

            #define m_FNAME                                         lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)
            #define m_Func                                          lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __func__, __LINE__)
            #define m_FUNCTION                                      lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __FUNCTION__, __LINE__)
            #define m_FILE_FUNC                                     lnSERIAL.printf((PGM_P) PSTR("[%s.%-10s:%04d] "), __FILENAME__, __func__, __LINE__)


            // #define m_FULLNAME                                      lnSERIAL.printf((PGM_P) "[%s]: ", extractFileName((char*)__FILE__, __appo__))
            #define m_NOW                                           lnSERIAL.printf((PGM_P) "[%s]: ", nowTime())
            #define lnPrint(...)                                    lnSERIAL.print(__VA_ARGS__)
            #define lnPrintLN(...)                                  lnSERIAL.println(__VA_ARGS__)
            #define lnPrintF(fmt, ...)       {                      lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_FN(fmt, ...)    {        m_FILE_FUNC;   lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_now(fmt, ...)   {        m_NOW;        lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_NowFN(fmt, ...) { m_NOW; m_FILE_FUNC;   lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
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


    // ============================
    // --- END  M A C R O S
    // ============================


#endif