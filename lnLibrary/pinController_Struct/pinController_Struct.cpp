//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.56.54
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0x
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()
#include "@SerialRead.h"
#include "@pinController_Struct.h"




// ========================================
// - Public use funtcions
// ========================================
void pinController_Struct::init(const char *name, uint8_t pin, uint8_t active_level) {
    pin_ = pin;
    name_ = name;
    on_level_ = active_level;
    on_  = on_level_;
    off_ = !on_level_;
    digitalWrite(pin_, off_); // start off
    snprintf(pinID_, 20, "[%02d.%-14s]:", pin_, name_);
    pinMode(pin_, OUTPUT);
    printf99_FN("%s inizializzato. active level: %s\n", pinID_,  on_level_ ? "ON" : "OFF");
}



void pinController_Struct::update() {
    uint32_t now = millis();

    if (fixed_) {
        // do nothing
    }
    else if (pulseOn_) {
        if (now - pulseOnStart_ >= pulseOnDuration_) {
            printf1_FN("%s pulseON on scaduto\n", pinID_); // Per debug
            _clearAll();
        }
    }

    else if (blinking_) {
        if (ledState_ && now - lastToggle_ >= onTime_) {
            _setLed(false);
            lastToggle_ = now;
        } else if (!ledState_ && now - lastToggle_ >= offTime_) {
            _setLed(true);
            lastToggle_ = now;
            if ( (temporaryBlinking_) && (--num_cycles_ <= 0) ) {
                printf1_FN("%s temporaryBlinking_ scaduto\n", pinID_); // Per debug
                _clearAll();
            }
        }
    }
}


// void (pinController_Struct::*pulseDC) = &pinController_Struct::startBlinkingDC;
// void pinController_Struct::startBlinkingDC(uint32_t period, uint8_t duty_cycle, int8_t cycles) {



void pinController_Struct::pulse(uint32_t duration) {
    if (!pulseOn_) {
        _setPulse();
        pulseOnDuration_ = duration;
        pulseOnStart_ = millis();
        _setLed(true);
        printf2_FN("%s pulseON. duration: %lu ms\n", pinID_,  pulseOnDuration_);
    }
}


void pinController_Struct::blinking(uint32_t onMs, uint32_t offMs, int8_t cycles) {
    if (!blinking_) {
        _setBlinking(cycles);
        onTime_ = onMs;
        offTime_ = offMs;
        lastToggle_ = millis();
        _setLed(true); // start on
        printf2_FN("%s blinking. ON: %lu ms, OFF: %lu ms (cycles: %d)\n", pinID_,  onTime_, offTime_, num_cycles_);
    }
}

// duty-cycle da 0.0 a 1.0
void pinController_Struct::blinking_dc(uint32_t period, float duty_cycle, int8_t cycles) {
    if (!blinking_) {
        float dutyCycle = constrain(duty_cycle, 0.0, 1.0); // importanti i decimali per avere un float point
        uint32_t on_duration = period*dutyCycle;
        uint32_t off_duration = period - on_duration;
        printf2_FN("%s duty_cycle: %.2f ON: %lums, OFF: %lums (cycles: %d)\n", pinID_, dutyCycle, on_duration, off_duration, cycles);
        blinking(on_duration, off_duration, cycles);
    }
}







void pinController_Struct::set(uint8_t req_state) {
    _setFixed();
    ledState_ = req_state;
    digitalWrite(pin_, req_state ? on_ : off_);
    printf99_FN("%s status: %d\n", pinID_, digitalRead(pin_));
}

void pinController_Struct::on() {
    set(on_);
}

void pinController_Struct::off_ifBlinking() {
    if (blinking_) {
        set(off_);
    }
}

void pinController_Struct::off() {
    set(off_);
}








// ========================================
// - Internal use funtcions
// ========================================
void pinController_Struct::_setLed(bool req_state) {
    ledState_ = req_state;
    digitalWrite(pin_, req_state ? on_ : off_);
}

void pinController_Struct::_setFixed() {
    _clearAll();
    num_cycles_ = 0;
    fixed_ = true;
}

void pinController_Struct::_setBlinking(int8_t cycles) {
    _clearAll();
    blinking_ = true;
    if (cycles > 0) {
        num_cycles_ = cycles;
        temporaryBlinking_ = true;
    }
}

void pinController_Struct::_setPulse() {
    _clearAll();
    pulseOn_ = true;
    num_cycles_ = 0;
}

void pinController_Struct::_clearAll() {
    fixed_ = pulseOn_ = blinking_ = temporaryBlinking_ = false;
    _setLed(false);

}

