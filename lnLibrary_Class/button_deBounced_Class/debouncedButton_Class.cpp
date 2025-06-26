/*
// updated by ...: Loreto Notarantonio
// Date .........: 26-06-2025 16.57.10
*/
#include "@debouncedButton_Class.h" // Include il nuovo header della classe

// ------------------------------------------------------
// Implementazione della classe DebouncedButton_Class.
// ------------------------------------------------------

/**
 * @brief Costruttore: inizializza un pulsante configurando il pin e lo stato iniziale.
 *
 * @param pin_nr Il numero del pin GPIO a cui è collegato il pulsante.
 * @param name Una stringa che identifica il pulsante (es. "MyButton").
 * @param pressedLogicLevel Il livello logico (LOW o HIGH) che indica che il pulsante è premuto.
 * - Usa LOW per un pulsante collegato tra pin e GND (con INPUT_PULLUP).
 * - Usa HIGH per un pulsante collegato tra pin e VCC (con INPUT, richiede pull-down esterno).
 */
DebouncedButton_Class::DebouncedButton_Class(int pin_nr, const char* name, int pressedLogicLevel) :
                m_pin(pin_nr),
                m_name(name),
                m_pressedLogicLevel(pressedLogicLevel),
                m_lastDebounceTime(0),
                m_buttonPressed(false) {

    // Configura il pin in base al pressedLogicLevel.
    if (m_pressedLogicLevel == LOW) {
        pinMode(m_pin, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno.
    } else {
        pinMode(m_pin, INPUT);        // Se premuto porta a HIGH, non usa pull-up interno (richiede pull-down esterno se flottante).
    }
    m_lastButtonState = digitalRead(m_pin); // Legge lo stato iniziale.
}

/**
 * @brief Legge lo stato di un pulsante con debounce.
 * Questa funzione deve essere chiamata ripetutamente nel loop principale.
 *
 * @param debounceDelay Il ritardo di debounce in millisecondi.
 * @return true se il pulsante è stato RILASCIATO dopo una pressione valida; false altrimenti.
 */
bool DebouncedButton_Class::read(unsigned long debounceDelay) {
    // Legge lo stato attuale del pin (lettura RAW).
    bool reading = digitalRead(m_pin);

    // Se la lettura RAW è cambiata rispetto all'ultima lettura RAW, resetta il timer di debounce.
    if (reading != m_lastButtonState) {
        m_lastDebounceTime = millis();
    }

    // Se è trascorso il tempo di debounce e lo stato RAW è stabile.
    if ((millis() - m_lastDebounceTime) > debounceDelay) {
        // Se lo stato RAW stabile è diverso dallo stato debounced corrente, aggiorna lo stato debounced.
        if (reading != m_buttonPressed) {
            m_buttonPressed = reading; // Aggiorna lo stato debounced del pulsante.

            // Se il pulsante è stato RILASCIATO.
            // Il pulsante è rilasciato quando il suo stato debounced è DIVERSO da `pressedLogicLevel`.
            if (m_buttonPressed != m_pressedLogicLevel) {
                return true; // Il pulsante è stato rilasciato dopo una pressione valida.
            }
        }
    }

    // Salva la lettura RAW attuale per la prossima iterazione.
    m_lastButtonState = reading;
    return false; // Il pulsante non è stato rilasciato in questo ciclo.
}