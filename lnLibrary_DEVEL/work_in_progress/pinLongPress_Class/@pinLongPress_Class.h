/*
// updated by ...: Loreto Notarantonio
// Date .........: 26-06-2025 16.47.42
*/
#pragma once
#include <Arduino.h>

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


// Forward declaration for the callback type, now accepting a pointer to the class
// class PinLongPress_Class;
// typedef void (*ButtonCallback)(PinLongPress_Class* self);

// Forward declaration for PinController_Class since it's used in notifyCurrentLevel
class PinController_Class;

// Class to manage the state and logic for a long-press button.
class PinLongPress_Class {
    public: // Public members (accessible from outside the class)
        // Constructor: replaces the init() function.
        // It's good practice to provide default arguments if some parameters are optional.
        // PinLongPress_Class(const char* name, int pin, int pressedLogicLevel,
        //              const uint32_t thresholds[], size_t thresholdsCount, ButtonCallback callback=nullptr);
        PinLongPress_Class(const char* name, int pin, int pressedLogicLevel,
                     const uint32_t thresholds[], size_t thresholdsCount);

        // Public methods to interact with the button object.
        bool read();
        void process(); // If this function handles ongoing button states, it might be called externally.
                        // Based on your original code, process() was part of the struct, but its implementation was not provided.
                        // I'm keeping it for now, but often read() or a specific update() method handles the main logic.
        void printStatus(bool prompt); // Renamed for clarity and consistency with class methods.

        // New: notifyCurrentLevel is now a member function
        void notifyCurrentLevel(PinController_Class* activeBuzzer = nullptr);


        // Getters for relevant private members if external access is needed.
        // These provide controlled read-only access to private data.
        const char* getName() const { return m_name; }
        const char* getPinID() const { return m_pinID; }
        int         getPin() const { return m_pin; }
        int         getPressedLogicLevel() const { return m_pressedLogicLevel; }
        bool        isButtonPressed() const { return m_buttonPressed; }
        uint32_t    getPressDuration() const { return m_pressDuration; }
        uint8_t     getCurrentPressLevel() const { return m_currentPressLevel; }
        uint8_t     getLastPressedLevel() const { return m_lastPressedLevel; }
        bool        hasMaxLevelReachedAndNotified() const { return m_maxLevelReachedAndNotified; }

        // Consider adding a reset method if needed to clear states after an action.
        void        resetState();


    private: // Private members (encapsulated, accessible only within the class)
        int             m_pin;
        const char*     m_name;
        char            m_pinID[21];
        int             m_pressedLogicLevel;

        bool            m_lastButtonState;
        uint32_t        m_lastDebounceTime; // Not explicitly used in the provided read(), but good to keep.

        bool            m_buttonPressed;
        uint32_t        m_pressStartTime;
        uint32_t        m_pressDuration;

        uint8_t         m_currentPressLevel;
        uint8_t         m_lastPressedLevel;
        bool            m_maxLevelReachedAndNotified;

        const uint32_t* m_pressThresholds;
        size_t          m_numThresholds;

        // ButtonCallback       onPressCallback_;

        // Private helper methods
        void checkNewLevel();

        // Static member for beep timing, shared across all PinLongPress_Class instances
        // if beep logic is truly global. Otherwise, make it a non-static private member.
        // Given the previous static in notifyCurrentButtonLevel, this is likely intended.
        static uint32_t m_lastBeepTime;
};


// No longer needed as it's a member function
// void notifyCurrentButtonLevel(PinLongPress_Class* p, PinController_Class* activeBuzzer = nullptr);