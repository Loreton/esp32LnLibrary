/*
// updated by ...: Loreto Notarantonio
// Date .........: 27-02-2025 19.39.35
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
    #define m_FNAME                     Serial.printf(PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)
    // - insert one of these if defined otherwise BLANK...,
    #define m_NOW                       Serial.printf("[%s]: ", nowTime())
    // #define m_NOW                       Serial.printf("[%s]: ", nowTimeDummy())
    // #define m_NOW
    // -------------------------------------------------

    #define SERIAL_DEBUG


    #ifdef SERIAL_DEBUG
        #define SERIAL_BEGIN(...)           Serial.begin(__VA_ARGS__)
        #define LN_PRINT(...)               Serial.print(__VA_ARGS__)
        #define lnPrintLN(...)             Serial.println(__VA_ARGS__)
        #define lnPrintF(...)              Serial.printf(__VA_ARGS__, __VA_ARGS__)
        #define lnPrintF_FN(...)           m_FNAME; Serial.printf( __VA_ARGS__, __VA_ARGS__)
        #define lnPrintF_Now(...)          m_NOW;   Serial.printf( __VA_ARGS__, __VA_ARGS__)
        #define lnPrintF_NowFN(...)        m_NOW;   m_FNAME; Serial.printf( __VA_ARGS__, __VA_ARGS__)
    #else
        #define SERIAL_BEGIN(...)
        #define LN_PRINT(...)
        #define lnPrintLN(...)
        #define lnPrintF(...)
        #define lnPrintF_FN(...)
        #define lnPrintF_Now(...)
        #define lnPrintF_NowFN(...)
    #endif



    // ============================
    // --- END  M A C R O S
    // ============================


#endif