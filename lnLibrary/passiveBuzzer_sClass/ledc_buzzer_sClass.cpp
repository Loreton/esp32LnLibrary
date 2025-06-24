//
// updated by ...: Loreto Notarantonio
// Date .........: 24-06-2025 17.04.44
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#define LOG_LEVEL_0x
#define LOG_LEVEL_2
#define LOG_LEVEL_99x
#include "@globalVars.h" // printf_XFN()

#include "ledc_buzzer_sClass.h"
// Costruttore: chiamato quando crei un oggetto Buzzer
passiveBuzzer_sc::passiveBuzzer_sc(const char* pin_name, int buzzerPin, int ledcChannel, int resBits) {
    _pin              = buzzerPin;
    _name             = pin_name;
    _channel          = ledcChannel;
    _resolutionBits   = resBits;
    _currentFrequency = 0; // Inizialmente nessuna frequenza

    _toneStartTime    = 0;
    _toneDuration     = 0;
    _isPlaying        = false;

    _scaleNotes       = nullptr;
    _numNotes         = 0;
    _currentNoteIndex = 0;
    _noteStartTime    = 0;
    _noteDuration     = 0;
    _isPlayingScale   = false;
    _scaleDirectionUp = true;

    int cx = snprintf(_pinID, PIN_ID_MAXLENGTH-6, "%[%s", _name);
    cx = snprintf(_pinID+cx, PIN_ID_MAXLENGTH-cx, ".%02d]:", _pin);
    // printf2_FN("Written %d/%d chars", cx, PIN_ID_LENGTH); Serial non ancora attiva
}


  // Metodo per inizializzare il buzzer
void passiveBuzzer_sc::begin() {
    unsigned long actualFreq = ledcSetup(_channel, 1, _resolutionBits); // Frequenza iniziale > 0 altrimenti da errore

    if (actualFreq == 0) {
        printf2_FN("%s [ERRORE] ledcSetup() FALLITO per canale %d. Controlla pin, canale e risoluzione.", _channel);
        // Puoi aggiungere qui una logica per gestire l'errore, es. bloccare l'esecuzione
        while(true); // Ferma il programma in caso di errore critico
    }
    else {
        // Inizializza il canale con una frequenza di base (può essere 0 o una bassa frequenza)
        // e la risoluzione. La frequenza verrà poi impostata da playTone/playScale.
        // ledcSetup(channel, 1, resolutionBits);
        ledcAttachPin(_pin, _channel);
        noTone(); // Assicurati che sia spento all'inizio
        printf2_FN("%s Channel: %d - Risoluzione: %d bit inizializzato.\n", _pinID, _channel, _resolutionBits);
    }
}

  // --- Funzionalità 1: Suono con Duty Cycle Variabile ---
  // Imposta la frequenza e un duty cycle specifico (0-100%)
void passiveBuzzer_sc::playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration) {
    if (_isPlaying || _isPlayingScale) {
        printf2_FN("%s occupato, impossibile avviare nuovo tono.\n", _pinID);
        return;
    }

    int maxDuty = (1 << _resolutionBits) - 1;
    int dutyValue = (int)(maxDuty * (dutyCyclePercent / 100.0));

    // Imposta la nuova frequenza e poi il duty cycle.
    // ledcSetup ricarica la frequenza e la risoluzione, ripartendo da capo.
    // E' il modo più affidabile per cambiare dinamicamente frequenza e duty cycle.
    ledcSetup(_channel, frequency, _resolutionBits);
    ledcWrite(_channel, dutyValue);

    _currentFrequency = frequency; // Aggiorna la frequenza corrente
    _toneStartTime = millis();
    _toneDuration = duration;
    _isPlaying = true;
    printf2_FN("%s Avvio tono DC freq: %d Hz - DC: %.2f%, Durata: %lu ms\n", _pinID, _currentFrequency, dutyCyclePercent, _toneDuration);

}

  // --- Funzionalità 2: Suono Fisso di Durata Variabile (Duty Cycle 50%) ---
  // Usa ledcWriteTone() per un duty cycle automatico del 50%
