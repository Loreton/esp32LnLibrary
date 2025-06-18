/*
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 12.16.36
*/


#ifndef __BUTTON_STRUCTURES_H__
    #define __BUTTON_STRUCTURES_H__


    // Struttura per mantenere lo stato di ogni pulsante.
    struct ButtonState {
        int pin;                  // Il pin GPIO a cui è collegato il pulsante.
        const char* name;         // Nome del pulsante per identificazione (opzionale, utile per debugging).
        int pressedLogicLevel;    // Livello logico che indica il pulsante premuto (LOW o HIGH).
        bool lastButtonState;     // Ultima lettura RAW del pin.
        unsigned long lastDebounceTime; // Ultimo momento in cui il pin ha cambiato stato RAW.
        bool buttonPressed;       // Stato debounced: true se premuto, false se rilasciato.
    };



    // functions prototypes
    bool readButton(ButtonState &btn, unsigned long debounceDelay=50);
    // void notifyCurrentButtonLevel(ButtonState &btn);
    void setupButton(ButtonState &btn,
                    int pin,
                    const char* name,
                    int pressedLogicLevel);

#endif