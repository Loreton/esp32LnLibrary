//
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 15.37.59
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__
#include <Arduino.h>

#define LOG_LEVEL_0
#define LOG_LEVEL_99x
#include "@logMacros.h" // printf:XFN()
#include "@buttonLongPress_Class.h" // Use the new class header

#include "@project_usefullFunctions.h" // Includes the function prototypes

// This 'relayState' is a global variable. If it should be managed
// by a specific class (e.g., a RelayControl class), that would be
// another good refactoring step. For now, keeping it global as per original.
bool relayState = false;

//###########################################################################
//# Action for the pumpState button
//###########################################################################
void pumpState_Action(ButtonLongPress_Class *p) { // Now accepts a pointer to ButtonLongPress_Class class
    // Access members using getters or directly if they are public for a specific reason,
    // though getters are preferred for encapsulation.
    switch (p->getCurrentPressLevel()) { // Use getter for currentPressLevel
        case PRESSED_LEVEL_1:
            printf99_FN("PRESSED_LEVEL_1\n");
            break;

        case PRESSED_LEVEL_2:
            printf99_FN("PRESSED_LEVEL_2\n");
            break;

        case PRESSED_LEVEL_3:
            printf99_FN("PRESSED_LEVEL_3\n");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(pressControlRelay_pin, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(pressControlRelay_pin, HIGH);
                printf0_FN("  --> Relè SPENTO!\n");
            }
            break;

        case PRESSED_LEVEL_4:
            printf99_FN("PRESSED_LEVEL_4\n");
            break;

        default:
            printf99_FN("Sconosciuto/Non Qualificato\n");
            break;
    }
}

//###########################################################################
//# Action for the startButton
//###########################################################################
void startButton_Action(ButtonLongPress_Class *p) { // Now accepts a pointer to ButtonLongPress_Class class
    // Access members using getters or directly if they are public for a specific reason,
    // though getters are preferred for encapsulation.
    switch (p->getCurrentPressLevel()) { // Use getter for currentPressLevel after released
        case PRESSED_LEVEL_1:
            printf99_FN("PRESSED_LEVEL_1\n");
            break;

        case PRESSED_LEVEL_2:
            printf99_FN("PRESSED_LEVEL_2\n");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(pressControlRelay_pin, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(pressControlRelay_pin, HIGH);
                printf0_FN("  --> Relè SPENTO!\n");
            }
            break;

        case PRESSED_LEVEL_3:
            printf99_FN("PRESSED_LEVEL_3\n");
            break;

        case PRESSED_LEVEL_4:
            printf99_FN("PRESSED_LEVEL_4\n");
            break;

        default:
            printf99_FN("Sconosciuto/Non Qualificato\n");
            break;
    }
}


//###########################################################################
//# Processes a button release event
//###########################################################################
void processButton(ButtonLongPress_Class *p) { // Now accepts a pointer to ButtonLongPress_Class class
    // Access members using getters
    printf0_FN("[%s] Rilasciato! Durata: %ld ms\n", p->getName(), p->getPressDuration());

    // Use getters for pin comparison
    if (p->getPin() == startButton_pin) {
        Serial.print("start button action\n");
        startButton_Action(p);
    }
    else if (p->getPin() == pumpState_pin) {
        pumpState_Action(p);
    }

    // *** RESET BUTTON STATE ***
    // After processing the data, we reset them for the next press.
    // The resetState() method now encapsulates these actions.
    p->resetState();
}

#endif