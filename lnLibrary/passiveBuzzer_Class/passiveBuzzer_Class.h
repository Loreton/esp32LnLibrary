//
// updated by ...: Loreto Notarantonio
// Date .........: 15-09-2025 14.21.21
//

#pragma once

#include <Arduino.h> // in testa anche per le definizioni dei type

class PassiveBuzzer_Class {
    public:
        // Constructor: Called when you create a PassiveBuzzer_Class object
        PassiveBuzzer_Class(void);

        // Destructor: Optional, but good practice for resource management (e.g., if you had dynamic memory allocations)
        // ~PassiveBuzzer_Class();

        void init(const char* pin_name, int buzzerPin, uint8_t active_level, int ledcChannel = 0, int resBits = 10);
        void begin();
        void playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration);
        // void playToneFixed(int frequency, uint32_t duration);
        void playFixedTone(uint16_t frequency, uint32_t duration, bool waitForPulseEnding=false);
        void playScale(uint16_t noteFrequencies[], uint8_t numberOfNotes, uint32_t singleNoteDuration, bool upDirection);
        void playScale2(uint16_t noteFrequencies[], uint8_t numberOfNotes, uint32_t singleNoteDuration[], bool upDirection, bool waitForEnding);
        void myNoTone();
        void update();
        bool isPlayingSomething();
        void waitForPulseEnding(int32_t timeOut);

        inline const char* pinID(void) const { return m_pinID; }


    private:
        int      m_pin = 0;
        const    char* m_name;
        char     m_pinID[21]; // conterrà [%-10s:%02d] p->pin, p->name,

        uint8_t  m_activeLevel = LOW; // Livello che attiva il relè (HIGH o LOW)
        uint8_t  m_on = HIGH;         // Livello che attiva il relè (HIGH o LOW)
        uint8_t  m_off = LOW;         // Livello che attiva il relè (HIGH o LOW)

        // The ESP32 has 16 channels which can generate 16 independent waveforms
        // We'll just choose PWM channel 0 here
        int      m_tonePwmChannel = 0;
        // int      m_channel = 0;
        int      m_resolutionBits = 10; // resolution bits=10
        int      m_currentFrequency = 0;

        // Variabili per la gestione del suono singolo non bloccante
        uint32_t m_toneStartTime = 0;
        uint32_t m_toneDuration = 0;
        bool     m_isPlaying = false;

        // Nuove variabili per la gestione della scala non bloccante
        uint16_t * m_scaleNotes = nullptr;
        int      m_numNotes = 0;
        int      m_currentNoteIndex = 0;
        uint32_t m_noteStartTime = 0;
        uint32_t m_noteDuration = 0;
        bool     m_isPlayingScale = false;
        bool     m_scaleDirectionUp = true;
};