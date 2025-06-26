//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 20.32.13
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#include <Arduino.h>

#define LOG_LEVEL_0
#define LOG_LEVEL_2
#define LOG_LEVEL_99x
#include "@globalVars.h" // printf:XFN()

#include "@pinLongPress_Class.h"
#include "@pinController_Class.h" // Include for PinController_Class usage

/**
 * @brief Constructor for the PinLongPress_Class class.
 * Initializes a button by configuring the pin and its initial state.
 *
 * @param name:               A string that identifies the button (e.g., "Button1").
 * @param pin:                The GPIO pin number the button is connected to.
 * @param pressedLogicLevel:  The logic level (LOW or HIGH) indicating the button is pressed.
 * - Use LOW for a button connected between pin and GND (with INPUT_PULLUP).
 * - Use HIGH for a button connected between pin and VCC (with INPUT, requires external pull-down).
 * @param thresholds:         Array of unsigned long containing time thresholds (in ms) for press levels.
 * Thresholds MUST be in ascending order. Index 0 corresponds to PRESSED_LEVEL_1, etc.
 * @param thresholdsCount:    The number of elements in the `thresholds` array.
 * @param callback:           Optional callback function to be executed on button press.
 */
PinLongPress_Class::PinLongPress_Class(const char* name, int pin, int pressedLogicLevel,
                           const unsigned long thresholds[], size_t thresholdsCount) {
                           // ButtonCallback callback) {
    pin_ = pin;
    name_ = name;
    snprintf(pinID_, sizeof(pinID_), "[%02d.%-14s]:", pin_, name_); // Use sizeof for buffer safety

    pressedLogicLevel_ = pressedLogicLevel;
    pressThresholds_ = thresholds;
    numThresholds_ = thresholdsCount;

    if (pressedLogicLevel_ == LOW) {
        pinMode(pin_, INPUT_PULLUP);
    } else {
        pinMode(pin_, INPUT);
    }

    lastButtonState_ = digitalRead(pin_);
    lastDebounceTime_ = 0; // Initialize, though not directly used in the provided read()
    buttonPressed_ = (lastButtonState_ == pressedLogicLevel_);
    pressStartTime_ = 0;
    pressDuration_ = 0;
    currentPressLevel_ = NO_PRESS;
    lastPressedLevel_ = NO_PRESS;
    maxLevelReachedAndNotified_ = false;
    // onPressCallback_ = callback;
}


/**
 * @brief Helper function to check for new press levels based on current press duration.
 * This is now a private member function.
 */
