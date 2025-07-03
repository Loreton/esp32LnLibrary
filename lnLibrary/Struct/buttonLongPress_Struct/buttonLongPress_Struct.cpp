//
// updated by ...: Loreto Notarantonio
// Date .........: 03-07-2025 07.29.29
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_2
#define LOG_LEVEL_99x
#include "@logMacros.h" // printf:XFN()
#include "@globalVars.h" // printf:XFN()
#include "@serialRead.h" // waitForEnter()
#include "@setPinID.h" // waitForEnter()

#include "@buttonLongPress_Struct.h"




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
                                             size_t thresholdsCount) {
    m_pin = pin;
    m_name = name;
    // snprintf(m_pinID, 20, "[%02d.%-14s]:", m_pin, m_name);
    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

    m_pressedLogicLevel = pressedLogicLevel;
    m_pressThresholds = thresholds;
    m_numThresholds = thresholdsCount;

    if (pressedLogicLevel == LOW) {
        pinMode(m_pin, INPUT_PULLUP);
    } else {
        pinMode(m_pin, INPUT);
    }

    m_lastButtonState = digitalRead(m_pin);
    m_buttonPressed = (m_lastButtonState == m_pressedLogicLevel);

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
bool ButtonLongPress_Struct::read() {
    bool state = digitalRead(m_pin);

    if (state != m_buttonPressed) {
        m_buttonPressed = state;

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
                if (m_onPressCallback) {
                    m_onPressCallback(this); // Pass 'this' (pointer to the current object)
                }

                // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante dopo averli letti.
                return true; // Il pulsante è stato rilasciato.
            }
      }
    }

    // Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo, aggiorna il livello di pressione.
    if (m_buttonPressed == m_pressedLogicLevel && m_pressStartTime != 0) {
        _checkNewLevel();

        if (m_currentPressLevel == static_cast<uint8_t>(m_numThresholds) && !m_maxLevelReachedAndNotified) {
            printf0_FN("[%s] MAX livello di pressione raggiunto: %d\n", m_name, m_currentPressLevel);
            m_maxLevelReachedAndNotified = true;
        }
    }

    m_lastButtonState = state;
    return false;
}

// void notifyCurrentButtonLevel(notifyButtonLevelCB *f, pinController_Struct *buzzer) {

// }




//######################################################
//#   External Functions
//#   External Functions
//#   External Functions
//#   External Functions
//######################################################



//###########################################################################
//#
//###########################################################################
void pinLongPressStatus(ButtonLongPress_Struct *p, bool prompt) {
    // printf0_FN("\t%-18s: %2d - (%d)\n" , "pin nr"         , p->pin                   , p->mode);
    printf0_FN("%s\n", p->m_pinID);
    printf0_FN("\t%-18s: %2d\n",   "pressedLogicLevel",          p->m_pressedLogicLevel); //,           str_pinLevel[p->m_pressedLogicLevel]);
    printf0_FN("\t%-18s: %2d\n",   "buttonPressed",              p->m_buttonPressed); //,               str_TrueFalse[p->m_buttonPressed]);
    printf0_FN("\t%-18s: %2d\n",   "maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified); //,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);

    printf0_FN("\t%-18s: %2d - %s\n",   "pressedLogicLevel",          p->m_pressedLogicLevel,           str_pinLevel[p->m_pressedLogicLevel]);
    printf0_FN("\t%-18s: %2d - %s\n",   "buttonPressed",              p->m_buttonPressed,               str_TrueFalse[p->m_buttonPressed]);
    printf0_FN("\t%-18s: %2d - %s\n",   "maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);

    printf0_FN("\t%-18s: nr:%2d --> [ ", "threshold"      , p->m_numThresholds);
    for (int8_t j=0; j < p->m_numThresholds; j++) {
        printf0("%2d, ", p->m_pressThresholds[j]);
    }
    printf0(" ]\n"); // close threshold line

    printf0("\n"); // blank line
    if (prompt) {
        waitForEnter();
    }
}



// ###########################################################################################
// #  FUNZIONI ESTERNE ALLA STRUTTURA MA COMUNCQUE CORRELATE
// #  Funzioni esterne alla struttura ma comuncque correlate
// #  Funzioni esterne alla struttura ma comuncque correlate
// #  FUNZIONI ESTERNE ALLA STRUTTURA MA COMUNCQUE CORRELATE
// ###########################################################################################



#define ALARM_BEEP_INTERVAL 2000
#include "@pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto

// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
//###########################################################################
//#
//###########################################################################
void notifyCurrentButtonLevel(ButtonLongPress_Struct *p, pinController_Struct *buzzer) {
    uint16_t beep_duration=200;
    static uint32_t lastBeepTime;
    // static bool alarming=false;
    if (p->m_buttonPressed == p->m_pressedLogicLevel) {
        if (p->m_currentPressLevel != NO_PRESS) {
            if (p->m_currentPressLevel != p->m_lastPressedLevel) {
                printf0_FN("[%s] Pressione in corso (ms:%06ld)\n", p->m_name, (millis() - p->m_pressStartTime));
                switch (p->m_currentPressLevel) {
                    case PRESSED_LEVEL_1:
                        printf2_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;


                    case PRESSED_LEVEL_2:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_3:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_4:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_5:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_6:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_7:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_8:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_9:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->m_currentPressLevel);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    default:
                        printf0_FN("sono nel default: %d\n", p->m_currentPressLevel);
                        break;

                }
                p->m_lastPressedLevel = p->m_currentPressLevel;
            }
            // --- LOGICA DEL BEEP OGNI 5 SECONDI ---
            if (p->m_maxLevelReachedAndNotified ) {
                if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
                    buzzer->pulse(1000);
                    lastBeepTime = millis();
                }
            }
        }

    } else { // (p->m_buttonPressed != p->m_pressedLogicLevel)
        // printf99_FN("clearing...\n");
        p->m_lastPressedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
        // lastBeepTime = 0; // Resetta il timer del beep quando il pulsante non è più premuto al livello massimo
        // buzzer->off_ifBlinking(); //siccome è usato per più pin rischiamo di chiudere un beep lanciato da un altra funzione
        // p->maxLevelReachedAndNotified_=false;
        // alarming=false;
    }


}

