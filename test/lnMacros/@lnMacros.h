/*
// updated by ...: Loreto Notarantonio
// Date .........: 08-02-2025 16.03.32
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
    // - insert one of these if defined otherwise BLANK...
    // #define m_NOW                       Serial.printf("[%s]: ", NowTime())
    #define m_NOW                       Serial.printf("[%s]: ", nowTimeDummy())
    // #define m_NOW
    // -------------------------------------------------

    #define m_FNAME                     Serial.printf(PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)


    #define SERIAL_BEGIN(...)           Serial.begin(__VA_ARGS__)
    #define LN_PRINT(...)               Serial.print(__VA_ARGS__)
    #define LN_PRINTln(...)             Serial.println(__VA_ARGS__)
    #define LN_PRINTf(...)              Serial.printf(__VA_ARGS__, __VA_ARGS__)

    #define LN_PRINTf_FN(...)           m_FNAME; Serial.printf( __VA_ARGS__, __VA_ARGS__)
    #define LN_PRINTf_Now(...)          m_NOW;   Serial.printf( __VA_ARGS__, __VA_ARGS__)
    #define LN_PRINTf_NowFN(...)        m_NOW;   m_FNAME; Serial.printf( __VA_ARGS__, __VA_ARGS__)


    // #define lnprintf(...)           Serial.printf(PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__);Serial.printf( __VA_ARGS__, __VA_ARGS__)


    #define print_null(...)

    // ============================
    // --- END  M A C R O S
    // ============================


#endif