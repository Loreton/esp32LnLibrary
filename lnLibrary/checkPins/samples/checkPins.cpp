//
// updated by ...: Loreto Notarantonio
// Date .........: 06-08-2025 13.46.13
//

#include <Arduino.h> // in testa anche per le definizioni dei type
#include "driver/ledc.h"

#include "lnSerialRead.h"
#define ESP32_WROOM_32E_2RELAY_MODULE   1
#define ESP32_WROOM_32_MODULE           2

// const uint8_t my_pins[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
// const uint8_t my_pins_[] = { 0, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};

#if ESP32_BOARD_TYPE == ESP32_WROOM_32E_2RELAY_MODULE
#elif ESP32_BOARD_TYPE == ESP32_WROOM_32_MODULE
#endif

typedef struct {
    uint8_t input[40];
    uint8_t output[40];
    uint8_t input_count;
    uint8_t output_count;
} PINS;
const char PROGMEM *str_pinLevel[] = {"LOW", "HIGH"};
// const char PROGMEM *str_input_output[] = {"INPUT", "OUTPUT"};
const char PROGMEM *TAB = "    ";

PINS myPins;
PINS *pins = &myPins;


extern char receivedChars[];


// ################################################################
// #
// ################################################################
void toggleOutputPin(uint8_t pin) {

    Serial.printf("\t[pin: %2d] ", pin);
    digitalWrite(pin, LOW); pinMode(pin, OUTPUT);

    digitalWrite(pin, LOW);
    delay(1000); Serial.printf(" - %s", str_pinLevel[digitalRead(pin)]);

    digitalWrite(pin, HIGH);
    delay(1000); Serial.printf(" - %s", str_pinLevel[digitalRead(pin)]);

    digitalWrite(pin, LOW);
    delay(1000); Serial.printf(" - %s", str_pinLevel[digitalRead(pin)]);

    pinMode(pin, INPUT);
    delay(1000);

    Serial.printf("\n");
}


// ################################################################
// #
// ################################################################
void pinOFF(uint8_t pin) {
    Serial.printf("\tsetting pin: %2d OFF", pin);

    digitalWrite(pin, LOW);
    delay(1000); Serial.printf(" - %s", str_pinLevel[digitalRead(pin)]);

    Serial.printf("\n");
}

// ################################################################
// #
// ################################################################
void pinON(uint8_t pin) {
    Serial.printf("\tsetting pin: %2d ON", pin);

    digitalWrite(pin, HIGH);
    delay(1000); Serial.printf(" - %s", str_pinLevel[digitalRead(pin)]);

    Serial.printf("\n");
}


// ################################################################
// #
// ################################################################
void processInputPin(uint8_t pin) {

    // Serial.printf("working on INPUT_PULLUP pin: %d\n", pin);
    pinMode(pin, INPUT_PULLUP);

    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]); waitForChar(" - change pin setting ['c' to continue]: ", 'c');
    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]); waitForChar(" - reset  pin setting ['c' to continue]: ", 'c');
    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]);
    Serial.printf("\n\n");
}


// ################################################################
// #
// ################################################################
void discovery_ESP32_Output_Pins( uint8_t my_pins[], uint8_t count) {
    Serial.printf("numero di pins %d\n", count);

    for (int i = 0; i < count; i++) {
        uint8_t pin = my_pins[i];
        toggleOutputPin(pin);
    }

}




// ################################################################
// #
// ################################################################
void discovery_ESP32_Input_Pins( uint8_t my_pins[], uint8_t count) {
    const char *TAB="\t";
    Serial.printf("numero di pins %d\n", count);
    Serial.printf("Per ogni pin:\n");
    Serial.printf("\t1. read/display current status and wait for 'c' char to continue\n");
    Serial.printf("\t     at this time you can change the pin status\n");
    Serial.printf("\t2. read/display new status and wait....\n");
    Serial.printf("\t     at this time you can release the pin to previous status\n");
    Serial.printf("\t2. read/display new status and go next pin\n");

    for (int i = 0; i < count; i++) {
        uint8_t pin = my_pins[i];
        processInputPin(pin);
    }
}

// pin 1 si blocca esp32
// pin 6-11 va in dump evitarli....

uint8_t all_pins[] =    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};



