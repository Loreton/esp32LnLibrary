/*
// updated by ...: Loreto Notarantonio
// Date .........: 27-06-2025 15.45.27
*/
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type

// ------------------------------------------------------
// Classe per gestire un pulsante con debounce.
// ------------------------------------------------------
class DebouncedButton_Class {
    public:
        // Costruttore: inizializza il pulsante.
        DebouncedButton_Class(int pin_nr, const char* name, int pressedLogicLevel);

        // Metodo per leggere lo stato del pulsante con debounce.
        bool read(unsigned long debounceDelay);

        // Metodo pubblico per accedere al nome del pulsante (utile per debugging).
        const char* getName() const { return m_name; }
        int getPin() const { return m_pin; }


    private:
        int m_pin;                   // Il pin GPIO a cui è collegato il pulsante.
        const char* m_name;          // Nome del pulsante per identificazione (opzionale, utile per debugging).
        char  m_pinID[21];           // [pin_name.pin_nr]:
        int m_pressedLogicLevel;     // Livello logico che indica il pulsante premuto (LOW o HIGH).
        bool m_lastButtonState;      // Ultima lettura RAW del pin.
        unsigned long m_lastDebounceTime; // Ultimo momento in cui il pin ha cambiato stato RAW.
        bool m_buttonPressed;        // Stato debounced: true se premuto, false se rilasciato.
};