//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 14.17.25
// ref:
// https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h> // in testa anche per le definizioni dei type




#define __I_AM_MAIN_CPP__
#include    "lnGlobalVars.h"

#include    "lnSerialRead.h"
#include    "PassiveBuzzer_Struct.h"


#define passiveBuzzer_pin 22 // OUTPUT

// --- Esempio di utilizzo ---

// Frequenze per una scala Do Maggiore (approssimate)
int C_major_scale[] = {
    262, // C4
    294, // D4
    330, // E4
    349, // F4
    392, // G4
    440, // A4
    494, // B4
    523  // C5
};
int num_notes_C_major = sizeof(C_major_scale) / sizeof(C_major_scale[0]);

// Crea un'istanza del buzzer
#define LEDC_CHANNEL 0
// PassiveBuzzer_Struct passiveBuzzer("passiveBz", passiveBuzzer_pin, LEDC_CHANNEL, 10); // Buzzer collegato al GPIO xx, canale LEDC 0, risoluzione 10 bit
PassiveBuzzer_Struct passiveBuzzer;

void setup() {
    Serial.begin(115200);
    delay(1000);
    myLog.init();
    LOG_DEBUG("ESP32 Buzzer Struct with Scale Example (Fixed ledcSetFreq error)\n");
    passiveBuzzer.init("passiveBz", passiveBuzzer_pin, LEDC_CHANNEL, 10); // Buzzer collegato al GPIO xx, canale LEDC 0, risoluzione 10 bit
    // passiveBuzzer.begin(); // Inizializza il buzzer
}

void loop() {
    // Chiamare update() per ogni istanza del buzzer nel loop principale
    passiveBuzzer.update();

    static unsigned long lastActionTime = 0;
    static int step = 0;
    const unsigned long PAUSE_BETWEEN_ACTIONS = 2000; // 2 secondi di pausa tra un'azione e l'altra

    // Esegue un'azione solo se il buzzer non sta suonando (né tono singolo né scala)
    // e il tempo di pausa è trascorso
    if (!passiveBuzzer.isPlayingSomething() && (millis() - lastActionTime >= PAUSE_BETWEEN_ACTIONS)) {
        switch (step) {
        case 0:
            passiveBuzzer.playToneFixed(784, 500); // Tono singolo Sol per 500ms
            step = 1;
            break;
        case 1:
            passiveBuzzer.playScale(C_major_scale, num_notes_C_major, 150, true); // Scala ascendente, 150ms per nota
            step = 2;
            break;
        case 2:
            passiveBuzzer.playScale(C_major_scale, num_notes_C_major, 200, false); // Scala discendente, 200ms per nota
            step = 3;
            break;
        case 3:
            passiveBuzzer.playToneDutyCycle(1000, 30.0, 600); // Tono con DC 30% per 600ms
            step = 4;
            break;
        default:
            passiveBuzzer.noTone();
            step = 0; // Ricomincia la sequenza
            LOG_DEBUG("Sequenza dimostrativa completata, riavvio.\n");
            waitForEnter();
            break;
        }
        lastActionTime = millis(); // Aggiorna il tempo dell'ultima azione avviata
    }
}


#endif