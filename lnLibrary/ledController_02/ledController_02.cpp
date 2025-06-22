//
// updated by ...: Loreto Notarantonio
// Date .........: 22-06-2025 19.56.05
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include "@SerialRead.h" // per waitForEnter()



#include "@ledController_02.h"




void ledController_sc::init(uint8_t pin, uint8_t active_level) {
    _pin = pin;
    // _inverted = active_level;
    _on_level = active_level;
    _on  = _on_level;
    _off = !_on_level;
    digitalWrite(_pin, _off); // start off
    pinMode(_pin, OUTPUT);
    // digitalWrite(_pin, _inverted ? HIGH : LOW); // start off
}



void ledController_sc::update() {
    unsigned long now = millis();
    // static int8_t cycles = 0;

    if (_fixed) {
        // do nothing
    }
    else if (_pulseOn) {
        if (now - _pulseOnStart >= _pulseOnDuration) {
            Serial.printf("LED (Pin: %d) pulseON on scaduto\n", _pin); // Per debug
            _clearAll();
        }
    }

    else if (_blinking) {
        if (_ledState && now - _lastToggle >= _onDuration) {
            _setLed(false);
            _lastToggle = now;
        } else if (!_ledState && now - _lastToggle >= _offDuration) {
            _setLed(true);
            _lastToggle = now;
            if ( (_temporaryBlinking) && (--_num_cycles <= 0) ) {
                Serial.printf("LED (Pin: %d) _temporaryBlinking scaduto\n", _pin); // Per debug
                _clearAll();
            }
        }
    }
}

// void ledController_sc::startBlinking(unsigned long onMs, unsigned long offMs) {
//     if (!_blinking) {
//         _setBlinking();
//         _nun_cycles = 0;
//         Serial.printf("LED (Pin: %d) starting Blinking\n", _pin); // Per debug
//         _onDuration = onMs;
//         _offDuration = offMs;
//         _lastToggle = millis();
//         _setLed(true); // start on
//     }
// }

void ledController_sc::startBlinking(unsigned long onMs, unsigned long offMs, int8_t cycles) {
    if (!_blinking) {
        _setBlinking(cycles);
        Serial.printf("LED (Pin: %d) starting Blinking\n", _pin); // Per debug
        _onDuration = onMs;
        _offDuration = offMs;
        _lastToggle = millis();
        _setLed(true); // start on
    }
}

// void ledController_sc::stopBlinking() {
//     _blinking = false;
//     _setLed(false);
// }

void ledController_sc::pulseOn(unsigned long duration) {
    if (!_pulseOn) {
        _setPulse();
        Serial.printf("LED (Pin: %d) pulseON for %lu msec\n", _pin, duration); // Per debug
        _pulseOnDuration = duration;
        _pulseOnStart = millis();
        _setLed(true);
    }

}


void ledController_sc::set(uint8_t req_state) {
    _setFixed();
    _ledState = req_state;
    digitalWrite(_pin, req_state ? _on : _off);
}

void ledController_sc::on() {
    set(_on);
}

void ledController_sc::off() {
    set(_off);
}




// Internal use funtcions
void ledController_sc::_setLed(bool req_state) {
    _ledState = req_state;
    digitalWrite(_pin, req_state ? _on : _off);
}

void ledController_sc::_setFixed() {
    _clearAll();
    _num_cycles = 0;
    _fixed = true;
}

void ledController_sc::_setBlinking(int8_t cycles) {
    _clearAll();
    _blinking = true;
    if (cycles > 0) {
        Serial.printf("LED (Pin: %d) _temporaryBlinking started\n", _pin); // Per debug
        _num_cycles = cycles;
        _temporaryBlinking = true;
    }
}

void ledController_sc::_setPulse() {
    _clearAll();
    _pulseOn = true;
    _num_cycles = 0;
}

void ledController_sc::_clearAll() {
    _fixed = _pulseOn = _blinking = _temporaryBlinking = false;
    _setLed(false);

}
