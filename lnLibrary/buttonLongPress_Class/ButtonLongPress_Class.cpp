//
// updated by ...: Loreto Notarantonio
// Date .........: 05-08-2025 11.40.16
//

#include <Arduino.h>

#include "lnGlobalVars.h" // Assicurati che questi siano disponibili
#include "lnSerialRead.h" // Assicurati che questi siano disponibili
#include "lnSetPinID.h"   // Assicurati che questi siano disponibili

#include "ButtonLongPress_Class.h" // Includi il nuovo header della classe

// Costruttore della classe.
// Utilizza la lista di inizializzazione per i membri.
ButtonLongPress_Class::ButtonLongPress_Class(void) {};



/**
 * @brief Inizializza un pulsante configurando il pin e lo stato iniziale.
 *
 * @param name:              Una stringa che identifica il pulsante (es. "Button1").
 * @param pin:               Il numero del pin GPIO a cui è collegato il pulsante.
 * @param pressedLogicLevel: Il livello logico (LOW o HIGH) che indica che il pulsante è premuto.
 * @param thresholds:        Array di uint32_t contenente le soglie di tempo (in ms) per i livelli di pressione.
 * @param thresholdsCount:   Il numero di elementi nell'array `thresholds`.
 */
void ButtonLongPress_Class::init(const char* name, uint8_t pin,
                                 bool pressedLogicLevel,
                                 const uint32_t thresholds[],
                                 int8_t thresholdsCount) {
    m_pin = pin;
    m_name = name;
    setPinID(m_pinID, sizeof(m_pinID) - 1, m_name, m_pin);

    m_numThresholds = (thresholdsCount > MAX_DEFINED_PRESS_LEVELS) ? MAX_DEFINED_PRESS_LEVELS : thresholdsCount;

    m_pressedLogicLevel = pressedLogicLevel;
    m_pressThresholds = thresholds;

    if (pressedLogicLevel == LOW) {
        pinMode(m_pin, INPUT_PULLUP);
    } else {
        pinMode(m_pin, INPUT);
    }

    // calcolo del nextGAP.
    // for (int8_t i = 0; i < m_numThresholds - 1; i++) {
    //     m_gapThresholds[i] = m_pressThresholds[i+1] - m_pressThresholds[i];
    // }
    // L'ultimo elemento può essere impostato all'ultimo gap per consistenza,
    // o gestito separatamente se non c'è un "gap successivo" definito.
    // if (m_numThresholds > 0) {
    //     m_gapThresholds[m_numThresholds - 1] = 0; // O un valore predefinito
    // }


    m_lastButtonState = digitalRead(m_pin) == m_pressedLogicLevel;
    m_buttonPressed = (m_lastButtonState == m_pressedLogicLevel);
    LOG_TRACE("[%s] initialized. active level: %s", m_pinID, str_pinLevel[m_pressedLogicLevel]);
}

// *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
void ButtonLongPress_Class::reset(void) {
    if (m_currentPressLevel != NO_PRESS) {
        LOG_INFO("[%s] has been released...(at level:%d)", m_pinID, m_currentPressLevel);
    }
    m_currentPressLevel = NO_PRESS;
    m_lastPressedLevel = NO_PRESS;
    m_elapsed = 0;
    m_maxLevelReachedAndNotified = false;
}






// ##############################################
// Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo,
// aggiorna il livello di pressione.
// ##############################################
void ButtonLongPress_Class::updatePressedLevel() {
    int8_t newLevel = NO_PRESS;


    for (int8_t i = m_numThresholds - 1; i >= 0; i--) {
        if (m_elapsed >= m_pressThresholds[i]) {
            newLevel = i + 1;
            break;
        }
    }

    if (newLevel > m_currentPressLevel && newLevel <= m_numThresholds) {
        m_lastPressedLevel = m_currentPressLevel;
        m_currentPressLevel = newLevel;
        LOG_DEBUG("[%s]: new pressed Level %d", m_pinID, newLevel);
        m_levelHasChanged=true;
    }


    displayPressedLevel();

}




//###########################################################################
//#
//###########################################################################
bool ButtonLongPress_Class::pressedLevelHasChanged(void) {
    if (m_levelHasChanged) {
        m_levelHasChanged = false;
        return true;
    }
    return false;
}





