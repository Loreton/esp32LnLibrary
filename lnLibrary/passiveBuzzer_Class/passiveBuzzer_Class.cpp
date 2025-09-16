//
// updated by ...: Loreto Notarantonio
// Date .........: 15-09-2025 14.38.57
//

#include <Arduino.h> // in testa anche per le definizioni dei type
#include "driver/ledc.h"

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_TRACE
#include <lnGlobalVars.h> // Assuming these are defined elsewhere
#include <lnLogger_Class.h> // Assuming these are defined elsewhere
#include <lnSetPinID.h>   // Assuming this is defined elsewhere
#include "passiveBuzzer_Class.h"




PassiveBuzzer_Class::PassiveBuzzer_Class(void) {};

//############################################################
// Constructor: Called when you create a PassiveBuzzer_Class object
//############################################################
void PassiveBuzzer_Class::init(const char* pin_name, int buzzerPin, uint8_t active_level, int ledcChannel, int resBits) {
    m_pin              = buzzerPin;
    m_name             = pin_name;

    m_activeLevel      = active_level;
    m_on               = m_activeLevel;
    m_off              = !m_activeLevel;

    m_tonePwmChannel   = ledcChannel;
    m_resolutionBits   = resBits;
    m_currentFrequency = 0; // Inizialmente nessuna frequenza

    m_toneStartTime    = 0;
    m_toneDuration     = 0;
    m_isPlaying        = false;

    m_scaleNotes       = nullptr;
    m_numNotes         = 0;
    m_currentNoteIndex = 0;
    m_noteStartTime    = 0;
    m_noteDuration     = 0;
    m_isPlayingScale   = false;
    m_scaleDirectionUp = true;

    setPinID(m_pinID, sizeof(m_pinID) - 1, m_name, m_pin);

    // Imposta il relè allo stato iniziale (spento)
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, m_off);

    LOG_TRACE("[%s] initialized. active level: %s", m_pinID, str_pinLevel[m_activeLevel]);
    // LOG_TRACE("[%s] initialized. active level: %d", m_pinID, m_activeLevel);
    begin();
}

// Destructor (example, uncomment if needed for resource cleanup)
// PassiveBuzzer_Class::~PassiveBuzzer_Class() {
//     // If you allocated m_scaleNotes dynamically with 'new', you would 'delete[]' it here.
//     // Since it's currently assigned from an external array, no deletion is needed here.
// }

// ###############################################################
// # Metodo per inizializzare il buzzer
// ###############################################################
void PassiveBuzzer_Class::begin() {
    uint32_t actualFreq = ledcSetup(m_tonePwmChannel, 1, m_resolutionBits); // Frequenza iniziale > 0 altrimenti da errore

    if (actualFreq == 0) {
        LOG_ERROR("%s [ERRORE] ledcSetup() FALLITO per canale %d. Controlla pin, canale e risoluzione.", m_tonePwmChannel);
        // Puoi aggiungere qui una logica per gestire l'errore, es. bloccare l'esecuzione
        while (true); // Ferma il programma in caso di errore critico
    }
    else {
        // Inizializza il canale con una frequenza di base (può essere 0 o una bassa frequenza)
        // e la risoluzione. La frequenza verrà poi impostata da playTone/playScale.
        ledcAttachPin(m_pin, m_tonePwmChannel);
        // ledcDetachPin(pin_nr); // se dovesse servire

        myNoTone(); // Assicurati che sia spento all'inizio
        LOG_TRACE("%s Channel: %d - Risoluzione: %d bit inizializzato.", m_pinID, m_tonePwmChannel, m_resolutionBits);
    }
}





