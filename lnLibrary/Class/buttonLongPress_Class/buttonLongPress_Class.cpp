//
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 09.31.52
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#include <Arduino.h>

#define LOG_LEVEL_0
#define LOG_LEVEL_2
#define LOG_LEVEL_99x
// #include "logMacros.h" // printf:XFN()
#include "lnString.h" // setPinID()
#include "lnGlobalVars.h" // strXXX()

#include "lnButtonLongPress_Class.h"
#include "lnLedController_Class.h" // Include for LedController_Class usage

// Initialize the static member variable outside the class definition
uint32_t ButtonLongPress_Class::m_lastBeepTime = 0;

/** non ha molto senso....
// 1. Implementation of the Default Constructor
ButtonLongPress_Class::ButtonLongPress_Class() {
    m_pin = -1; // -1 indicates an uninitialized/invalid pin
    m_name = "????"; // A literal string for safety
    snprintf(m_pinID, sizeof(m_pinID), "[%02d.%-14s]:", m_pin, m_name);

}
*/

ButtonLongPress_Class::ButtonLongPress_Class(void) {};


/**
 * @brief Constructor for the ButtonLongPress_Class class.
 * Initializes a button by configuring the pin and its initial state.
 *
 * @param name:               A string that identifies the button (e.g., "Button1").
 * @param pin:                The GPIO pin number the button is connected to.
 * @param pressedLogicLevel:  The logic level (LOW or HIGH) indicating the button is pressed.
 * - Use LOW for a button connected between pin and GND (with INPUT_PULLUP).
 * - Use HIGH for a button connected between pin and VCC (with INPUT, requires external pull-down).
 * @param thresholds:         Array of uint32_t containing time thresholds (in ms) for press levels.
 * Thresholds MUST be in ascending order. Index 0 corresponds to PRESSED_LEVEL_1, etc.
 * @param thresholdsCount:    The number of elements in the `thresholds` array.
 * @param callback:           Optional callback function to be executed on button press.
 */
// ButtonLongPress_Class::ButtonLongPress_Class(const char* name, int pin, int pressedLogicLevel,
//                            const uint32_t thresholds[], size_t thresholdsCount,
//                            ButtonCallback callback) {
/*
ButtonLongPress_Class::ButtonLongPress_Class(const char* name, int8_t pin, int8_t pressedLogicLevel,
                           const uint32_t thresholds[], size_t thresholdsCount) {
    m_pin = pin;
    m_name = name;
    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);



    m_pressedLogicLevel = pressedLogicLevel;
    m_pressThresholds = thresholds;
    m_numThresholds = thresholdsCount;

    if (m_pressedLogicLevel == LOW) {
        pinMode(m_pin, INPUT_PULLUP);
    } else {
        pinMode(m_pin, INPUT);
    }

    m_lastButtonState = digitalRead(m_pin);
    m_buttonPressed = (m_lastButtonState == m_pressedLogicLevel);

}

*/
void ButtonLongPress_Class::init(const char* name, int8_t pin, int8_t pressedLogicLevel, const uint32_t thresholds[], uint8_t thresholdsCount) {
    m_pin = pin;
    m_name = name;
    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

    m_pressedLogicLevel = pressedLogicLevel;
    m_pressThresholds = thresholds;
    m_numThresholds = thresholdsCount;

    if (m_pressedLogicLevel == LOW) {
        pinMode(m_pin, INPUT_PULLUP);
    } else {
        pinMode(m_pin, INPUT);
    }

    m_lastButtonState = digitalRead(m_pin);
    m_buttonPressed = (m_lastButtonState == m_pressedLogicLevel);

}


/**
 * @brief Helper function to check for new press levels based on current press duration.
 * This is now a private member function.
 */
void ButtonLongPress_Class::checkNewLevel() {
    uint32_t currentPressTime = millis() - m_pressStartTime;
    uint8_t newLevel = NO_PRESS;

    for (int i = m_numThresholds - 1; i >= 0; i--) {
        if (currentPressTime >= m_pressThresholds[i]) {
            newLevel = static_cast<uint8_t>(i + 1);
            break;
        }
    }

    if (newLevel > m_currentPressLevel && newLevel <= m_numThresholds) {
        m_currentPressLevel = newLevel;
    }
}

/**
 * @brief Reads the button state with debounce and detects press levels.
 * This function should be called repeatedly in the main loop.
 *
 * @return: true if the button was RELEASED after a valid press; false otherwise.
 *
 * Details about press duration and level reached are available within the class instance.
 * It's the responsibility of the calling function to reset pressDuration_, currentPressLevel_
 * and maxLevelReachedAndNotified_ after processing them.
 */
bool ButtonLongPress_Class::read() {
    bool state = digitalRead(m_pin);

    // State change detection (simple edge detection)
    if (state != m_buttonPressed) {
        // Update debounced state
        m_buttonPressed = state;

        // If the button has just been PRESSED (debounced state is now pressedLogicLevel).
        if (m_buttonPressed == m_pressedLogicLevel) {
            m_pressStartTime = millis(); // Record the press start time.
            // DO NOT reset level parameters here; the caller will reset after processing the *previous* release.
        }
        // If the button has just been RELEASED (debounced state is now different from pressedLogicLevel).
        else {
            // Calculate press duration only if it was actually pressed.
            if (m_pressStartTime != 0) {
                m_pressDuration = millis() - m_pressStartTime;
                m_pressStartTime = 0; // Reset the press start timer.

                // Execute the callback if set and the button was pressed.
                // if (onPressCallback_) {
                //     onPressCallback_(this); // Pass 'this' (pointer to the current object)
                // }

                // DO NOT reset level parameters here; the caller will reset after reading them.
                return true; // The button was released.
            }
        }
    }

    // If the button is currently PRESSED (debounced) and the timer is active, update the press level.
    if (m_buttonPressed == m_pressedLogicLevel && m_pressStartTime != 0) {
        checkNewLevel();

        if (m_currentPressLevel == static_cast<uint8_t>(m_numThresholds) && !m_maxLevelReachedAndNotified) {
            printf0_FN("[%s] MAX press level reached: %d\n", m_name, m_currentPressLevel);
            m_maxLevelReachedAndNotified = true;
        }
    }

    m_lastButtonState = state; // Update last raw state for the next cycle.
    return false;
}

