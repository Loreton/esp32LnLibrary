/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-04-2025 11.34.13
*/



// ---------------------------------
// - lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
#include "@mainStructures.h"
#include "@pinOperations.h"



// #########################################
// #    I N P U T     pin;
// # Ex.: readPin(pinStruct);
// #########################################
uint8_t readInputPin(io_input_pin_struct_t *p) {
    uint8_t cur_state = digitalRead(p->pin);
    p->changedState = (cur_state == p->lastState) ? false : true;
    p->isPressed    = (cur_state == p->active_level) ? true : false;
    p->is_ON         = p->isPressed;
    p->is_OFF        = ! p->is_ON;

    if (p->changedState) {
        // printf1_NFN("Caller name: %pS\n", __builtin_return_address(1)); //https://gcc.gnu.org/onlinedocs/gcc/Return-Address.html
        printf2_NFN("%s ph_state %d - ipPressed: %d [%s]\n", p->pinID, p->ph_state, p->isPressed, str_OffOn[p->isPressed]);
        p->lastState    = p->ph_state;
        p->ph_state     = cur_state;
    }

    return p->is_ON;
}



// #########################################
// #    O U T P U T     pin;
// # Ex.: readPin(pinStruct);
// #########################################
// uint8_t isActed(io_output_pin_struct_t *p) {
// inline uint8_t isActed(io_output_pin_struct_t *p) {return readPin(p); }

uint8_t readOutputPin(io_output_pin_struct_t *p) {
    if (isPulsetimeExpired(p)) { // nel caso fosse expired e se auto_on_off dovrebbe spegenre pin
        if (p->pulsetime.auto_on_off) {
            pinOFF(p);
        }
    }

    uint8_t cur_state = digitalRead(p->pin);
    p->changedState = (cur_state == p->lastState) ? false : true;
    p->is_acted = (cur_state == p->active_level) ? true : false;
    p->is_ON = p->is_acted;
    p->is_OFF = !p->is_acted;

    if (p->changedState) {
        p->lastState    = p->ph_state;
        p->ph_state     = cur_state;
        printf2_NFN("%s changed state to %s\n", p->pinID, str_OffOn[p->is_acted]);
    }
    return p->is_acted;
}





// #########################################
// # Controlla il livello/tempo di pressione di un tasto
// # e lo riporta nel campo opportuna
// # return: true se il livello è cambiato
// #########################################
bool chkPressedLevel(io_input_pin_struct_t *p) {
    uint8_t nElements = p->n_thresholds-1; // set to last as default
    int8_t curr_pressed_level = 0; // set to last as default

    printf2_NFN("%s current level %d/%d\n", p->pinID, p->pressedLevel, p->n_thresholds-2);
    for (int8_t j=nElements; j>=0; j--) { // skip del primo
        if ( (millis() - p->startedMillis) >= p->thresholds[j]) {
            curr_pressed_level = j;
            break;
        }
    }

    if (curr_pressed_level > p->pressedLevel) {
        p->pressedLevel = curr_pressed_level;
        printf2_NFN("%s new level %d/%d\n", p->pinID, p->pressedLevel, p->n_thresholds-2);
        if (p->pressedLevel > 0)
            passiveBuzzerPulse(400*p->pressedLevel, 200);
            // tone(passiveBuzzer->pin, 400*p->pressedLevel, 200);
        return true;
    }
    return false;
}





// #########################################
// #    I N P U T     pin;
// # return: pressedLevel
// #########################################
int8_t readLongPressPin(io_input_pin_struct_t *p) {
    readInputPin(p);

    p->isReleased = false; // default
    if (p->changedState) {
        if (p->isPressed) {
            printf2_NFN("%s pressed, level: %d\n", p->pinID, p->pressedLevel);
            p->startedMillis = millis(); // reset pressed millis
            p->pressedLevel  = NO_BUTTON_PRESSED; // reset
        }
        else {
            p->isReleased = true;  // activate it
            if (p->pressedLevel > NO_BUTTON_PRESSED) {
                printf2_NFN("%s released, level: %d\n", p->pinID, p->pressedLevel);
            }
        }
    }
    else if (p->isPressed) {
        chkPressedLevel(p);
    }

    return p->pressedLevel;
}











// #########################################
// # Ex.: toggle output pin - NON funziona???
// #    NON funziona se ad esempio in un LED non si mette la resistenza.
// #########################################
void pinToggle(io_output_pin_struct_t *p) {
    printf2_NFN("%s status before %d\n", p->pinID, digitalRead(p->pin));
    digitalWrite(p->pin ,!digitalRead(p->pin));
    printf2_NFN("%s status after %d\n", p->pinID, digitalRead(p->pin));
}






// #########################################
// # Ex.: check if pulsetime;expired
// #########################################
void pinOFF(io_output_pin_struct_t *p) {
    digitalWrite(p->pin, p->OFF);
    stopPulsetime(p);
    printf1_NFN("%s to OFF: %d\n", p->pinID, digitalRead(p->pin));
}


void pinON(io_output_pin_struct_t *p) {
    digitalWrite(p->pin, p->ON);
    printf1_NFN("%s to ON: %d\n", p->pinID, digitalRead(p->pin));
}




void pulseGenerator(io_output_pin_struct_t *p, uint32_t onTime, uint32_t offTime) {
    static uint32_t previousMillis = 0;
    static bool isOn = false;

    uint32_t currentMillis = millis();

    if (isOn && currentMillis - previousMillis >= onTime) {
        // digitalWrite(p->pin, p->OFF);
        pinOFF(p);
        previousMillis = currentMillis;
        isOn = false;
    }
    else if (!isOn && currentMillis - previousMillis >= offTime) {
        previousMillis = currentMillis;
        // digitalWrite(p->pin, p->ON);
        pinON(p);
        isOn = true;
    }
}




