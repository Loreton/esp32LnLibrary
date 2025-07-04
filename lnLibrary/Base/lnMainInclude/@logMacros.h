/*
// updated by ...: Loreto Notarantonio
// Date .........: 04-07-2025 14.50.28
*/
#pragma once

#include <Arduino.h>

extern char    debug_filename[];
extern int8_t debug_filename_length;

    /* definition to expand macro then apply to pragma message */
    #define VALUE_TO_STRING(x) #x
    #define VALUE(x) VALUE_TO_STRING(x)
    #define VAR_NAME_VALUE(var) #var "="  VALUE(var)
    #define PRINT_MACRO(var) #var "="  VALUE(var)
    // #define PRINT_MACRO(var) #var "='" VALUE(var) "'"

    /* Some example here */
    // #pragma message(VAR_NAME_VALUE(NOT_DEFINED))
    // #pragma message(VAR_NAME_VALUE(DEFINED_BUT_NO_VALUE))
    // #pragma message(VAR_NAME_VALUE(DEFINED_INT))
    // #pragma message(VAR_NAME_VALUE(DEFINED_STR))
    // #pragma message(VAR_NAME_VALUE(LORETO))
    // #pragma message(VAR_NAME_VALUE(LORETO2))


    // -------------------------------------------------
    #ifndef  __FILENAME__
        // per Arduino
        #include <string.h>
        #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) // remove path
        //#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
    #endif

    #define __FILE_LINE_ID__ ({ \
        static char out[128]; \
        const char *filename = strrchr(__FILE__, '/'); \
        filename = filename ? filename + 1 : __FILE__; \
        char *dot = strrchr(filename, '.'); \
        size_t len = dot ? (size_t)(dot - filename) : strlen(filename); \
        snprintf(out, sizeof(out), "%.*s.%03d", (int)len, filename, __LINE__); \
        out; \
    })

    // #define DEBUG_LOG_COLOR(fmt, ...) \
    //     Serial.printf("\033[1;32m[%s] %s(): " fmt "\033[0m\n", __FILE_LINE_ID__, __func__, ##__VA_ARGS__)
    // #define DEBUG_LOG(fmt, ...) \
    //     Serial.printf("[%s] " fmt "\n", __FILE_LINE_ID__, ##__VA_ARGS__)


    // #define __FILE_NO_EXT__ ({ \
    //                             static char buffer[128]; /* Adjust size as needed, ensure it's large enough for your longest filename */ \
    //                             strncpy(buffer, __FILENAME__, sizeof(buffer) - 1); \
    //                             buffer[sizeof(buffer) - 1] = '\0'; /* Ensure null-termination */ \
    //                             char *dot = strrchr(buffer, '.'); \
    //                             if (dot) { \
    //                                 *dot = '\0'; /* Null-terminate at the dot */ \
    //                             } \
    //                             buffer; \
    //                         })

    //                             // char *basename = strrchr(__FILENAME__, '/'); /*Estrai il nome file senza path*/ \
    //                             // basename = (basename) ? basename + 1 : __FILENAME__;  /*se non c'è '/', usa tutto*/ \
    //                             // static uint8_t pin_len; \
    //                             // static uint8_t index=0; \







    char * nowTime(void); // se è definita in time o ntptime commentare quella presente nin main.cpp
    // const char*  setFnameLine(char buffer[], int MAXLENGTH, const char *path, int lineNo);
    // -------------------------------------------------



    #define lnSERIAL Serial

    #ifdef lnSERIAL
        #if defined(ARDUINO_ARCH_ESP32)
            // #define __ln_FNAME__ __FILENAME__
            #define __ln_FNAME__ __FILE_LINE_ID__

            // #define m_FNAME                                         lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __ln_FNAME__, __LINE__)
            #define m_FNAME                                         lnSERIAL.printf((PGM_P) PSTR("[%-20s] "), __FILE_LINE_ID__)
            // #define m_FNAME                                         lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)
            // #define m_FNAME_ext_non_funziona                                     lnSERIAL.printf((PGM_P) "[%s]:", setFnameLine(debug_filename, debug_filename_length, __FILENAME__, __LINE__) )
            #define m_Func                                          lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __func__, __LINE__)
            #define m_FUNCTION                                      lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __FUNCTION__, __LINE__)
            // #define m_FILE_FUNC                                     lnSERIAL.printf((PGM_P) PSTR("[%s.%-10s:%04d] "), __ln_FNAME__, __func__, __LINE__)


            // #define m_FULLNAME                                      lnSERIAL.printf((PGM_P) "[%s]: ", extractFileName((char*)__FILE__, __appo__))
            #define m_NOW                                           lnSERIAL.printf((PGM_P) "[%s]: ", nowTime())
            #define lnPrint(...)                                    lnSERIAL.print(__VA_ARGS__)
            #define lnPrintLN(...)                                  lnSERIAL.println(__VA_ARGS__)
            #define lnPrintF(fmt, ...)       {                      lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_FN(fmt, ...)    {        m_FNAME;      lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            // #define lnPrintF_FF(fmt, ...)    {        m_FILE_FUNC;  lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_now(fmt, ...)   {        m_NOW;        lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
            #define lnPrintF_NowFN(fmt, ...) { m_NOW; m_FNAME;      lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
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



    #ifdef  LOG_LEVEL_0
        #define printf0             lnPrintF
        #define printf0_FN          lnPrintF_FN
        #define printf0_NFN         lnPrintF_NowFN
    #else
        #define printf0
        #define printf0_FN
        #define printf0_NFN
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

    #ifdef  LOG_LEVEL_99  // come debug solo durante i test di uno specifico modulo
        #define printf99             lnPrintF
        #define printf99_FN          lnPrintF_FN
        #define printf99_NFN         lnPrintF_NowFN
    #else
        #define printf99
        #define printf99_FN
        #define printf99_NFN
    #endif





// #endif