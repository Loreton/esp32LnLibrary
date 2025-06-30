//
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 14.50.49
//
#include <Arduino.h>

#define LOG_LEVEL_1
#define LOG_LEVEL_2
#define LOG_LEVEL_99
#include "@logMacros.h"
#include "@lnString.h" // per setPinID()

#include "@ledController_Class.h" // Changed to new class header



// Constructor definition
LedController_Class::LedController_Class(void) {};

/*LedController_Class::LedController_Class(const char *name, uint8_t pin, uint8_t pressedLogicLevel) :
                m_pin(pin),
                m_name(name),
                m_onLevel(pressedLogicLevel) {

    m_on  = m_onLevel;
    m_off = !m_onLevel;
    digitalWrite(m_pin, m_off); // start off
    pinMode(m_pin, OUTPUT);

    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

}
*/
void LedController_Class::init(const char *name, uint8_t pin, uint8_t pressedLogicLevel) {
    m_pin=pin;
    m_name=name;
    m_onLevel=pressedLogicLevel;
    m_on  = m_onLevel;
    m_off = !m_onLevel;

    digitalWrite(m_pin, m_off); // start off
    pinMode(m_pin, OUTPUT);

    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);
    /**
    const size_t PIN_ID_MAXLENGTH = sizeof(m_pinID)-1;
    int cx = snprintf(m_pinID, PIN_ID_MAXLENGTH - 6, "[%s", m_name);
    snprintf(m_pinID + cx, PIN_ID_MAXLENGTH - cx, ".%02d]:", m_pin);
    */
}

void LedController_Class::update() {
    uint32_t now = millis();

    if (m_fixed) {
        // do nothing
    } else if (m_pulseOn) {
        if (now - m_pulseOnStart >= m_pulseOnDuration) {
            printf1_FN("%s pulseON expired\n", m_pinID); // For debug
            clearAll();
        }
    } else if (m_blinking) {
        if (m_ledState && now - m_lastToggle >= m_onTime) {
            setLed(false);
            m_lastToggle = now;
        } else if (!m_ledState && now - m_lastToggle >= m_offTime) {
            setLed(true);
            m_lastToggle = now;
            if ((m_temporaryBlinking) && (--m_num_cycles <= 0)) {
                printf1_FN("%s temporaryBlinking expired\n", m_pinID); // For debug
                clearAll();
            }
        }
    }
}

void LedController_Class::pulse(uint32_t duration) {
    if (!m_pulseOn) {
        setPulse();
        m_pulseOnDuration = duration;
        m_pulseOnStart = millis();
        setLed(true);
        printf2_FN("%s pulseON. duration: %lu ms\n", m_pinID,  m_pulseOnDuration);
    }
}

void LedController_Class::blinking(uint32_t onMs, uint32_t offMs, int8_t cycles) {
    if (!m_blinking) {
        setBlinking(cycles);
        m_onTime = onMs;
        m_offTime = offMs;
        m_lastToggle = millis();
        setLed(true); // start on
        printf2_FN("%s blinking. ON: %lu ms, OFF: %lu ms (cycles: %d)\n", m_pinID,  m_onTime, m_offTime, m_num_cycles);
    }
}

// duty-cycle from 0.0 to 1.0
void LedController_Class::blinking_dc(uint32_t period, float duty_cycle, int8_t cycles) {
    if (!m_blinking) {
        float dutyCycle = constrain(duty_cycle, 0.0, 1.0); // important decimals for a float point
        uint32_t on_duration = period * dutyCycle;
        uint32_t off_duration = period - on_duration;
        printf2_FN("%s duty_cycle: %.2f ON: %lums, OFF: %lums (cycles: %d)\n", m_pinID, dutyCycle, on_duration, off_duration, cycles);
        blinking(on_duration, off_duration, cycles);
    }
}

void LedController_Class::set(uint8_t req_state) {
    setFixed();
    m_ledState = req_state;
    digitalWrite(m_pin, req_state ? m_on : m_off);
    printf99_FN("%s status: %d\n", m_pinID, digitalRead(m_pin));
}

void LedController_Class::on() {
    set(m_on);
}

void LedController_Class::off_ifBlinking() {
    if (m_blinking) {
        set(m_off);
    }
}

void LedController_Class::off() {
    set(m_off);
}

// ========================================
// - Internal use functions
// ========================================
void LedController_Class::setLed(bool req_state) {
    m_ledState = req_state;
    digitalWrite(m_pin, req_state ? m_on : m_off);
}

void LedController_Class::setFixed() {
    clearAll();
    m_num_cycles = 0;
    m_fixed = true;
}

void LedController_Class::setBlinking(int8_t cycles) {
    clearAll();
    m_blinking = true;
    if (cycles > 0) {
        m_num_cycles = cycles;
        m_temporaryBlinking = true;
    }
}

void LedController_Class::setPulse() {
    clearAll();
    m_pulseOn = true;
    m_num_cycles = 0;
}

void LedController_Class::clearAll() {
    m_fixed = m_pulseOn = m_blinking = m_temporaryBlinking = false;
    setLed(false);
}