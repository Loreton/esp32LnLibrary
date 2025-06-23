//
// updated by ...: Loreto Notarantonio
// Date .........: 23-06-2025 20.13.08
//

#include <Arduino.h>    // in testa anche per le definizioni dei type


#include "@SerialRead.h"
#include "@pinController_sc.h"



// ========================================
// - Public use funtcions
// ========================================
void pinController_sc::init(const char *name, uint8_t pin, uint8_t active_level) {
    _pin = pin;
    _name = name;
    _on_level = active_level;
    _on  = _on_level;
    _off = !_on_level;
    digitalWrite(_pin, _off); // start off
    snprintf(_pinID, 20, "[%02d.%-14s]:", _pin, _name);
    pinMode(_pin, OUTPUT);
    Serial.printf("%s inizializzato. active level: %s\n", _pinID,  _on_level ? "ON" : "OFF");
}



void pinController_sc::update() {
    uint32_t now = millis();

    if (_fixed) {
        // do nothing
    }
    else if (_pulseOn) {
        if (now - _pulseOnStart >= _pulseOnDuration) {
            Serial.printf("%s pulseON on scaduto\n", _pinID); // Per debug
            _clearAll();
        }
    }

    else if (_blinking) {
        if (_ledState && now - _lastToggle >= _onTime) {
            _setLed(false);
            _lastToggle = now;
        } else if (!_ledState && now - _lastToggle >= _offTime) {
            _setLed(true);
            _lastToggle = now;
            if ( (_temporaryBlinking) && (--_num_cycles <= 0) ) {
                Serial.printf("%s _temporaryBlinking scaduto\n", _pinID); // Per debug
                _clearAll();
            }
        }
    }
}


// void (pinController_sc::*pulseDC) = &pinController_sc::startBlinkingDC;
// void pinController_sc::startBlinkingDC(uint32_t period, uint8_t duty_cycle, int8_t cycles) {



void pinController_sc::pulse(uint32_t duration) {
    if (!_pulseOn) {
        _setPulse();
        _pulseOnDuration = duration;
        _pulseOnStart = millis();
        _setLed(true);
        Serial.printf("%s pulseON. duration: %lu ms\n", _pinID,  _pulseOnDuration);
    }
}


void pinController_sc::blinking(uint32_t onMs, uint32_t offMs, int8_t cycles) {
    if (!_blinking) {
        _setBlinking(cycles);
        _onTime = onMs;
        _offTime = offMs;
        _lastToggle = millis();
        _setLed(true); // start on
        Serial.printf("%s blinking. ON: %lu ms, OFF: %lu ms (cycles: %d)\n", _pinID,  _onTime, _offTime, _num_cycles);
        // waitForEnter();
        // Serial.printf("%s blinking. ON: %lu ms, OFF: %lu ms (cycles: %d)\n", _pinID,  _onTime, _offTime, _num_cycles);
    }
}

// duty-cycle da 0.0 a 1.0
void pinController_sc::blinking_dc(uint32_t period, float duty_cycle, int8_t cycles) {
    if (!_blinking) {
        float dutyCycle = constrain(duty_cycle, 0.0, 1.0); // importanti i decimali per avere un float point
        uint32_t on_duration = period*dutyCycle;
        uint32_t off_duration = period - on_duration;
        Serial.printf("%s duty_cycle: %.2f ON: %lums, OFF: %lums (cycles: %d)\n", _pinID, dutyCycle, on_duration, off_duration, cycles);
        blinking(on_duration, off_duration, cycles);
    }
}


void pinController_sc::set(uint8_t req_state) {
    _setFixed();
    _ledState = req_state;
    digitalWrite(_pin, req_state ? _on : _off);
}

void pinController_sc::on() {
    set(_on);
}

void pinController_sc::off() {
    set(_off);
}








// ========================================
// - Internal use funtcions
// ========================================
void pinController_sc::_setLed(bool req_state) {
    _ledState = req_state;
    digitalWrite(_pin, req_state ? _on : _off);
}

void pinController_sc::_setFixed() {
    _clearAll();
    _num_cycles = 0;
    _fixed = true;
}

void pinController_sc::_setBlinking(int8_t cycles) {
    _clearAll();
    _blinking = true;
    if (cycles > 0) {
        _num_cycles = cycles;
        _temporaryBlinking = true;
    }
}

void pinController_sc::_setPulse() {
    _clearAll();
    _pulseOn = true;
    _num_cycles = 0;
}

void pinController_sc::_clearAll() {
    _fixed = _pulseOn = _blinking = _temporaryBlinking = false;
    _setLed(false);

}

