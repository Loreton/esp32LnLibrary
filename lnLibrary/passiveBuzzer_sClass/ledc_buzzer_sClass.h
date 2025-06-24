//
// updated by ...: Loreto Notarantonio
// Date .........: 24-06-2025 16.59.23
//

#pragma once

#include <Arduino.h>    // in testa anche per le definizioni dei type


#define PIN_ID_MAXLENGTH 17
// --- Definizione della Struttura Buzzer ---
typedef struct passiveBuzzer_sc {
    int      _pin;
    const    char*  _name;
    char     _pinID[PIN_ID_MAXLENGTH+1];  // conterrà [%-10s:%02d] p->pin, p->name,
    int      _channel;
    int      _resolutionBits;
    int      _currentFrequency; //             Aggiunta per      tenere   traccia della   frequenza corrente

    //       Variabili         per            la       gestione del      suono   singolo non       bloccante
    uint32_t _toneStartTime;
    uint32_t _toneDuration;
    bool     _isPlaying;

    //       Nuove             variabili      per      la       gestione della   scala   non       bloccante
    int*     _scaleNotes;
    int      _numNotes;
    int      _currentNoteIndex;
    uint32_t _noteStartTime;
    uint32_t _noteDuration;
    bool     _isPlayingScale;
    bool     _scaleDirectionUp;

  // Costruttore: chiamato quando crei un oggetto Buzzer
    passiveBuzzer_sc(const char* pin_name, int buzzerPin, int ledcChannel, int resBits = 10);
    // init(int buzzerPin, int ledcChannel, int resBits = 10);
    void begin();
    void playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration);
    void playToneFixed(int frequency, uint32_t duration);
    void playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection);
    void noTone();
    void handle();
    bool isPlayingSomething();
} passiveBuzzer_sc;

