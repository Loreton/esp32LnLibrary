//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 18.06.14
//
#include <Arduino.h>

#define LOG_LEVEL_0x
#define LOG_LEVEL_99
// Assuming these are custom log macros and SerialRead, adjust paths if necessary.
// For this conversion, I'm commenting them out to ensure compilation without them.
// You'll need to ensure these paths are correct or provide mock implementations
// if they are critical and not part of the standard Arduino environment.
// #include "@logMacros.h" // printf:XFN()
// #include "@SerialRead.h"
#include "@pinController_Class.h" // Changed to new class header

// Placeholder for log macros if not available
#ifndef printf99_FN
#define printf99_FN(...) Serial.printf(__VA_ARGS__)
#endif
#ifndef printf2_FN
#define printf2_FN(...) Serial.printf(__VA_ARGS__)
#endif
#ifndef printf1_FN
#define printf1_FN(...) Serial.printf(__VA_ARGS__)
#endif


// Constructor definition
PinController_Class::PinController_Class(const char *name, uint8_t pin, uint8_t active_level) :
                pin_(pin),
                name_(name),
                on_level_(active_level) {

    on_  = on_level_;
    off_ = !on_level_;
    digitalWrite(pin_, off_); // start off
    snprintf(pinID_, sizeof(pinID_) -1, "[%02d.%-14s]:", pin_, name_); // Use sizeof for buffer safety
    pinMode(pin_, OUTPUT);
    printf99_FN("%s initialized. active level: %s\n", pinID_,  on_level_ ? "ON" : "OFF");
}

void PinController_Class::update() {
    uint32_t now = millis();

    if (fixed_) {
        // do nothing
    } else if (pulseOn_) {
        if (now - pulseOnStart_ >= pulseOnDuration_) {
            printf1_FN("%s pulseON expired\n", pinID_); // For debug
            _clearAll();
        }
    } else if (blinking_) {
        if (ledState_ && now - lastToggle_ >= onTime_) {
            _setLed(false);
            lastToggle_ = now;
        } else if (!ledState_ && now - lastToggle_ >= offTime_) {
            _setLed(true);
            lastToggle_ = now;
            if ((temporaryBlinking_) && (--num_cycles_ <= 0)) {
                printf1_FN("%s temporaryBlinking expired\n", pinID_); // For debug
                _clearAll();
            }
        }
    }
}

void PinController_Class::pulse(uint32_t duration) {
    if (!pulseOn_) {
        _setPulse();
        pulseOnDuration_ = duration;
        pulseOnStart_ = millis();
        _setLed(true);
        printf2_FN("%s pulseON. duration: %lu ms\n", pinID_,  pulseOnDuration_);
    }
}

void PinController_Class::blinking(uint32_t onMs, uint32_t offMs, int8_t cycles) {
    if (!blinking_) {
        _setBlinking(cycles);
        onTime_ = onMs;
        offTime_ = offMs;
        lastToggle_ = millis();
        _setLed(true); // start on
        printf2_FN("%s blinking. ON: %lu ms, OFF: %lu ms (cycles: %d)\n", pinID_,  onTime_, offTime_, num_cycles_);
    }
}

// duty-cycle from 0.0 to 1.0
void PinController_Class::blinking_dc(uint32_t period, float duty_cycle, int8_t cycles) {
    if (!blinking_) {
        float dutyCycle = constrain(duty_cycle, 0.0, 1.0); // important decimals for a float point
        uint32_t on_duration = period * dutyCycle;
        uint32_t off_duration = period - on_duration;
        printf2_FN("%s duty_cycle: %.2f ON: %lums, OFF: %lums (cycles: %d)\n", pinID_, dutyCycle, on_duration, off_duration, cycles);
        blinking(on_duration, off_duration, cycles);
    }
}

void PinController_Class::set(uint8_t req_state) {
    _setFixed();
    ledState_ = req_state;
    digitalWrite(pin_, req_state ? on_ : off_);
    printf99_FN("%s status: %d\n", pinID_, digitalRead(pin_));
}

void PinController_Class::on() {
    set(on_);
}

void PinController_Class::off_ifBlinking() {
    if (blinking_) {
        set(off_);
    }
}

void PinController_Class::off() {
    set(off_);
}

// ========================================
// - Internal use functions
// ========================================
void PinController_Class::_setLed(bool req_state) {
    ledState_ = req_state;
    digitalWrite(pin_, req_state ? on_ : off_);
}

void PinController_Class::_setFixed() {
    _clearAll();
    num_cycles_ = 0;
    fixed_ = true;
}

void PinController_Class::_setBlinking(int8_t cycles) {
    _clearAll();
    blinking_ = true;
    if (cycles > 0) {
        num_cycles_ = cycles;
        temporaryBlinking_ = true;
    }
}

void PinController_Class::_setPulse() {
    _clearAll();
    pulseOn_ = true;
    num_cycles_ = 0;
}

void PinController_Class::_clearAll() {
    fixed_ = pulseOn_ = blinking_ = temporaryBlinking_ = false;
    _setLed(false);
}