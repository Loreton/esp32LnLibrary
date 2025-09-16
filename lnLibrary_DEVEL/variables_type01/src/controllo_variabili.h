/*
// updated by ...: Loreto Notarantonio
// Date .........: 05-06-2025 08.28.26
*/


// #define STRINGIFY(x) #x
// #define TOSTRING(x) STRINGIFY(x)

// // #define MY_MSG(x)  # pragma message "x is defined as " TOSTRING(x)
// // #define MYFLAG 1
// #ifdef MYFLAG
//     # pragma message "MYFLAG is defined as " TOSTRING(MYFLAG)
// #else
//     # pragma message "MYFLAG is not defined"
//     // # warning "MYFLAG is defined as " TOSTRING(MYFLAG)
// #endif


#ifndef __LOG_MACROS_H__
    #define __LOG_MACROS_H__


    #ifndef VARIABILE_01
        #error "VARIABILE_01 not defined"
    #endif

    #ifndef ESP32_WROOM_32E_MODULE
        #error "ESP32_WROOM_32E_MODULE not defined"
    #endif

    #ifndef ESP32_WROOM_32E_2RELAY_MODULE
        #error "ESP32_WROOM_32E_2RELAY_MODULE not defined"
    #endif

    #ifndef DEVEL
        #error "DEVEL not defined"
    #endif

    #ifndef PRODUCTION
        #error "PRODUCTION not defined"
    #endif

    #ifndef RELEASE_TYPE
        #error "RELEASE_TYPE not defined"
    #endif

    #ifndef ESP32_BOARD_TYPE
        #error "ESP32_BOARD_TYPE not defined"
    #endif


#endif

