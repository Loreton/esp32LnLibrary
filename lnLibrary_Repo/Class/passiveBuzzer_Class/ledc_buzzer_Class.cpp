//
// updated by ...: Loreto Notarantonio
// Date .........: 09-07-2025 13.39.54
//

// file: ledc_buzzer_Class.cpp
#include <Arduino.h>

#include "lnLogger.h" // Assuming this contains printf_XFN() and similar
#include "lnGlobalVars.h" // Assuming this contains printf_XFN() and similar

#include "ledc_buzzer_Class.h" // Include the new class header

// Constructor: Called when you create a Buzzer object
passiveBuzzer_Class::passiveBuzzer_Class(const char* pin_name, int buzzerPin, int ledcChannel, int resBits) {
    m_pin = buzzerPin;
    m_name = pin_name;
    m_channel = ledcChannel;
    m_resolutionBits = resBits;
    m_currentFrequency = 0; // Initially no frequency

    m_toneStartTime = 0;
    m_toneDuration = 0;
    m_isPlaying = false;

    m_scaleNotes = nullptr;
    m_numNotes = 0;
    m_currentNoteIndex = 0;
    m_noteStartTime = 0;
    m_noteDuration = 0;
    m_isPlayingScale = false;
    m_scaleDirectionUp = true;


    size_t PIN_ID_MAXLENGTH = sizeof(m_pinID)-1;
    int cx = snprintf(m_pinID, PIN_ID_MAXLENGTH - 6, "[%s", m_name);
    snprintf(m_pinID + cx, PIN_ID_MAXLENGTH - cx, ".%02d]:", m_pin);
}

// Method to initialize the buzzer
void passiveBuzzer_Class::begin() {
    unsigned long actualFreq = ledcSetup(m_channel, 1, m_resolutionBits); // Initial frequency > 0, otherwise it errors

    if (actualFreq == 0) {
        LOG_INFO("[ERRORE] ledcSetup() FALLITO per canale %d. Controlla pin, canale e risoluzione.\n", m_channel);
        // You can add error handling logic here, e.g., halt execution
        while (true); // Halts the program in case of a critical error
    } else {
        // Initializes the channel with a base frequency (can be 0 or a low frequency)
        // and the resolution. The frequency will then be set by playTone/playScale.
        ledcAttachPin(m_pin, m_channel);
        noTone(); // Ensure it's off at the start
        LOG_INFO("%s Channel: %d - Risoluzione: %d bit inizializzato.\n", m_pinID, m_channel, m_resolutionBits);
    }
}

// --- Feature 1: Sound with Variable Duty Cycle ---
// Sets the frequency and a specific duty cycle (0-100%)
void passiveBuzzer_Class::playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration) {
    if (m_isPlaying || m_isPlayingScale) {
        LOG_INFO("%s occupato, impossibile avviare nuovo tono.\n", m_pinID);
        return;
    }

    int maxDuty = (1 << m_resolutionBits) - 1;
    int dutyValue = (int)(maxDuty * (dutyCyclePercent / 100.0));

    // Sets the new frequency and then the duty cycle.
    // ledcSetup reloads the frequency and resolution, starting over.
    // This is the most reliable way to dynamically change frequency and duty cycle.
    ledcSetup(m_channel, frequency, m_resolutionBits);
    ledcWrite(m_channel, dutyValue);

    m_currentFrequency = frequency; // Update current frequency
    m_toneStartTime = millis();
    m_toneDuration = duration;
    m_isPlaying = true;
    LOG_INFO("%s Avvio tono DC freq: %d Hz - DC: %.2f%%, Durata: %lu ms\n", m_pinID, m_currentFrequency, dutyCyclePercent, m_toneDuration);
}

// --- Feature 2: Fixed Sound of Variable Duration (50% Duty Cycle) ---
// Uses ledcWriteTone() for an automatic 50% duty cycle
void passiveBuzzer_Class::playToneFixed(int frequency, uint32_t duration) {
    if (m_isPlaying || m_isPlayingScale) {
        LOG_INFO("%s occupato, impossibile avviare nuovo tono.\n", m_pinID);
        return;
    }

    ledcWriteTone(m_channel, frequency); // Sets frequency, automatic 50% duty cycle
    m_currentFrequency = frequency; // Update current frequency

    m_toneStartTime = millis();
    m_toneDuration = duration;
    m_isPlaying = true;
    LOG_INFO("%s Avvio tono DC freq: %d Hz - Durata: %lu ms\n", m_pinID, m_currentFrequency, m_toneDuration);
}

// Feature 3: Play a Scale (Non-Blocking)
void passiveBuzzer_Class::playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection) {
    if (m_isPlaying || m_isPlayingScale) {
        LOG_INFO("%s occupato, impossibile avviare la scala.\n", m_pinID);
        return;
    }

    if (numberOfNotes == 0 || noteFrequencies == nullptr) {
        LOG_INFO("%s suonare una scala vuota.\n", m_pinID); // Corrected message, "occupato" was misleading here
        return;
    }

    m_scaleNotes = noteFrequencies;
    m_numNotes = numberOfNotes;
    m_noteDuration = singleNoteDuration;
    m_scaleDirectionUp = upDirection;
    m_isPlayingScale = true;

    // Initialize the first note of the scale
    if (m_scaleDirectionUp) {
        m_currentNoteIndex = 0;
    } else {
        m_currentNoteIndex = m_numNotes - 1;
    }

    // Start playing the first note
    ledcWriteTone(m_channel, m_scaleNotes[m_currentNoteIndex]);
    m_currentFrequency = m_scaleNotes[m_currentNoteIndex]; // Update current frequency
    m_noteStartTime = millis();
    LOG_INFO("%s Avvio scala %s\n", m_pinID, (m_scaleDirectionUp ? "Ascendente" : "Discendente"));
}

// Method to stop any sound (single tone or scale)
void passiveBuzzer_Class::noTone() {
    ledcWrite(m_channel, 0); // Set duty cycle to 0 to turn off completely
    m_currentFrequency = 0; // Reset current frequency
    m_isPlaying = false;
    m_isPlayingScale = false;
}

// Method to be called repeatedly in loop() to handle tone end or scale progress
void passiveBuzzer_Class::handle() {
    if (m_isPlaying) {
        if (millis() - m_toneStartTime >= m_toneDuration) {
            noTone();
            LOG_INFO("%s Tono singolo terminato.\n", m_pinID);
        }
    } else if (m_isPlayingScale) {
        if (millis() - m_noteStartTime >= m_noteDuration) {
            // Time to change note
            if (m_scaleDirectionUp) {
                m_currentNoteIndex++;
            } else {
                m_currentNoteIndex--;
            }

            // Check if the scale is finished
            if ((m_scaleDirectionUp && m_currentNoteIndex >= m_numNotes) || (!m_scaleDirectionUp && m_currentNoteIndex < 0)) {
                noTone();
                LOG_INFO("%s Scala terminata.\n", m_pinID);
            } else {
                // Play the next note
                ledcWriteTone(m_channel, m_scaleNotes[m_currentNoteIndex]);
                m_currentFrequency = m_scaleNotes[m_currentNoteIndex]; // Update current frequency
                m_noteStartTime = millis(); // Reset start time for the new note
                LOG_DEBUG("%s nota %d.\n", m_pinID, m_scaleNotes[m_currentNoteIndex]);
            }
        }
    }
}

// Method to check if the buzzer is playing something (either single tone or scale)
bool passiveBuzzer_Class::isPlayingSomething() {
    return m_isPlaying || m_isPlayingScale;
}