void passiveBuzzer_sc::playToneFixed(int frequency, uint32_t duration) {
    if (_isPlaying || _isPlayingScale) {
        printf2_FN("%s occupato, impossibile avviare nuovo tono.\n", _pinID);
        return;
    }

    ledcWriteTone(_channel, frequency); // Imposta frequenza, duty cycle 50% automatico
    _currentFrequency = frequency; // Aggiorna la frequenza corrente

    _toneStartTime = millis();
    _toneDuration = duration;
    _isPlaying = true;
    printf2_FN("%s Avvio tono DC freq: %d Hz - Durata: %lu ms\n", _pinID, _currentFrequency, _toneDuration);
}





  // Funzionalità 3: Suona una Scala (Non Bloccante)
void passiveBuzzer_sc::playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection) {
    if (_isPlaying || _isPlayingScale) {
        printf2_FN("%s occupato, impossibile avviare la scala.\n", _pinID);
      return;
    }

    if (numberOfNotes == 0 || noteFrequencies == nullptr) {
        printf2_FN("%s occupato, suonare una scala vuota.\n", _pinID);
        return;
    }

    _scaleNotes = noteFrequencies;
    _numNotes = numberOfNotes;
    _noteDuration = singleNoteDuration;
    _scaleDirectionUp = upDirection;
    _isPlayingScale = true;

    // Inizializza la prima nota della scala
    if (_scaleDirectionUp) {
        _currentNoteIndex = 0;
    } else {
        _currentNoteIndex = _numNotes - 1;
    }

    // Inizia a suonare la prima nota
    ledcWriteTone(_channel, _scaleNotes[_currentNoteIndex]);
    _currentFrequency = _scaleNotes[_currentNoteIndex]; // Aggiorna la frequenza corrente
    _noteStartTime = millis();
    printf2_FN("%s Avvio scala %s\n", _pinID, (_scaleDirectionUp ? "Ascendente" : "Discendente"));
}

  // Metodo per fermare qualsiasi suono (singolo o scala)
void passiveBuzzer_sc::noTone() {
    ledcWrite(_channel, 0); // Imposta il duty cycle a 0 per spegnere completamente
    // Non è necessario chiamare ledcSetFreq(channel, 0) qui,
    // dato che ledcSetup o ledcWriteTone lo faranno alla prossima chiamata.
    _currentFrequency = 0; // Resetta la frequenza corrente
    _isPlaying = false;
    _isPlayingScale = false;
}





// Metodo da chiamare ripetutamente nel loop() per gestire la fine del tono o il progresso della scala
void passiveBuzzer_sc::handle() {
    if (_isPlaying) {
        if (millis() - _toneStartTime >= _toneDuration) {
            noTone();
            printf2_FN("%s Tono singolo terminato.\n", _pinID);
        }
    }
    else if (_isPlayingScale) {
        if (millis() - _noteStartTime >= _noteDuration) {
        // Tempo per cambiare nota
            if (_scaleDirectionUp) {
                _currentNoteIndex++;
            } else {
                _currentNoteIndex--;
            }

                // Controlla se la scala è finita
            if ((_scaleDirectionUp && _currentNoteIndex >= _numNotes) || (!_scaleDirectionUp && _currentNoteIndex < 0)) {
              noTone();
                printf2_FN("%s Scala terminata.\n", _pinID);
            }
            else {
                  // Suona la prossima nota
                ledcWriteTone(_channel, _scaleNotes[_currentNoteIndex]);
                _currentFrequency = _scaleNotes[_currentNoteIndex]; // Aggiorna la frequenza corrente
                _noteStartTime = millis(); // Resetta il tempo di inizio per la nuova nota
                printf99_FN("%s nota %d.\n", _pinID, _scaleNotes[_currentNoteIndex]);
            }
        }
    }
}

  // Metodo per verificare se il buzzer sta suonando (sia tono singolo che scala)
bool passiveBuzzer_sc::isPlayingSomething() {
    return _isPlaying || _isPlayingScale;
}

