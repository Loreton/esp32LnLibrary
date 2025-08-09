//
// updated by ...: Loreto Notarantonio
// Date .........: 09-08-2025 08.42.57
//


#ifdef __ln_MODULE_DEBUG_TEST__


#include "lnLogger_Class.h"


void setup() {
    Serial.begin(115200); // Initialize Serial communication
    lnLog.init();         // Initialize the logger's mutex
    // ... rest of your setup code
}


void loop() {
    LOG_INFO("This is an info message.");
    LOG_WARN("Something might be wrong here: %d", 42);
    LOG_ERROR("Critical error occurred in file %s at line %d", __FILE__, __LINE__);
    LOG_DEBUG("Debugging variable value: %s", VAR_NAME_VALUE(some_variable));
    delay(1000);
}


#endif