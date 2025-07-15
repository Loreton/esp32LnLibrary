//
// updated by ...: Loreto Notarantonio
// Date .........: 13-07-2025 13.55.36
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "lnLogger.h"
#include "lnGlobalVars.h"
#include "lnSerialRead.h" // waitForEnter()
#include "lnSetPinID.h" // waitForEnter()

#define ALARM_BEEP_INTERVAL 2000
#include "LedController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto

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
    LOG_DEBUG("m_numThresholds: %d/%d", m_numThresholds, MAX_DEFINED_PRESS_LEVELS);

    m_pressedLogicLevel = pressedLogicLevel;
    m_pressThresholds = thresholds;

    if (pressedLogicLevel == LOW) {
        pinMode(m_pin, INPUT_PULLUP);
    } else {
        pinMode(m_pin, INPUT);
    }

    // calcolo del nextGAP.
    // dobbiamo gestire l'ultimo ed allora lo mettiamo uguale al penultimo... da verificare
    for (int8_t i = 0; i<m_numThresholds-1; i++) {
        m_gapThresholds[i] = thresholds[i+1] - thresholds[i];
        m_gapThresholds[i+1] = m_gapThresholds[i];
        LOG_DEBUG("[%d]threshold: %lu - GAP: %lu", i, thresholds[i], m_gapThresholds[i]);
    }

    // m_lastButtonState = digitalRead(m_pin);
    m_lastButtonState =  digitalRead(m_pin) == m_pressedLogicLevel;
    m_buttonPressed = (m_lastButtonState == m_pressedLogicLevel);
    m_lastDebounceTime = millis();        // Inizializza il tempo di debounce
    m_debouncedState = m_lastButtonState;   // Inizializza lo stato debounced

}

// *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
void ButtonLongPress_Struct::reset(void) {
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    m_currentPressLevel = NO_PRESS;
    m_lastPressedLevel = NO_PRESS;
    m_pressDuration = 0;
    m_maxLevelReachedAndNotified = false;

}




// ##############################################
// Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo,
// aggiorna il livello di pressione.
// ##############################################
void ButtonLongPress_Struct::_checkNewLevel() {
    uint32_t currentPressTime = millis() - m_pressStartTime;
    uint8_t newLevel = NO_PRESS;

    for (int i = m_numThresholds - 1; i >= 0; i--) {
        if (currentPressTime >= m_pressThresholds[i]) {
            newLevel = static_cast<uint8_t>(i + 1);
            break;
        }
    }

    if (newLevel > m_currentPressLevel && newLevel <= m_numThresholds) {
        m_currentPressLevel = newLevel;
    }
}



/**
 * @brief Legge lo stato di un pulsante con debounce e rileva i livelli di pressione.
 * Questa funzione deve essere chiamata ripetutamente nel loop principale.
 *
 * @param btn:            Riferimento alla struttura ButtonState del pulsante da leggere.
 * @param debounceDelay:  Il tempo di debounce in millisecondi.
 * @return:               true se il pulsante è stato RILASCIATO dopo una pressione valida; false altrimenti.
 *
 *  I dettagli sulla durata della pressione e sul livello raggiunto sono disponibili nella struttura `btn`.
 *  E' responsabilità della funzione chiamante resettare `_pressDuration`, `_currentPressLevel`
 *      e `_maxLevelReachedAndNotified` dopo averli processati.
 */
