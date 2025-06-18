/*
// updated by ...: Loreto Notarantonio
// Date .........: 05-06-2025 08.38.57
*/


#include <Arduino.h>

// char msg[] = "1,20,300,4000,50000";

#include "controllo_variabili.h"


// #define check_Var(...) { ifdef (__VA_ARGS__) #warning "ciao" else #error "error" #endif }

int  main() {
    Serial.println("start main\n");


    #if RELEASE_TYPE == DEVEL
        Serial.printf("RELEASE_TYPE: devel %d\n", DEVEL);
    #elif RELEASE_TYPE == PRODUCTION
        Serial.printf("RELEASE_TYPE: production %d\n", PRODUCTION);
    #else
        Serial.printf("RELEASE_TYPE: %d - NOT valid\n", RELEASE_TYPE);
    #endif

    #if ESP32_BOARD_TYPE == ESP32_WROOM_32E_2RELAY_MODULE
        Serial.printf("ESP32_BOARD_TYPE: ESP32_WROOM_32E_2RELAY_MODULE %d\n", ESP32_WROOM_32E_2RELAY_MODULE);
    #elif ESP32_BOARD_TYPE == ESP32_WROOM_32E_MODULE
        Serial.printf("ESP32_BOARD_TYPE: ESP32_WROOM_32E_MODULE %d\n", ESP32_WROOM_32E_MODULE);
    #else
        Serial.printf("ESP32_BOARD_TYPE: %d - NOT valid\n", ESP32_BOARD_TYPE);
    #endif



    Serial.println("\nend main");
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    main();
}

void loop() {}
