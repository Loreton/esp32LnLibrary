//
// updated by ...: Loreto Notarantonio
// Date .........: 24-07-2025 20.18.18
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "driver/ledc.h"
#include "lnLogger.h"
#include "lnSetPinID.h"


#include "lnGlobalVars.h"
#include "PassiveBuzzer_Struct.h"

// Costruttore: chiamato quando crei un oggetto Buzzer
PassiveBuzzer_Struct::PassiveBuzzer_Struct(void) {}

// Costruttore: chiamato quando crei un oggetto Buzzer
void PassiveBuzzer_Struct::init(const char* pin_name, int buzzerPin, uint8_t active_level, int ledcChannel, int resBits) {
    m_pin              = buzzerPin;
    m_name             = pin_name;

    m_activeLevel      = active_level;
    m_on               = m_activeLevel;
    m_off              = !m_activeLevel;

    m_channel          = ledcChannel;
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


    setPinID(m_pinID, sizeof(m_pinID)-1, m_name,  m_pin);

    // Imposta il relè allo stato iniziale (spento)
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, m_off);

    // LOG_INFO("%s initialized. active level: %s", m_pinID,  m_pressedLogicLevel ? "HIGH"; "LOW");
    LOG_NOTIFY("[%s] initialized. active level: %s", m_pinID, str_pinLevel[m_activeLevel]);
}


  // Metodo per inizializzare il buzzer
void PassiveBuzzer_Struct::begin() {
    unsigned long actualFreq = ledcSetup(m_channel, 1, m_resolutionBits); // Frequenza iniziale > 0 altrimenti da errore

    if (actualFreq == 0) {
        LOG_INFO("%s [ERRORE] ledcSetup() FALLITO per canale %d. Controlla pin, canale e risoluzione.", m_channel);
        // Puoi aggiungere qui una logica per gestire l'errore, es. bloccare l'esecuzione
        while(true); // Ferma il programma in caso di errore critico
    }
    else {
        // Inizializza il canale con una frequenza di base (può essere 0 o una bassa frequenza)
        // e la risoluzione. La frequenza verrà poi impostata da playTone/playScale.
        // ledcSetup(channel, 1, resolutionBits);
        ledcAttachPin(m_pin, m_channel);
        noTone(); // Assicurati che sia spento all'inizio
        LOG_INFO("%s Channel: %d - Risoluzione: %d bit inizializzato.", m_pinID, m_channel, m_resolutionBits);
    }
}

  // --- Funzionalità 1: Suono con Duty Cycle Variabile ---
  // Imposta la frequenza e un duty cycle specifico (0-100%)
void PassiveBuzzer_Struct::playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration) {
    if (m_isPlaying || m_isPlayingScale) {
        LOG_INFO("%s occupato, impossibile avviare nuovo tono.", m_pinID);
        return;
    }

    int maxDuty = (1 << m_resolutionBits) - 1;
    int dutyValue = (int)(maxDuty * (dutyCyclePercent / 100.0));

    // Imposta la nuova frequenza e poi il duty cycle.
    // ledcSetup ricarica la frequenza e la risoluzione, ripartendo da capo.
    // E' il modo più affidabile per cambiare dinamicamente frequenza e duty cycle.
    ledcSetup(m_channel, frequency, m_resolutionBits);
    ledcWrite(m_channel, dutyValue);

    m_currentFrequency = frequency; // Aggiorna la frequenza corrente
    m_toneStartTime = millis();
    m_toneDuration = duration;
    m_isPlaying = true;
    LOG_INFO("%s Avvio tono DC freq: %d Hz - DC: %.2f%, Durata: %lu ms", m_pinID, m_currentFrequency, dutyCyclePercent, m_toneDuration);

}

  // --- Funzionalità 2: Suono Fisso di Durata Variabile (Duty Cycle 50%) ---
  // Usa ledcWriteTone() per un duty cycle automatico del 50%
void PassiveBuzzer_Struct::playToneFixed(int frequency, uint32_t duration) {
    if (m_isPlaying || m_isPlayingScale) {
        LOG_INFO("%s occupato, impossibile avviare nuovo tono.", m_pinID);
        return;
    }

    ledcWriteTone(m_channel, frequency); // Imposta frequenza, duty cycle 50% automatico
    m_currentFrequency = frequency; // Aggiorna la frequenza corrente

    m_toneStartTime = millis();
    m_toneDuration = duration;
    m_isPlaying = true;
    LOG_INFO("%s Avvio tono DC freq: %d Hz - Durata: %lu ms", m_pinID, m_currentFrequency, m_toneDuration);
}





  // Funzionalità 3: Suona una Scala (Non Bloccante)
void PassiveBuzzer_Struct::playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection) {
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
    m_noteDuration = singleNoteDuration;
    m_scaleDirectionUp = upDirection;
    m_isPlayingScale = true;

    // Inizializza la prima nota della scala
    if (m_scaleDirectionUp) {
        m_currentNoteIndex = 0;
    } else {
        m_currentNoteIndex = m_numNotes - 1;
    }

    // Inizia a suonare la prima nota
    ledcWriteTone(m_channel, m_scaleNotes[m_currentNoteIndex]);
    m_currentFrequency = m_scaleNotes[m_currentNoteIndex]; // Aggiorna la frequenza corrente
    m_noteStartTime = millis();
    LOG_INFO("%s Avvio scala %s", m_pinID, (m_scaleDirectionUp ? "Ascendente" : "Discendente"));
}

  // Metodo per fermare qualsiasi suono (singolo o scala)
void PassiveBuzzer_Struct::noTone() {
    ledcWrite(m_channel, 0); // Imposta il duty cycle a 0 per spegnere completamente
    // Non è necessario chiamare ledcSetFreq(channel, 0) qui,
    // dato che ledcSetup o ledcWriteTone lo faranno alla prossima chiamata.
    m_currentFrequency = 0; // Resetta la frequenza corrente
    m_isPlaying = false;
    m_isPlayingScale = false;
}





// Metodo da chiamare ripetutamente nel loop() per gestire la fine del tono o il progresso della scala
void PassiveBuzzer_Struct::update() {
    if (m_isPlaying) {
        if (millis() - m_toneStartTime >= m_toneDuration) {
            noTone();
            LOG_INFO("%s Tono singolo terminato", m_pinID);
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
              noTone();
                LOG_INFO("%s Scala terminata.", m_pinID);
            }
            else {
                  // Suona la prossima nota
                ledcWriteTone(m_channel, m_scaleNotes[m_currentNoteIndex]);
                m_currentFrequency = m_scaleNotes[m_currentNoteIndex]; // Aggiorna la frequenza corrente
                m_noteStartTime = millis(); // Resetta il tempo di inizio per la nuova nota
                LOG_DEBUG("%s nota %d", m_pinID, m_scaleNotes[m_currentNoteIndex]);
            }
        }
    }
}

  // Metodo per verificare se il buzzer sta suonando (sia tono singolo che scala)
bool PassiveBuzzer_Struct::isPlayingSomething() {
    return m_isPlaying || m_isPlayingScale;
}

