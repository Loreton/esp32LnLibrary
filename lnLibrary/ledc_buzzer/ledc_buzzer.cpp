//
// updated by ...: Loreto Notarantonio
// Date .........: 21-06-2025 19.07.52
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf_XFN()

#include "ledc_buzzer.h"



// --- Implementazione delle Funzioni Esterne ---

void buzzer_init(Buzzer_t* b, uint8_t pin, uint8_t channel) {
    // Inizializza i membri della struct
    b->pin           = pin;
    b->channel       = channel;
    b->toneStartTime = 0;
    b->toneDuration  = 0;
    b->isPlaying     = false;
    b->isAttached    = false;

    // bool rcode = ledcAttachChannel(buzzer_pin, 1000, resolution, channel);
    ledcSetup(b->channel, 1000, 10);
    ledcAttachPin(b->pin, b->channel);
    printf0_FN("Buzzer su pin: %d, channel: %d\n", b->pin, b->channel);
    b->isAttached    = true;
    buzzer_noTone(b);

}

void buzzer_playTone(Buzzer_t* b, int frequency, unsigned long duration) {
    if (!b->isPlaying) {
        ledcWriteTone(b->channel, frequency);
        b->toneStartTime = millis();
        b->toneDuration = duration;
        b->isPlaying = true;

        printf99_FN("Avvio tono (Pin %d) %d Hz per %lu ms\n", b->pin, frequency, duration);
    } else {
        printf99_FN("Buzzer (Pin %d) occupato, impossibile avviare nuovo tono.\n", b->pin);
    }
}

void buzzer_noTone(Buzzer_t* b) {
    ledcWriteTone(b->channel, 0);
    b->isPlaying = false;
}

void buzzer_handle(Buzzer_t* b) {
    if (b->isPlaying) {
        if (millis() - b->toneStartTime >= b->toneDuration) {
            buzzer_noTone(b);
            printf99_FN("Tono terminato (Pin %d)\n", b->pin);
        }
    }
}

bool buzzer_isPlayingTone(Buzzer_t* b) {
    return b->isPlaying;
}

void buzzer_detach(Buzzer_t* b) {
    if (b->isAttached) {
        buzzer_noTone(b);
        ledcDetachPin(b->pin); // Potrebbe causare avvisi se non era attaccato, ma non errori gravi
        b->isAttached = false;
        printf0_FN("Buzzer detached.\n");
    }
}
