/*
// updated by ...: Loreto Notarantonio
// Date .........: 25-07-2025 10.48.46
*/

#pragma once
#include <Arduino.h>

// #include "@pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto

#ifdef __ln_MODULE_DEBUG_TEST__
    #define passiveBuzzer_pin           22  // OUTPUT
    #define activeBuzzer_pin            23  // OUTPUT
    #define pressControlLED_pin         25  // OUTPUT
    #define pumpLED_pin                 26  // OUTPUT

    #define pressControlRelay_pin       16  // OUTPUT
    #define pumpHornAlarm_pin           17  // OUTPUT

    #define pressControlState_pin       18  // INPUT
    #define pumpState_pin               19  // INPUT
    #define startButton_pin             21  // INPUT
#endif

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


struct ButtonLongPress_Struct;
typedef void (*ButtonLongPressCallback)(struct ButtonLongPress_Struct* self);
typedef void (*BeepCallBack)(struct ButtonLongPress_Struct* self, uint32_t beep_duration);
// questa essendo uguale non la uso
// typedef void (*notifyLevelCallback)(struct ButtonLongPress_Struct* self);






// Struttura per mantenere lo stato di ogni pulsante.
typedef struct ButtonLongPress_Struct { // io_input_pin_struct_t
    uint8_t         m_pin = 0;
    const char*     m_name;                       // Nome del pulsante per identificazione.
    char            m_pinID[21];                  // contiene [pin:%02d.%-15s] p->pin, p->name,
    bool            m_pressedLogicLevel = false;          // Livello logico che indica il pulsante premuto (LOW o HIGH).

    bool            m_lastButtonState=false;            // Ultima lettura RAW del pin.
    bool            m_buttonPressed=false;              // Stato debounced: true se premuto, false se rilasciato.
    uint32_t        m_pressStartTime=0;             // Timestamp quando il pulsante è stato premuto.
    // uint32_t        m_pressDuration=0;              // Durata dell'ultima pressione (in ms).
    uint32_t        m_elapsed=0;                    // tempo passato dalla pressione (in ms).

    uint8_t         m_currentPressLevel=NO_PRESS;          // Livello di pressione attualmente raggiunto (aggiornato durante la pressione).
    uint8_t         m_lastPressedLevel=NO_PRESS;          // Livello di pressione precedentemente salvato.
    bool            m_maxLevelReachedAndNotified=false; // Flag per indicare se il massimo livello è stato già notificato.

    // Array e dimensione delle soglie specifiche per questo pulsante.
    const uint32_t* m_pressThresholds;
    uint32_t        m_gapThresholds[MAX_DEFINED_PRESS_LEVELS]; // inseriamo il gap tra i vari livelli di threshold per gestire eventuale allarme
    int8_t          m_numThresholds=0;


    // costruttore
    ButtonLongPress_Struct(void);

    // functions prototypes
    bool read(ButtonLongPressCallback callback=nullptr);
    bool released(ButtonLongPressCallback callback=nullptr);
    void init(const char* name, uint8_t pin, bool pressedLogicLevel, const uint32_t thresholds[], int8_t thresholdsCount );
    void process();
    void _checkNewLevel();

    // void pressingLevelNotification(BeepCallBack onPressCallback=nullptr);
    void pressingLevelNotification(ButtonLongPressCallback onPressCallback=nullptr);
    void showStatus(void);
    void reset(void);

    inline bool isMaxLevelReached(void) const {return m_maxLevelReachedAndNotified;};
    inline bool isPressed(void) const {return (m_buttonPressed == m_pressedLogicLevel); };
    inline uint8_t pressedLevel(void) const {return m_currentPressLevel; };

    // inline functions
    const char* pinID(void) const { return m_pinID; }


} ButtonLongPress_Struct;




// ###########################################################################################
// #  Funzioni esterne alla struttura ma comuncque correlate
// #  Funzioni esterne alla struttura ma comuncque correlate
// #  Funzioni esterne alla struttura ma comuncque correlate
// #  Funzioni esterne alla struttura ma comuncque correlate
// ###########################################################################################
    // #include "@pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto

    // void notifyCurrentButtonLevel(ButtonLongPress_Struct *p, pinController_Struct *activeBuzzer = nullptr);
    // void pinLongPressDisplayStatus(ButtonLongPress_Struct *p, bool prompt);

    void ext_showStatus(ButtonLongPress_Struct *p);