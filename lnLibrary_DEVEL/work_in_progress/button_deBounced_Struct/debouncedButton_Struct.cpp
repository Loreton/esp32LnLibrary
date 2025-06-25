/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.44.48
*/

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@debouncedButton_Struct.h"

// ------------------------------------------------------
// Struttura per mantenere lo stato di ogni pulsante.
// con funzioni interne alla struttura
// ------------------------------------------------------
/** con Alias struct_type
 * @brief Inizializza un pulsante configurando il pin e lo stato iniziale.
 *
 * @param btn Riferimento alla struttura ButtonState del pulsante.
 * @param pin Il numero del pin GPIO a cui è collegato il pulsante.
 * @param name Una stringa che identifica il pulsante (es. "MyButton").
 * @param pressedLogicLevel Il livello logico (LOW o HIGH) che indica che il pulsante è premuto.
 * - Usa LOW per un pulsante collegato tra pin e GND (con INPUT_PULLUP).
 * - Usa HIGH per un pulsante collegato tra pin e VCC (con INPUT, richiede pull-down esterno).
 */
void deBouncedButton_sc::init(int pin_nr, const char* name, int pressedLogicLevel) {
    pin_ = pin_nr;
    name_ = name;
    pressedLogicLevel_ = pressedLogicLevel;

    // Configura il pin in base al pressedLogicLevel.
    if (pressedLogicLevel == LOW) {
        pinMode(pin_, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno.
    } else {
        pinMode(pin_, INPUT);        // Se premuto porta a HIGH, non usa pull-up interno (richiede pull-down esterno se flottante).
    }

    lastButtonState_ = digitalRead(pin_); // Legge lo stato iniziale.
    lastDebounceTime_ = 0;
    buttonPressed_ = false; // Stato iniziale debounced.
}




/**
 * @brief Legge lo stato di un pulsante con debounce.
 * Questa funzione deve essere chiamata ripetutamente nel loop principale.
 *
 * @param btn Riferimento alla struttura ButtonState del pulsante da leggere.
 * @return true se il pulsante è stato RILASCIATO dopo una pressione valida; false altrimenti.
 */
bool deBouncedButton_sc::read(unsigned long debounceDelay) {

    // Legge lo stato attuale del pin (lettura RAW).
    bool reading = digitalRead(pin_);

    // Se la lettura RAW è cambiata rispetto all'ultima lettura RAW, resetta il timer di debounce.
    if (reading != lastButtonState_) {
        lastDebounceTime_ = millis();
    }

    // Se è trascorso il tempo di debounce e lo stato RAW è stabile.
    if ((millis() - lastDebounceTime_) > debounceDelay) {
        // Se lo stato RAW stabile è diverso dallo stato debounced corrente, aggiorna lo stato debounced.
        if (reading != buttonPressed_) {
            buttonPressed_ = reading; // Aggiorna lo stato debounced del pulsante.

            // Se il pulsante è stato RILASCIATO.
            // Il pulsante è rilasciato quando il suo stato debounced è DIVERSO da `pressedLogicLevel`.
            if (buttonPressed_ != pressedLogicLevel_) {
                return true; // Il pulsante è stato rilasciato dopo una pressione valida.
            }
        }
    }

    // Salva la lettura RAW attuale per la prossima iterazione.
    lastButtonState_ = reading;
    return false; // Il pulsante non è stato rilasciato in questo ciclo.
}


