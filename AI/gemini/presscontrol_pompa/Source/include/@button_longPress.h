/*
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 11.28.58
*/


#ifndef __BUTTON_STRUCTURES_H__
    #define __BUTTON_STRUCTURES_H__


    // Definisce i possibili tipi di pressione del pulsante.
    enum ButtonPressType {
        NO_PRESS = 0,        // Nessuna pressione rilevata o ancora in corso.
        SHORT_PRESS,         // Pressione breve (il primo livello valido dopo il debounce).
        MEDIUM_PRESS,        // Pressione media.
        LONG_PRESS,          // Pressione lunga.
        VERY_LONG_PRESS,     // Pressione molto lunga.
        // ... aggiungi altri livelli se necessario
        MAX_DEFINED_PRESS_LEVELS // Utile per scopi interni, non un vero "tipo di pressione".
    };




    // Struttura per mantenere lo stato di ogni pulsante.
    struct ButtonState {
        int                     pin;
        const char*             name;                       // Nome del pulsante per identificazione.
        int                     pressedLogicLevel;          // Livello logico che indica il pulsante premuto (LOW o HIGH).

        bool                    lastButtonState;            // Ultima lettura RAW del pin.
        unsigned long           lastDebounceTime;           // Ultimo momento in cui il pin ha cambiato stato RAW.

        bool                    buttonPressed;              // Stato debounced: true se premuto, false se rilasciato.
        unsigned long           pressStartTime;             // Timestamp quando il pulsante è stato premuto.
        unsigned long           pressDuration;              // Durata dell'ultima pressione (in ms).

        ButtonPressType         currentPressLevel;          // Livello di pressione attualmente raggiunto (aggiornato durante la pressione).
        bool                    maxLevelReachedAndNotified; // Flag per indicare se il massimo livello è stato già notificato.

        // Array e dimensione delle soglie specifiche per questo pulsante.
        const unsigned long*    pressThresholds;
        size_t                  numThresholds;
    };



    // functions prototypes
    bool readButton(ButtonState &btn, unsigned long debounceDelay=50);
    void notifyCurrentButtonLevel(ButtonState &btn);
    void setupButton(ButtonState &btn,
                    int pin,
                    const char* name,
                    int pressedLogicLevel,
                    const unsigned long thresholds[],
                    size_t thresholdsCount);

#endif