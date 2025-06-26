/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 17.52.55
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
        const char* getName() const { return name_; }
        int getPin() const { return pin_; }


    private:
        int pin_;                   // Il pin GPIO a cui è collegato il pulsante.
        const char* name_;          // Nome del pulsante per identificazione (opzionale, utile per debugging).
        int pressedLogicLevel_;     // Livello logico che indica il pulsante premuto (LOW o HIGH).
        bool lastButtonState_;      // Ultima lettura RAW del pin.
        unsigned long lastDebounceTime_; // Ultimo momento in cui il pin ha cambiato stato RAW.
        bool buttonPressed_;        // Stato debounced: true se premuto, false se rilasciato.
};