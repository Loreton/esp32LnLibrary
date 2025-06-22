//
// updated by ...: Loreto Notarantonio
// Date .........: 21-06-2025 20.05.27
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#pragma once

#include <Arduino.h>    // in testa anche per le definizioni dei type


typedef struct Buzzer_func_inside {
    int             _pin;
    int             _channel;
    unsigned long   _toneStartTime;
    unsigned long   _toneDuration;
    bool            _isPlaying;
    bool            _isAttached;
    int16_t         _scaleUpNotes[] = {262, 294, 330, 349, 392, 440, 494, 523}; // Frequenze di Do, Re, Mi, Fa, Sol, La, Si, Do (ottava successiva);
    int16_t         _scaleDnNotes[] = {523, 494, 440, 392, 349, 330, 294, 262};

    void init(uint8_t pin, uint8_t channel);
    void playTone(int frequency, unsigned long duration);
    void noTone();
    void handle();
    bool isPlayingTone();
    void detach();
} Buzzer_func_inside;

