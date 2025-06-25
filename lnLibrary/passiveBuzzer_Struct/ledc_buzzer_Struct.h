//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.37.02
//

#pragma once

#include <Arduino.h>    // in testa anche per le definizioni dei type


#define PIN_ID_MAXLENGTH 17
// --- Definizione della Struttura Buzzer ---
typedef struct passiveBuzzer_sc {
    int      pin_;
    const    char*  name_;
    char     pinID_[PIN_ID_MAXLENGTH+1];  // conterrà [%-10s:%02d] p->pin, p->name,
    int      channel_;
    int      resolutionBits_;
    int      currentFrequency_; //             Aggiunta per      tenere   traccia della   frequenza corrente

    //       Variabili         per            la       gestione del      suono   singolo non       bloccante
    uint32_t toneStartTime_;
    uint32_t toneDuration_;
    bool     isPlaying_;

    //       Nuove             variabili      per      la       gestione della   scala   non       bloccante
    int*     scaleNotes_;
    int      numNotes_;
    int      currentNoteIndex_;
    uint32_t noteStartTime_;
    uint32_t noteDuration_;
    bool     isPlayingScale_;
    bool     scaleDirectionUp_;

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

