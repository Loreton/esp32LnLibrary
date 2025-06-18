//
// updated by ...: Loreto Notarantonio
// Date .........: 17-06-2025 19.14.22
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//
#pragma once
#include <Arduino.h>


class Led {
    private:
        int pin;
        bool stato;

    public:
        // Costruttore
        Led(int ledPin);

        // Metodo per accendere il LED
        void accendi(void);

        // Metodo per spegnere il LED
        void spegni(void);

        // Metodo per invertire lo stato del LED
        void inverti(void);

        // Metodo per ottenere lo stato attuale del LED
        bool isAcceso(void);
};


