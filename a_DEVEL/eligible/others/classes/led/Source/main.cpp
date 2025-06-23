//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 19.15.44
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type
#include "led_class.h"


// Definizione della classe LED
Led led1(26); // Crea un'istanza della classe LED, collegata al pin 26

void setup() {
    // Inizializzazione della porta seriale per il debug
    Serial.begin(115200);
    delay(1000);
    Serial.println("Inizio programma");
}

void loop() {
    // Aziona il LED
    // led1.accendi();
    // delay(1000);
    // led1.spegni();
    // delay(1000);
    led1.inverti();
    delay(1000);

    // Stampa lo stato del LED
    Serial.print("Stato del LED: ");
    Serial.println(led1.isAcceso() ? "Acceso" : "Spento");
    delay(1000);
}