//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 13.55.54
//


#ifdef __ln_MODULE_DEBUG_TEST__
// #include "lnLogger_Struct.h"
#include "lnLogger_Class.h"

#include "lnTime_Class.h"

// LnTime_Class lnTime; // Crea un'istanza della classe LnTime

void setup() {
    Serial.begin(115200);
    delay(2000);
    lnLog.init();

    lnTime.setup(); // Chiama il metodo setup della tua istanza di LnTime
}

void loop() {
    // Esempi di utilizzo dei metodi della classe:
    if (lnTime.isSecondOClock()) {
        LOG_INFO("Current time: %s", lnTime.nowTime());
        // Stampa il tempo completo usando la funzione dedicata
        struct tm current_time_info = lnTime.getTimeStruct();
        printLocalTime(&current_time_info); // Chiamata alla funzione esterna
    }

    if (lnTime.isMinuteOClock()) {
        LOG_WARNING("It's a new minute!");
        print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    delay(100); // Piccolo ritardo per non sovraccaricare la CPU
}

#endif