//###########################################################################
//#
//###########################################################################
void ButtonLongPress_Class::displayPressedLevel(bool forceDisplay) {

    m_elapsed = millis() - m_pressStartTime; // update continuo


    #define PRESSING_DISPLAY_TIME 60*1000
    if (forceDisplay || m_levelHasChanged || (m_elapsed - m_lastDisplayTime > PRESSING_DISPLAY_TIME) ) {
        uint32_t nextGAP_ms;
        uint32_t msToNextLevel;

        m_lastDisplayTime = m_elapsed;

        // Controlla l'indice prima di accedere all'array m_gapThresholds
        if (m_currentPressLevel > 0 && m_currentPressLevel < m_numThresholds) {
            uint8_t level = m_currentPressLevel;

            if (level<m_numThresholds-1) {
                nextGAP_ms = m_pressThresholds[level+1]-m_pressThresholds[level];
            } else if (level>=m_numThresholds-1) {
                nextGAP_ms = m_pressThresholds[level]-m_pressThresholds[level-1]; // calcoliamo sempre l'ultimo
            }

            // * calcola next pressing level GAP
            msToNextLevel = (m_elapsed > nextGAP_ms) ? (m_elapsed - nextGAP_ms) : (nextGAP_ms - m_elapsed);

            // * calcola and display elapsed time
            char elapsedBUFFER[16];   lnTime.timeStamp(elapsedBUFFER,   sizeof(elapsedBUFFER),   m_elapsed, false);
            char nextLevelBUFFER[16]; lnTime.timeStamp(nextLevelBUFFER, sizeof(nextLevelBUFFER), msToNextLevel, false);

            //* display
            LOG_NOTIFY("[%s]:", m_pinID);
            LOG_NOTIFY("\tpressed Level   %d/%d", m_currentPressLevel, m_numThresholds);
            LOG_TRACE( "\tthreshold[%d]:  %8lu", level,  m_pressThresholds[level]);
            LOG_TRACE( "\tthreshold[%d]:  %8lu", level+1,  m_pressThresholds[level+1] );
            LOG_TRACE( "\tGAP:            %8lu", nextGAP_ms );
            LOG_NOTIFY("\telapsed:        %8lu - [hms]: %s", m_elapsed, elapsedBUFFER);
            LOG_NOTIFY("\tnext level in:  %8lu - [hms]: %s", msToNextLevel, nextLevelBUFFER);

        }
    }

}







/* NO bouncing extra, usa il primo livello di threshold come tale */
bool ButtonLongPress_Class::read(ButtonLongPressCallback onPressCallback) {
    return released(onPressCallback);
}

/* NO bouncing extra, usa il primo livello di threshold come tale */
bool ButtonLongPress_Class::released(ButtonLongPressCallback onPressCallback) {
    bool currentState = digitalRead(m_pin);

    // State change detection (simple edge detection)
    if (currentState != m_buttonPressed) {
        // Update debounced state
        m_buttonPressed = currentState;

        // If the button has just been PRESSED (debounced state is now pressedLogicLevel).
        if (m_buttonPressed == m_pressedLogicLevel) {
            m_pressStartTime = millis(); // Record the press start time.
            // DO NOT reset level parameters here; the caller will reset after processing the *previous* release.
        }
        // If the button has just been RELEASED (debounced state is now different from pressedLogicLevel).
        else {
            // Calculate press duration only if it was actually pressed.
            if (m_pressStartTime != 0) {
                m_elapsed = millis() - m_pressStartTime;
                m_pressStartTime = 0; // Reset the press start timer.

                // usiamo il primo livello come bouncing
                if (m_elapsed > m_pressThresholds[0]) {
                    if (onPressCallback) {
                        onPressCallback(this); // Pass 'this' (pointer to the current object)
                    }
                    return true; // The button was released.
                } else {
                    reset(); // Reset state if press was too short (bouncing)
                    return false;
                }
            }
        }
    }

    // If the button is currently PRESSED (debounced) and the timer is active, update the press level.
    if (m_buttonPressed == m_pressedLogicLevel && m_pressStartTime != 0) {
        updatePressedLevel();

        if (m_currentPressLevel == static_cast<uint8_t>(m_numThresholds) && !m_maxLevelReachedAndNotified) {
            LOG_INFO("[%s] MAX press level reached: %d\n", m_name, m_currentPressLevel);
            m_maxLevelReachedAndNotified = true;
        }
    }

    m_lastButtonState = currentState; // Update last raw state for the next cycle.
    return false;
}





// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
//###########################################################################
//#
//###########################################################################
void ButtonLongPress_Class::pressingLevelNotification(ButtonLongPressCallback onPressCallback) {
    // static uint32_t lastMsgTime = 0; // Anche qui, considera se deve essere un membro `m_lastMsgTime` // per essere per-istanza.

    uint32_t next_interval;
    uint32_t phase_beep_duration; // Variabile non utilizzata nell'implementazione attuale senza buzzer

    if (m_buttonPressed == m_pressedLogicLevel) {
        if (m_currentPressLevel != NO_PRESS) {
            if (onPressCallback) {
                onPressCallback(this);
            }
            else {
                displayPressedLevel();  // per permettere di richiamarlo dall'esterno....

                // --- LOGICA DEL BEEP OGNI 5 SECONDI quando si raggiunge il MAX-LEVEL---
                #define ALARM_BEEP_INTERVAL 2000
                if (m_maxLevelReachedAndNotified) {
                    if (millis() - m_lastMsgTime >= ALARM_BEEP_INTERVAL) {
                        LOG_WARNING("[%s] ALARM! max pressed level %d reached", m_pinID, m_currentPressLevel);
                        m_lastMsgTime = millis();
                    }
                }
            } // end else callback
        }

    } else { // (m_buttonPressed != m_pressedLogicLevel)
        m_lastPressedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
    }
}

//###########################################################################
//#
//###########################################################################
void ButtonLongPress_Class::showStatus(void) {
    // ext_showStatus(this);
    LOG_INFO("%s", m_pinID);
    const char *TAB="    ";
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_pressedLogicLevel", m_pressedLogicLevel, str_pinLevel[m_pressedLogicLevel]);
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_buttonPressed", m_buttonPressed, str_TrueFalse[m_buttonPressed]);
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_maxLevelReachedAndNotified", m_maxLevelReachedAndNotified, str_TrueFalse[m_maxLevelReachedAndNotified]);
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_lastButtonState", m_lastButtonState, str_OnOff[m_lastButtonState]);
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_lastPressedLevel", m_lastPressedLevel, str_OnOff[m_lastPressedLevel]);

    LOG_INFO("%s%-18s: %2d", TAB, "m_currentPressLevel", m_currentPressLevel);
    LOG_INFO("%s%-18s: %lu", TAB, "m_pressStartTime", m_pressStartTime);
    LOG_INFO("%s%-18s: %lu", TAB, "m_elapsed", m_elapsed);

    LOG_INFO("%snum_thresholds: %2d", TAB, m_numThresholds);
    for (int8_t j = 0; j < m_numThresholds; j++) {
        uint32_t next_gap = (j < m_numThresholds-1) ? (m_pressThresholds[j+1]-m_pressThresholds[j]) : 0;
        LOG_INFO("%sthreshold[%d]: %-lu - next GAP: %-ld", TAB, j, m_pressThresholds[j], next_gap );
    }
    LOG_INFO(""); // blank line
}

//######################################################
//#    External Functions
//######################################################

//###########################################################################
//#
//###########################################################################
void ext_showStatus(ButtonLongPress_Class *p) {
    LOG_INFO("%s", p->pinID());
    #if 0
    const char *TAB="    ";
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_pressedLogicLevel", p->m_pressedLogicLevel, str_pinLevel[p->m_pressedLogicLevel]);
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_buttonPressed", p->m_buttonPressed, str_TrueFalse[p->m_buttonPressed]);
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified, str_TrueFalse[p->m_maxLevelReachedAndNotified]);
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_lastButtonState", p->m_lastButtonState, str_OnOff[p->m_lastButtonState]);
    LOG_INFO("%s%-18s: %2d - %s", TAB, "m_lastPressedLevel", p->m_lastPressedLevel, str_OnOff[p->m_lastPressedLevel]);

    LOG_INFO("%s%-18s: %2d", TAB, "m_currentPressLevel", p->m_currentPressLevel);
    LOG_INFO("%s%-18s: %lu", TAB, "m_pressStartTime", p->m_pressStartTime);
    LOG_INFO("%s%-18s: %lu", TAB, "m_elapsed", p->m_elapsed);

    LOG_INFO("%snum_thresholds: %2d", TAB, p->m_numThresholds);
    for (int8_t j = 0; j < p->m_numThresholds; j++) {
        LOG_INFO("%sthreshold[%d]: %lu", TAB, j, p->m_pressThresholds[j]);
    }
    LOG_INFO(""); // blank line
    #endif
}