//
// updated by ...: Loreto Notarantonio
// Date .........: 15-09-2025 15.27.09
// ref:
// https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h> // in testa anche per le definizioni dei type


#define  LOG_MODULE_LEVEL LOG_LEVEL_INFO

#define __I_AM_MAIN_CPP__
#include    <lnLogger_Class.h>
#include    <lnSerialRead.h>
#include    <passiveBuzzer_Class.h>

#include    "pitches.h" // in testa anche per le definizioni dei type


#define passiveBuzzer_pin 22 // OUTPUT

// --- Esempio di utilizzo ---

// Frequenze per una scala Do Maggiore (approssimate)
uint16_t C_major_scale[] = {
    262, // C4
    294, // D4
    330, // E4
    349, // F4
    392, // G4
    440, // A4
    494, // B4
    523  // C5
};

uint16_t lnScale01[] = {
    100,
    110,
    120,
    130,
    140,
    150,
    160,
    170
};

uint16_t lnScale021[] = {
    392,
    660,
    392,
    660,
    392,
    660,
    392,
    660,
};

uint32_t duration1[] = {
    1000,
    167,
    167,
    167,
    1000,
    167,
    167,
    167,
};


uint8_t C_major_num_notes = sizeof(C_major_scale) / sizeof(C_major_scale[0]);

// Crea un'istanza del buzzer
#define LEDC_CHANNEL 0
// PassiveBuzzer_Class passiveBuzzer("passiveBz", passiveBuzzer_pin, LEDC_CHANNEL, 10); // Buzzer collegato al GPIO xx, canale LEDC 0, risoluzione 10 bit
PassiveBuzzer_Class passiveBuzzer;



void sirenaBitonale() {
    uint32_t duration[]   = {1000, 167, 167, 167, 1000, 167, 167, 167, 1000 };
    uint16_t tone[]       = { 392, 660, 392, 660,  392, 660, 392, 660, 392 };
    uint8_t nTones = sizeof(tone) / sizeof(tone[0]);

    for (uint8_t index=0; index < nTones; index++) {
        LOG_SPEC("[%d/%d] - freq: %7lu  duration: %lu", index, nTones, tone[index], duration[index] );
        passiveBuzzer.playFixedTone(tone[index], duration[index], true);
    }
}




void sirenaBitonale2() {
    uint32_t duration[]   = {1000, 167, 167, 167, 1000, 167, 167, 167, 1000 };
    uint16_t tone[]       = { 196, 330, 196, 330,  196, 330, 196, 330, 196 };
    uint8_t nTones = sizeof(tone) / sizeof(tone[0]);

    for (uint8_t index=0; index < nTones; index++) {
        LOG_SPEC("[%d/%d] - freq: %7lu  duration: %lu", index, nTones, tone[index], duration[index] );
        passiveBuzzer.playFixedTone(tone[index], duration[index], true);
    }
}



void setup() {
    Serial.begin(115200);
    delay(1000);
    lnLog.init();
    LOG_DEBUG("ESP32 Buzzer Struct with Scale Example (Fixed ledcSetFreq error)\n");

    passiveBuzzer.init("passiveBz", passiveBuzzer_pin, LEDC_CHANNEL, 10); // Buzzer collegato al GPIO xx, canale LEDC 0, risoluzione 10 bit
    // passiveBuzzer.begin(); // Inizializza il buzzer
}

void loop() {
    // Chiamare update() per ogni istanza del buzzer nel loop principale
    passiveBuzzer.update();

    static uint32_t lastActionTime = 0UL;
    static uint8_t nextStep = 0;
    const uint32_t PAUSE_BETWEEN_ACTIONS = 2000UL; // 2 secondi di pausa tra un'azione e l'altra

    // Esegue un'azione solo se il buzzer non sta suonando (né tono singolo né scala)
    // e il tempo di pausa è trascorso
    if (!passiveBuzzer.isPlayingSomething() && (millis() - lastActionTime >= PAUSE_BETWEEN_ACTIONS)) {
        switch (nextStep) {

            case 0:
                LOG_INFO("playFixedTone(784, 500)");
                passiveBuzzer.playFixedTone(784, 500); // Tono singolo Sol per 500ms
                nextStep++;
                break;

            case 1:
                LOG_INFO("playScale(C_major_scale, C_major_num_notes, 150, true)");
                passiveBuzzer.playScale(C_major_scale, C_major_num_notes, 150, true); // Scala ascendente, 150ms per nota
                nextStep++;
                break;

            case 2:
                LOG_INFO("playScale(C_major_scale, C_major_num_notes, 200, false)");
                passiveBuzzer.playScale(C_major_scale, C_major_num_notes, 200, false); // Scala discendente, 200ms per nota
                nextStep++;
                break;

            case 3:
                LOG_INFO("playToneDutyCycle(1000, 30.0, 600)");
                passiveBuzzer.playToneDutyCycle(1000, 30.0, 600); // Tono con DC 30% per 600ms
                nextStep++;
                break;

            case 4:
                // LOG_INFO("playScale(lnScale01, sizeof(lnScale01), 200, true)");
                // passiveBuzzer.playScale(lnScale01, sizeof(lnScale01), 200, true); // Scala discendente, 200ms per nota
                nextStep++;
                break;

            case 5:
                LOG_INFO("sirenaBitonale()");
                sirenaBitonale();
                nextStep++;
                break;

            case 6:
                LOG_INFO("sirenaBitonale2()");
                sirenaBitonale2();
                nextStep++;
                break;

            default:
                passiveBuzzer.myNoTone();
                nextStep = 0; // Ricomincia la sequenza
                LOG_DEBUG("Sequenza dimostrativa completata, riavvio.\n");
                waitForEnter();
                break;
            }
            lastActionTime = millis(); // Aggiorna il tempo dell'ultima azione avviata
            LOG_INFO("\n");

            delay(1000);


        }
}


#endif