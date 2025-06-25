/*
// updated by ...: Loreto Notarantonio
// Date .........: 21-06-2025 15.53.50
*/

#include <Arduino.h>

#pragma once
#include "@SerialRead.h"

#define RELAY_PIN           16
#define startButton_pin     21
#define pumpState_pin       19  // INPUT
#define activeBuzzer_pin    23  // OUTPUT
#define passiveBuzzer_pin   22  // OUTPUT

#define fPROMPT             true



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




    // Struttura per mantenere lo stato di ogni pulsante.
    typedef struct ButtonState_t { // io_input_pin_struct_t
        int                     pin;
        const char*             name;                       // Nome del pulsante per identificazione.
        char                    pinID[21];                  // contiene [pin:%02d.%-15s] p->pin, p->name,
        int                     pressedLogicLevel;          // Livello logico che indica il pulsante premuto (LOW o HIGH).

        bool                    lastButtonState;            // Ultima lettura RAW del pin.
        unsigned long           lastDebounceTime;           // Ultimo momento in cui il pin ha cambiato stato RAW.

        bool                    buttonPressed;              // Stato debounced: true se premuto, false se rilasciato.
        unsigned long           pressStartTime;             // Timestamp quando il pulsante è stato premuto.
        unsigned long           pressDuration;              // Durata dell'ultima pressione (in ms).

        // ButtonPressedLevel      currentPressLevel;          // Livello di pressione attualmente raggiunto (aggiornato durante la pressione).
        // ButtonPressedLevel      lastPrintedLevel;          // Livello di pressione attualmente raggiunto (aggiornato durante la pressione).
        uint8_t      currentPressLevel;          // Livello di pressione attualmente raggiunto (aggiornato durante la pressione).
        uint8_t      lastPrintedLevel;          // Livello di pressione attualmente raggiunto (aggiornato durante la pressione).
        bool                    maxLevelReachedAndNotified; // Flag per indicare se il massimo livello è stato già notificato.

        // Array e dimensione delle soglie specifiche per questo pulsante.
        const unsigned long*    pressThresholds;
        size_t                  numThresholds;
    } ButtonState_t;




// functions prototypes
bool readButton(ButtonState_t *btn);
void notifyCurrentButtonLevel(ButtonState_t *btn, uint8_t buzzer_pin=99);
void setupButton(ButtonState_t *btn,
                int pin,
                const char* name,
                int pressedLogicLevel,
                const unsigned long thresholds[],
                size_t thresholdsCount);

void processButton(ButtonState_t *btn);
void buttonLP_pinStatus(ButtonState_t *p, bool prompt);



void attachBuzzer(uint8_t buzzer_pin, uint8_t resolution, uint8_t channel=0) ;
void playTone(int frequency, int duration, uint8_t channel=0);
void noTone(uint8_t channel);
void detachBuzzer(uint8_t buzzer_pin);

