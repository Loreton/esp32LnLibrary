//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 16.39.08
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#define LOG_LEVEL_0x
#define LOG_LEVEL_2
#define LOG_LEVEL_99x
#include "@globalVars.h" // printf_XFN()

#include "ledc_buzzer_Struct.h"
// Costruttore: chiamato quando crei un oggetto Buzzer
passiveBuzzer_sc::passiveBuzzer_sc(const char* pin_name, int buzzerPin, int ledcChannel, int resBits) {
    pin_              = buzzerPin;
    name_             = pin_name;
    channel_          = ledcChannel;
    resolutionBits_   = resBits;
    currentFrequency_ = 0; // Inizialmente nessuna frequenza

    toneStartTime_    = 0;
    toneDuration_     = 0;
    isPlaying_        = false;

    scaleNotes_       = nullptr;
    numNotes_         = 0;
    currentNoteIndex_ = 0;
    noteStartTime_    = 0;
    noteDuration_     = 0;
    isPlayingScale_   = false;
    scaleDirectionUp_ = true;

    int cx = snprintf(pinID_, PIN_ID_MAXLENGTH-6, "%[%s", name_);
    cx = snprintf(pinID_+cx, PIN_ID_MAXLENGTH-cx, ".%02d]:", pin_);
    // printf2_FN("Written %d/%d chars", cx, PIN_ID_LENGTH); Serial non ancora attiva
}


  // Metodo per inizializzare il buzzer
void passiveBuzzer_sc::begin() {
    unsigned long actualFreq = ledcSetup(channel_, 1, resolutionBits_); // Frequenza iniziale > 0 altrimenti da errore

    if (actualFreq == 0) {
        printf2_FN("%s [ERRORE] ledcSetup() FALLITO per canale %d. Controlla pin, canale e risoluzione.", channel_);
        // Puoi aggiungere qui una logica per gestire l'errore, es. bloccare l'esecuzione
        while(true); // Ferma il programma in caso di errore critico
    }
    else {
        // Inizializza il canale con una frequenza di base (può essere 0 o una bassa frequenza)
        // e la risoluzione. La frequenza verrà poi impostata da playTone/playScale.
        // ledcSetup(channel, 1, resolutionBits);
        ledcAttachPin(pin_, channel_);
        noTone(); // Assicurati che sia spento all'inizio
        printf2_FN("%s Channel: %d - Risoluzione: %d bit inizializzato.\n", pinID_, channel_, resolutionBits_);
    }
}

  // --- Funzionalità 1: Suono con Duty Cycle Variabile ---
  // Imposta la frequenza e un duty cycle specifico (0-100%)
void passiveBuzzer_sc::playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration) {
    if (isPlaying_ || isPlayingScale_) {
        printf2_FN("%s occupato, impossibile avviare nuovo tono.\n", pinID_);
        return;
    }

    int maxDuty = (1 << resolutionBits_) - 1;
    int dutyValue = (int)(maxDuty * (dutyCyclePercent / 100.0));

    // Imposta la nuova frequenza e poi il duty cycle.
    // ledcSetup ricarica la frequenza e la risoluzione, ripartendo da capo.
    // E' il modo più affidabile per cambiare dinamicamente frequenza e duty cycle.
    ledcSetup(channel_, frequency, resolutionBits_);
    ledcWrite(channel_, dutyValue);

    currentFrequency_ = frequency; // Aggiorna la frequenza corrente
    toneStartTime_ = millis();
    toneDuration_ = duration;
    isPlaying_ = true;
    printf2_FN("%s Avvio tono DC freq: %d Hz - DC: %.2f%, Durata: %lu ms\n", pinID_, currentFrequency_, dutyCyclePercent, toneDuration_);

}

  // --- Funzionalità 2: Suono Fisso di Durata Variabile (Duty Cycle 50%) ---
  // Usa ledcWriteTone() per un duty cycle automatico del 50%
