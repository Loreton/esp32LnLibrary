//
// updated by ...: Loreto Notarantonio
// Date .........: 20-02-2025 10.21.47
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include <WiFi.h>

// --- correlated
#include "@lnMacros.h"


// ---------------------------------
// macros Aliases
// ---------------------------------
// #define print           ln_print
#define printf0          lnPrintF


void printSizeOf() {
    printf0("%-10s %d \n", "bool", sizeof(bool));
    printf0("%-10s %d \n", "int8_t", sizeof(int8_t));
    printf0("%-10s %d \n", "int16_t", sizeof(int16_t));
    printf0("%-10s %d \n", "int32_t", sizeof(int32_t));
    printf0("%-10s %d \n", "long", sizeof(long));
    printf0("%-10s %d \n", "float", sizeof(float));
    printf0("%-10s %d \n", "int64_t", sizeof(int64_t));
    printf0("%-10s %d \n", "long long", sizeof(long long));
    printf0("%-10s %d \n", "double", sizeof(double_t));
}



void checkOutputPin(uint8_t pin, uint8_t mode) {

    Serial.printf("pin: %d - mode: %d", pin, mode);

    // delay(3000); Serial.print(" - LOW"); digitalWrite(pin, LOW); pinMode(pin, OUTPUT_OPEN_DRAIN);
    delay(3000); Serial.print(" - LOW"); digitalWrite(pin, LOW); pinMode(pin, mode);
    delay(3000); Serial.print(" - HIGH");  digitalWrite(pin, HIGH);
    delay(3000); Serial.print(" - LOW"); digitalWrite(pin, LOW);
    delay(3000); pinMode(pin, INPUT);

    Serial.printf("\n");
}



    // ============================
    // --- individua il tipo di dato e lo stampa
    // ============================
    inline const char * typeStr (int64_t    var) { return " int64 (lld to print)"; }
    inline const char * typeStr (int32_t    var) { return " int32 (ld to print)"; }
    inline const char * typeStr (int16_t    var) { return " int16 (d to print)"; }
    inline const char * typeStr (int8_t     var) { return " int8  (d to print)"; }

    inline const char * typeStr (uint64_t   var) { return " uint64 (llu to print)"; }
    inline const char * typeStr (uint32_t   var) { return " uint32 (lu to print)"; }
    inline const char * typeStr (uint16_t   var) { return " uint16 (d/u to print)"; }
    inline const char * typeStr (uint8_t    var) { return " uint8  (d/u to print)"; }

    inline const char * typeStr (long       var) { return " long "; }
    inline const char * typeStr (float      var) { return " float "; }
    inline const char * typeStr (const char *var) { return " char *"; }
    inline const char * typeStr (String     var) { return " String "; }

    /* examples to call
        Serial.println("");
        Serial.printf("msg.sender.id        - type of data: %s\n", typeStr(msg.sender.id));
        Serial.printf("msg.member.id        - type of data: %s\n", typeStr(msg.member.id));
        Serial.printf("msg.sender.username  - type of data: %s\n", typeStr(msg.sender.username));
        Serial.println("");
    */