void PinLongPress_Class::_checkNewLevel() {
    unsigned long currentPressTime = millis() - pressStartTime_;
    uint8_t newLevel = NO_PRESS;

    for (int i = numThresholds_ - 1; i >= 0; i--) {
        if (currentPressTime >= pressThresholds_[i]) {
            newLevel = static_cast<uint8_t>(i + 1);
            break;
        }
    }

    if (newLevel > currentPressLevel_ && newLevel <= numThresholds_) {
        currentPressLevel_ = newLevel;
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
bool PinLongPress_Class::read() {
    bool state = digitalRead(pin_);

    // State change detection (simple edge detection)
    if (state != buttonPressed_) {
        // Update debounced state
        buttonPressed_ = state;

        // If the button has just been PRESSED (debounced state is now pressedLogicLevel).
        if (buttonPressed_ == pressedLogicLevel_) {
            pressStartTime_ = millis(); // Record the press start time.
            // DO NOT reset level parameters here; the caller will reset after processing the *previous* release.
        }
        // If the button has just been RELEASED (debounced state is now different from pressedLogicLevel).
        else {
            // Calculate press duration only if it was actually pressed.
            if (pressStartTime_ != 0) {
                pressDuration_ = millis() - pressStartTime_;
                pressStartTime_ = 0; // Reset the press start timer.

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
    if (buttonPressed_ == pressedLogicLevel_ && pressStartTime_ != 0) {
        _checkNewLevel();

        if (currentPressLevel_ == static_cast<uint8_t>(numThresholds_) && !maxLevelReachedAndNotified_) {
            printf0_FN("[%s] MAX press level reached: %d\n", name_, currentPressLevel_);
            maxLevelReachedAndNotified_ = true;
        }
    }

    lastButtonState_ = state; // Update last raw state for the next cycle.
    return false;
}

/**
 * @brief Resets the button's press state variables.
 * This is helpful after an action has been taken based on a press,
 * preparing the button for the next press cycle.
 */
void PinLongPress_Class::resetState() {
    pressDuration_ = 0;
    currentPressLevel_ = NO_PRESS;
    lastPressedLevel_ = NO_PRESS;
    maxLevelReachedAndNotified_ = false;
    pressStartTime_ = 0; // Ensure start time is reset if not already.
    buttonPressed_ = (lastButtonState_ == pressedLogicLevel_); // Re-evaluate buttonPressed based on current raw state
}


// ###########################################################################
// #
// ###########################################################################
void PinLongPress_Class::printStatus(bool prompt) {
    printf0_FN("%s\n", pinID_);
    printf0_FN("\t%-18s: %2d - %s\n",   "pressedLogicLevel",           pressedLogicLevel_,           str_pinLevel[pressedLogicLevel_]);
    printf0_FN("\t%-18s: %2d - %s\n",   "buttonPressed",               buttonPressed_,               str_TrueFalse[buttonPressed_]);
    printf0_FN("\t%-18s: %2d - %s\n",   "maxLevelReachedAndNotified", maxLevelReachedAndNotified_,  str_TrueFalse[maxLevelReachedAndNotified_]);

    printf0_FN("\t%-18s: nr:%2d --> [ ", "threshold", numThresholds_);
    for (int8_t j = 0; j < numThresholds_; j++) {
        printf0("%2ld, ", pressThresholds_[j]); // Use %ld for unsigned long
    }
    printf0(" ]\n"); // close threshold line

    printf0("\n"); // blank line
    if (prompt) {
        waitForEnter();
    }
}


// ###########################################################################################
// #  EXTERNAL FUNCTIONS (if they remain external)
// ###########################################################################################

#define ALARM_BEEP_INTERVAL 2000

// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
//###########################################################################
//#
//###########################################################################
void notifyCurrentButtonLevel(PinLongPress_Class *p, PinController_Class *buzzer) {
    uint16_t beep_duration = 200;
    static uint32_t lastBeepTime; // Static to maintain state across calls

    if (p->isButtonPressed() == p->getPressedLogicLevel()) { // Use getter for private member
        if (p->getCurrentPressLevel() != NO_PRESS) { // Use getter
            if (p->getCurrentPressLevel() != p->getLastPressedLevel()) { // Use getters
                printf0_FN("[%s] Pressione in corso (ms:%06ld)\n", p->getName(), (millis() - p->getPressDuration())); // Use getters
                switch (p->getCurrentPressLevel()) { // Use getter
                    case PRESSED_LEVEL_1:
                        printf2_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    case PRESSED_LEVEL_2:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    case PRESSED_LEVEL_3:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    case PRESSED_LEVEL_4:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    case PRESSED_LEVEL_5:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    case PRESSED_LEVEL_6:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    case PRESSED_LEVEL_7:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    case PRESSED_LEVEL_8:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    case PRESSED_LEVEL_9:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->getCurrentPressLevel());
                        buzzer->pulse(beep_duration);
                        break;
                    default:
                        printf0_FN("sono nel default: %d\n", p->getCurrentPressLevel());
                        break;
                }
                // Directly setting lastPressedLevel__ via the pointer,
                // consider adding a setter if this were a strict OOP design.
                // For a class, this would typically be `p->setLastPressedLevel(p->getCurrentPressLevel());`
                // But for now, since it's an external function, it's accessing through the pointer directly.
                // If you want full encapsulation, this external function should be refactored into the class.
                // For simplicity, directly accessing through pointer for now given the context.
                // p->lastPressedLevel_ = p->currentPressLevel_; // Accessing private member directly via pointer
                p->setLastPressedLevel(p->getCurrentPressLevel()); // Accessing private member directly via pointer
            }
            // --- BEEP LOGIC EVERY ALARM_BEEP_INTERVAL SECONDS ---
            if (p->hasMaxLevelReachedAndNotified()) { // Use getter
                if (millis() - lastBeepTime >= ALARM_BEEP_INTERVAL) {
                    buzzer->pulse(1000);
                    lastBeepTime = millis();
                }
            }
        }
    } else { // Button is not pressed or released
        // printf99_FN("clearing...\n");
        // Reset last pressed level for the next press cycle.
        // p->lastPressedLevel_ = NO_PRESS; // Directly accessing private member via pointer
        p->setLastPressedLevel(NO_PRESS); // Accessing private member directly via pointer
        // lastBeepTime = 0; // Reset beep timer when button is not at max level
        // buzzer->off_ifBlinking(); // This might interfere if buzzer is used by other functions
    }
}