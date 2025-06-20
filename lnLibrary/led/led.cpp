//
// updated by ...: Loreto Notarantonio
// Date .........: 20-06-2025 08.08.25
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include "@SerialRead.h" // per waitForEnter()

#include "@led_structure.h"



void LED_init(LED_t *led, int ledPin, unsigned long period, uint8_t duty_cycle, bool active_level) {
    led->dutyCycle = constrain(duty_cycle/100.0, 0, 1); // important la divisione per 100.0 per avere un float point
    // duty_cycle = constrain(duty_cycle, 0.0, 1.0); // important il decimale x.0 per avere un float point
    unsigned long onTime  = period*led->dutyCycle;
    unsigned long offTime = period - onTime;
    LED_init(led, ledPin, onTime, offTime, active_level);
}

/**
 * @brief Inizializza un LED configurando il suo pin e le sue proprietà.
 * Questa funzione prepara la struttura LED_t per l'uso.
 * @param ledPtr Puntatore alla struttura LED_t da inizializzare.
 * @param ledPin Il pin GPIO a cui è collegato il LED.
 * @param onMs Il tempo di accensione in millisecondi.
 * @param offMs Il tempo di spegnimento in millisecondi.
 * @param inverseLog True se si usa logica inversa, False altrimenti.
 */
void LED_init(LED_t *led, int ledPin, unsigned long onTime, unsigned long offTime, bool active_level) {

    // Inizializza la struttura LED_t tramite il puntatore
    led->state          = false; // Inizialmente spento
    led->pin            = ledPin;
    led->activeLevel    = active_level;
    led->on             = active_level;
    led->off            = !active_level;
    led->onTime         = onTime;
    led->offTime        = offTime;
    led->previousMillis = 0;

    // Configura il pin come OUTPUT
    digitalWrite(led->pin, led->off);
    pinMode(ledPin, OUTPUT);

    Serial.printf("LED (Pin: %d) inizializzato. DC=%f, ON: %lu ms, OFF: %lu ms, active level: %s\n", led->pin, led->dutyCycle, led->onTime, led->offTime, led->activeLevel ? "ON" : "OFF");
    // waitForEnter("please 'ENTER' to continue");
}

/**
 * @brief Aggiorna lo stato di un singolo LED in modo non bloccante.
 * Questa funzione dovrebbe essere chiamata frequentemente per ogni LED nel loop principale.
 * @param ledPtr Puntatore alla struttura LED_t del LED da aggiornare.
 */
void LED_update(LED_t *led) {
    unsigned long currentMillis = millis(); // Ottiene il tempo corrente

    // Se il LED è attualmente ACCESO
    if (led->state) {
        // Controlla se è ora di SPEGNERE il LED
        if (currentMillis - led->previousMillis >= led->onTime) {
            led->state = false; // Aggiorna lo stato a spento
            // Scrive il valore sul pin tenendo conto della logica inversa
            // digitalWrite(led->pin, led->inverseLogic ? HIGH : LOW);
            digitalWrite(led->pin, led->off);
            led->previousMillis = currentMillis; // Aggiorna il tempo dell'ultimo cambio
            Serial.printf("LED (Pin: %d) SPENTO\n", led->pin); // Per debug
            // waitForEnter("please 'ENTER' to continue");
        }
    } else { // Se il LED è attualmente SPENTO
      // Controlla se è ora di ACCENDERE il LED
        if (currentMillis - led->previousMillis >= led->offTime) {
            led->state = true; // Aggiorna lo stato ad acceso
            digitalWrite(led->pin, led->on);
            led->previousMillis = currentMillis; // Aggiorna il tempo dell'ultimo cambio
            Serial.printf("LED (Pin: %d) ACCESO\n", led->pin); // Per debug
            // waitForEnter("please 'ENTER' to continue");
        }
    }
    // Serial.printf("LED (Pin: %d) state: %d\n", led->pin, led->state);
}

/**
 * @brief Imposta direttamente lo stato di un LED (ACCESO o SPENTO).
 * @param ledPtr Puntatore alla struttura LED_t del LED.
 * @param turnOn True per accendere il LED, False per spegnerlo.
 */
void LED_setState(LED_t *led, bool turnOn) {
    led->state = turnOn;
    digitalWrite(led->pin, led->activeLevel ? !turnOn : turnOn);
    led->previousMillis = millis(); // Resetta il timer per evitare salti inaspettati
    Serial.printf("LED (Pin: %d) forzato a %s\n", led->pin, turnOn ? "ACCESO" : "SPENTO");
    // waitForEnter("please 'ENTER' to continue");

}

// --- Funzioni del Modulo LED (lavorano su puntatori a LED_t) ---
/**
 * @brief Imposta un nuovo ciclo di duty cycle per un LED specifico.
 * @param ledPtr Puntatore alla struttura LED_t del LED.
 * @param newOnMs Il nuovo tempo di accensione in millisecondi.
 * @param newOffMs Il nuovo tempo di spegnimento in millisecondi.
 */
void LED_setDutyCycle(LED_t *led, unsigned long period, uint8_t duty_cycle) {
    led->dutyCycle = constrain(duty_cycle/100.0, 0, 1); // important la divisione per 100.0 per avere un float point
    // led->dutyCycle = constrain(duty_cycle, 0.0, 1.0); // important la divisione per 100.0 per avere un float point
    led->onTime = period*led->dutyCycle;
    led->offTime = period - led->onTime;

    // Resetta il timer per applicare immediatamente il nuovo ciclo
    led->previousMillis = millis();
    Serial.printf("LED (Pin: %d) duty cycle aggiornato: DC=%.2f ON=%lu ms, OFF=%lu ms\n", led->pin, led->dutyCycle, led->onTime, led->offTime);
    // waitForEnter("please 'ENTER' to continue");
}