//############################################################
// --- Funzionalità 1: Suono con Duty Cycle Variabile ---
// Imposta la frequenza e un duty cycle specifico (0-100%)
//############################################################
void PassiveBuzzer_Class::playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration) {
    if (m_isPlaying || m_isPlayingScale) {
        LOG_INFO("%s occupato, impossibile avviare nuovo tono.", m_pinID);
        return;
    }

    int maxDuty = (1 << m_resolutionBits) - 1;
    int dutyValue = (int)(maxDuty * (dutyCyclePercent / 100.0));

    // Imposta la nuova frequenza e poi il duty cycle.
    // ledcSetup ricarica la frequenza e la risoluzione, ripartendo da capo.
    // E' il modo più affidabile per cambiare dinamicamente frequenza e duty cycle.
    ledcSetup(m_tonePwmChannel, frequency, m_resolutionBits);
    ledcWrite(m_tonePwmChannel, dutyValue);

    m_currentFrequency = frequency; // Aggiorna la frequenza corrente
    m_toneStartTime = millis();
    m_toneDuration = duration;
    m_isPlaying = true;
    LOG_INFO("%s Avvio tono DC freq: %d Hz - DC: %.2f%, Durata: %lu ms", m_pinID, m_currentFrequency, dutyCyclePercent, m_toneDuration);
}






//############################################################
// --- Funzionalità 2: Suono Fisso di Durata Variabile (Duty Cycle 50%) ---
// Usa ledcWriteTone() per un duty cycle automatico del 50%
//############################################################
void PassiveBuzzer_Class::playFixedTone(uint16_t frequency, uint32_t duration, bool waitForEnding) {
    if (m_isPlaying || m_isPlayingScale) {
        LOG_INFO("%s occupato, impossibile avviare nuovo tono.", m_pinID);
        return;
    }

    ledcWriteTone(m_tonePwmChannel, frequency); // esp32-hal-ledc.h double ledcWriteTone(uint8_t chan, double freq)
    m_currentFrequency = frequency; // Aggiorna la frequenza corrente

    m_toneStartTime = millis();
    m_toneDuration = duration;
    m_isPlaying = true;
    LOG_DEBUG("%s Avvio tono DC freq: %d Hz - Durata: %lu ms", m_pinID, m_currentFrequency, m_toneDuration);
    if (waitForEnding) {
        waitForPulseEnding(duration*2);
    }
}



//############################################################
// Funzionalità 3: Suona una Scala (Non Bloccante)
//############################################################
void PassiveBuzzer_Class::playScale(uint16_t noteFrequencies[], uint8_t numberOfNotes, uint32_t singleNoteDuration, bool upDirection) {
    if (m_isPlaying || m_isPlayingScale) {
        LOG_INFO("%s occupato, impossibile avviare la scala.", m_pinID);
        return;
    }

    if (numberOfNotes == 0 || noteFrequencies == nullptr) {
        LOG_INFO("%s occupato, suonare una scala vuota.", m_pinID);
        return;
    }

    m_scaleNotes = noteFrequencies;
    m_numNotes = numberOfNotes;
    m_noteDuration = singleNoteDuration; // viene controllata nell'update
    m_scaleDirectionUp = upDirection;
    m_isPlayingScale = true;

    // Inizializza la prima nota della scala
    if (m_scaleDirectionUp) {
        m_currentNoteIndex = 0;
    } else {
        m_currentNoteIndex = m_numNotes - 1;
    }

    LOG_INFO("%s Avvio scala %s", m_pinID, (m_scaleDirectionUp ? "Ascendente" : "Discendente"));
    // ledcAttachPin(m_pin, m_tonePwmChannel);

    // Inizia a suonare la prima nota
    ledcWriteTone(m_tonePwmChannel, m_scaleNotes[m_currentNoteIndex]);
    m_currentFrequency = m_scaleNotes[m_currentNoteIndex]; // Aggiorna la frequenza corrente
    m_noteStartTime = millis();
}



//############################################################
// Funzionalità 3: Suona una Scala (Non Bloccante)
//############################################################
// void PassiveBuzzer_Class::playScale2(uint16_t noteFrequencies[], uint8_t numberOfNotes, uint32_t singleNoteDuration[], bool upDirection, bool waitForEnding) {
//     if (m_isPlaying || m_isPlayingScale) {
//         LOG_INFO("%s occupato, impossibile avviare la scala.", m_pinID);
//         return;
//     }

