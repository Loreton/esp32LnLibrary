//
// updated by ...: Loreto Notarantonio
// Date .........: 25-06-2025 17.10.44
//

// file: ledc_buzzer_Class.cpp
#include <Arduino.h>
#define LOG_LEVEL_0x // Consider reviewing these log levels if they are globally defined elsewhere
#define LOG_LEVEL_2
#define LOG_LEVEL_99x
#include "@globalVars.h" // Assuming this contains printf_XFN() and similar

#include "ledc_buzzer_Class.h" // Include the new class header

// Constructor: Called when you create a Buzzer object
passiveBuzzer_Class::passiveBuzzer_Class(const char* pin_name, int buzzerPin, int ledcChannel, int resBits) {
    pin_ = buzzerPin;
    name_ = pin_name;
    channel_ = ledcChannel;
    resolutionBits_ = resBits;
    currentFrequency_ = 0; // Initially no frequency

    toneStartTime_ = 0;
    toneDuration_ = 0;
    isPlaying_ = false;

    scaleNotes_ = nullptr;
    numNotes_ = 0;
    currentNoteIndex_ = 0;
    noteStartTime_ = 0;
    noteDuration_ = 0;
    isPlayingScale_ = false;
    scaleDirectionUp_ = true;

    int cx = snprintf(pinID_, PIN_ID_MAXLENGTH - 6, "[%s", name_);
    snprintf(pinID_ + cx, PIN_ID_MAXLENGTH - cx, ".%02d]:", pin_);
    // printf2_FN("Written %d/%d chars", cx, PIN_ID_LENGTH); // Serial not yet active
}

// Method to initialize the buzzer
void passiveBuzzer_Class::begin() {
    unsigned long actualFreq = ledcSetup(channel_, 1, resolutionBits_); // Initial frequency > 0, otherwise it errors

    if (actualFreq == 0) {
        printf2_FN("[ERRORE] ledcSetup() FALLITO per canale %d. Controlla pin, canale e risoluzione.\n", channel_);
        // You can add error handling logic here, e.g., halt execution
        while (true); // Halts the program in case of a critical error
    } else {
        // Initializes the channel with a base frequency (can be 0 or a low frequency)
        // and the resolution. The frequency will then be set by playTone/playScale.
        ledcAttachPin(pin_, channel_);
        noTone(); // Ensure it's off at the start
        printf2_FN("%s Channel: %d - Risoluzione: %d bit inizializzato.\n", pinID_, channel_, resolutionBits_);
    }
}

// --- Feature 1: Sound with Variable Duty Cycle ---
// Sets the frequency and a specific duty cycle (0-100%)
void passiveBuzzer_Class::playToneDutyCycle(int frequency, float dutyCyclePercent, uint32_t duration) {
    if (isPlaying_ || isPlayingScale_) {
        printf2_FN("%s occupato, impossibile avviare nuovo tono.\n", pinID_);
        return;
    }

    int maxDuty = (1 << resolutionBits_) - 1;
    int dutyValue = (int)(maxDuty * (dutyCyclePercent / 100.0));

    // Sets the new frequency and then the duty cycle.
    // ledcSetup reloads the frequency and resolution, starting over.
    // This is the most reliable way to dynamically change frequency and duty cycle.
    ledcSetup(channel_, frequency, resolutionBits_);
    ledcWrite(channel_, dutyValue);

    currentFrequency_ = frequency; // Update current frequency
    toneStartTime_ = millis();
    toneDuration_ = duration;
    isPlaying_ = true;
    printf2_FN("%s Avvio tono DC freq: %d Hz - DC: %.2f%%, Durata: %lu ms\n", pinID_, currentFrequency_, dutyCyclePercent, toneDuration_);
}

// --- Feature 2: Fixed Sound of Variable Duration (50% Duty Cycle) ---
// Uses ledcWriteTone() for an automatic 50% duty cycle
void passiveBuzzer_Class::playToneFixed(int frequency, uint32_t duration) {
    if (isPlaying_ || isPlayingScale_) {
        printf2_FN("%s occupato, impossibile avviare nuovo tono.\n", pinID_);
        return;
    }

    ledcWriteTone(channel_, frequency); // Sets frequency, automatic 50% duty cycle
    currentFrequency_ = frequency; // Update current frequency

    toneStartTime_ = millis();
    toneDuration_ = duration;
    isPlaying_ = true;
    printf2_FN("%s Avvio tono DC freq: %d Hz - Durata: %lu ms\n", pinID_, currentFrequency_, toneDuration_);
}

// Feature 3: Play a Scale (Non-Blocking)
void passiveBuzzer_Class::playScale(int noteFrequencies[], int numberOfNotes, uint32_t singleNoteDuration, bool upDirection) {
    if (isPlaying_ || isPlayingScale_) {
        printf2_FN("%s occupato, impossibile avviare la scala.\n", pinID_);
        return;
    }

    if (numberOfNotes == 0 || noteFrequencies == nullptr) {
        printf2_FN("%s suonare una scala vuota.\n", pinID_); // Corrected message, "occupato" was misleading here
        return;
    }

    scaleNotes_ = noteFrequencies;
    numNotes_ = numberOfNotes;
    noteDuration_ = singleNoteDuration;
    scaleDirectionUp_ = upDirection;
    isPlayingScale_ = true;

    // Initialize the first note of the scale
    if (scaleDirectionUp_) {
        currentNoteIndex_ = 0;
    } else {
        currentNoteIndex_ = numNotes_ - 1;
    }

    // Start playing the first note
    ledcWriteTone(channel_, scaleNotes_[currentNoteIndex_]);
    currentFrequency_ = scaleNotes_[currentNoteIndex_]; // Update current frequency
    noteStartTime_ = millis();
    printf2_FN("%s Avvio scala %s\n", pinID_, (scaleDirectionUp_ ? "Ascendente" : "Discendente"));
}

// Method to stop any sound (single tone or scale)
void passiveBuzzer_Class::noTone() {
    ledcWrite(channel_, 0); // Set duty cycle to 0 to turn off completely
    currentFrequency_ = 0; // Reset current frequency
    isPlaying_ = false;
    isPlayingScale_ = false;
}

// Method to be called repeatedly in loop() to handle tone end or scale progress
void passiveBuzzer_Class::handle() {
    if (isPlaying_) {
        if (millis() - toneStartTime_ >= toneDuration_) {
            noTone();
            printf2_FN("%s Tono singolo terminato.\n", pinID_);
        }
    } else if (isPlayingScale_) {
        if (millis() - noteStartTime_ >= noteDuration_) {
            // Time to change note
            if (scaleDirectionUp_) {
                currentNoteIndex_++;
            } else {
                currentNoteIndex_--;
            }

            // Check if the scale is finished
            if ((scaleDirectionUp_ && currentNoteIndex_ >= numNotes_) || (!scaleDirectionUp_ && currentNoteIndex_ < 0)) {
                noTone();
                printf2_FN("%s Scala terminata.\n", pinID_);
            } else {
                // Play the next note
                ledcWriteTone(channel_, scaleNotes_[currentNoteIndex_]);
                currentFrequency_ = scaleNotes_[currentNoteIndex_]; // Update current frequency
                noteStartTime_ = millis(); // Reset start time for the new note
                printf99_FN("%s nota %d.\n", pinID_, scaleNotes_[currentNoteIndex_]);
            }
        }
    }
}

// Method to check if the buzzer is playing something (either single tone or scale)
bool passiveBuzzer_Class::isPlayingSomething() {
    return isPlaying_ || isPlayingScale_;
}