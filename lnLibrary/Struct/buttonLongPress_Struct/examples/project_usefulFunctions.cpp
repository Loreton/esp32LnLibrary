//
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 09.39.30
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__
#include <Arduino.h>    // in testa anche per le definizioni dei type

// #define LOG_LEVEL_0
// #define LOG_LEVEL_99
#include "lnLogger.h"

#include "ButtonLongPress_Struct.h"




//###########################################################################
//#
//###########################################################################
void processButton(ButtonLongPress_Struct *p) {
    LOG_INFO("[%s[  Rilasciato! Durata: %ld ms", p->m_name, p->m_pressDuration);
    // Serial.print("Livello finale raggiunto: ");

    if (p->m_pin == startButton_pin) {
        Serial.print("start button action\n");
        // startButton_Action(p);
    }
    else if (p->m_pin == pumpState_pin) {
        // pumpState_Action(p);
    }


    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->m_currentPressLevel = NO_PRESS;
    p->m_lastPressedLevel = NO_PRESS;
    p->m_pressDuration = 0;
    p->m_maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.
}




#endif