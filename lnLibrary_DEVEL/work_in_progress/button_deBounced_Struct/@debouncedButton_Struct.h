/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.44.14
*/
#pragma once
#include <Arduino.h>    // in testa anche per le definizioni dei type


// ------------------------------------------------------
// Struttura per mantenere lo stato di ogni pulsante.
// con funzioni interne alla struttura
// ------------------------------------------------------
typedef struct deBouncedButton_sc {
    int                 pin_;               // Il pin GPIO a cui è collegato il pulsante.
    const char*         name_;              // Nome del pulsante per identificazione (opzionale, utile per debugging).
    int                 pressedLogicLevel_; // Livello logico che indica il pulsante premuto (LOW o HIGH).
    bool                lastButtonState_;   // Ultima lettura RAW del pin.
    unsigned long       lastDebounceTime_;  // Ultimo momento in cui il pin ha cambiato stato RAW.
    bool                buttonPressed_;     // Stato debounced: true se premuto, false se rilasciato.

    void init(int pin_nr, const char* name, int pressedLogicLevel);
    bool read(unsigned long debounceDelay);

} deBouncedButton_sc; // end of structure
