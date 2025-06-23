//
// updated by ...: Loreto Notarantonio
// Date .........: 22-06-2025 16.05.50
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



// Funzione di esempio per suonare una semplice scala
// int notes[] = {262, 294, 330, 349, 392, 440, 494, 523, -1}; // Frequenze di Do, Re, Mi, Fa, Sol, La, Si, Do (ottava successiva)
// notes[] passare il parametro come &notes[0]
void Buzzer_func_inside::playScale(uint16_t *notes[], int duration) {
  // int notes[] = {262, 294, 330, 349, 392, 440, 494, 523}; // Frequenze di Do, Re, Mi, Fa, Sol, La, Si, Do (ottava successiva)
  // int noteDuration = 200; // Durata di ogni nota in ms
    uint16_t *p = &notes[0];
    uint8_t lun=0;
    while (*notes != END_OF_ARRAY) {
        lun++;
        p++;
    }
    return lun;

  for (int i = 0; i < 8; i++) {
    playTone(notes[i], noteDuration);
    delay(50); // Breve pausa tra le note
  }
  noTone();
}


// Funzione di esempio per suonare una scala in modo non bloccante
void Buzzer_func_inside::playScaleNonBlocking(int notes[], int noteDuration) {
    // static int notes[] = {262, 294, 330, 349, 392, 440, 494, 523}; // Frequenze di Do, Re, Mi, Fa, Sol, La, Si, Do
    // static int noteDuration = 200; // Durata di ogni nota in ms
    static int currentNoteIndex = 0;
    static unsigned long lastNoteChangeTime = 0;
    static bool scaleActive = false; // Stato per gestire l'inizio e la fine della scala

    // Inizializza la scala se non è attiva
    if (!scaleActive) {
        currentNoteIndex = 0;
        lastNoteChangeTime = millis();
        scaleActive = true;
        isTonePlaying = true; // Indica che qualcosa sta suonando (la scala)
        Serial.println("Inizio scala non bloccante...");
        playToneNonBlocking(notes[currentNoteIndex], noteDuration);
    }

    // Se la scala è attiva, gestisci il passaggio alla nota successiva
    if (scaleActive) {
        if (millis() - lastNoteChangeTime >= (noteDuration + 50)) { // 50ms di pausa tra le note
            currentNoteIndex++;
            if (currentNoteIndex < (sizeof(notes) / sizeof(notes[0]))) {
                playToneNonBlocking(notes[currentNoteIndex], noteDuration);
                lastNoteChangeTime = millis();
            } else {
                // Scala finita
                noTone();
                scaleActive = false;
                Serial.println("Scala terminata.");
            }
        }
    }
}