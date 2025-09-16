/*
// updated by ...: Loreto Notarantonio
// Date .........: 20-06-2025 17.17.10
*/

#pragma once

// --- Definizione della Struttura Buzzer ---
// Nota: tutti i membri sono pubblici per default in C/C++ struct
typedef struct Buzzer_t {
    int pin;
    int channel;
    unsigned long toneStartTime;
    unsigned long toneDuration;
    bool isPlaying;
    bool isAttached;
} Buzzer_t;



// --- Dichiarazioni delle Funzioni Esterne ---
// Le funzioni prendono un puntatore alla struct Buzzer come primo argomento
void buzzer_init(Buzzer_t* b, uint8_t pin, uint8_t channel);
void buzzer_playTone(Buzzer_t* b, int frequency, unsigned long duration);
void buzzer_noTone(Buzzer_t* b);
void buzzer_handle(Buzzer_t* b);
bool buzzer_isPlayingTone(Buzzer_t* b);
void buzzer_detach(Buzzer_t* b);