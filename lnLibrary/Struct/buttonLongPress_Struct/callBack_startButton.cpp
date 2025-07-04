//
// updated by ...: Loreto Notarantonio
// Date .........: 04-07-2025 16.03.58
//
#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros2.h"



#include "@buttonLongPress_Struct.h"
#include "@pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "callBackFunctions.h" // per functions protoype





extern pinController_Struct activeBuzzer;
pinController_Struct *buzzer = &activeBuzzer;





#define ALARM_BEEP_INTERVAL 2000
void startButtonNotificationHandlerCB(ButtonLongPress_Struct* p) {
    uint16_t beep_duration=200;
    static uint32_t lastBeepTime;

    if (p->m_currentPressLevel != p->m_lastPressedLevel) {
        LOG_INFO("[%s] Pressione in corso (ms:%06ld)", p->m_pinID, (millis() - p->m_pressStartTime));
        switch (p->m_currentPressLevel) {
            case PRESSED_LEVEL_1:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;


            case PRESSED_LEVEL_2:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_3:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_4:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_5:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_6:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_7:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_8:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
                break;

            case PRESSED_LEVEL_9:
                LOG_DEBUG("notify PRESSED_LEVEL: %d", p->m_currentPressLevel);
                buzzer->pulse(beep_duration);
                // notifyBuzzer(buzzer);
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
            buzzer->pulse(1000);
            lastBeepTime = millis();
        }
    }

}


//###########################################################################
//#
//###########################################################################
void startButtonHandlerCB(ButtonLongPress_Struct *p) {
    static bool relayState = false;
    switch (p->m_currentPressLevel) {
        case PRESSED_LEVEL_1:
            LOG_DEBUG("PRESSED_LEVEL_1");
            break;

        case PRESSED_LEVEL_2:
            LOG_DEBUG("PRESSED_LEVEL_2");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(pressControlRelay_pin, LOW);
                LOG_INFO("  --> Relè ACCESO!");
            } else {
                digitalWrite(pressControlRelay_pin, HIGH);
                LOG_INFO("  --> Relè SPENTO!");
            }
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

    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->m_currentPressLevel = NO_PRESS;
    p->m_lastPressedLevel = NO_PRESS;
    p->m_pressDuration = 0;
    p->m_maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.

}


#if 0

   uint16_t beep_duration = 300;
    uint16_t phase_beep_duration;
    uint32_t next_interval;
    uint32_t elapsed;

    // Check if the button is currently pressed according to its logic level
    if (m_buttonPressed == m_pressedLogicLevel) {
        // If a press level has been reached (not NO_PRESS)
        if (m_currentPressLevel != NO_PRESS) {
            // If the current level is different from the last notified level, print and beep
            if (m_currentPressLevel != m_lastPressedLevel) {
                elapsed = millis() - m_pressStartTime;
                next_interval = m_pressThresholds[m_currentPressLevel+1] - m_pressThresholds[m_currentPressLevel];
                if (m_currentPressLevel < m_numThresholds) {
                    next_interval = m_pressThresholds[m_currentPressLevel] - m_pressThresholds[m_currentPressLevel-1];
                }
                LOG_INFO("[%s] elapsed ms:%6ld - PRESSED_LEVEL_%d/%d\n", m_pinID, elapsed, m_currentPressLevel, m_numThresholds);
                phase_beep_duration = next_interval / 5;
                LOG_INFO("[%s] next_interval: %lu - beep_duration: %lu\n", m_pinID, next_interval, phase_beep_duration);

                // per evitare che il beep sia più lungo del tempo del livello
                // phase_beep_duration = beep_duration*m_currentPressLevel;
                    // if (phase_beep_duration > next_interval) {
                    // phase_beep_duration = beep_duration*m_currentPressLevel-1;
                    // }
                    // LOG_INFO("[%s] next interval in: %ld ms \n", m_pinID, m_currentPressLevel, next_interval);
                // }
                // LOG_INFO("[%s] ms:%6ld - PRESSED_LEVEL_%d  next level in: %ld ms\n", (millis() - m_pressStartTime), m_pinID, m_currentPressLevel, next_interval);
                switch (m_currentPressLevel) {
                    case PRESSED_LEVEL_1:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_2:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_3:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_4:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_5:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_6:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_7:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_8:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_9:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    default:
                        LOG_INFO("sono nel default: %d\n", m_currentPressLevel);
                        break;
                }
                m_lastPressedLevel = m_currentPressLevel; // Update the last notified level
            }

            // --- BEEP LOGIC EVERY ALARM_BEEP_INTERVAL ---
            if (m_maxLevelReachedAndNotified) {
                if (millis() - m_lastBeepTime >= ALARM_BEEP_INTERVAL) {
                    if (buzzer) buzzer->pulse(1000);
                    m_lastBeepTime = millis();
                }
            }
        }
    } else { // Button is not pressed or has been released
        m_lastPressedLevel = NO_PRESS; // Reset for the next press cycle
    }
}



#endif
#endif