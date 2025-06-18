/*
// updated by ...: Loreto Notarantonio
// Date .........: 12-06-2025 08.02.38
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


#ifndef __LOG_VARIABLES_TYPE01_H__
    #define __LOG_VARIABLES_TYPE01_H__




    #ifndef ln_esp32_WROOM_32E_MODULE
        #error "ln_esp32_WROOM_32E_MODULE not defined"
    #endif

    #ifndef ln_esp32_WROOM_32E_MODULE_2RELAY
        #error "ln_esp32_WROOM_32E_MODULE_2RELAY not defined"
    #endif

    #ifndef ln_esp32_DEVEL
        #error "ln_esp32_DEVEL not defined"
    #endif

    #ifndef ln_esp32_PRODUCTION
        #error "ln_esp32_PRODUCTION not defined"
    #endif

    #ifndef ln_esp32_RELEASE_TYPE
        #error "ln_esp32_RELEASE_TYPE not defined"
    #endif

    #ifndef ln_esp32_BOARD_TYPE
        #error "ln_esp32_BOARD_TYPE not defined"
    #endif

    #ifndef ln_esp32_LORETO
        #error "ln_esp32_LORETO not defined"
    #endif

    #if ln_esp32_LORETO == LORETO
        #warning "ln_esp32_LORETO OK"
        #pragma message PRINT_MACRO(ln_esp32_LORETO)
    #else
        #error "ln_esp32_LORETO bad"
        #pragma message PRINT_MACRO(ln_esp32_LORETO)
    #endif


#endif

