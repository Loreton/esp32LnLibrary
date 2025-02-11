/*
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 08.55.21
*/


#include "@pinDefinitions.h"

// --- others
#include "@lnMacros.h"
#include "@outputPinFunctions.h"
// #include "@main.h"




// #define dstat_dbg        //           ln_printf
// #define displpin_dbg0               ln_printf
#define debug_fn               ln_printf_FN
#define debug_f                 ln_printf
#define fVerbose true
#define debug00_fn                    ln_printf_FN
#define debug01_fn    //                ln_printfFN
#define debug02_fn   //                ln_printfFN


unsigned long elapsed;
unsigned long now;



void pinDisplayState(io_input_pin_struct_t *p) {
    debug_fn("%s: \n", p->name);
    debug_f("\t%-18s: %2d - (%s)\n" , "pin nr"        , p->pin              , p->strMode);
    debug_f("\t%-18s: %2d - %s\n"   , "active level"  , p->active_level     , str_pinLevel[p->active_level]);
    debug_f("\t%-18s: %2d - %s\n"   , "current level" , p->state            , str_pinLevel[p->state]);
    debug_f("\t%-18s: %2d\n"        , "last level"    , p->lastState);
    debug_f("\t%-18s: %2d - %s\n"   , "pressed"       , p->isPressed        , str_action[p->isPressed]);
    debug_f("\t%-18s: %2d - %s\n"   , "released"      , p->isReleased       , str_action[p->isPressed]);
    debug_f("\t%-18s: nr:  %2d"     , "thresholds"    , p->thresholds_len);

    debug_f("  [ ");
    for (int8_t j=0; j<p->thresholds_len; j++) {
        debug_f("%2d, ", p->thresholds[j]);
    }
    debug_f(" ]\n");

    debug_f("\t%-18s: lvl: %s\n",      "default pressedLevel",     THRESHOLD_LEVEL_TYPES[p->pressedLevel]);
}



// #########################################
// # Ex.: readPin(pinStruct, true);
// #########################################
void resetInputPin(io_input_pin_struct_t *p) {
    p->isPressed = false;
    p->isReleased = false;
}



// ################################################################
// # p->isPressed can be LOW or HIGH
// # so you can check for normally closed/opened button
// # return:
// #      0 no action:
// #      1 short press detected:
// #      2 long press detected:
// ################################################################
bool isPressedLevelChanged(io_input_pin_struct_t *p) {
    bool changed=false;
    elapsed = now - p->startedMillis;
    uint8_t LAST_LEVEL = p->thresholds_len-1; // set to last as default

    uint8_t pressed_level = 0; // set to last as default

    if (p->pressedLevel >= LAST_LEVEL)  {
        debug01_fn("max level already reached %d\n", p->pressedLevel);
        return false;
    }
    else  {
        debug02_fn("current: %d - lastPressedLevel: %d (max_levels: %d) \n", p->pressedLevel, p->lastPressedLevel, LAST_LEVEL);
        debug02_fn("startedMillis: %d - elapsed: %d\n", p->startedMillis, elapsed);
        for (int8_t j=LAST_LEVEL; j>=0; j--) {
            if (elapsed >= p->thresholds[j]) {
                pressed_level = j;
                break;
            }
        }
        debug02_fn("pressed_level: %d - current: %d\n", pressed_level, p->pressedLevel);
        if (pressed_level > p->pressedLevel) {
            p->lastPressedLevel = p->pressedLevel;
            p->pressedLevel = pressed_level;
            changed=true;
            debug02_fn("pressed_level updated to %d\n", p->pressedLevel);
        }
        debug02_fn("\n");
    }

    return changed;
}



// #########################################
// # Ex.: readPin(pinStruct, true);
// #########################################
bool readInputPin(io_input_pin_struct_t *p, uint8_t signal_pin, bool is_buzzer) {
    uint8_t state = digitalRead(p->pin);
    now=millis();
    // unsigned long elapsed;

    p->changedState = (state != p->lastState) ? true : false;

    if (p->changedState) {
        debug02_fn("\n");
        debug01_fn("%s changed\n", p->name);
        p->state        = state;
        // p->strLevel     = str_pinLevel[p->state]; // text visualization
        p->lastState    = state;
        p->isPressed    = (p->state == p->active_level) ? true : false;
        p->isReleased   = !p->isPressed;
        // p->strAction    = str_action[p->isPressed];

        if (p->isPressed) {
            debug00_fn("%s pressed\n", p->name);
            p->startedMillis = now; // reset pressed millis
            p->pressedLevel=BUTTON_NO_PRESS; // azzera
            p->lastPressedLevel=BUTTON_NO_PRESS; // azzera
            debug00_fn("startedMillis: %d - pressedLevel: %d - lastPressedLevel: %d\n\n", p->startedMillis, p->pressedLevel, p->lastPressedLevel);

        }

        else {
            debug02_fn("\n");
            debug00_fn("%s released\n", p->name);
            isPressedLevelChanged(p);
            debug00_fn("%s released after %lu millis [%s]\n", p->name, elapsed, THRESHOLD_LEVEL_TYPES[p->pressedLevel]);
        }
    }


    // -------------------------------------------------
    // serve solo per segnalare il salto di livello del pressed time
    // -------------------------------------------------
    if ( (p->isPressed) && (signal_pin != 99) ) {
        elapsed = now - p->startedMillis;
        debug02_fn("calling check pressed level\n");
        bool press_changed = isPressedLevelChanged(p);
        if (press_changed) {
            debug00_fn("%s [%s] - press changed %lu millis\n", p->name, THRESHOLD_LEVEL_TYPES[p->pressedLevel], elapsed);
            debug02_fn("sending signal\n");
            if (is_buzzer) {
                int frequency = 400+(200*p->pressedLevel);
                unsigned long duration = 200+(100*p->pressedLevel);
                pulseSound(signal_pin, frequency, duration); // pulseSound(uint8_t pin, int frequency, uint16_t duration)
            } else {
                pinHighPulse(signal_pin, 200, 4);
            }

        }
        // debug00_fn("\n\n\n");
    }


    return state;
}

