/*
// updated by ...: Loreto Notarantonio
// Date .........: 21-06-2025 17.17.35
*/
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type


// ------------------------------------------------------
// Struttura per mantenere lo stato di ogni pulsante.
// con funzioni interne alla struttura
// ------------------------------------------------------
typedef struct deBouncedButton_func_inside {
    int                 _pin;               // Il pin GPIO a cui è collegato il pulsante.
    const char*         _name;              // Nome del pulsante per identificazione (opzionale, utile per debugging).
    int                 _pressedLogicLevel; // Livello logico che indica il pulsante premuto (LOW o HIGH).
    bool                _lastButtonState;   // Ultima lettura RAW del pin.
    unsigned long       _lastDebounceTime;  // Ultimo momento in cui il pin ha cambiato stato RAW.
    bool                _buttonPressed;     // Stato debounced: true se premuto, false se rilasciato.

    void setup(int pin_nr, const char* name, int pressedLogicLevel);
    bool read(unsigned long debounceDelay);

} deBouncedButton_func_inside; // end of structure
