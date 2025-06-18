//
// updated by ...: Loreto Notarantonio
// Date .........: 18-06-2025 13.27.37
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@button_structures.h"


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
void setupButton(ButtonState_t *btn, int pin, const char* name, int pressedLogicLevel,
                 const unsigned long thresholds[], size_t thresholdsCount) {
    btn->pin = pin;
    btn->name = name;
    btn->pressedLogicLevel = pressedLogicLevel;
    btn->pressThresholds = thresholds;
    btn->numThresholds = thresholdsCount;

    if (pressedLogicLevel == LOW) {
        pinMode(btn->pin, INPUT_PULLUP);
    } else {
        pinMode(btn->pin, INPUT);
    }

    btn->lastButtonState = digitalRead(btn->pin);
    btn->lastDebounceTime = 0;
    btn->buttonPressed = (btn->lastButtonState == btn->pressedLogicLevel);
    btn->pressStartTime = 0;
    btn->pressDuration = 0;
    btn->currentPressLevel = NO_PRESS;
    btn->maxLevelReachedAndNotified = false;
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
bool readButton(ButtonState_t *btn, unsigned long debounceDelay) {
    bool state = digitalRead(btn->pin);

    if (state != btn->lastButtonState) {
        btn->lastDebounceTime = millis();
    }

    if ((millis() - btn->lastDebounceTime) > debounceDelay) {
        // superato il debounce time
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
    }

    // Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo, aggiorna il livello di pressione.
    if (btn->buttonPressed == btn->pressedLogicLevel && btn->pressStartTime != 0) {
        unsigned long currentPressTime = millis() - btn->pressStartTime;
        ButtonPressType newLevel = NO_PRESS;

        for (int i = btn->numThresholds - 1; i >= 0; i--) {
            if (currentPressTime >= btn->pressThresholds[i]) {
                newLevel = static_cast<ButtonPressType>(i + 1);
                break;
            }
        }

        if (newLevel > btn->currentPressLevel && newLevel <= btn->numThresholds) {
            btn->currentPressLevel = newLevel;
        }

        if (btn->currentPressLevel == static_cast<ButtonPressType>(btn->numThresholds) && !btn->maxLevelReachedAndNotified) {
            Serial.printf("[%s] MAX livello di pressione raggiunto!\n", btn->name);
            btn->maxLevelReachedAndNotified = true;
        }
    }

    btn->lastButtonState = state;
    return false;
}



// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
void notifyCurrentButtonLevel(ButtonState_t *btn) {

    if (btn->currentPressLevel != NO_PRESS) {
        static ButtonPressType lastPrintedLevel = NO_PRESS;
        if (btn->currentPressLevel != lastPrintedLevel) {
            Serial.print("[");
            Serial.print(btn->name);
            Serial.print("] Pressione in corso, livello: ");
            switch (btn->currentPressLevel) {
                case SHORT_PRESS: Serial.println("BREVE"); break;
                case MEDIUM_PRESS: Serial.println("MEDIA"); break;
                case LONG_PRESS: Serial.println("LUNGA"); break;
                case VERY_LONG_PRESS: Serial.println("MOLTO LUNGA"); break;
                default: break;
            }
            lastPrintedLevel = btn->currentPressLevel;
        }
    }

}

