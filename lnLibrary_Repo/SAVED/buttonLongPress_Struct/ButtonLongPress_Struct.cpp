//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 14.15.27
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// #include "lnLogger_Class.h"
// #include "lnTime_Class.h" // mSecTo_HHMMSS(

#include "lnGlobalVars.h"
#include "lnSerialRead.h" // waitForEnter()
#include "lnSetPinID.h" // waitForEnter()

#include "ButtonLongPress_Struct.h"


// costruttore
ButtonLongPress_Struct::ButtonLongPress_Struct(void) {}

/**
 * @brief Inizializza un pulsante configurando il pin e lo stato iniziale.
 *
 * @param btn::                 Riferimento alla struttura ButtonState_t del pulsante.
 * @param pin:                  Il numero del pin GPIO a cui è collegato il pulsante.
 * @param name:                 Una stringa che identifica il pulsante (es. "Button1").
 * @param pressedLogicLevel:    Il livello logico (LOW o HIGH) che indica che il pulsante è premuto.
 *                                  - Usa LOW per un pulsante collegato tra pin e GND (con INPUT_PULLUP).
 *                                  - Usa HIGH per un pulsante collegato tra pin e VCC (con INPUT, richiede pull-down esterno).
 * @param thresholds:           Array di uint32_t contenente le soglie di tempo (in ms) per i livelli di pressione.
 *                                  Le soglie DEVONO essere in ordine crescente. L'indice 0 corrisponde a SHORT_PRESS, ecc.
 * @param thresholdsCount:      Il numero di elementi nell'array `thresholds`.
*/
void ButtonLongPress_Struct::init(const char* name, uint8_t pin,
                                             bool pressedLogicLevel,
                                             const uint32_t thresholds[],
                                             int8_t thresholdsCount) {
    m_pin = pin;
    m_name = name;
    // snprintf(m_pinID, 20, "[%02d.%-14s]:", m_pin, m_name);
    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

    m_numThresholds = (thresholdsCount > MAX_DEFINED_PRESS_LEVELS) ? MAX_DEFINED_PRESS_LEVELS : thresholdsCount;

    m_pressedLogicLevel = pressedLogicLevel;
    m_pressThresholds = thresholds;

    if (pressedLogicLevel == LOW) {
        pinMode(m_pin, INPUT_PULLUP);
    } else {
        pinMode(m_pin, INPUT);
    }

    // calcolo del nextGAP.
    for (int8_t i = 0; i<m_numThresholds-1; i++) {
        m_gapThresholds[i] = m_pressThresholds[i+1] - m_pressThresholds[i];
        m_gapThresholds[i+1] = m_gapThresholds[i];
    }

    m_lastButtonState =  digitalRead(m_pin) == m_pressedLogicLevel;
    m_buttonPressed = (m_lastButtonState == m_pressedLogicLevel);
    // ext_showStatus(this);
    LOG_NOTIFY("[%s] initialized. active level: %s", m_pinID, str_pinLevel[m_pressedLogicLevel]);

}






// *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
void ButtonLongPress_Struct::reset(void) {
    m_currentPressLevel = NO_PRESS;
    m_lastPressedLevel = NO_PRESS;
    // m_pressDuration = 0;
    m_elapsed = 0;
    m_maxLevelReachedAndNotified = false;
}





// ##############################################
// Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo,
// aggiorna il livello di pressione.
// ##############################################
void ButtonLongPress_Struct::_checkNewLevel() {
    static char timestamp[16]; // Buffer statico per il timestamp
    static uint32_t lastDisplayTime = 0;
    int8_t newLevel = NO_PRESS;

    m_elapsed = millis() - m_pressStartTime;

    for (int8_t i = m_numThresholds - 1; i >= 0; i--) {
        if (m_elapsed >= m_pressThresholds[i]) {
            newLevel = i + 1;
            break;
        }
    }

    if (newLevel > m_currentPressLevel && newLevel <= m_numThresholds) {
        m_currentPressLevel = newLevel;
    }

    // notifica elapsed ogni minuto....
    if (m_elapsed - lastDisplayTime > 60000) {
        lastDisplayTime = m_elapsed;
        uint32_t ms_to_next_level = m_pressThresholds[m_currentPressLevel] - m_pressThresholds[m_currentPressLevel-1] - m_elapsed;

        LOG_INFO("[%s]:", m_pinID);
        LOG_INFO("\tPRESSED_LEVEL    %d/%d", m_currentPressLevel, m_numThresholds);
        LOG_NOTIFY("\telapsed:            %s", lnTime.mSecTo_HHMMSSms(m_elapsed));
        LOG_NOTIFY("\ttime to next level: %s", lnTime.mSecTo_HHMMSSms(ms_to_next_level));


        // ext_showStatus(this);
    }
}





