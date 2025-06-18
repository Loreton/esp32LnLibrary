/*
// updated by ...: Loreto Notarantonio
// Date .........: 11-06-2025 20.21.57
*/


#include <Arduino.h>
// Various macro definitions to test

#include "controllo_variabili.h"


#define ST(x) #x
#define STR(x) ST(x)
#define PRINT_MACRO(var) #var "=[" STR(var) "]"



// Example prints at compile-time
#pragma message PRINT_MACRO(ln_esp32_RELEASE_TYPE)
#pragma message PRINT_MACRO(ln_esp32_PRODUCTION)
#pragma message PRINT_MACRO(ln_esp32_DEVEL)
#pragma message PRINT_MACRO(ln_esp32_BOARD_TYPE)
#pragma message PRINT_MACRO(ln_esp32_WROOM_32E_MODULE)
#pragma message PRINT_MACRO(ln_esp32_WROOM_32E_MODULE_2RELAY)
#pragma message PRINT_MACRO(ln_esp32_LORETO)





int  main() {
    Serial.println("start main\n");
    printf("Hello World.\n\n");

    Serial.printf("ln_esp32_RELEASE_TYPE:            %d\n", ln_esp32_RELEASE_TYPE);
    Serial.printf("ln_esp32_PRODUCTION:              %d\n", ln_esp32_PRODUCTION);
    Serial.printf("ln_esp32_DEVEL:                   %d\n", ln_esp32_DEVEL);
    Serial.printf("ln_esp32_BOARD_TYPE:              %d\n", ln_esp32_BOARD_TYPE);
    Serial.printf("ln_esp32_WROOM_32E_MODULE:        %d\n", ln_esp32_WROOM_32E_MODULE);
    Serial.printf("ln_esp32_WROOM_32E_MODULE_2RELAY: %d\n", ln_esp32_WROOM_32E_MODULE_2RELAY);

    #if ln_esp32_RELEASE_TYPE == ln_esp32_DEVEL
        Serial.printf("ln_esp32_RELEASE_TYPE: devel %d\n", ln_esp32_DEVEL);
    #elif ln_esp32_RELEASE_TYPE == ln_esp32_PRODUCTION
        Serial.printf("ln_esp32_RELEASE_TYPE: production %d\n", ln_esp32_PRODUCTION);
    #else
        Serial.printf("ln_esp32_RELEASE_TYPE: %d - NOT valid\n", ln_esp32_RELEASE_TYPE);
    #endif

    #if ln_esp32_BOARD_TYPE == ln_esp32_WROOM_32E_MODULE_2RELAY
        Serial.printf("ln_esp32_BOARD_TYPE: %d\n", ln_esp32_WROOM_32E_MODULE_2RELAY);
    #elif ln_esp32_BOARD_TYPE == ln_esp32_WROOM_32E_MODULE
        Serial.printf("ln_esp32_BOARD_TYPE: %d\n", ln_esp32_WROOM_32E_MODULE);
    #else
        Serial.printf("ln_esp32_BOARD_TYPE: %d - NOT valid\n", ln_esp32_BOARD_TYPE);
    #endif



    Serial.println("\nend main");
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    main();
}

void loop() {}
