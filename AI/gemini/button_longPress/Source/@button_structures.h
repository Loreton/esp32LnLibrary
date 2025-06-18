/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-06-2025 13.33.08
*/

#pragma once
// #ifndef __BUTTON_STRUCTURES_H__
//     #define __BUTTON_STRUCTURES_H__

    #define m_FNAME                  lnSERIAL.printf((PGM_P) PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)
    #define lnPrintF_FN(fmt, ...)    { m_FNAME;      lnSERIAL.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
    #define printf0_FN          lnPrintF_FN

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
    typedef struct ButtonState_t {
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
    } ButtonState_t;



    // functions prototypes
    bool readButton(ButtonState_t *btn, unsigned long debounceDelay=50);
    void notifyCurrentButtonLevel(ButtonState_t *btn);
    void setupButton(ButtonState_t *btn,
                    int pin,
                    const char* name,
                    int pressedLogicLevel,
                    const unsigned long thresholds[],
                    size_t thresholdsCount);

// #endif