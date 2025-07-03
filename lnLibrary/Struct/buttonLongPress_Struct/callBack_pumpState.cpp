//
// updated by ...: Loreto Notarantonio
// Date .........: 03-07-2025 17.39.43
//
#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()



#include "@buttonLongPress_Struct.h"
#include "@pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "callBackFunctions.h" // per functions protoype





extern pinController_Struct activeBuzzer;
pinController_Struct *buzzer1 = &activeBuzzer;



#define ALARM_BEEP_INTERVAL 2000

void pumpStateNotificationHandlerCB(ButtonLongPress_Struct* p) {
    uint16_t beep_duration=200;
    static uint32_t lastBeepTime;

    if (p->m_currentPressLevel != p->m_lastPressedLevel) {
        printf0_FN("[%s] Pressione in corso (ms:%06ld)\n", p->m_pinID, (millis() - p->m_pressStartTime));
        switch (p->m_currentPressLevel) {
            case PRESSED_LEVEL_1:
                printf2_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;


            case PRESSED_LEVEL_2:
                printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_3:
                printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_4:
                printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_5:
                printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_6:
                printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_7:
                printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_8:
                printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_9:
                printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                buzzer1->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            default:
                printf0_FN("sono nel default: %d\n", p->m_currentPressLevel);
                break;
        }
        p->m_lastPressedLevel = p->m_currentPressLevel;
    }
    // --- LOGICA DEL BEEP OGNI 5 SECONDI quando si raggiunge il MAX-LEVEL---
    if (p->m_maxLevelReachedAndNotified ) {
        if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
            buzzer1->pulse(1000);
            lastBeepTime = millis();
        }
    }

}



//###########################################################################
//#
//###########################################################################
void pumpStateHandlerCB(ButtonLongPress_Struct *p) {
    static bool relayState = false;
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

    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->m_currentPressLevel = NO_PRESS;
    p->m_lastPressedLevel = NO_PRESS;
    p->m_pressDuration = 0;
    p->m_maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.

}

#endif