/* NO bouncing extra, usa il primo livello di threshold come tale */
bool ButtonLongPress_Struct::read(ButtonLongPressCallback onPressCallback) {
    return released(onPressCallback);
} // DA ELIMINARE

/* NO bouncing extra, usa il primo livello di threshold come tale */
bool ButtonLongPress_Struct::released(ButtonLongPressCallback onPressCallback) {
    bool currentState = digitalRead(m_pin);

    // State change detection (simple edge detection)
    if (currentState != m_buttonPressed) {
        // Update debounced state
        m_buttonPressed = currentState;

        // If the button has just been PRESSED (debounced state is now pressedLogicLevel).
        if (m_buttonPressed == m_pressedLogicLevel) {
            m_pressStartTime = millis(); // Record the press start time.
            // LOG_INFO("[%s] MAX press level reached: %d\n", m_name, m_currentPressLevel);
            // DO NOT reset level parameters here; the caller will reset after processing the *previous* release.
        }
        // If the button has just been RELEASED (debounced state is now different from pressedLogicLevel).
        else {
            // Calculate press duration only if it was actually pressed.
            if (m_pressStartTime != 0) {
                // m_pressDuration = millis() - m_pressStartTime;
                m_elapsed = millis() - m_pressStartTime;
                m_pressStartTime = 0; // Reset the press start timer.

                // usiamo il primo livello come bouncing
                // if (m_pressDuration > m_pressThresholds[0]) {
                if (m_elapsed > m_pressThresholds[0]) {
                    if (onPressCallback) {                      // Execute the callback if set and the button was pressed.
                        onPressCallback(this);                  // Pass 'this' (pointer to the current object)
                    }

                    // DO NOT reset level parameters here; the caller will reset after reading them.
                    return true; // The button was released.
                }
                else {
                    reset();
                    return false;
                }
            }
        }
    }

    // If the button is currently PRESSED (debounced) and the timer is active, update the press level.
    if (m_buttonPressed == m_pressedLogicLevel && m_pressStartTime != 0) {
        _checkNewLevel();

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
void ButtonLongPress_Struct::pressingLevelNotification(ButtonLongPressCallback onPressCallback) {
    // vedi pressingLevelNotification_SAMPLE() com esempio
    static uint32_t lastMsgTime;
    uint32_t next_interval;
    // uint32_t elapsed;
    uint32_t phase_beep_duration;

    if (m_buttonPressed == m_pressedLogicLevel) {
        if (m_currentPressLevel != NO_PRESS) {
            if (onPressCallback) {
                onPressCallback(this);
            }
            else {
                if (m_currentPressLevel != m_lastPressedLevel) {
                    next_interval = m_gapThresholds[m_currentPressLevel];

                    // elapsed = millis() - m_pressStartTime;
                    LOG_NOTIFY("[%s] PRESSED_LEVEL %d/%d - elapsed ms:%6lu - next_interval: %lu",
                                    m_pinID,
                                    m_currentPressLevel,
                                    m_numThresholds,
                                    m_elapsed,
                                    next_interval);

                    m_lastPressedLevel = m_currentPressLevel;
                }

                // --- LOGICA DEL BEEP OGNI 5 SECONDI quando si raggiunge il MAX-LEVEL---
                #define ALARM_BEEP_INTERVAL 2000
                if (m_maxLevelReachedAndNotified ) {
                    if (millis() - lastMsgTime >= ALARM_BEEP_INTERVAL) {
                        LOG_WARNING("[%s] ALARM! max pressed level %d reached", m_pinID, m_currentPressLevel);
                        lastMsgTime = millis();
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
void ButtonLongPress_Struct::showStatus(void) {
    ext_showStatus(this);
}




//######################################################
//#   External Functions
//#   External Functions
//#   External Functions
//#   External Functions
//######################################################


//###########################################################################
//#
//###########################################################################
void ext_showStatus(ButtonLongPress_Struct *p) {
    LOG_INFO("%s", p->m_pinID);
    const char *TAB="    ";
    LOG_INFO("%s%-18s: %2d - %s", TAB,  "m_pressedLogicLevel",          p->m_pressedLogicLevel,           str_pinLevel[p->m_pressedLogicLevel]);
    LOG_INFO("%s%-18s: %2d - %s", TAB,  "m_buttonPressed",              p->m_buttonPressed,               str_TrueFalse[p->m_buttonPressed]);
    LOG_INFO("%s%-18s: %2d - %s", TAB,  "m_maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);
    LOG_INFO("%s%-18s: %2d - %s", TAB,  "m_lastButtonState",            p->m_lastButtonState,             str_OnOff[p->m_lastButtonState]);
    LOG_INFO("%s%-18s: %2d - %s", TAB,  "m_lastPressedLevel",           p->m_lastPressedLevel,            str_OnOff[p->m_lastPressedLevel]);

    LOG_INFO("%s%-18s: %2d",  TAB,      "m_currentPressLevel",          p->m_currentPressLevel);
    LOG_INFO("%s%-18s: %lu",  TAB,      "m_pressStartTime",             p->m_pressStartTime);
    // LOG_INFO("%s%-18s: %lu",  TAB,     "m_pressDuration",              p->m_pressDuration);
    LOG_INFO("%s%-18s: %lu",  TAB,      "m_elapsed",                    p->m_elapsed);


    LOG_INFO("%snum_thresholds: %2d" , TAB,  p->m_numThresholds);
    for (int8_t j=0; j < p->m_numThresholds; j++) {
        LOG_INFO("%sthreshold[%d]: %lu", TAB, j, p->m_pressThresholds[j]);
    }

    LOG_INFO(""); // blank line

}




#if 0
// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
//###########################################################################
//#
//###########################################################################
void pressingLevelNotification_SAMPLE(ButtonLongPressCallback callback) {
    static uint32_t lastBeepTime;
    uint32_t next_interval;
    uint32_t elapsed;
    uint32_t phase_beep_duration;

    if (b->m_buttonPressed == b->m_pressedLogicLevel) {
        if (b->m_currentPressLevel != NO_PRESS) {
            if (b->m_currentPressLevel != b->m_lastPressedLevel) {
                elapsed = millis() - b->m_pressStartTime;

                // calcolo per gestione durata beep (se richiesto...)
                next_interval = b->m_gapThresholds[b->m_currentPressLevel];
                // phase_beep_duration = next_interval / 5 * b->m_currentPressLevel; // arbitrario.... facciamo un beep che è u1/5 del next_threshold time
                phase_beep_duration = 1000 * b->m_currentPressLevel; // arbitrario....
                LOG_NOTIFY("[%s] PRESSED_LEVEL %d/%d - elapsed ms:%6lu - next_interval: %lu - beep_duration: %lu",
                                b->m_pinID,
                                b->m_currentPressLevel,
                                b->m_numThresholds,
                                elapsed,
                                next_interval,
                                phase_beep_duration);

                switch (b->m_currentPressLevel) {
                    case PRESSED_LEVEL_1:
                    case PRESSED_LEVEL_2:
                    case PRESSED_LEVEL_3:
                    case PRESSED_LEVEL_4:
                    case PRESSED_LEVEL_5:
                    case PRESSED_LEVEL_6:
                    case PRESSED_LEVEL_7:
                    case PRESSED_LEVEL_8:
                    case PRESSED_LEVEL_9:
                        if (callback) {
                            callback(this, phase_beep_duration);
                        }
                        break;

                    default:
                        LOG_INFO("sono nel default: %d", b->m_currentPressLevel);
                        break;
                }
                b->m_lastPressedLevel = b->m_currentPressLevel;
            }
            // --- LOGICA DEL BEEP OGNI 5 SECONDI quando si raggiunge il MAX-LEVEL---
            #define ALARM_BEEP_INTERVAL 2000
            if (b->m_maxLevelReachedAndNotified ) {
                if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
                    if (callback) {
                        callback(this, 1000);
                    }
                    LOG_WARNING("[%s] ALARM! max pressed level %d reached", b->m_pinID, b->m_currentPressLevel);
                    lastBeepTime = millis();
                }
            }
        }

    } else { // (b->m_buttonPressed != b->m_pressedLogicLevel)
        b->m_lastPressedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
    }
}

#endif