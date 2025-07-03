//
// updated by ...: Loreto Notarantonio
// Date .........: 02-07-2025 13.42.52
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__
#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()
// #include "@globalVars.h" // printf:XFN()

#include "@buttonLongPress_Struct.h"

#include "@main_test.h"



//###########################################################################
//#
//###########################################################################
bool relayState = false;
void pumpState_Action(ButtonLongPress_Struct *p) {
    switch (p->m_currentPressLevel) {
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
void startButton_Action(ButtonLongPress_Struct *p) {
    // printf99_FN("sono qui p->m_currentPressLevel %d\n", p->m_currentPressLevel);
    switch (p->m_currentPressLevel) {
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
void processButton(ButtonLongPress_Struct *p) {
    printf0_FN("[%s[  Rilasciato! Durata: %ld ms\n", p->m_name, p->m_pressDuration);
    // Serial.print("Livello finale raggiunto: ");

    if (p->m_pin == startButton_pin) {
        Serial.print("start button action\n");
        startButton_Action(p);
    }
    else if (p->m_pin == pumpState_pin) {
        pumpState_Action(p);
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