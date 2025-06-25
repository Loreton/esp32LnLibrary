#include <Arduino.cpp>

// Definizione del pin a cui è collegato il buzzer
const int buzzerPin = 27; // Puoi scegliere un altro pin GPIO se preferisci

// Canale LEDC da utilizzare per il buzzer (0-15)
const int ledcChannel = 0;

// Risoluzione in bit del duty cycle (10 bit = 0-1023)
const int resolution = 10;

// Funzione per suonare un tono per una certa durata
void playTone(int frequency, int duration) {
  // Imposta la frequenza per il canale LEDC
  ledcWriteTone(ledcChannel, frequency);
  delay(duration);
}

// Funzione per fermare il suono
void noTone() {
  ledcWriteTone(ledcChannel, 0); // Imposta la frequenza a 0 per fermare l'oscillazione
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

void setupBuzzer() {

}


void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Buzzer Tone Example");

  // Configura il canale LEDC
  // ledcSetup(channel, freq, resolution_bits);
  // La frequenza iniziale qui non è critica, verrà impostata in ledcWriteTone
  ledcSetup(ledcChannel, 1000, resolution);

  // Collega il canale LEDC al pin GPIO
  ledcAttachPin(buzzerPin, ledcChannel);
}

void loop() {
  Serial.println("Suonando un La (440 Hz)");
  playTone(440, 500); // Frequenza: 440 Hz (La), Durata: 500 ms
  delay(1000);

  Serial.println("Suonando un Do (523 Hz)");
  playTone(523, 500); // Frequenza: 523 Hz (Do), Durata: 500 ms
  delay(1000);

  Serial.println("Suonando un Mi (659 Hz)");
  playTone(659, 500); // Frequenza: 659 Hz (Mi), Durata: 500 ms
  delay(1000);

  Serial.println("Suonando un Sol (784 Hz)");
  playTone(784, 500); // Frequenza: 784 Hz (Sol), Durata: 500 ms
  delay(1000);

  Serial.println("Nessun suono per 1 secondo");
  noTone(); // Ferma il suono
  delay(1000);

  Serial.println("Suonando una scala");
  playScale();
  delay(2000);
}