//     if (numberOfNotes == 0 || noteFrequencies == nullptr) {
//         LOG_INFO("%s occupato, suonare una scala vuota.", m_pinID);
//         return;
//     }

//     m_scaleNotes = noteFrequencies;
//     m_numNotes = numberOfNotes;
//     m_scaleDirectionUp = upDirection;
//     m_isPlayingScale = true;

//     // Inizializza la prima nota della scala
//     if (m_scaleDirectionUp) {
//         m_currentNoteIndex = 0;
//     } else {
//         m_currentNoteIndex = m_numNotes - 1;
//     }

//     LOG_INFO("%s Avvio scala %s", m_pinID, (m_scaleDirectionUp ? "Ascendente" : "Discendente"));
//     // ledcAttachPin(m_pin, m_tonePwmChannel);

//     // Inizia a suonare la prima nota
//     m_noteDuration = singleNoteDuration[m_currentNoteIndex]; // viene controllata nell'update
//     ledcWriteTone(m_tonePwmChannel, m_scaleNotes[m_currentNoteIndex]);
//     m_currentFrequency = m_scaleNotes[m_currentNoteIndex]; // Aggiorna la frequenza corrente
//     m_noteStartTime = millis();
// }



//############################################################
// Metodo per fermare qualsiasi suono (singolo o scala)
//############################################################
void PassiveBuzzer_Class::myNoTone() {
    ledcWrite(m_tonePwmChannel, 0); // Imposta il duty cycle a 0 per spegnere completamente
    m_currentFrequency = 0;  // Resetta la frequenza corrente
    m_isPlaying = false;
    m_isPlayingScale = false;
    // ledcDetachPin(m_pin); // detach pin
}





//############################################################
// Metodo da chiamare ripetutamente nel loop() per gestire la fine del tono o il progresso della scala
//############################################################
void PassiveBuzzer_Class::update() {
    if (m_isPlaying) {
        if (millis() - m_toneStartTime >= m_toneDuration) {
            myNoTone();
            LOG_DEBUG("%s Tono singolo terminato", m_pinID);
        }
    }
    else if (m_isPlayingScale) {
        if (millis() - m_noteStartTime >= m_noteDuration) {
            // Tempo per cambiare nota
            if (m_scaleDirectionUp) {
                m_currentNoteIndex++;
            } else {
                m_currentNoteIndex--;
            }

            // Controlla se la scala è finita
            if ((m_scaleDirectionUp && m_currentNoteIndex >= m_numNotes) || (!m_scaleDirectionUp && m_currentNoteIndex < 0)) {
                myNoTone();
                LOG_INFO("%s Scala terminata.", m_pinID);
            }
            else {
                // ledcAttachPin(m_pin, m_tonePwmChannel);

                // Suona la prossima nota
                ledcWriteTone(m_tonePwmChannel, m_scaleNotes[m_currentNoteIndex]);
                m_currentFrequency = m_scaleNotes[m_currentNoteIndex]; // Aggiorna la frequenza corrente
                m_noteStartTime = millis(); // Resetta il tempo di inizio per la nuova nota
                LOG_DEBUG("%s nota %d", m_pinID, m_scaleNotes[m_currentNoteIndex]);
            }
        }
    }
}




//############################################################
// Metodo per verificare se il buzzer sta suonando (sia tono singolo che scala)
//############################################################
bool PassiveBuzzer_Class::isPlayingSomething() {
    return m_isPlaying || m_isPlayingScale;
}


// #############################################################
// # il timeout non è vincolante, se il suono finisce prima si esce...
// #############################################################
void PassiveBuzzer_Class::waitForPulseEnding(int32_t timeOut) {
    while (isPlayingSomething() && timeOut > 0) {
        delay(10);
        timeOut -= 10;
        update();
        LOG_TRACE("\t[%s] waiting for the end of pulse", m_pinID);
    }
}
