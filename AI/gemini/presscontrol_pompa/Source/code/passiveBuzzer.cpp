//
// updated by ...: Loreto Notarantonio
// Date .........: 16-06-2025 13.08.48
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

// Funzione per suono a frequenze crescenti (accensione pompa)
void playAscendingTone() {
  int frequencies[] = {262, 294, 330, 349}; // C4, D4, E4, F4
  for (int freq : frequencies) {
    tone(passiveBuzzer_pin, freq);
    delay(100);
  }
  noTone(passiveBuzzer_pin);
}

// Funzione per suono a frequenze decrescenti (spegnimento pompa)
void playDescendingTone() {
  int frequencies[] = {349, 330, 294, 262}; // F4, E4, D4, C4
  for (int freq : frequencies) {
    tone(passiveBuzzer_pin, freq);
    delay(100);
  }
  noTone(passiveBuzzer_pin);
}