/**
 * @brief Resets the button's press state variables.
 * This is helpful after an action has been taken based on a press,
 * preparing the button for the next press cycle.
 */
void ButtonLongPress_Class::resetState() {
    m_pressDuration = 0;
    m_currentPressLevel = NO_PRESS;
    m_lastPressedLevel = NO_PRESS;
    m_maxLevelReachedAndNotified = false;
    m_pressStartTime = 0; // Ensure start time is reset if not already.
    m_buttonPressed = (m_lastButtonState == m_pressedLogicLevel); // Re-evaluate buttonPressed based on current raw state
}


// ###########################################################################
// #
// ###########################################################################
void ButtonLongPress_Class::printStatus(bool prompt) {
    printf0_FN("%s\n", m_pinID);
    printf0_FN("\t%-18s: %2d - %s\n",   "pressedLogicLevel",           m_pressedLogicLevel,           str_pinLevel[m_pressedLogicLevel]);
    printf0_FN("\t%-18s: %2d - %s\n",   "buttonPressed",               m_buttonPressed,               str_TrueFalse[m_buttonPressed]);
    printf0_FN("\t%-18s: %2d - %s\n",   "maxLevelReachedAndNotified", m_maxLevelReachedAndNotified,  str_TrueFalse[m_maxLevelReachedAndNotified]);

    printf0_FN("\t%-18s: nr:%2d --> [ ", "threshold", m_numThresholds);
    for (int8_t j = 0; j < m_numThresholds; j++) {
        printf0("%2ld, ", m_pressThresholds[j]); // Use %ld for uint32_t
    }
    printf0(" ]\n"); // close threshold line

    printf0("\n"); // blank line
    if (prompt) {
        // waitForEnter();
    }
}


// ###########################################################################################
// #  NOTIFY CURRENT BUTTON LEVEL - NOW A MEMBER FUNCTION
// ###########################################################################################

#define ALARM_BEEP_INTERVAL 2000


/**
 * @brief Notifies continuously about the current press level while the button is pressed.
 * This function is now a member of the ButtonLongPress_Class class.
 *
 * @param buzzer: Pointer to an active buzzer object to provide audible feedback.
 */
void ButtonLongPress_Class::notifyCurrentLevel(LedController_Class *buzzer) {
    uint16_t beep_duration = 300;
    uint16_t phase_beep_duration;
    uint32_t next_interval;
    uint32_t elapsed;

    // Check if the button is currently pressed according to its logic level
    if (m_buttonPressed == m_pressedLogicLevel) {
        // If a press level has been reached (not NO_PRESS)
        if (m_currentPressLevel != NO_PRESS) {
            // If the current level is different from the last notified level, print and beep
            if (m_currentPressLevel != m_lastPressedLevel) {
                elapsed = millis() - m_pressStartTime;
                next_interval = m_pressThresholds[m_currentPressLevel+1] - m_pressThresholds[m_currentPressLevel];
                if (m_currentPressLevel < m_numThresholds) {
                    next_interval = m_pressThresholds[m_currentPressLevel] - m_pressThresholds[m_currentPressLevel-1];
                }
                printf0_FN("[%s] elapsed ms:%6ld - PRESSED_LEVEL_%d/%d\n", m_pinID, elapsed, m_currentPressLevel, m_numThresholds);
                phase_beep_duration = next_interval / 5;
                printf0_FN("[%s] next_interval: %lu - beep_duration: %lu\n", m_pinID, next_interval, phase_beep_duration);

                // per evitare che il beep sia più lungo del tempo del livello
                // phase_beep_duration = beep_duration*m_currentPressLevel;
                    // if (phase_beep_duration > next_interval) {
                    // phase_beep_duration = beep_duration*m_currentPressLevel-1;
                    // }
                    // printf0_FN("[%s] next interval in: %ld ms \n", m_pinID, m_currentPressLevel, next_interval);
                // }
                // printf0_FN("[%s] ms:%6ld - PRESSED_LEVEL_%d  next level in: %ld ms\n", (millis() - m_pressStartTime), m_pinID, m_currentPressLevel, next_interval);
                switch (m_currentPressLevel) {
                    case PRESSED_LEVEL_1:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_2:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_3:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_4:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_5:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_6:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_7:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_8:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    case PRESSED_LEVEL_9:
                        if (buzzer) buzzer->pulse(phase_beep_duration);
                        break;
                    default:
                        printf0_FN("sono nel default: %d\n", m_currentPressLevel);
                        break;
                }
                m_lastPressedLevel = m_currentPressLevel; // Update the last notified level
            }

            // --- BEEP LOGIC EVERY ALARM_BEEP_INTERVAL ---
            if (m_maxLevelReachedAndNotified) {
                if (millis() - m_lastBeepTime >= ALARM_BEEP_INTERVAL) {
                    if (buzzer) buzzer->pulse(1000);
                    m_lastBeepTime = millis();
                }
            }
        }
    } else { // Button is not pressed or has been released
        m_lastPressedLevel = NO_PRESS; // Reset for the next press cycle
    }
}



