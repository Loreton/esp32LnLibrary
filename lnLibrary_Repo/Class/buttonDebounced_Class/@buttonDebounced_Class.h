/*
// updated by ...: Loreto Notarantonio
// Date .........: 29-06-2025 18.03.17
*/
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type

// ------------------------------------------------------
// Classe per gestire un pulsante con debounce.
// ------------------------------------------------------
class ButtonDebounced_Class {
    public:
        // Costruttore: inizializza il pulsante.
        ButtonDebounced_Class(void);

        // ButtonDebounced_Class(const char* name, uint8_t pin_nr, uint8_t pressedLogicLevel); // l'ho eliminato, preferisco la init....
        void init(const char* name, uint8_t pin_nr, uint8_t pressedLogicLevel);

        // Metodo per leggere lo stato del pulsante con debounce.
        bool read(unsigned long debounceDelay);

        // Metodo pubblico per accedere al nome del pulsante (utile per debugging).
        // const char* getName() const { return m_name; }
        const char* pinID() const { return m_pinID; }
        const char* name() const { return m_name; }
        uint8_t     getPin() const { return m_pin; }
        uint8_t     pin() const { return m_pin; }


    private:
        const char*   m_name;                      // Nome del pulsante per identificazione (opzionale, utile per debugging).
        char          m_pinID[21];                 // [pin_name.pin_nr]:
        // bool          m_initialized       = false; // Il pin GPIO a cui è collegato il pulsante.
        uint8_t       m_pin               = 0;     // Il pin GPIO a cui è collegato il pulsante.
        uint8_t       m_pressedLogicLevel = 0;     // Livello logico che indica il pulsante premuto (LOW o HIGH).
        unsigned long m_lastDebounceTime  = 0;     // Ultimo momento in cui il pin ha cambiato stato RAW.
        bool          m_lastButtonState   = false; // Ultima lettura RAW del pin.
        bool          m_buttonPressed     = false; // Stato debounced: true se premuto, false se rilasciato.
};