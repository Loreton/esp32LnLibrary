/*
// updated by ...: Loreto Notarantonio
// Date .........: 02-07-2025 18.15.23
*/
#pragma once
#include <Arduino.h>

#ifdef __ln_MODULE_DEBUG_TEST__
    // Define pins globally if they are truly global constants,
    // or consider making them static const members if they relate
    // specifically to a single class instance or are part of a related system.
    // For now, keeping them as defines is acceptable if they are used broadly.
    #define passiveBuzzer_pin            22  // OUTPUT
    #define activeBuzzer_pin             23  // OUTPUT
    #define pressControlLED_pin          25  // OUTPUT
    #define pumpLED_pin                  26  // OUTPUT

    #define pressControlRelay_pin        16  // OUTPUT
    #define pumpHornAlarm_pin            17  // OUTPUT

    #define pressControlState_pin        18  // INPUT
    #define pumpState_pin                19  // INPUT
    #define startButton_pin              21  // INPUT
#endif

// Defines the possible button press levels.
enum ButtonPressedLevel : uint8_t {
    NO_PRESS = 0, // No press detected or still ongoing.
    PRESSED_LEVEL_1,      // Short press (the first valid level after debounce).
    PRESSED_LEVEL_2,      // Medium press.
    PRESSED_LEVEL_3,      // Long press.
    PRESSED_LEVEL_4,      // Very long press.
    PRESSED_LEVEL_5,      // Very long press.
    PRESSED_LEVEL_6,      // Very long press.
    PRESSED_LEVEL_7,      // Very long press.
    PRESSED_LEVEL_8,      // Very long press.
    PRESSED_LEVEL_9,      // Very long press.
    MAX_DEFINED_PRESS_LEVELS // Useful for internal purposes, not a true "press type".
};





typedef void (*BuzzerCallback)(uint16_t duration);

// Forward declaration for LedController_Class since it's used in notifyCurrentLevel
class LedController_Class;

// Forward declaration for the callback type, now accepting a pointer to the class
class ButtonLongPress_Class;
typedef void (*ButtonCallback)(ButtonLongPress_Class* self);


// typedef void (*CB_notifyHandler)(class ButtonLongPress_Class* self);
// typedef void (*buzzerCB)(uint16_t duration);
// typedef void (*ButtonCallback)(struct pinLongPress_sc* self);



// Class to manage the state and logic for a long-press button.
class ButtonLongPress_Class {
    public: // Public members (accessible from outside the class)

        // 1. Default Constructor (new)
        // ButtonLongPress_Class();

        ButtonLongPress_Class(void);

        // Constructor: replaces the init() function.
        // It's good practice to provide default arguments if some parameters are optional.
        // ButtonLongPress_Class(const char* name, int pin, int pressedLogicLevel,
        //              const uint32_t thresholds[], size_t thresholdsCount, ButtonCallback callback=nullptr);
        // ButtonLongPress_Class(const char* name, int8_t pin, int8_t pressedLogicLevel, const uint32_t thresholds[], size_t thresholdsCount);

        // Public methods to interact with the button object.
        void init(const char* name, int8_t pin, int8_t pressedLogicLevel, const uint32_t thresholds[], uint8_t thresholdsCount);
        bool read();
        void process(); // If this function handles ongoing button states, it might be called externally.
                        // Based on your original code, process() was part of the struct, but its implementation was not provided.
                        // I'm keeping it for now, but often read() or a specific update() method handles the main logic.
        void printStatus(bool prompt); // Renamed for clarity and consistency with class methods.

        // New: notifyCurrentLevel is now a member function
        void notifyCurrentLevel(LedController_Class* activeBuzzer = nullptr);


        // Getters for relevant private members if external access is needed.
        // These provide controlled read-only access to private data.
        const char* name() const { return m_name; }
        const char* pinID() const { return m_pinID; }
        int         getPin() const { return m_pin; }
        int         getPressedLogicLevel() const { return m_pressedLogicLevel; }
        bool        isButtonPressed() const { return m_buttonPressed; }
        uint32_t    getPressDuration() const { return m_pressDuration; }
        uint8_t     getCurrentPressLevel() const { return m_currentPressLevel; }
        uint8_t     getLastPressedLevel() const { return m_lastPressedLevel; }
        bool        hasMaxLevelReachedAndNotified() const { return m_maxLevelReachedAndNotified; }
        // new GET
        // int         pin() const { return m_pin; }
        // int         pressedLogicLevel() const { return m_pressedLogicLevel; }
        // bool        buttonPressed() const { return m_buttonPressed; }
        // uint32_t    pressDuration() const { return m_pressDuration; }
        // uint8_t     currentPressLevel() const { return m_currentPressLevel; }
        // uint8_t     lastPressedLevel() const { return m_lastPressedLevel; }
        // // new PUT
        // void     setCurrentPressLevel() { bool level };


        // Consider adding a reset method if needed to clear states after an action.
        void        resetState();



    private: // Private members (encapsulated, accessible only within the class)
        int8_t          m_pin                        = 99;
        const char*     m_name                       = nullptr;
        char            m_pinID[18+1];                    // [pin_name.pin_nr]:  char pinID[10+1+6+2]; // name_len + 1 + 6+2
        int8_t          m_pressedLogicLevel          = LOW;

        bool            m_lastButtonState            = false;
        uint32_t        m_lastDebounceTime           = 0; // Not explicitly used in the provided read(), but good to keep.

        bool            m_buttonPressed              = false;
        uint32_t        m_pressStartTime             = 0;
        uint32_t        m_pressDuration              = 0;

        uint8_t         m_currentPressLevel          = NO_PRESS;
        uint8_t         m_lastPressedLevel           = NO_PRESS;
        bool            m_maxLevelReachedAndNotified = false;

        const uint32_t* m_pressThresholds            = nullptr;
        uint8_t          m_numThresholds              = 0;


        // Private helper methods
        void checkNewLevel();

        // Static member for beep timing, shared across all ButtonLongPress_Class instances
        // if beep logic is truly global. Otherwise, make it a non-static private member.
        // Given the previous static in notifyCurrentButtonLevel, this is likely intended.
        static uint32_t m_lastBeepTime;
};


// No longer needed as it's a member function
// void notifyCurrentButtonLevel(ButtonLongPress_Class* p, LedController_Class* activeBuzzer = nullptr);