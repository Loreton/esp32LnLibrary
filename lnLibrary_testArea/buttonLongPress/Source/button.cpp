//
// updated by ...: Loreto Notarantonio
// Date .........: 18-06-2025 20.19.14
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@button_structures.h"

void displayPinStatus(ButtonState_t *p, bool prompt) ; // pinDebug.cpp




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



// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
void notifyCurrentButtonLevel(ButtonState_t *btn) {
    if (btn->buttonPressed == btn->pressedLogicLevel) {
        if (btn->currentPressLevel != NO_PRESS) {
            if (btn->currentPressLevel != btn->lastPrintedLevel) {
                printf0_FN("[%s] Pressione in corso (ms:%06ld), livello: ", btn->name, (millis() - btn->pressStartTime));
                switch (btn->currentPressLevel) {
                    case PRESSED_LEVEL_1: Serial.println("PRESSED_LEVEL_1"); break;
                    case PRESSED_LEVEL_2: Serial.println("PRESSED_LEVEL_2"); break;
                    case PRESSED_LEVEL_3: Serial.println("PRESSED_LEVEL_3"); break;
                    case PRESSED_LEVEL_4: Serial.println("PRESSED_LEVEL_4"); break;
                    default: break;
                }
                btn->lastPrintedLevel = btn->currentPressLevel;
            }
        }

    } else {
        // btn->lastPrintedLevel = NO_PRESS;
        btn->lastPrintedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
    }


}

bool relayState = false;
void pumpState_Action(ButtonState_t *btn) {
    switch (btn->currentPressLevel) {
        case PRESSED_LEVEL_1:
            Serial.println("PRESSED_LEVEL_1");
            break;

        case PRESSED_LEVEL_2:
            Serial.println("PRESSED_LEVEL_2");
            break;

        case PRESSED_LEVEL_3:
            Serial.println("PRESSED_LEVEL_3");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(RELAY_PIN, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(RELAY_PIN, HIGH);
                printf0_FN("  --> Relè SPENTO!\n");
            }
            break;

        case PRESSED_LEVEL_4:
            Serial.println("PRESSED_LEVEL_4");
            break;

        default:
            Serial.println("Sconosciuto/Non Qualificato");
            break;
    }
}

void startButton_Action(ButtonState_t *btn) {
    switch (btn->currentPressLevel) {
        case PRESSED_LEVEL_1:
            Serial.println("PRESSED_LEVEL_1");
            break;

        case PRESSED_LEVEL_2:
            Serial.println("PRESSED_LEVEL_2");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(RELAY_PIN, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(RELAY_PIN, HIGH);
                printf0_FN("  --> Relè SPENTO!\n");
            }
            break;

        case PRESSED_LEVEL_3:
            Serial.println("PRESSED_LEVEL_3");
            break;

        case PRESSED_LEVEL_4:
            Serial.println("PRESSED_LEVEL_4");
            break;

        default:
            Serial.println("Sconosciuto/Non Qualificato");
            break;
    }
}





void processButton(ButtonState_t *btn) {

    printf0_FN("[%s[  Rilasciato! Durata: %ld ms\n", btn->name, btn->pressDuration);
    Serial.print("Livello finale raggiunto: ");

    if (btn->pin == startButton_pin) {
        startButton_Action(btn);
    }
    else if (btn->pin == pumpState_pin) {
        pumpState_Action(btn);
    }
    // switch (btn->currentPressLevel) {
    //     case PRESSED_LEVEL_1:
    //         Serial.println("PRESSED_LEVEL_1");
    //         break;

    //     case PRESSED_LEVEL_2:
    //         Serial.println("PRESSED_LEVEL_2");
    //         relayState = !relayState;
    //         if (relayState) {
    //             digitalWrite(RELAY_PIN, LOW);
    //             printf0_FN("  --> Relè ACCESO!\n");
    //         } else {
    //             digitalWrite(RELAY_PIN, HIGH);
    //             printf0_FN("  --> Relè SPENTO!\n");
    //         }
    //         break;

    //     case PRESSED_LEVEL_3:
    //         Serial.println("PRESSED_LEVEL_3");
    //         break;

    //     case PRESSED_LEVEL_4:
    //         Serial.println("PRESSED_LEVEL_4");
    //         break;

    //     default:
    //         Serial.println("Sconosciuto/Non Qualificato");
    //         break;
    // }


    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    btn->currentPressLevel = NO_PRESS;
    btn->lastPrintedLevel = NO_PRESS;
    btn->pressDuration = 0;
    btn->maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.
}

