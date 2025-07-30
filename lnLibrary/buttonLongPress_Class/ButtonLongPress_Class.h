/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025
// Converted from ButtonLongPress_Struct to ButtonLongPress_Class
*/

#pragma once
#include <Arduino.h>

// Definisce i possibili tipi di pressione del pulsante.
enum ButtonPressedLevel : uint8_t {
    NO_PRESS = 0,             // Nessuna pressione rilevata o ancora in corso.
    PRESSED_LEVEL_1,          // Pressione breve (il primo livello valido dopo il debounce).
    PRESSED_LEVEL_2,          // Pressione media.
    PRESSED_LEVEL_3,          // Pressione lunga.
    PRESSED_LEVEL_4,          // Pressione molto lunga.
    PRESSED_LEVEL_5,          // Pressione molto lunga.
    PRESSED_LEVEL_6,          // Pressione molto lunga.
    PRESSED_LEVEL_7,          // Pressione molto lunga.
    PRESSED_LEVEL_8,          // Pressione molto lunga.
    PRESSED_LEVEL_9,          // Pressione molto lunga.
    // ... aggiungi altri livelli se necessario
    MAX_DEFINED_PRESS_LEVELS // Utile per scopi interni, non un vero "tipo di pressione".
};

// Dichiarazione forward della classe per i callback
class ButtonLongPress_Class;

// Definizione del tipo di funzione di callback.
// Ora il callback riceve un puntatore all'istanza della classe ButtonLongPress_Class.
typedef void (*ButtonLongPressCallback)(ButtonLongPress_Class* self);

// Classe per gestire il rilevamento della pressione lunga di un pulsante.
class ButtonLongPress_Class {
public: // Membri pubblici

    // Costruttore: permette l'inizializzazione al momento della creazione dell'oggetto.
    // Ho aggiunto anche i parametri di inizializzazione per rendere il costruttore più utile.
    ButtonLongPress_Class(void);

    // Metodi pubblici

    void init(const char* name, uint8_t pin, bool pressedLogicLevel, const uint32_t thresholds[], int8_t thresholdsCount);
    bool read(ButtonLongPressCallback callback = nullptr);
    bool released(ButtonLongPressCallback callback = nullptr); // Simile a read, forse da consolidare.
    void process(); // Questo metodo sembra non essere chiamato esternamente nel tuo codice.
    void pressingLevelNotification(ButtonLongPressCallback onPressCallback = nullptr);
    void showStatus(void);
    void reset(void);
    bool pressedLevelChanged(bool fDisplay=false);

    // Funzioni inline (getter)
    inline bool maxLevelReached(void) const { return m_maxLevelReachedAndNotified; }
    inline bool isPressed(void) const { return (m_buttonPressed == m_pressedLogicLevel); }
    inline uint8_t currentPressLevel(void) const { return m_currentPressLevel; }
    inline uint8_t lastPressedLevel(void) const { return m_lastPressedLevel; }
    inline const char* pinID(void) const { return m_pinID; }

    // Membri pubblici che potrebbero essere consultati esternamente (es. per debug o display)
    // Considera se questi debbano essere `private` con getter specifici.
    uint8_t           m_pin = 0;
    const char* m_name;
    char              m_pinID[21];
    bool              m_pressedLogicLevel = false;
    uint32_t          m_elapsed = 0; // Tempo trascorso dalla pressione

private: // Membri privati (accessibili solo dall'interno della classe)
    bool              m_lastButtonState = false;
    bool              m_buttonPressed = false;
    uint32_t          m_pressStartTime = 0;
    uint8_t           m_currentPressLevel = NO_PRESS;
    uint8_t           m_lastPressedLevel = NO_PRESS;
    bool              m_maxLevelReachedAndNotified = false;

    const uint32_t* m_pressThresholds;
    uint32_t          m_gapThresholds[MAX_DEFINED_PRESS_LEVELS];
    int8_t            m_numThresholds = 0;

    uint32_t          m_lastDisplayTime = 0;
    uint32_t          m_lastMsgTime = 0;

    // Metodi privati
    void updatePressedLevel();
};

// Funzioni esterne correlate (se necessario mantenerle fuori dalla classe)
// Potresti anche considerare di rendere `ext_showStatus` un metodo statico o un semplice helper.
void ext_showStatus(ButtonLongPress_Class *p);