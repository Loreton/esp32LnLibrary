//
// updated by ...: Loreto Notarantonio
// Date .........: 21-06-2025 19.04.49
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf_XFN()

#include "ledc_buzzer_sClass.h"




void Buzzer_func_inside::init(uint8_t pin, uint8_t channel) {
    // Inizializza i membri della struct
    _pin           = pin;
    _channel       = channel;
    _toneStartTime = 0;
    _toneDuration  = 0;
    _isPlaying     = false;
    _isAttached    = false;

    // bool rcode = ledcAttachChannel(buzzer_pin, 1000, resolution, channel);
    ledcSetup(_channel, 1000, 10);
    ledcAttachPin(_pin, _channel);
    printf0_FN("Buzzer su pin: %d, channel: %d\n", _pin, _channel);
    _isAttached    = true;
    noTone();

}

void Buzzer_func_inside::playTone(int frequency, unsigned long duration) {
    if (!_isPlaying) {
        ledcWriteTone(_channel, frequency);
        _toneStartTime = millis();
        _toneDuration = duration;
        _isPlaying = true;

        printf99_FN("Avvio tono (Pin %d) %d Hz per %lu ms\n", _pin, frequency, duration);
    } else {
        printf99_FN("Buzzer (Pin %d) occupato, impossibile avviare nuovo tono.\n", _pin);
    }
}

void Buzzer_func_inside::noTone() {
    ledcWriteTone(_channel, 0);
    _isPlaying = false;
}

void Buzzer_func_inside::handle() {
    if (_isPlaying) {
        if (millis() - _toneStartTime >= _toneDuration) {
            noTone();
            printf99_FN("Tono terminato (Pin %d)\n", _pin);
        }
    }
}

bool Buzzer_func_inside::isPlayingTone() {
    return _isPlaying;
}

void Buzzer_func_inside::detach() {
    if (_isAttached) {
        noTone();
        ledcDetachPin(_pin); // Potrebbe causare avvisi se non era attaccato, ma non errori gravi
        _isAttached = false;
        printf0_FN("Buzzer detached.\n");
    }
}
