//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 18.06.53
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>

// It's generally better to define callbacks within the class if they
// are specific to the class's instances, or as a std::function.
// For simplicity in this conversion, we'll remove the original typedef
// as direct member functions are preferred in a class.

class PinController_Class {
    public: // Public members and methods
        // Constructor
        PinController_Class(const char* name, uint8_t pin, uint8_t active_level = HIGH);

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
        bool isBlinking() const { return blinking_; }
        bool isPulseOn() const { return pulseOn_; }
        bool isFixed() const { return fixed_; }
        bool getLedState() const { return ledState_; }
        const char* getName() const { return name_; } // Added for consistency
        const char* getPinID() const { return pinID_; } // Added for consistency

    private: // Private members and methods (formerly struct members and internal functions)
        uint8_t     pin_;
        const char* name_;
        char        pinID_[21];

        uint8_t     on_level_ = HIGH;
        uint8_t     on_       = HIGH;
        uint8_t     off_      = LOW;

        // Blinking parameters
        uint32_t    onTime_     = 500;
        uint32_t    offTime_    = 500;
        bool        blinking_   = false;
        int8_t      num_cycles_ = 0;
        bool        temporaryBlinking_ = false;

        // Fixed on parameters
        bool        fixed_           = true;
        bool        pulseOn_         = false;
        uint32_t    pulseOnStart_    = 0;
        uint32_t    pulseOnDuration_ = 0;

        // Internals
        bool        ledState_   = false;
        uint32_t    lastToggle_ = 0;

        // Internal helper functions (now private methods)
        void _setLed(bool on);
        void _setFixed();
        void _setBlinking(int8_t cycles = 0);
        void _setPulse();
        void _clearAll();
};