// ################################################################
// #
// ################################################################
uint8_t copyArray(uint8_t  *src, uint8_t *dest, uint8_t len) {
    for (int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
    return len;
}







// ################################################################
// #
// ################################################################
void setup() {
    Serial.begin(115200);
    delay(2200);
    uint8_t len;

    Serial.printf("please enter board to be checked:\n");
    Serial.printf("1. AZ_DELIVERY_ESP32_WROOM_32\n");
    Serial.printf("2. ESP32_WROOM_32E_2_RELAYs\n");

    char choice = waitForAnyChar("12");
    Serial.printf("\n\n");


    if (choice == '1') {
        Serial.printf("testing pins of board: AZ_DELIVERY_ESP32_WROOM_32\n");
        uint8_t i_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
        pins->input_count = copyArray(i_pins, pins->input, sizeof(i_pins));

        uint8_t o_pins[] =  { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
        pins->output_count = copyArray(o_pins, pins->output, sizeof(o_pins));

    }
    else if (choice == '2') {
        Serial.printf("testing pins of board: ESP32_WROOM_32E_2_RELAYs\n");

        uint8_t i_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
        pins->input_count = copyArray(i_pins, pins->input, sizeof(i_pins));

        uint8_t o_pins[] =  { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
        pins->output_count = copyArray(o_pins, pins->output, sizeof(o_pins));
    }



}




// ################################################################
// #
// ################################################################
void allOut(uint8_t  *pins, uint8_t count) {
    Serial.printf("press 'q' to exit in any moment\n");
    for (int i = 0; i < count; i++) {
        if (Serial.available() > 0) {
            char chr = Serial.read();
            if (chr == 'q') {
                Serial.printf("exiting on user request!\n");
                return;
            }
        }
        toggleOutputPin(pins[i]);
    }
}


// ################################################################
// #
// ################################################################
bool isValidPin(uint8_t pin_nr, uint8_t  *pins, uint8_t count) {
    bool found=false;
    for (int i = 0; i < count; i++) {
        if (pins[i] == pin_nr) {
            found=true;
            break;
        }
    }
    // if (!found) {
    //     Serial.printf("\n\t\tVALID pins: ");
    //     for (int i = 0; i < count; i++) {
    //         Serial.printf("%d, ", pins[i]);
    //     }
    //     Serial.printf("\n");    }
    return found;
}




// const char TAB=""
// ################################################################
// #
// ################################################################
void loop() {
    // static char mode = 0;


    // mi aspetto il primo char = 'o' oppure 'i'
    // il resto è il numero del pin

    // Serial.printf("\n\n\t\tINPUT pins: ");
    // for (int i = 0; i < pins->input_count; i++) {
    //     Serial.printf("%d, ", pins->input[i]);
    // }
    // Serial.printf("\n");

    // Serial.printf("\t\tOUTPUT pins:");
    // for (int i = 0; i < pins->output_count; i++) {
    //     Serial.printf("%d, ", pins->output[i]);
    // }
    // Serial.printf("\n");



    Serial.printf("\n%splease enter pin to check:\n", TAB);
    Serial.printf("%sixx (INPUT)\n", TAB);
    Serial.printf("%soxx (toggle OUTPUT)\n", TAB);
    Serial.printf("%slxx (set OUTPUT-LOW)\n", TAB);
    Serial.printf("%shxx (set OUTPUT-HIGH)\n", TAB);
    Serial.printf("%sfxx (1000Hz on pin)\n", TAB);
    Serial.printf("%swhere xx is the pin_nr >: ", TAB);

    uint8_t n_chars=readSerialData("iohlf0123456789", 4); // max 3 char, es,: i12
    // if ( (n_chars < 1) || (receivedChars[0] != 'i' && receivedChars[0] != 'o') ) {
    if ( (n_chars < 1)  ) {
        return;
    }

    char mode=receivedChars[0];
    int32_t pin_nr=atoi(&receivedChars[1]);



    if (mode == 'i') {
        if (isValidPin(pin_nr, pins->input, pins->input_count)) {
            Serial.printf("%spin %ld as INPUT", TAB, pin_nr);
            processInputPin(pin_nr);
        }

        else {
            Serial.printf("\n%spin %d not in list, available INPUT pins:\n\t", TAB, pin_nr);
            for (int i = 0; i < pins->input_count; i++) {
                Serial.printf("%d, ", pins->input[i]);
            }
        }

    }

    else if (mode == 'o' || mode == 'l' || mode == 'h' || mode == 'f') {
        if (isValidPin(pin_nr, pins->output, pins->output_count)) {
            Serial.printf("%spin %ld as OUTPUT (mode: %c)", TAB, pin_nr, mode);

            pinMode(pin_nr, OUTPUT);
            digitalWrite(pin_nr, LOW);

            if (mode == 'l') {
                pinOFF(pin_nr);
            }
            else if (mode == 'o') {
                toggleOutputPin(pin_nr);
            }
            else if (mode == 'h') {
                pinON(pin_nr);
            }
            else if (mode == 'f') {
                int channel = 0;
                int frequency=1000;
                ledcAttachPin(pin_nr, channel); // channel
                ledcWriteTone(channel, frequency); // Imposta frequenza, duty cycle 50% automatico
                delay(1000);
                ledcWrite(channel, 0); // spegni
                ledcDetachPin(pin_nr); //

                // tone(pin_nr, 800, 2000);
            }
        }

        else {
            Serial.printf("\n%spin %d not in list, available OUTPUT pins:\n\t", TAB, pin_nr);
            for (int i = 0; i < pins->output_count; i++) {
                Serial.printf("%d, ", pins->output[i]);
            }
        }
    }

}
