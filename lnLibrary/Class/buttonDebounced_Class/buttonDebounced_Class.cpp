/*
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 09.31.11
*/
#include "@buttonDebounced_Class.h" // Include il nuovo header della classe
#include "lnString.h" // per setPinID()


// void setPinID(char buffer[], int MAXLENGTH, const char *name, int pin); // in lnString.h

// ------------------------------------------------------
// Implementazione della classe ButtonDebounced_Class.
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
ButtonDebounced_Class::ButtonDebounced_Class(void){};

/** --- l'ho eliminato preferisco la init()
ButtonDebounced_Class::ButtonDebounced_Class(const char* name, uint8_t pin_nr, uint8_t pressedLogicLevel) :
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

    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

}
*/
void ButtonDebounced_Class::init(const char* name, uint8_t pin_nr, uint8_t pressedLogicLevel) {
    m_pin = pin_nr;
    m_name = name;
    m_pressedLogicLevel = pressedLogicLevel;
    m_lastDebounceTime = 0;
    m_buttonPressed = false;
    // m_initialized=true;

    // Configura il pin in base al pressedLogicLevel.
    if (m_pressedLogicLevel == LOW) {
        pinMode(m_pin, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno.
    } else {
        pinMode(m_pin, INPUT);        // Se premuto porta a HIGH, non usa pull-up interno (richiede pull-down esterno se flottante).
    }
    m_lastButtonState = digitalRead(m_pin); // Legge lo stato iniziale.

    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);
    /**
    const size_t PIN_ID_MAXLENGTH = sizeof(m_pinID)-1;
    int cx = snprintf(m_pinID, PIN_ID_MAXLENGTH - 6, "[%s", m_name);
    snprintf(m_pinID + cx, PIN_ID_MAXLENGTH - cx, ".%02d]:", m_pin);
    */
}

/**
 * @brief Legge lo stato di un pulsante con debounce.
 * Questa funzione deve essere chiamata ripetutamente nel loop principale.
 *
 * @param debounceDelay Il ritardo di debounce in millisecondi.
 * @return true se il pulsante è stato RILASCIATO dopo una pressione valida; false altrimenti.
 */
bool ButtonDebounced_Class::read(unsigned long debounceDelay) {
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

