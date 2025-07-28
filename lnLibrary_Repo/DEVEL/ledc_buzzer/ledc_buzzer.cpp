//
// updated by ...: Loreto Notarantonio
// Date .........: 09-07-2025 13.51.57
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// #include "@logMacros.h" // printf_XFN()

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

        // printf99_FN("Avvio tono (Pin %d) %d Hz per %lu ms\n", b->pin, frequency, duration);
    } else {
        // printf99_FN("Buzzer (Pin %d) occupato, impossibile avviare nuovo tono.\n", b->pin);
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
            // printf99_FN("Tono terminato (Pin %d)\n", b->pin);
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
        // printf0_FN("Buzzer detached.\n");
    }
}



// Funzione di esempio per suonare una semplice scala
void playScale() {
  int notes[] = {262, 294, 330, 349, 392, 440, 494, 523}; // Frequenze di Do, Re, Mi, Fa, Sol, La, Si, Do (ottava successiva)
  int noteDuration = 200; // Durata di ogni nota in ms

  for (int i = 0; i < 8; i++) {
    playTone(notes[i], noteDuration);
    delay(50); // Breve pausa tra le note
  }
  noTone();
}


// Funzione di esempio per suonare una scala in modo non bloccante
void playScaleNonBlocking() {
  static int notes[] = {262, 294, 330, 349, 392, 440, 494, 523}; // Frequenze di Do, Re, Mi, Fa, Sol, La, Si, Do
  static int noteDuration = 200; // Durata di ogni nota in ms
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