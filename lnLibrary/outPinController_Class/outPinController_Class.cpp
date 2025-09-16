//
// updated by ...: Loreto Notarantonio
// Date .........: 12-09-2025 16.49.19
//

#include <Arduino.h>     // in testa anche per le definizioni dei type


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>
#include <lnGlobalVars.h> // Assicurati che questi includano ancora i valori necessari
#include <lnSetPinID.h>
#include <lnSerialRead.h>
#include "outPinController_Class.h" // Includi il nuovo header della classe

// costruttore
outPinController_Class::outPinController_Class(void) {}

// ========================================
// - Public use functions
// ========================================
void outPinController_Class::init(const char *name, uint8_t pin, uint8_t active_level) {
    m_pin           = pin;
    m_name          = name;

    m_activeLevel   = active_level;
    m_on            = m_activeLevel;
    m_off           = !m_activeLevel;

    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, m_off); // start off
    LOG_TRACE("[%s] initialized. active level: %s", m_pinID, str_pinLevel[m_activeLevel]);
}


void outPinController_Class::update() {
    uint32_t now = millis();

    if (m_fixed) {
        // do nothing
    } else if (m_pulseOn) {
        if (now - m_pulseOnStart >= m_pulseOnDuration) {
            LOG_DEBUG("%s pulseON on scaduto", m_pinID); // Per debug
            reset();
        }
    } else if (m_blinking) {
        if (m_ledState && now - m_lastToggle >= m_onTime) {
            off();
            m_lastToggle = now;
        } else if (!m_ledState && now - m_lastToggle >= m_offTime) {
            on();
            m_lastToggle = now;

            if ((m_temporaryBlinking) && (--m_numCycles <= 0)) {
                LOG_DEBUG("%s m_temporaryBlinking scaduto", m_pinID); // Per debug
                reset();
            }
        }
    }
}


void outPinController_Class::pulse(uint32_t duration, bool waitForEnding) {
    if (!m_pulseOn) {
        reset();
        m_pulseOn = true;

        m_pulseOnDuration = duration;
        m_pulseOnStart = millis();
        on();
        LOG_DEBUG("%s pulseON. duration: %lu ms", m_pinID,  m_pulseOnDuration);
    } else {
        LOG_DEBUG("%s pulseON already active: %lu ms", m_pinID,  m_pulseOnDuration);
    }
    if (waitForEnding) {
        waitForPulseEnding(duration*2);
        off();
    }
}


void outPinController_Class::blinking(uint32_t onMs, uint32_t offMs, int8_t cycles, bool waitForEnding) {
    if (!m_blinking) {
        reset();
        m_blinking = true;

        if (cycles == 0) {waitForEnding = false; } // se cycles == 0 allora non consideriamo il wait altrimenti sarebbe infinito

        // se m_numCycles == 0 (default) il numero di cicli è infinito fino al reset
        m_numCycles = (cycles > 0) ? cycles : 0;
        m_temporaryBlinking = (cycles > 0) ? true : false;

        m_onTime = onMs;
        m_offTime = offMs;
        m_lastToggle = millis();
        on(); // start on
        LOG_DEBUG("%s blinking. ON: %lu ms, OFF: %lu ms (cycles: %d)", m_pinID,  m_onTime, m_offTime, m_numCycles);
    }
    else {
        LOG_DEBUG("%s already blinking. ON: %lu ms, OFF: %lu ms (cycles: %d)", m_pinID,  m_onTime, m_offTime, m_numCycles);
    }

    if (waitForEnding) {
        uint32_t duration = (onMs + offMs) * cycles;
        waitForPulseEnding(duration);
        off();
    }
}

// duty-cycle da 0.0 a 1.0
void outPinController_Class::blinking_dutyCycle(uint32_t period, float duty_cycle, int8_t cycles, bool waitForEnding) {
    if (!m_blinking) {
        float dutyCycle = constrain(duty_cycle, 0.0, 1.0); // importanti i decimali per avere un float point
        uint32_t on_duration = period * dutyCycle;
        uint32_t off_duration = period - on_duration;
        LOG_DEBUG("%s duty_cycle: %.2f ON: %lums, OFF: %lums (cycles: %d)", m_pinID, dutyCycle, on_duration, off_duration, cycles);
        blinking(on_duration, off_duration, cycles, waitForEnding);
    }
}


void outPinController_Class::_set(uint8_t req_state) {
    m_ledState = req_state;
    digitalWrite(m_pin, req_state ? m_on : m_off);
    LOG_TRACE("%s status: %d", m_pinID, digitalRead(m_pin));
}

void outPinController_Class::reset() {
    m_fixed = m_pulseOn = m_blinking = m_temporaryBlinking = false;
    m_numCycles = 0;
    off();
}

bool outPinController_Class::isPlayingSomething() {
    return m_fixed || m_pulseOn || m_blinking || m_temporaryBlinking;
}



// #############################################################
// # il timeout non è vincolante, se il suono finisce prima si esce...
// #############################################################
void outPinController_Class::waitForPulseEnding(int32_t timeOut) {
    while (isPlayingSomething() && timeOut > 0) {
        delay(10);
        timeOut -= 10;
        update();
        LOG_TRACE("\t[%s] waiting for the end of pulse", m_pinID);
    }
}
