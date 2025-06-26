//
// updated by ...: Loreto Notarantonio
// Date .........: 26-06-2025 17.12.37
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
                m_pin(pin),
                m_name(name),
                m_on_level(active_level) {

    m_on  = m_on_level;
    m_off = !m_on_level;
    digitalWrite(m_pin, m_off); // start off
    snprintf(m_pinID, sizeof(m_pinID) -1, "[%02d.%-14s]:", m_pin, m_name); // Use sizeof for buffer safety
    pinMode(m_pin, OUTPUT);
    printf99_FN("%s initialized. active level: %s\n", m_pinID,  m_on_level ? "ON" : "OFF");
}

void PinController_Class::update() {
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

void PinController_Class::pulse(uint32_t duration) {
    if (!m_pulseOn) {
        setPulse();
        m_pulseOnDuration = duration;
        m_pulseOnStart = millis();
        setLed(true);
        printf2_FN("%s pulseON. duration: %lu ms\n", m_pinID,  m_pulseOnDuration);
    }
}

void PinController_Class::blinking(uint32_t onMs, uint32_t offMs, int8_t cycles) {
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
void PinController_Class::blinking_dc(uint32_t period, float duty_cycle, int8_t cycles) {
    if (!m_blinking) {
        float dutyCycle = constrain(duty_cycle, 0.0, 1.0); // important decimals for a float point
        uint32_t on_duration = period * dutyCycle;
        uint32_t off_duration = period - on_duration;
        printf2_FN("%s duty_cycle: %.2f ON: %lums, OFF: %lums (cycles: %d)\n", m_pinID, dutyCycle, on_duration, off_duration, cycles);
        blinking(on_duration, off_duration, cycles);
    }
}

void PinController_Class::set(uint8_t req_state) {
    setFixed();
    m_ledState = req_state;
    digitalWrite(m_pin, req_state ? m_on : m_off);
    printf99_FN("%s status: %d\n", m_pinID, digitalRead(m_pin));
}

void PinController_Class::on() {
    set(m_on);
}

void PinController_Class::off_ifBlinking() {
    if (m_blinking) {
        set(m_off);
    }
}

void PinController_Class::off() {
    set(m_off);
}

// ========================================
// - Internal use functions
// ========================================
void PinController_Class::setLed(bool req_state) {
    m_ledState = req_state;
    digitalWrite(m_pin, req_state ? m_on : m_off);
}

void PinController_Class::setFixed() {
    clearAll();
    m_num_cycles = 0;
    m_fixed = true;
}

void PinController_Class::setBlinking(int8_t cycles) {
    clearAll();
    m_blinking = true;
    if (cycles > 0) {
        m_num_cycles = cycles;
        m_temporaryBlinking = true;
    }
}

void PinController_Class::setPulse() {
    clearAll();
    m_pulseOn = true;
    m_num_cycles = 0;
}

void PinController_Class::clearAll() {
    m_fixed = m_pulseOn = m_blinking = m_temporaryBlinking = false;
    setLed(false);
}