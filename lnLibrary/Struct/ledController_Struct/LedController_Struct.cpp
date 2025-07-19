//
// updated by ...: Loreto Notarantonio
// Date .........: 19-07-2025 15.29.00
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "lnLogger.h" // printf:XFN()
#include "lnSetPinID.h" // printf:XFN()
#include "lnSerialRead.h"
#include "LedController_Struct.h"


// costructor
LedController_Struct::LedController_Struct(void) {}

// ========================================
// - Public use funtcions
// ========================================
void LedController_Struct::init(const char *name, uint8_t pin, uint8_t active_level) {
    m_pin         = pin;
    m_name        = name;

    m_activeLevel = active_level;
    m_on          = m_activeLevel;
    m_off         = !m_activeLevel;

    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

    pinMode(m_pin, OUTPUT); digitalWrite(m_pin, m_off); // start off
    LOG_DEBUG("%s inizializzato. active level: %s", m_pinID,  m_activeLevel ? "ON" : "OFF");
}



void LedController_Struct::update() {
    uint32_t now = millis();

    if (m_fixed) {
        // do nothing
    }
    else if (m_pulseOn) {
        if (now - m_pulseOnStart >= m_pulseOnDuration) {
            LOG_TRACE("%s pulseON on scaduto", m_pinID); // Per debug
            _clearAll();
        }
    }

    else if (m_blinking) {
        if (m_ledState && now - m_lastToggle >= m_onTime) {
            _setLed(false);
            m_lastToggle = now;
        } else if (!m_ledState && now - m_lastToggle >= m_offTime) {
            _setLed(true);
            m_lastToggle = now;
            if ( (m_temporaryBlinking) && (--m_numCycles <= 0) ) {
                LOG_TRACE("%s m_temporaryBlinking scaduto", m_pinID); // Per debug
                _clearAll();
            }
        }
    }
}


// void (LedController_Struct::*pulseDC) = &LedController_Struct::startBlinkingDC;
// void LedController_Struct::startBlinkingDC(uint32_t period, uint8_t duty_cycle, int8_t cycles) {



void LedController_Struct::pulse(uint32_t duration) {
    if (!m_pulseOn) {
        _setPulse();
        m_pulseOnDuration = duration;
        m_pulseOnStart = millis();
        _setLed(true);
        LOG_TRACE("%s pulseON. duration: %lu ms", m_pinID,  m_pulseOnDuration);
    }
}


void LedController_Struct::blinking(uint32_t onMs, uint32_t offMs, int8_t cycles) {
    if (!m_blinking) {
        _setBlinking(cycles);
        m_onTime = onMs;
        m_offTime = offMs;
        m_lastToggle = millis();
        _setLed(true); // start on
        LOG_TRACE("%s blinking. ON: %lu ms, OFF: %lu ms (cycles: %d)", m_pinID,  m_onTime, m_offTime, m_numCycles);
    }
}

// duty-cycle da 0.0 a 1.0
void LedController_Struct::blinking_dutyCycle(uint32_t period, float duty_cycle, int8_t cycles) {
    if (!m_blinking) {
        float dutyCycle = constrain(duty_cycle, 0.0, 1.0); // importanti i decimali per avere un float point
        uint32_t on_duration = period*dutyCycle;
        uint32_t off_duration = period - on_duration;
        LOG_TRACE("%s duty_cycle: %.2f ON: %lums, OFF: %lums (cycles: %d)", m_pinID, dutyCycle, on_duration, off_duration, cycles);
        blinking(on_duration, off_duration, cycles);
    }
}







void LedController_Struct::set(uint8_t req_state) {
    _setFixed();
    m_ledState = req_state;
    digitalWrite(m_pin, req_state ? m_on : m_off);
    LOG_DEBUG("%s status: %d", m_pinID, digitalRead(m_pin));
}

// void LedController_Struct::on() {
//     set(m_on);
// }

void LedController_Struct::off_ifBlinking() {
    if (m_blinking) {
        set(m_off);
    }
}

// void LedController_Struct::off() {
//     set(m_off);
// }








// ========================================
// - Internal use funtcions
// ========================================
void LedController_Struct::_setLed(bool req_state) {
    m_ledState = req_state;
    digitalWrite(m_pin, req_state ? m_on : m_off);
}

void LedController_Struct::_setFixed() {
    _clearAll();
    m_numCycles = 0;
    m_fixed = true;
}

void LedController_Struct::_setBlinking(int8_t cycles) {
    _clearAll();
    m_blinking = true;
    if (cycles > 0) {
        m_numCycles = cycles;
        m_temporaryBlinking = true;
    }
}

void LedController_Struct::_setPulse() {
    _clearAll();
    m_pulseOn = true;
    m_numCycles = 0;
}

void LedController_Struct::_clearAll() {
    m_fixed = m_pulseOn = m_blinking = m_temporaryBlinking = false;
    _setLed(false);

}

