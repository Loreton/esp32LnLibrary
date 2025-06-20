//
// updated by ...: Loreto Notarantonio
// Date .........: 20-06-2025 17.18.43
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()
#include "@button_LongPress.h"

#include "@LCD_Buzzer.h"



/**
 * @brief Inizializza un pulsante configurando il pin e lo stato iniziale.
 *
 * @param btn::                 Riferimento alla struttura ButtonState_t del pulsante.
 * @param pin:                  Il numero del pin GPIO a cui è collegato il pulsante.
 * @param name:                 Una stringa che identifica il pulsante (es. "Button1").
 * @param pressedLogicLevel:    Il livello logico (LOW o HIGH) che indica che il pulsante è premuto.
 *                                  - Usa LOW per un pulsante collegato tra pin e GND (con INPUT_PULLUP).
 *                                  - Usa HIGH per un pulsante collegato tra pin e VCC (con INPUT, richiede pull-down esterno).
 * @param thresholds:           Array di unsigned long contenente le soglie di tempo (in ms) per i livelli di pressione.
 *                                  Le soglie DEVONO essere in ordine crescente. L'indice 0 corrisponde a SHORT_PRESS, ecc.
 * @param thresholdsCount:      Il numero di elementi nell'array `thresholds`.
*/
void setupButton(ButtonState_t *p,  int pin, const char* name,
                                    int pressedLogicLevel,
                                    const unsigned long thresholds[],
                                    size_t thresholdsCount) {
    p->pin = pin;
    p->name = name;
    snprintf(p->pinID, 20, "[%02d.%-14s]:", p->pin, p->name);
    p->pressedLogicLevel = pressedLogicLevel;
    p->pressThresholds = thresholds;
    p->numThresholds = thresholdsCount;

    if (pressedLogicLevel == LOW) {
        pinMode(p->pin, INPUT_PULLUP);
    } else {
        pinMode(p->pin, INPUT);
    }

    p->lastButtonState = digitalRead(p->pin);
    p->lastDebounceTime = 0;
    p->buttonPressed = (p->lastButtonState == p->pressedLogicLevel);
    p->pressStartTime = 0;
    p->pressDuration = 0;
    p->currentPressLevel = NO_PRESS;
    p->lastPrintedLevel = NO_PRESS;
    p->maxLevelReachedAndNotified = false;
}



