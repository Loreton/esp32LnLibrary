//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 16.48.14
//
#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "lnLogger.h"



#include "ButtonLongPress_Class.h"
#include "LedController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "callBackFunctions.h" // per functions protoype





extern LedController_Struct activeBuzzer;
LedController_Struct *buzzer2 = &activeBuzzer;




//###########################################################################
//# richiamata quando il pulsante è premuto
//###########################################################################
#define ALARM_BEEP_INTERVAL 2000
void startButtonNotificationHandlerCB(ButtonLongPress_Class* p) {
    uint16_t beep_duration=200;
    static uint32_t lastBeepTime;
    uint32_t next_interval;
    uint32_t elapsed;
    uint16_t phase_beep_duration;

    if (p->m_currentPressLevel != p->m_lastPressedLevel) {
        elapsed = millis() - p->m_pressStartTime;
        next_interval = p->m_gapThresholds[p->m_currentPressLevel];
        LOG_INFO("[%s] Pressione in corso (ms:%06ld)", p->m_pinID, elapsed);

        LOG_INFO("[%s] PRESSED_LEVEL_%d/%d - elapsed ms:%6lu", p->m_pinID, p->m_currentPressLevel, p->m_numThresholds, elapsed);

        phase_beep_duration = next_interval / 5; // arbitrario.... facciamo un beep che è u1/5 del next_threshold time
        LOG_INFO("[%s] next_interval: %lu - beep_duration: %lu", p->m_pinID, next_interval, phase_beep_duration);
        switch (p->m_currentPressLevel) {
            case PRESSED_LEVEL_1:
            case PRESSED_LEVEL_2:
            case PRESSED_LEVEL_3:
            case PRESSED_LEVEL_4:
            case PRESSED_LEVEL_5:
            case PRESSED_LEVEL_6:
            case PRESSED_LEVEL_7:
            case PRESSED_LEVEL_8:
            case PRESSED_LEVEL_9:
                buzzer2->pulse(beep_duration);
                break;

            default:
                LOG_INFO("sono nel default: %d", p->m_currentPressLevel);
                break;
        }
        p->m_lastPressedLevel = p->m_currentPressLevel;
    }
    // --- LOGICA DEL BEEP OGNI 5 SECONDI quando si raggiunge il MAX-LEVEL---
    if (p->m_maxLevelReachedAndNotified ) {
        if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
            buzzer2->pulse(1000);
            lastBeepTime = millis();
        }
    }

}



//###########################################################################
//# richiamata quando il pulsante viene rilasciato
//###########################################################################
void startButtonHandlerCB(ButtonLongPress_Class *p) {
    static bool relayState = false;
    switch (p->m_currentPressLevel) {
        case PRESSED_LEVEL_1:
            LOG_DEBUG("PRESSED_LEVEL_1");
            break;

        case PRESSED_LEVEL_2:
            LOG_DEBUG("PRESSED_LEVEL_2");
            // pressControlRelay.toggle();
            // relayState = !relayState;
            // if (relayState) {
            //     digitalWrite(pressControlRelay_pin, LOW);
            //     LOG_INFO("  --> Relè ACCESO!");
            // } else {
            //     digitalWrite(pressControlRelay_pin, HIGH);
            //     LOG_INFO("  --> Relè SPENTO!");
            // }
            break;

        case PRESSED_LEVEL_3:
            LOG_DEBUG("PRESSED_LEVEL_3");
            break;

        case PRESSED_LEVEL_4:
            LOG_DEBUG("PRESSED_LEVEL_4");
            break;

        default:
            LOG_DEBUG("Sconosciuto/Non Qualificato");
            break;
    }

    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->reset();

}

#endif