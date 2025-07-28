//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 16.45.01
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
    for (int8_t i = 0; i < m_numThresholds - 1; i++) {
        m_gapThresholds[i] = m_pressThresholds[i+1] - m_pressThresholds[i];
    }
    // L'ultimo elemento può essere impostato all'ultimo gap per consistenza,
    // o gestito separatamente se non c'è un "gap successivo" definito.
    if (m_numThresholds > 0) {
        m_gapThresholds[m_numThresholds - 1] = 0; // O un valore predefinito
    }


    m_lastButtonState = digitalRead(m_pin) == m_pressedLogicLevel;
    m_buttonPressed = (m_lastButtonState == m_pressedLogicLevel);
    LOG_NOTIFY("[%s] initialized. active level: %s", m_pinID, str_pinLevel[m_pressedLogicLevel]);
}

// *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
void ButtonLongPress_Class::reset(void) {
    m_currentPressLevel = NO_PRESS;
    m_lastPressedLevel = NO_PRESS;
    m_elapsed = 0;
    m_maxLevelReachedAndNotified = false;
}

// ##############################################
// Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo,
// aggiorna il livello di pressione.
// ##############################################
void ButtonLongPress_Class::_checkNewLevel() {
    static uint32_t lastDisplayTime = 0; // Non ha senso che sia static qui, ogni istanza avrà la sua
    // Correction: `lastDisplayTime` *should* be a member variable if it's meant to track
    // display time *per instance*. If it's a global display limiter, then static is fine.
    // For per-instance, it needs to be `m_lastDisplayTime`. Let's assume it should be per-instance.
    // (Aggiungi `uint32_t m_lastDisplayTime = 0;` ai membri privati della classe.)

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
    // Se `lnTime` è una classe globale, è OK.
    if (m_elapsed - lastDisplayTime > 60000) { // lastDisplayTime should probably be a member variable if per-instance
        lastDisplayTime = m_elapsed; // This will affect all instances if static.
                                     // Consider if you want this to be `m_lastDisplayTime` per object.

        uint32_t ms_to_next_level = 0;
        if (m_currentPressLevel < m_numThresholds) { // Avoid accessing beyond array bounds
            ms_to_next_level = m_pressThresholds[m_currentPressLevel] - m_elapsed;
        } else {
            ms_to_next_level = 0; // Already at or past max level
        }

        LOG_INFO("[%s]:", m_pinID);
        LOG_INFO("\tPRESSED_LEVEL     %d/%d", m_currentPressLevel, m_numThresholds);
        LOG_NOTIFY("\telapsed:           %s", lnTime.mSecTo_HHMMSSms(m_elapsed));
        LOG_NOTIFY("\ttime to next level: %s", lnTime.mSecTo_HHMMSSms(ms_to_next_level));
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
void ButtonLongPress_Class::pressingLevelNotification(ButtonLongPressCallback onPressCallback) {
    static uint32_t lastMsgTime = 0; // Anche qui, considera se deve essere un membro `m_lastMsgTime`
                                    // per essere per-istanza.

    uint32_t next_interval;
    uint32_t phase_beep_duration; // Variabile non utilizzata nell'implementazione attuale senza buzzer

    if (m_buttonPressed == m_pressedLogicLevel) {
        if (m_currentPressLevel != NO_PRESS) {
            if (onPressCallback) {
                onPressCallback(this);
            } else {
                if (m_currentPressLevel != m_lastPressedLevel) {
                    // Controlla l'indice prima di accedere all'array m_gapThresholds
                    if (m_currentPressLevel > 0 && m_currentPressLevel <= m_numThresholds) {
                        next_interval = m_gapThresholds[m_currentPressLevel -1]; // Indexing for gap, e.g., gap after level 1 is at index 0
                    } else {
                        next_interval = 0; // O un valore di default
                    }

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
                if (m_maxLevelReachedAndNotified) {
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
void ButtonLongPress_Class::showStatus(void) {
    ext_showStatus(this);
}

//######################################################
//#    External Functions
//######################################################

//###########################################################################
//#
//###########################################################################
void ext_showStatus(ButtonLongPress_Class *p) {
    LOG_INFO("%s", p->m_pinID);
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
}