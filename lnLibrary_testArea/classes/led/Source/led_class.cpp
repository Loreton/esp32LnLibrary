//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 19.13.39
//

#include "led_class.h" // Include l'header della classe
// Non serve #include <Arduino.h> qui, dato che è già in led_class.h
// Se però ci fossero funzioni di Arduino usate solo nell'implementazione, andrebbe qui.

// Implementazione del Costruttore
Led::Led(int ledPin) {
    pin = ledPin;
    pinMode(pin, OUTPUT);
    stato = false;
}

// Implementazione del metodo per accendere il LED
void Led::accendi() {
    digitalWrite(pin, HIGH);
    stato = true;
}

// Implementazione del metodo per spegnere il LED
void Led::spegni() {
    digitalWrite(pin, LOW);
    stato = false;
}

// Implementazione del metodo per invertire lo stato del LED
void Led::inverti() {
    stato = !stato;
    digitalWrite(pin, stato ? HIGH : LOW);
}

// Implementazione del metodo per ottenere lo stato attuale del LED
bool Led::isAcceso() {
    return stato;
}