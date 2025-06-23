/*
// updated by ...: Loreto Notarantonio
// Date .........: 23-06-2025 16.51.03
*/

#pragma once
#include <Arduino.h>

#define passiveBuzzer_pin           22  // OUTPUT
#define activeBuzzer_pin            23  // OUTPUT
#define pressControlLED_pin         25  // OUTPUT
#define pumpLED_pin                 26  // OUTPUT

#define pressControlRelay_pin       16  // OUTPUT
#define pumpHornAlarm_pin           17  // OUTPUT

#define pressControlState_pin       18  // INPUT
#define pumpState_pin               19  // INPUT
#define startButton_pin             21  // INPUT


// Definisce i possibili tipi di pressione del pulsante.
enum ButtonPressedLevel : uint8_t {
    NO_PRESS = 0, // Nessuna pressione rilevata o ancora in corso.
    PRESSED_LEVEL_1,     // Pressione breve (il primo livello valido dopo il debounce).
    PRESSED_LEVEL_2,     // Pressione media.
    PRESSED_LEVEL_3,     // Pressione lunga.
    PRESSED_LEVEL_4,     // Pressione molto lunga.
    PRESSED_LEVEL_5,     // Pressione molto lunga.
    PRESSED_LEVEL_6,     // Pressione molto lunga.
    PRESSED_LEVEL_7,     // Pressione molto lunga.
    PRESSED_LEVEL_8,     // Pressione molto lunga.
    PRESSED_LEVEL_9,     // Pressione molto lunga.
    // ... aggiungi altri livelli se necessario
    MAX_DEFINED_PRESS_LEVELS // Utile per scopi interni, non un vero "tipo di pressione".
} ;


typedef void (*ButtonCallback)(struct pinLongPress_sc* self);
// esempio di callback
// void myButtonHandler_sample(pinLongPress_sc* btn) {
//     Serial.printf("Hai premuto: %s\n", btn->_name);
// }



// Struttura per mantenere lo stato di ogni pulsante.
typedef struct pinLongPress_sc { // io_input_pin_struct_t
    int                     _pin;
    const char*             _name;                       // Nome del pulsante per identificazione.
    char                    _pinID[21];                  // contiene [pin:%02d.%-15s] p->pin, p->name,
    int                     _pressedLogicLevel;          // Livello logico che indica il pulsante premuto (LOW o HIGH).

    bool                    _lastButtonState;            // Ultima lettura RAW del pin.
    unsigned long           _lastDebounceTime;           // Ultimo momento in cui il pin ha cambiato stato RAW.

    bool                    _buttonPressed;              // Stato debounced: true se premuto, false se rilasciato.
    unsigned long           _pressStartTime;             // Timestamp quando il pulsante è stato premuto.
    unsigned long           _pressDuration;              // Durata dell'ultima pressione (in ms).

    uint8_t                 _currentPressLevel;          // Livello di pressione attualmente raggiunto (aggiornato durante la pressione).
    uint8_t                 _lastPrintedLevel;          // Livello di pressione attualmente raggiunto (aggiornato durante la pressione).
    bool                    _maxLevelReachedAndNotified; // Flag per indicare se il massimo livello è stato già notificato.

    // Array e dimensione delle soglie specifiche per questo pulsante.
    const unsigned long*    _pressThresholds;
    size_t                  _numThresholds;

    //  Callback per la gestione della pressione
    ButtonCallback          _onPressCallback;


    // functions prototypes
    bool read(void);
    // void init(const char* name, int pin, int pressedLogicLevel, const unsigned long thresholds[], size_t thresholdsCount);
    void init(const char* name, int pin, int pressedLogicLevel, const unsigned long thresholds[], size_t thresholdsCount, ButtonCallback callback = nullptr);
    void process();
    void _checkNewLevel();
    // void notifyCurrentButtonLevel(ButtonState_t *btn, uint8_t buzzer_pin=99);

} pinLongPress_sc;

