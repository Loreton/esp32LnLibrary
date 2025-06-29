//
// updated by ...: Loreto Notarantonio
// Date .........: 29-06-2025 18.22.05
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>

// It's generally better to define callbacks within the class if they
// are specific to the class's instances, or as a std::function.
// For simplicity in this conversion, we'll remove the original typedef
// as direct member functions are preferred in a class.

class LedController_Class {
    public: // Public members and methods
        // Constructor
        LedController_Class(const char* name, uint8_t pin, uint8_t active_level = HIGH);

        // Public methods
        void update();
        void blinking(uint32_t onMs, uint32_t offMs, int8_t cycles = 0);
        void blinking_dc(uint32_t period, float duty_cycle, int8_t cycles = 0);
        void pulse(uint32_t duration);
        void off();
        void off_ifBlinking();
        void on();
        void set(uint8_t state);

        // Optional: Getters for certain internal states if needed externally
        bool isBlinking() const { return m_blinking; }
        bool isPulseOn() const { return m_pulseOn; }
        bool isFixed() const { return m_fixed; }
        bool getLedState() const { return m_ledState; }
        const char* getName() const { return m_name; } // Added for consistency
        const char* getPinID() const { return m_pinID; } // Added for consistency

    private: // Private members and methods (formerly struct members and internal functions)
        uint8_t     m_pin;
        const char* m_name;
        char        m_pinID[21];

        uint8_t     m_on_level = HIGH;
        uint8_t     m_on       = HIGH;
        uint8_t     m_off      = LOW;

        // Blinking parameters
        uint32_t    m_onTime     = 500;
        uint32_t    m_offTime    = 500;
        bool        m_blinking   = false;
        int8_t      m_num_cycles = 0;
        bool        m_temporaryBlinking = false;

        // Fixed on parameters
        bool        m_fixed           = true;
        bool        m_pulseOn         = false;
        uint32_t    m_pulseOnStart    = 0;
        uint32_t    m_pulseOnDuration = 0;

        // Internals
        bool        m_ledState   = false;
        uint32_t    m_lastToggle = 0;

        // Internal helper functions (now private methods)
        void setLed(bool on);
        void setFixed();
        void setBlinking(int8_t cycles = 0);
        void setPulse();
        void clearAll();
};