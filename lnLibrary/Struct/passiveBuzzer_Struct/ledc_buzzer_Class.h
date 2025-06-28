//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 17.11.35
//
// file: ledc_buzzer_Class.h
#pragma once

#include <Arduino.h>

#define PIN_ID_MAXLENGTH 17

// --- Definizione della Classe Buzzer ---
class passiveBuzzer_Class { // Changed from typedef struct to class
    public: // Public members are accessible from outside the class
        // Constructor
        passiveBuzzer_Class(const char* pin_name, int buzzerPin, int ledcChannel, int resBits = 10);

        // Public methods
        void begin();
        void playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration);
        void playToneFixed(int frequency, uint32_t duration);
        void playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection);
        void noTone();
        void handle();
        bool isPlayingSomething();

    private: // Private members are only accessible from within the class
        int pin_;
        const char* name_;
        char pinID_[PIN_ID_MAXLENGTH + 1];
        int channel_;
        int resolutionBits_;
        int currentFrequency_;

        // Variables for non-blocking single tone management
        uint32_t toneStartTime_;
        uint32_t toneDuration_;
        bool isPlaying_;

        // Variables for non-blocking scale management
        int* scaleNotes_;
        int numNotes_;
        int currentNoteIndex_;
        uint32_t noteStartTime_;
        uint32_t noteDuration_;
        bool isPlayingScale_;
        bool scaleDirectionUp_;
};