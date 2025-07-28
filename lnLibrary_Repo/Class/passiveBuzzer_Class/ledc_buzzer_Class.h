//
// updated by ...: Loreto Notarantonio
// Date .........: 27-06-2025 15.24.41
//
// file: ledc_buzzer_Class.h
#pragma once

#include <Arduino.h>

// #define PIN_ID_MAXLENGTH 17

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
        int m_pin;
        const char* m_name;
        char m_pinID[17];
        int m_channel;
        int m_resolutionBits;
        int m_currentFrequency;

        // Variables for non-blocking single tone management
        uint32_t m_toneStartTime;
        uint32_t m_toneDuration;
        bool m_isPlaying;

        // Variables for non-blocking scale management
        int* m_scaleNotes;
        int m_numNotes;
        int m_currentNoteIndex;
        uint32_t m_noteStartTime;
        uint32_t m_noteDuration;
        bool m_isPlayingScale;
        bool m_scaleDirectionUp;
};