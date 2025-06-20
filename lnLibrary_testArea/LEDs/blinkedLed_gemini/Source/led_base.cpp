//
// updated by ...: Loreto Notarantonio
// Date .........: 19-06-2025 16.14.14
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "@led_structure.h"


// --- Funzioni del Modulo LED (lavorano su puntatori a LedInfo) ---

/**
 * @brief Inizializza un LED configurando il suo pin e le sue proprietà.
 * Questa funzione prepara la struttura LedInfo per l'uso.
 * @param ledPtr Puntatore alla struttura LedInfo da inizializzare.
 * @param ledPin Il pin GPIO a cui è collegato il LED.
 * @param onMs Il tempo di accensione in millisecondi.
 * @param offMs Il tempo di spegnimento in millisecondi.
 * @param inverseLog True se si usa logica inversa, False altrimenti.
 */
void LED_init(LedInfo* ledPtr, int ledPin, unsigned long onMs, unsigned long offMs, bool active_level) {
    // Configura il pin come OUTPUT
    pinMode(ledPin, OUTPUT);

    // Inizializza la struttura LedInfo tramite il puntatore
    ledPtr->pin = ledPin;
    ledPtr->state = false; // Inizialmente spento
    ledPtr->previousMillis = 0;
    ledPtr->onTime = onMs;
    ledPtr->offTime = offMs;
    // ledPtr->inverseLogic = inverseLog;
    ledPtr->on = active_level;
    ledPtr->off = !active_level;

    // Imposta lo stato iniziale del pin (spento) in base alla logica
    // digitalWrite(ledPtr->pin, ledPtr->inverseLogic ? HIGH : LOW);
    digitalWrite(ledPtr->pin, ledPtr->off);

    Serial.printf("LED (Pin: %d) inizializzato. ON: %lu ms, OFF: %lu ms, active level: %s\n",
                  ledPtr->pin, onMs, offMs, ledPtr->on ? "ON" : "OFF");
}

/**
 * @brief Aggiorna lo stato di un singolo LED in modo non bloccante.
 * Questa funzione dovrebbe essere chiamata frequentemente per ogni LED nel loop principale.
 * @param ledPtr Puntatore alla struttura LedInfo del LED da aggiornare.
 */
void LED_update(LedInfo* ledPtr) {
    unsigned long currentMillis = millis(); // Ottiene il tempo corrente

    // Se il LED è attualmente ACCESO
    if (ledPtr->state) {
        // Controlla se è ora di SPEGNERE il LED
        if (currentMillis - ledPtr->previousMillis >= ledPtr->onTime) {
            ledPtr->state = false; // Aggiorna lo stato a spento
            // Scrive il valore sul pin tenendo conto della logica inversa
            // digitalWrite(ledPtr->pin, ledPtr->inverseLogic ? HIGH : LOW);
            digitalWrite(ledPtr->pin, ledPtr->on);
            ledPtr->previousMillis = currentMillis; // Aggiorna il tempo dell'ultimo cambio
            // Serial.printf("LED (Pin: %d) SPENTO\n", ledPtr->pin); // Per debug
        }
    } else { // Se il LED è attualmente SPENTO
      // Controlla se è ora di ACCENDERE il LED
        if (currentMillis - ledPtr->previousMillis >= ledPtr->offTime) {
            ledPtr->state = true; // Aggiorna lo stato ad acceso
            // Scrive il valore sul pin tenendo conto della logica inversa
            // digitalWrite(ledPtr->pin, ledPtr->inverseLogic ? LOW : HIGH);
            digitalWrite(ledPtr->pin, ledPtr->off);
            ledPtr->previousMillis = currentMillis; // Aggiorna il tempo dell'ultimo cambio
            // Serial.printf("LED (Pin: %d) ACCESO\n", ledPtr->pin); // Per debug
        }
    }
    Serial.printf("LED (Pin: %d) state: %d\n", ledPtr->pin, ledPtr->state);
}

/**
 * @brief Imposta direttamente lo stato di un LED (ACCESO o SPENTO).
 * @param ledPtr Puntatore alla struttura LedInfo del LED.
 * @param turnOn True per accendere il LED, False per spegnerlo.
 */
void LED_setState(LedInfo* ledPtr, bool turnOn) {
    ledPtr->state = turnOn;
    digitalWrite(ledPtr->pin, ledPtr->on ? !turnOn : turnOn);
    ledPtr->previousMillis = millis(); // Resetta il timer per evitare salti inaspettati
    Serial.printf("LED (Pin: %d) forzato a %s\n", ledPtr->pin, turnOn ? "ACCESO" : "SPENTO");
}

/**
 * @brief Imposta un nuovo ciclo di duty cycle per un LED specifico.
 * @param ledPtr Puntatore alla struttura LedInfo del LED.
 * @param newOnMs Il nuovo tempo di accensione in millisecondi.
 * @param newOffMs Il nuovo tempo di spegnimento in millisecondi.
 */
void LED_setDutyCycle(LedInfo* ledPtr, unsigned long newOnMs, unsigned long newOffMs) {
    ledPtr->onTime = newOnMs;
    ledPtr->offTime = newOffMs;
    // Resetta il timer per applicare immediatamente il nuovo ciclo
    ledPtr->previousMillis = millis();
    Serial.printf("LED (Pin: %d) duty cycle aggiornato: ON=%lu ms, OFF=%lu ms\n",
                  ledPtr->pin, newOnMs, newOffMs);
}
