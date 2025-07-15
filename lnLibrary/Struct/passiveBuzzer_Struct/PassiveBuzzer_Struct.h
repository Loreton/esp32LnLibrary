//
// updated by ...: Loreto Notarantonio
// Date .........: 14-07-2025 15.12.25
//

#pragma once

    #include <Arduino.h>    // in testa anche per le definizioni dei type


    #define PIN_ID_MAXLENGTH 17
    // --- Definizione della Struttura Buzzer ---


    typedef struct PassiveBuzzer_Struct {
        int      m_pin              = 0;
        const    char*  m_name;
        char     m_pinID[PIN_ID_MAXLENGTH+1];  // conterrà [%-10s:%02d] p->pin, p->name,


        uint8_t  m_activeLevel=LOW; //              Livello che attiva il relè (HIGH o LOW)
        uint8_t  m_on=HIGH; //              Livello che attiva il relè (HIGH o LOW)
        uint8_t  m_off=LOW; //              Livello che attiva il relè (HIGH o LOW)

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

        // Costruttore: chiamato quando crei un oggetto Buzzer
        PassiveBuzzer_Struct(void);

        // PassiveBuzzer_Struct(const char* pin_name, int buzzerPin, int ledcChannel, int resBits = 10);
        void init(const char* pin_name, int buzzerPin, uint8_t active_level, int ledcChannel, int resBits = 10);
        // init(int buzzerPin, int ledcChannel, int resBits = 10);
        void begin();
        void playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration);
        void playToneFixed(int frequency, uint32_t duration);
        void playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection);
        void noTone();
        void handle();
        bool isPlayingSomething();
    } PassiveBuzzer_Struct;