void passiveBuzzer_sc::playToneFixed(int frequency, uint32_t duration) {
    if (isPlaying_ || isPlayingScale_) {
        printf2_FN("%s occupato, impossibile avviare nuovo tono.\n", pinID_);
        return;
    }

    ledcWriteTone(channel_, frequency); // Imposta frequenza, duty cycle 50% automatico
    currentFrequency_ = frequency; // Aggiorna la frequenza corrente

    toneStartTime_ = millis();
    toneDuration_ = duration;
    isPlaying_ = true;
    printf2_FN("%s Avvio tono DC freq: %d Hz - Durata: %lu ms\n", pinID_, currentFrequency_, toneDuration_);
}





  // Funzionalità 3: Suona una Scala (Non Bloccante)
void passiveBuzzer_sc::playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection) {
    if (isPlaying_ || isPlayingScale_) {
        printf2_FN("%s occupato, impossibile avviare la scala.\n", pinID_);
      return;
    }

    if (numberOfNotes == 0 || noteFrequencies == nullptr) {
        printf2_FN("%s occupato, suonare una scala vuota.\n", pinID_);
        return;
    }

    scaleNotes_ = noteFrequencies;
    numNotes_ = numberOfNotes;
    noteDuration_ = singleNoteDuration;
    scaleDirectionUp_ = upDirection;
    isPlayingScale_ = true;

    // Inizializza la prima nota della scala
    if (scaleDirectionUp_) {
        currentNoteIndex_ = 0;
    } else {
        currentNoteIndex_ = numNotes_ - 1;
    }

    // Inizia a suonare la prima nota
    ledcWriteTone(channel_, scaleNotes_[currentNoteIndex_]);
    currentFrequency_ = scaleNotes_[currentNoteIndex_]; // Aggiorna la frequenza corrente
    noteStartTime_ = millis();
    printf2_FN("%s Avvio scala %s\n", pinID_, (scaleDirectionUp_ ? "Ascendente" : "Discendente"));
}

  // Metodo per fermare qualsiasi suono (singolo o scala)
void passiveBuzzer_sc::noTone() {
    ledcWrite(channel_, 0); // Imposta il duty cycle a 0 per spegnere completamente
    // Non è necessario chiamare ledcSetFreq(channel, 0) qui,
    // dato che ledcSetup o ledcWriteTone lo faranno alla prossima chiamata.
    currentFrequency_ = 0; // Resetta la frequenza corrente
    isPlaying_ = false;
    isPlayingScale_ = false;
}





// Metodo da chiamare ripetutamente nel loop() per gestire la fine del tono o il progresso della scala
void passiveBuzzer_sc::handle() {
    if (isPlaying_) {
        if (millis() - toneStartTime_ >= toneDuration_) {
            noTone();
            printf2_FN("%s Tono singolo terminato.\n", pinID_);
        }
    }
    else if (isPlayingScale_) {
        if (millis() - noteStartTime_ >= noteDuration_) {
        // Tempo per cambiare nota
            if (scaleDirectionUp_) {
                currentNoteIndex_++;
            } else {
                currentNoteIndex_--;
            }

                // Controlla se la scala è finita
            if ((scaleDirectionUp_ && currentNoteIndex_ >= numNotes_) || (!scaleDirectionUp_ && currentNoteIndex_ < 0)) {
              noTone();
                printf2_FN("%s Scala terminata.\n", pinID_);
            }
            else {
                  // Suona la prossima nota
                ledcWriteTone(channel_, scaleNotes_[currentNoteIndex_]);
                currentFrequency_ = scaleNotes_[currentNoteIndex_]; // Aggiorna la frequenza corrente
                noteStartTime_ = millis(); // Resetta il tempo di inizio per la nuova nota
                printf99_FN("%s nota %d.\n", pinID_, scaleNotes_[currentNoteIndex_]);
            }
        }
    }
}

  // Metodo per verificare se il buzzer sta suonando (sia tono singolo che scala)
bool passiveBuzzer_sc::isPlayingSomething() {
    return isPlaying_ || isPlayingScale_;
}