bool ButtonLongPress_Struct::read(ButtonCallback onPressCallback) {
    bool currentState = digitalRead(m_pin); // Legge lo stato attuale del pin

    // Controlla se lo stato attuale è cambiato rispetto all'ultimo stato letto
    if (currentState != m_lastButtonState) {
        m_lastDebounceTime = millis(); // Resetta il timer di debounce
    }

    // Se è passato abbastanza tempo dal cambio di stato (superiore al tempo di debounce)
    if ((millis() - m_lastDebounceTime) > 300) { // 300ms di debounce
        // Se lo stato attuale è diverso dallo stato debounced
        if (currentState != m_debouncedState) {
            m_debouncedState = currentState; // Aggiorna lo stato debounced

            // La logica seguente è quella che avevi già, ma ora usa m_debouncedState
            // invece di m_buttonPressed per la prima verifica dello stato.
            // m_buttonPressed sarà aggiornato con lo stato debounced.
            if (m_debouncedState != m_buttonPressed) {
                m_buttonPressed = m_debouncedState; // Aggiorna lo stato "ufficiale" del pulsante dopo debounce
                LOG_DEBUG("[%s] state: %d", m_pinID, m_debouncedState);

                // Se il pulsante è stato PREMUTO (il suo stato debounced è ora il pressedLogicLevel).
                if (m_buttonPressed == m_pressedLogicLevel) {
                    m_pressStartTime = millis(); // Registra il momento della pressione.
                    // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante al rilascio precedente.
                }
                // Se il pulsante è stato RILASCIATO (il suo stato debounced è ora diverso dal pressedLogicLevel).
                else {
                    // Calcola la durata della pressione solo se era stato effettivamente premuto.
                    if (m_pressStartTime != 0) {
                        m_pressDuration = millis() - m_pressStartTime;
                        m_pressStartTime = 0; // Resetta il timer di inizio pressione.

                        // Execute the callback if set and the button was pressed.
                        if (onPressCallback) {
                            onPressCallback(this); // Pass 'this' (pointer to the current object)
                        }

                        // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante dopo averli letti.
                        return true; // Il pulsante è stato rilasciato.
                    }
                }
            }
        }
    }

    // Se il pulsante è attualmente PREMUTO e il timer è attivo, aggiorna il livello di pressione.
    if (m_buttonPressed == m_pressedLogicLevel && m_pressStartTime != 0) {
        _checkNewLevel();

        if (m_currentPressLevel == static_cast<uint8_t>(m_numThresholds) && !m_maxLevelReachedAndNotified) {
            LOG_INFO("[%s] MAX livello di pressione raggiunto: %d", m_name, m_currentPressLevel);
            m_maxLevelReachedAndNotified = true;
        }
    }

    m_lastButtonState = currentState; // Aggiorna l'ultimo stato letto per il prossimo ciclo
    return false;
}



// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
//###########################################################################
//#
//###########################################################################
void ButtonLongPress_Struct::notifyCurrentButtonLevel(BeepCallBack buzzerBeep) {
    uint16_t beep_duration=200;
    static uint32_t lastBeepTime;
    uint32_t next_interval;
    uint32_t elapsed;
    uint16_t phase_beep_duration;

    if (m_buttonPressed == m_pressedLogicLevel) {
        if (m_currentPressLevel != NO_PRESS) {
            if (m_currentPressLevel != m_lastPressedLevel) {

                // calcolo per gestione durata beep (se richiesto...)
                elapsed = millis() - m_pressStartTime;
                next_interval = m_gapThresholds[m_currentPressLevel];
                LOG_DEBUG("[%s] PRESSED_LEVEL_%d/%d - elapsed ms:%6lu", m_pinID, m_currentPressLevel, m_numThresholds, elapsed);
                phase_beep_duration = next_interval / 5; // arbitrario.... facciamo un beep che è u1/5 del next_threshold time
                LOG_DEBUG("[%s] next_interval: %lu - beep_duration: %lu", m_pinID, next_interval, phase_beep_duration);

                switch (m_currentPressLevel) {
                    case PRESSED_LEVEL_1:
                    case PRESSED_LEVEL_2:
                    case PRESSED_LEVEL_3:
                    case PRESSED_LEVEL_4:
                    case PRESSED_LEVEL_5:
                    case PRESSED_LEVEL_6:
                    case PRESSED_LEVEL_7:
                    case PRESSED_LEVEL_8:
                    case PRESSED_LEVEL_9:
                        if (buzzerBeep) {
                            buzzerBeep(this, beep_duration);
                        }
                        break;

                    default:
                        LOG_INFO("sono nel default: %d", m_currentPressLevel);
                        break;
                }
                m_lastPressedLevel = m_currentPressLevel;
            }
            // --- LOGICA DEL BEEP OGNI 5 SECONDI quando si raggiunge il MAX-LEVEL---
            if (m_maxLevelReachedAndNotified ) {
                if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
                    if (buzzerBeep) {
                        buzzerBeep(this, 1000);
                    }
                    LOG_WARN("[%s] ALARM! max pressed level %d reached", m_pinID, m_currentPressLevel);
                    // buzzer1->pulse(1000);
                    lastBeepTime = millis();
                }
            }
        }

    } else { // (m_buttonPressed != m_pressedLogicLevel)
        m_lastPressedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
    }
}





//###########################################################################
//#
//###########################################################################
void ButtonLongPress_Struct::showStatus(ButtonCallback showStatus_callback) {
    if (showStatus_callback) {
        showStatus_callback(this);
    }
}




//######################################################
//#   External Functions
//#   External Functions
//#   External Functions
//#   External Functions
//######################################################

