//
// updated by ...: Loreto Notarantonio
// Date .........: 24-06-2025 10.01.39
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__
#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@globalVars.h" // printf:XFN()

#include "@pinLongPress_sc.h"

#include "@main_test.h"



//###########################################################################
//#
//###########################################################################
bool relayState = false;
void pumpState_Action(pinLongPress_sc *p) {
    switch (p->_currentPressLevel) {
        case PRESSED_LEVEL_1:
            printf99_FN("PRESSED_LEVEL_1\n");
            break;

        case PRESSED_LEVEL_2:
            printf99_FN("PRESSED_LEVEL_2\n");
            break;

        case PRESSED_LEVEL_3:
            printf99_FN("PRESSED_LEVEL_3\n");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(pressControlRelay_pin, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(pressControlRelay_pin, HIGH);
                printf0_FN("  --> Relè SPENTO!\n");
            }
            break;

        case PRESSED_LEVEL_4:
            printf99_FN("PRESSED_LEVEL_4\n");
            break;

        default:
            printf99_FN("Sconosciuto/Non Qualificato\n");
            break;
    }
}

//###########################################################################
//#
//###########################################################################
void startButton_Action(pinLongPress_sc *p) {
    // printf99_FN("sono qui p->_currentPressLevel %d\n", p->_currentPressLevel);
    switch (p->_currentPressLevel) {
        case PRESSED_LEVEL_1:
            printf99_FN("PRESSED_LEVEL_1\n");
            break;

        case PRESSED_LEVEL_2:
            printf99_FN("PRESSED_LEVEL_2\n");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(pressControlRelay_pin, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(pressControlRelay_pin, HIGH);
                printf0_FN("  --> Relè SPENTO!\n");
            }
            break;

        case PRESSED_LEVEL_3:
            printf99_FN("PRESSED_LEVEL_3\n");
            break;

        case PRESSED_LEVEL_4:
            printf99_FN("PRESSED_LEVEL_4\n");
            break;

        default:
            printf99_FN("Sconosciuto/Non Qualificato\n");
            break;
    }
}




//###########################################################################
//#
//###########################################################################
void processButton(pinLongPress_sc *p) {
    printf0_FN("[%s[  Rilasciato! Durata: %ld ms\n", p->_name, p->_pressDuration);
    // Serial.print("Livello finale raggiunto: ");

    if (p->_pin == startButton_pin) {
        Serial.print("start button action\n");
        startButton_Action(p);
    }
    else if (p->_pin == pumpState_pin) {
        pumpState_Action(p);
    }


    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->_currentPressLevel = NO_PRESS;
    p->_lastPressedLevel = NO_PRESS;
    p->_pressDuration = 0;
    p->_maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.
}




#endif