/*
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 08.38.38
*/

#include <Arduino.h>

#ifndef __LN_MACROS_H__
    #define __LN_MACROS_H__
    char          * nowTime(void);
    #define Now nowTime

    #ifndef  __FILENAME__
        // per Arduino
        #include <string.h>
        #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) // remove path
        //#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
    #endif

    #define SERIAL_BEGIN(...)       Serial.begin(__VA_ARGS__)
    #define lnprintf(...)           Serial.printf(PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__);Serial.printf( __VA_ARGS__, __VA_ARGS__)

    #define m_FNAME                   Serial.printf(PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)
    #define m_NOW                     Serial.printf("[%s]: ", Now())

    #define ln_print                  Serial.print
    #define ln_println                Serial.println
    #define ln_printf(...)            Serial.printf( __VA_ARGS__, __VA_ARGS__)

    #define ln_printf_FN(...)         m_FNAME; Serial.printf( __VA_ARGS__, __VA_ARGS__)
    #define ln_printf_Now(...)        m_NOW;   Serial.printf( __VA_ARGS__, __VA_ARGS__)
    #define ln_printf_NowFN(...)      m_NOW;   m_FNAME; Serial.printf( __VA_ARGS__, __VA_ARGS__)

    // ============================
    // --- END  M A C R O S
    // ============================


#endif