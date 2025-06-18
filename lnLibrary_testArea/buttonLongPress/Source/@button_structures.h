/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-06-2025 20.17.59
*/

#pragma once
#include "@SerialRead.h"

#define RELAY_PIN           16
#define startButton_pin     21
#define pumpState_pin       19  // INPUT


#define m_FNAME                  Serial.printf((PGM_P) PSTR("[%-20s:%04d] "), __FILENAME__, __LINE__)
#define lnPrintF(fmt, ...)       {               Serial.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
#define lnPrintF_FN(fmt, ...)    { m_FNAME;      Serial.printf_P((PGM_P) PSTR(fmt), ## __VA_ARGS__); }
#define printf0_FN              lnPrintF_FN
#define printf0                 lnPrintF




    // Definisce i possibili tipi di pressione del pulsante.
    // enum ButtonPressedLevel_ {
    //     NO_PRESS = 0, // Nessuna pressione rilevata o ancora in corso.
    //     PRESSED_LEVEL_1,     // Pressione breve (il primo livello valido dopo il debounce).
    //     PRESSED_LEVEL_2,     // Pressione media.
    //     PRESSED_LEVEL_3,     // Pressione lunga.
    //     PRESSED_LEVEL_4,     // Pressione molto lunga.
    //     PRESSED_LEVEL_5,     // Pressione molto lunga.
    //     PRESSED_LEVEL_6,     // Pressione molto lunga.
    //     PRESSED_LEVEL_7,     // Pressione molto lunga.
    //     PRESSED_LEVEL_8,     // Pressione molto lunga.
    //     PRESSED_LEVEL_9,     // Pressione molto lunga.
    //     // ... aggiungi altri livelli se necessario
    //     MAX_DEFINED_PRESS_LEVELS // Utile per scopi interni, non un vero "tipo di pressione".
    // } ;


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
void notifyCurrentButtonLevel(ButtonState_t *btn);
void setupButton(ButtonState_t *btn,
                int pin,
                const char* name,
                int pressedLogicLevel,
                const unsigned long thresholds[],
                size_t thresholdsCount);

void processButton(ButtonState_t *btn);




    #ifdef __I_AM_MAIN_CPP__
        const char PROGMEM *str_action[]     = {"released", "pressed"};
        const char PROGMEM *str_pinLevel[]   = {"LOW", "HIGH"};
        const char PROGMEM *str_TrueFalse[]  = {"FALSE", "TRUE"};
        const char PROGMEM *str_OffOn[]      = {"OFF", "ON"};
        const char PROGMEM *str_ON           = "ON";
        const char PROGMEM *str_OFF          = "OFF";
        const char PROGMEM *str_INPUT        = "INPUT";
        const char PROGMEM *str_INPUT_PULLUP = "INPUT_PULLUP";
        const char PROGMEM *str_OUTPUT       = "OUTPUT";
        // ------- in pinOperations.cpp ....devo capire perché non posso spostarli
        // const char * PROGMEM THRESHOLD_LEVEL_TYPES[] = {"NO_PRESSED_BUTTON", "PRESSED_LEVEL_01", "PRESSED_LEVEL_02", "PRESSED_LEVEL_03", "PRESSED_LEVEL_04", "PRESSED_LEVEL_05", "PRESSED_LEVEL_06", "OVERFLOW_TIME"};
        // const int8_t THRESHOLDS_LEVELS_TYPES_length = sizeof(THRESHOLD_LEVEL_TYPES)/sizeof(char *);
    #else
        extern const char PROGMEM *str_action[];
        extern const char PROGMEM *str_pinLevel[];
        extern const char PROGMEM *str_TrueFalse[];
        extern const char PROGMEM *str_OffOn[];
        extern const char PROGMEM *str_ON;
        extern const char PROGMEM *str_OFF;
        extern const char PROGMEM *str_INPUT;
        extern const char PROGMEM *str_INPUT_PULLUP;
        extern const char PROGMEM *str_OUTPUT;
        // extern const char * PROGMEM THRESHOLD_LEVEL_TYPES[];
        // extern const int8_t THRESHOLDS_LEVELS_TYPES_length;
    #endif
