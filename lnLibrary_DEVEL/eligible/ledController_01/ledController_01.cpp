//
// updated by ...: Loreto Notarantonio
// Date .........: 22-06-2025 17.51.48
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
// #include "@SerialRead.h" // per waitForEnter()



#include "@ledController_01.h"



void led_sClass::init(int ledPin, unsigned long period, uint8_t duty_cycle, bool active_level) {
    _dutyCycle = constrain(duty_cycle/100.0, 0, 1); // important la divisione per 100.0 per avere un float point
    // duty_cycle = constrain(duty_cycle, 0.0, 1.0); // important il decimale x.0 per avere un float point
    unsigned long onTime  = period*_dutyCycle;
    unsigned long offTime = period - onTime;
    led_sClass::init(led, ledPin, onTime, offTime, active_level);
}

/**
 * @brief Inizializza un LED configurando il suo pin e le sue proprietà.
 * Questa funzione prepara la struttura led_sClass::t per l'uso.
 * @param ledPtr Puntatore alla struttura led_sClass::t da inizializzare.
 * @param ledPin Il pin GPIO a cui è collegato il LED.
 * @param onMs Il tempo di accensione in millisecondi.
 * @param offMs Il tempo di spegnimento in millisecondi.
 * @param inverseLog True se si usa logica inversa, False altrimenti.
 */
void led_sClass::init(int ledPin, unsigned long onTime, unsigned long offTime, bool active_level) {

    // Inizializza la struttura led_sClass::t tramite il puntatore
    _state          = false; // Inizialmente spento
    _pin            = ledPin;
    _invertedLogic  = active_level;
    _on             = active_level;
    _off            = !active_level;
    _onTime         = onTime;
    _offTime        = offTime;
    _previousMillis = 0;

    // Configura il pin come OUTPUT
    digitalWrite(_pin, _off);
    pinMode(ledPin, OUTPUT);

    Serial.printf("LED (Pin: %d) inizializzato. DC=%f, ON: %lu ms, OFF: %lu ms, active level: %s\n", _pin, _dutyCycle, _onTime, _offTime, _invertedLogic ? "ON" : "OFF");
    // waitForEnter("please 'ENTER' to continue");
}

/**
 * @brief Aggiorna lo stato di un singolo LED in modo non bloccante.
 * Questa funzione dovrebbe essere chiamata frequentemente per ogni LED nel loop principale.
 */
void led_sClass::update() {
    unsigned long currentMillis = millis(); // Ottiene il tempo corrente

    // Se il LED è attualmente ACCESO
    if (_state) {
        // Controlla se è ora di SPEGNERE il LED
        if (currentMillis - _previousMillis >= _onTime) {
            _state = false; // Aggiorna lo stato a spento
            // Scrive il valore sul pin tenendo conto della logica inversa
            digitalWrite(_pin, _off);
            _previousMillis = currentMillis; // Aggiorna il tempo dell'ultimo cambio
            Serial.printf("LED (Pin: %d) SPENTO\n", _pin); // Per debug
        }
    } else { // Se il LED è attualmente SPENTO
      // Controlla se è ora di ACCENDERE il LED
        if (currentMillis - _previousMillis >= _offTime) {
            _state = true; // Aggiorna lo stato ad acceso
            digitalWrite(_pin, _on);
            _previousMillis = currentMillis; // Aggiorna il tempo dell'ultimo cambio
            Serial.printf("LED (Pin: %d) ACCESO\n", _pin); // Per debug
        }
    }
}

/**
 * @brief Imposta direttamente lo stato di un LED (ACCESO o SPENTO).
 * @param turnOn True per accendere il LED, False per spegnerlo.
 */
void led_sClass::setState(bool turnOn) {
    _state = turnOn;
    digitalWrite(_pin, _invertedLogic ? !turnOn : turnOn);
    _previousMillis = millis(); // Resetta il timer per evitare salti inaspettati
    Serial.printf("LED (Pin: %d) forzato a %s\n", _pin, turnOn ? "ON" : "OFF");

}

/**
 * @brief Imposta direttamente lo stato di un LED (ACCESO o SPENTO).
 * @param turnOn True per accendere il LED, False per spegnerlo.
 */
void led_sClass::turnOnFor(uint32_t msec) {
    digitalWrite(_pin, _on);
    _onTime         = msec;
    _offTime        = 0;
    _previousMillis = millis(); // Resetta il timer per evitare salti inaspettati
    Serial.printf("LED (Pin: %d) forzato a %s\n", _pin, _state ? "ON" : "OFF");
}


/**
 * @brief Imposta direttamente lo stato di un LED (ACCESO o SPENTO).
 * @param turnOn True per accendere il LED, False per spegnerlo.
 */
bool led_sClass::readState() {
    _state = digitalRead(_pin);
    if (digitalRead(_pin) == _invertedLogic) {
        _state = _invertedLogic;
    } else {
        _state = !_invertedLogic;
    }
    Serial.printf("LED (Pin: %d) is: %s\n", _pin, _state ? "ON" : "OFF");
}

// --- Funzioni del Modulo LED (lavorano su puntatori a led_sClass::t) ---
/**
 * @brief Imposta un nuovo ciclo di duty cycle per un LED specifico.
 */
void led_sClass::setDutyCycle(unsigned long period, uint8_t duty_cycle) {
    _dutyCycle = constrain(duty_cycle/100.0, 0, 1); // important la divisione per 100.0 per avere un float point
    _onTime = period*_dutyCycle;
    _offTime = period - _onTime;

    // Resetta il timer per applicare immediatamente il nuovo ciclo
    _previousMillis = millis();
    Serial.printf("LED (Pin: %d) duty cycle aggiornato: DC=%.2f ON=%lu ms, OFF=%lu ms\n", _pin, _dutyCycle, _onTime, _offTime);
}