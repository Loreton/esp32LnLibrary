//
// updated by ...: Loreto Notarantonio
// Date .........: 23-06-2025 14.13.23
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()
#include "@pinLongPress_sc.h"




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
// void pinLongPress_sc::init(const char* name, int pin, int pressedLogicLevel, const unsigned long thresholds[], size_t thresholdsCount) {
void pinLongPress_sc::init(const char* name, int pin,
                                             int pressedLogicLevel,
                                             const unsigned long thresholds[],
                                             size_t thresholdsCount,
                                             ButtonCallback callback) {
    _pin = pin;
    _name = name;
    snprintf(_pinID, 20, "[%02d.%-14s]:", _pin, _name);

    _pressedLogicLevel = pressedLogicLevel;
    _pressThresholds = thresholds;
    _numThresholds = thresholdsCount;

    if (pressedLogicLevel == LOW) {
        pinMode(_pin, INPUT_PULLUP);
    } else {
        pinMode(_pin, INPUT);
    }

    _lastButtonState = digitalRead(_pin);
    _lastDebounceTime = 0;
    _buttonPressed = (_lastButtonState == _pressedLogicLevel);
    _pressStartTime = 0;
    _pressDuration = 0;
    _currentPressLevel = NO_PRESS;
    _lastPrintedLevel = NO_PRESS;
    _maxLevelReachedAndNotified = false;
    _onPressCallback = callback;
}



// ##############################################
// Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo,
// aggiorna il livello di pressione.
// ##############################################
void pinLongPress_sc::_checkNewLevel() {
    unsigned long currentPressTime = millis() - _pressStartTime;
    uint8_t newLevel = NO_PRESS;

    for (int i = _numThresholds - 1; i >= 0; i--) {
        if (currentPressTime >= _pressThresholds[i]) {
            // newLevel = static_cast<ButtonPressedLevel>(i + 1);
            newLevel = static_cast<uint8_t>(i + 1);
            break;
        }
    }

    if (newLevel > _currentPressLevel && newLevel <= _numThresholds) {
        _currentPressLevel = newLevel;
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
bool pinLongPress_sc::read() {
    bool state = digitalRead(_pin);

    if (state != _buttonPressed) {
        _buttonPressed = state;

        // Se il pulsante è stato PREMUTO (il suo stato debounced è ora il pressedLogicLevel).
        if (_buttonPressed == _pressedLogicLevel) {
            _pressStartTime = millis(); // Registra il momento della pressione.
            // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante al rilascio precedente.
        }
        // Se il pulsante è stato RILASCIATO (il suo stato debounced è ora diverso dal pressedLogicLevel).
        else {
            // Calcola la durata della pressione solo se era stato effettivamente premuto.
            if (_pressStartTime != 0) {
                _pressDuration = millis() - _pressStartTime;
                _pressStartTime = 0; // Resetta il timer di inizio pressione.

                // NON resettiamo qui i parametri di livello, lo farà la funzione chiamante dopo averli letti.
                return true; // Il pulsante è stato rilasciato.
            }
      }
    }

    // Se il pulsante è attualmente PREMUTO (debounced) e il timer è attivo, aggiorna il livello di pressione.
    if (_buttonPressed == _pressedLogicLevel && _pressStartTime != 0) {
        _checkNewLevel();

        // if (_currentPressLevel == static_cast<ButtonPressedLevel>(_numThresholds) && !_maxLevelReachedAndNotified) {
        if (_currentPressLevel == static_cast<uint8_t>(_numThresholds) && !_maxLevelReachedAndNotified) {
            printf0_FN("[%s] MAX livello di pressione raggiunto: %d\n", _name, _currentPressLevel);
            _maxLevelReachedAndNotified = true;
        }
    }

    _lastButtonState = state;
    return false;
}


// if (_buttonPressed && _onPressCallback) {
//     _onPressCallback(this);
// }
