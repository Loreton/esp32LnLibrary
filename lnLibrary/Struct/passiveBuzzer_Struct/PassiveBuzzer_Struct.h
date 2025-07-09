//
// updated by ...: Loreto Notarantonio
// Date .........: 09-07-2025 08.06.00
//

#pragma once

    #include <Arduino.h>    // in testa anche per le definizioni dei type


    #define PIN_ID_MAXLENGTH 17
    // --- Definizione della Struttura Buzzer ---


    typedef struct PassiveBuzzer_Struct {
        int      m_pin              = 0;
        const    char*  m_name;
        char     m_pinID[PIN_ID_MAXLENGTH+1];  // conterrà [%-10s:%02d] p->pin, p->name,
        int      m_channel          = 0;
        int      m_resolutionBits   = 10; // resolution bits=10
        int      m_currentFrequency = 0;

        //       Variabili         per            la       gestione del      suono   singolo non       bloccante
        uint32_t m_toneStartTime    = 0;
        uint32_t m_toneDuration     = 0;
        bool     m_isPlaying        = false;

        //       Nuove             variabili      per      la       gestione della   scala   non       bloccante
        int*     m_scaleNotes       = nullptr;
        int      m_numNotes         = 0;
        int      m_currentNoteIndex = 0;
        uint32_t m_noteStartTime    = 0;
        uint32_t m_noteDuration     = 0;
        bool     m_isPlayingScale   = false;
        bool     m_scaleDirectionUp = true;

        PassiveBuzzer_Struct(void);
      // Costruttore: chiamato quando crei un oggetto Buzzer
        // PassiveBuzzer_Struct(const char* pin_name, int buzzerPin, int ledcChannel, int resBits = 10);
        void init(const char* pin_name, int buzzerPin, int ledcChannel, int resBits = 10);
        // init(int buzzerPin, int ledcChannel, int resBits = 10);
        void begin();
        void playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration);
        void playToneFixed(int frequency, uint32_t duration);
        void playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection);
        void noTone();
        void handle();
        bool isPlayingSomething();
    } PassiveBuzzer_Struct;