// ##############################################
// Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo,
// aggiorna il livello di pressione.
// ##############################################
void _checkNewLevel(ButtonState_t *btn) {
    unsigned long currentPressTime = millis() - btn->pressStartTime;
    // ButtonPressedLevel newLevel = NO_PRESS;
    uint8_t newLevel = NO_PRESS;

    for (int i = btn->numThresholds - 1; i >= 0; i--) {
        if (currentPressTime >= btn->pressThresholds[i]) {
            // newLevel = static_cast<ButtonPressedLevel>(i + 1);
            newLevel = static_cast<uint8_t>(i + 1);
            break;
        }
    }

    if (newLevel > btn->currentPressLevel && newLevel <= btn->numThresholds) {
        btn->currentPressLevel = newLevel;
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
 *  E' responsabilità della funzione chiamante resettare `btn->pressDuration`, `btn->currentPressLevel`
 *      e `btn->maxLevelReachedAndNotified` dopo averli processati.
 */
bool readButton(ButtonState_t *btn) {
    bool state = digitalRead(btn->pin);

    if (state != btn->buttonPressed) {
        btn->buttonPressed = state;

        // Se il pulsante è stato PREMUTO (il suo stato debounced è ora il pressedLogicLevel).
        if (btn->buttonPressed == btn->pressedLogicLevel) {
            btn->pressStartTime = millis(); // Registra il momento della pressione.
            // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante al rilascio precedente.
        }
        // Se il pulsante è stato RILASCIATO (il suo stato debounced è ora diverso dal pressedLogicLevel).
        else {
            // Calcola la durata della pressione solo se era stato effettivamente premuto.
            if (btn->pressStartTime != 0) {
                btn->pressDuration = millis() - btn->pressStartTime;
                btn->pressStartTime = 0; // Resetta il timer di inizio pressione.

                // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante dopo averli letti.
                return true; // Il pulsante è stato rilasciato.
            }
      }
    }

    // Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo, aggiorna il livello di pressione.
    if (btn->buttonPressed == btn->pressedLogicLevel && btn->pressStartTime != 0) {
        _checkNewLevel(btn);

        // if (btn->currentPressLevel == static_cast<ButtonPressedLevel>(btn->numThresholds) && !btn->maxLevelReachedAndNotified) {
        if (btn->currentPressLevel == static_cast<uint8_t>(btn->numThresholds) && !btn->maxLevelReachedAndNotified) {
            printf0_FN("[%s] MAX livello di pressione raggiunto: %d\n", btn->name, btn->currentPressLevel);
            btn->maxLevelReachedAndNotified = true;
        }
    }

    btn->lastButtonState = state;
    return false;
}


void notifyBuzzer(uint8_t pin, uint16_t msec=300) {
    // printf99_FN("Buzzer pin: %d\n", pin);
    if (pin != 99) {
        digitalWrite(pin, HIGH);
        delay(msec);
        digitalWrite(pin, LOW);
    }
}


// const int BUZZER_PIN = 2; // Sostituisci con il pin a cui è collegato il tuo buzzer
const unsigned long BEEP_INTERVAL = 5000; // 5 secondi in millisecondi
unsigned long lastBeepTime = 0;




// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
//###########################################################################
//#
//###########################################################################
void notifyCurrentButtonLevel(ButtonState_t *btn, uint8_t buzzer_pin) {
    if (btn->buttonPressed == btn->pressedLogicLevel) {
        if (btn->currentPressLevel != NO_PRESS) {
            if (btn->currentPressLevel != btn->lastPrintedLevel) {
                printf0_FN("[%s] Pressione in corso (ms:%06ld)\n", btn->name, (millis() - btn->pressStartTime));
                switch (btn->currentPressLevel) {
                    case PRESSED_LEVEL_1:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    case PRESSED_LEVEL_2:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    case PRESSED_LEVEL_3:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    case PRESSED_LEVEL_4:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    case PRESSED_LEVEL_5:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        // if (btn->maxLevelReachedAndNotified) {
                        //     notifyBuzzer(buzzer_pin);
                        // }
                        // se mi trovo qui è perché il pin non è ancora stato rilasciato
                        // se mi trovo qui è perché il pin è ancora pressed
                        // se volessi mandare un allarme continuo....
                        // devo inserirlo nell'ultimo livello previsto per lo specifico pin
                        // if (btn->pin == startButton_pin && btn->maxLevelReachedAndNotified == true) {
                        //     btn->maxLevelReachedAndNotified = false; // reset del flag
                        //     btn->currentPressLevel--;
                        //     // btn->currentPressLevel = MAX_DEFINED_PRESS_LEVELS;
                        // }
                        break;

                    default:
                        // printf99_FN("notify MAX_DEFINED_PRESS_LEVELS: %d\n", btn->currentPressLevel);
                        // notifyBuzzer(buzzer_pin, 300);
                        // // se mi trovo qui è perché il pin non è ancora stato rilasciato
                        // // se volessi mandare un allarme....:
                        // btn->currentPressLevel--;
                        // btn->maxLevelReachedAndNotified = false; // reset del flag
                        // buttonLP_pinStatus(btn, true);
                        break;

                }
                btn->lastPrintedLevel = btn->currentPressLevel;
            }
            // --- LOGICA DEL BEEP OGNI 5 SECONDI ---
            if (btn->maxLevelReachedAndNotified) {
                if (millis() - lastBeepTime >= BEEP_INTERVAL) {
                    attachBuzzer(passiveBuzzer_pin, 10, 0);
                    playTone(1000, 500, 0);
                    lastBeepTime = millis();
                    printf99_FN("  [BEEP] Livello massimo ancora raggiunto!\n");
                }
            }
        }

    } else { // (btn->buttonPressed != btn->pressedLogicLevel)
        // btn->lastPrintedLevel = NO_PRESS;
        btn->lastPrintedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
        // Resetta il timer del beep quando il pulsante non è più premuto al livello massimo
        lastBeepTime = 0;
        noTone(0);
        // detachBuzzer(activeBuzzer_pin);
    }


}
