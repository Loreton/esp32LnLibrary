//
// updated by ...: Loreto Notarantonio
// Date .........: 18-06-2025 20.10.45
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@button_structures.h"
// #include "@SerialRead.h"



void displayPinStatus(ButtonState_t *p, bool prompt) {
    // printf0_FN("\t%-18s: %2d - (%d)\n" , "pin nr"         , p->pin                   , p->mode);
    printf0_FN("%s\n", p->pinID);
    printf0_FN("\t%-18s: %2d - %s\n",   "pressedLogicLevel",          p->pressedLogicLevel,           str_pinLevel[p->pressedLogicLevel]);
    printf0_FN("\t%-18s: %2d - %s\n",   "buttonPressed",              p->buttonPressed,               str_TrueFalse[p->buttonPressed]);
    printf0_FN("\t%-18s: %2d - %s\n",   "maxLevelReachedAndNotified", p->maxLevelReachedAndNotified,  str_TrueFalse[p->maxLevelReachedAndNotified]);

    printf0_FN("\t%-18s: %2d\n",        "currentPressLevel",          p->currentPressLevel);
    printf0_FN("\t%-18s: %2d\n",        "lastPrintedLevel",           p->lastPrintedLevel);
    printf0_FN("\t%-18s: %2d\n",        "numThresholds",              p->numThresholds);

    printf0_FN("\t%-18s: nr:%2d --> [ ", "threshold"      , p->numThresholds);
    for (int8_t j=0; j<p->numThresholds; j++) {
        printf0("%2d, ", p->pressThresholds[j]);
    }
    printf0(" ]\n"); // close threshold line

    // printf0_FN("\t%-18s: lvl: %s\n",      "default pressedLevel",     THRESHOLD_LEVEL_TYPES[p->pressedLevel]);
    // printf0_FN("%s is %s\n", p->pinID, str_OffOn[p->is_ON]);
    printf0("\n"); // blank line
    if (prompt) {
        waitForChar("pelase enter 'c'", 'c');
    }
}
