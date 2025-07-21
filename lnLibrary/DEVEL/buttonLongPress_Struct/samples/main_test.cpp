//
// updated by ...: Loreto Notarantonio
// Date .........: 21-07-2025 12.12.22
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__


#include <Arduino.h>    // in testa anche per le definizioni dei type


#define __I_AM_MAIN_CPP__
#include "lnLogger.h"
#include "lnGlobalVars.h" // printf:XFN()
#include "lnSerialRead.h" // waitForEnter()

#include "../ButtonLongPress_Struct.h"



ButtonLongPress_Struct startButton;



void pinsInitialization(void) ;

void setup() {
    Serial.begin(115200);
    while (!Serial) {};
    delay(2000);
    myLog.begin();
    pinsInitialization();
    startButton.showStatus();

}


bool first_run=true;
void loop() {
    if (first_run) {
        first_run=false;
        LOG_INFO("processing started....");
    }

    if (startButton.read()) {
        startButton.showStatus();
    }
    delay(10);

}


#endif // #ifdef __ln_MODULE_DEBUG_TEST__
