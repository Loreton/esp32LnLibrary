//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 17.01.45
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_2
#define LOG_LEVEL_99x
#include "@globalVars.h" // printf:XFN()

#include "@pinLongPress_Struct.h"




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
// void pinLongPress_Struct::init(const char* name, int pin, int pressedLogicLevel, const unsigned long thresholds[], size_t thresholdsCount) {
void pinLongPress_Struct::init(const char* name, int pin,
                                             int pressedLogicLevel,
                                             const unsigned long thresholds[],
                                             size_t thresholdsCount,
                                             ButtonCallback callback) {
    pin_ = pin;
    name_ = name;
    snprintf(pinID_, 20, "[%02d.%-14s]:", pin_, name_);

    pressedLogicLevel_ = pressedLogicLevel;
    pressThresholds_ = thresholds;
    numThresholds_ = thresholdsCount;

    if (pressedLogicLevel == LOW) {
        pinMode(pin_, INPUT_PULLUP);
    } else {
        pinMode(pin_, INPUT);
    }

    lastButtonState_ = digitalRead(pin_);
    lastDebounceTime_ = 0;
    buttonPressed_ = (lastButtonState_ == pressedLogicLevel_);
    pressStartTime_ = 0;
    pressDuration_ = 0;
    currentPressLevel_ = NO_PRESS;
    // lastPrintedLevel_ = NO_PRESS;
    lastPressedLevel_ = NO_PRESS;
    maxLevelReachedAndNotified_ = false;
    onPressCallback_ = callback;
}



// ##############################################
// Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo,
// aggiorna il livello di pressione.
// ##############################################
void pinLongPress_Struct::_checkNewLevel() {
    unsigned long currentPressTime = millis() - pressStartTime_;
    uint8_t newLevel = NO_PRESS;

    for (int i = numThresholds_ - 1; i >= 0; i--) {
        if (currentPressTime >= pressThresholds_[i]) {
            // newLevel = static_cast<ButtonPressedLevel>(i + 1);
            newLevel = static_cast<uint8_t>(i + 1);
            break;
        }
    }

    if (newLevel > currentPressLevel_ && newLevel <= numThresholds_) {
        currentPressLevel_ = newLevel;
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
bool pinLongPress_Struct::read() {
    bool state = digitalRead(pin_);

    if (state != buttonPressed_) {
        buttonPressed_ = state;

        // Se il pulsante è stato PREMUTO (il suo stato debounced è ora il pressedLogicLevel).
        if (buttonPressed_ == pressedLogicLevel_) {
            pressStartTime_ = millis(); // Registra il momento della pressione.
            // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante al rilascio precedente.
        }
        // Se il pulsante è stato RILASCIATO (il suo stato debounced è ora diverso dal pressedLogicLevel).
        else {
            // Calcola la durata della pressione solo se era stato effettivamente premuto.
            if (pressStartTime_ != 0) {
                pressDuration_ = millis() - pressStartTime_;
                pressStartTime_ = 0; // Resetta il timer di inizio pressione.

                // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante dopo averli letti.
                return true; // Il pulsante è stato rilasciato.
            }
      }
    }

    // Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo, aggiorna il livello di pressione.
    if (buttonPressed_ == pressedLogicLevel_ && pressStartTime_ != 0) {
        _checkNewLevel();

        if (currentPressLevel_ == static_cast<uint8_t>(numThresholds_) && !maxLevelReachedAndNotified_) {
            printf0_FN("[%s] MAX livello di pressione raggiunto: %d\n", name_, currentPressLevel_);
            maxLevelReachedAndNotified_ = true;
        }
    }

    lastButtonState_ = state;
    return false;
}


// if (buttonPressed_ && onPressCallback_) {
//     onPressCallback_(this);
// }

//###########################################################################
//#
//###########################################################################
void pinLongPressStatus(pinLongPress_Struct *p, bool prompt) {
    // printf0_FN("\t%-18s: %2d - (%d)\n" , "pin nr"         , p->pin                   , p->mode);
    printf0_FN("%s\n", p->pinID_);
    printf0_FN("\t%-18s: %2d\n",   "pressedLogicLevel",          p->pressedLogicLevel_); //,           str_pinLevel[p->pressedLogicLevel_]);
    printf0_FN("\t%-18s: %2d\n",   "buttonPressed",              p->buttonPressed_); //,               str_TrueFalse[p->buttonPressed_]);
    printf0_FN("\t%-18s: %2d\n",   "maxLevelReachedAndNotified", p->maxLevelReachedAndNotified_); //,  str_TrueFalse[p->maxLevelReachedAndNotified_]);

    printf0_FN("\t%-18s: %2d - %s\n",   "pressedLogicLevel",          p->pressedLogicLevel_,           str_pinLevel[p->pressedLogicLevel_]);
    printf0_FN("\t%-18s: %2d - %s\n",   "buttonPressed",              p->buttonPressed_,               str_TrueFalse[p->buttonPressed_]);
    printf0_FN("\t%-18s: %2d - %s\n",   "maxLevelReachedAndNotified", p->maxLevelReachedAndNotified_,  str_TrueFalse[p->maxLevelReachedAndNotified_]);

    printf0_FN("\t%-18s: nr:%2d --> [ ", "threshold"      , p->numThresholds_);
    for (int8_t j=0; j < p->numThresholds_; j++) {
        printf0("%2d, ", p->pressThresholds_[j]);
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
void notifyCurrentButtonLevel(pinLongPress_Struct *p, pinController_Struct *buzzer) {
    uint16_t beep_duration=200;
    static uint32_t lastBeepTime;
    // static bool alarming=false;
    if (p->buttonPressed_ == p->pressedLogicLevel_) {
        if (p->currentPressLevel_ != NO_PRESS) {
            if (p->currentPressLevel_ != p->lastPressedLevel_) {
                printf0_FN("[%s] Pressione in corso (ms:%06ld)\n", p->name_, (millis() - p->pressStartTime_));
                switch (p->currentPressLevel_) {
                    case PRESSED_LEVEL_1:
                        printf2_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;


                    case PRESSED_LEVEL_2:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_3:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_4:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_5:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_6:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_7:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_8:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    case PRESSED_LEVEL_9:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->currentPressLevel_);
                        buzzer->pulse(beep_duration);
                        // notifyBuzzer(buzzer);
                        break;

                    default:
                        printf0_FN("sono nel default: %d\n", p->currentPressLevel_);
                        break;

                }
                p->lastPressedLevel_ = p->currentPressLevel_;
            }
            // --- LOGICA DEL BEEP OGNI 5 SECONDI ---
            if (p->maxLevelReachedAndNotified_ ) {
                if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
                    buzzer->pulse(1000);
                    lastBeepTime = millis();
                }
            }
        }

    } else { // (p->buttonPressed_ != p->pressedLogicLevel_)
        // printf99_FN("clearing...\n");
        p->lastPressedLevel_ = NO_PRESS; // Reset per il prossimo ciclo di pressione
        // lastBeepTime = 0; // Resetta il timer del beep quando il pulsante non è più premuto al livello massimo
        // buzzer->off_ifBlinking(); //siccome è usato per più pin rischiamo di chiudere un beep lanciato da un altra funzione
        // p->maxLevelReachedAndNotified_=false;
        // alarming=false;
    }


}

