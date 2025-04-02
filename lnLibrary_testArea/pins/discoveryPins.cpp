//
// updated by ...: Loreto Notarantonio
// Date .........: 02-04-2025 18.13.09
//

#include <Arduino.h> // in testa anche per le definizioni dei type
#include "@SerialRead.h"


// const uint8_t my_pins[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
// const uint8_t my_pins_[] = { 0, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};

typedef struct {
    uint8_t input[40];
    uint8_t output[40];
    uint8_t input_count;
    uint8_t output_count;
} PINS;
const char PROGMEM *str_pinLevel[] = {"LOW", "HIGH"};
const char PROGMEM *TAB = "    ";

PINS myPins;
PINS *pins = &myPins;

// prototypes
int32_t readSerialChar(void);
int32_t readSerialInt(void);
bool waitForChar(char chr);


void _checkOutputPin(uint8_t pin) {

    Serial.printf("\t[pin: %d] ", pin);
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


void discovery_ESP32_Output_Pins( uint8_t my_pins[], uint8_t count) {
    Serial.printf("numero di pins %d\n", count);

    for (int i = 0; i < count; ++i) {
        uint8_t pin = my_pins[i];
        _checkOutputPin(pin);
    }

}


void _checkInputPin(uint8_t pin) {

    // Serial.printf("working on INPUT_PULLUP pin: %d\n", pin);
    pinMode(pin, INPUT_PULLUP);

    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]); Serial.printf(" - change pin setting ['c' to continue]: "); waitForChar('c');
    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]); Serial.printf(" - reset pin setting ['c' to continue]: "); waitForChar('c');
    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]);
    Serial.printf("\n\n");
}




void discovery_ESP32_Input_Pins( uint8_t my_pins[], uint8_t count) {
    const char *TAB="\t";
    Serial.printf("numero di pins %d\n", count);
    Serial.printf("Per ogni pin:\n");
    Serial.printf("\t1. read/display current status and wait for 'c' char to continue\n");
    Serial.printf("\t     at this time you can change the pin status\n");
    Serial.printf("\t2. read/display new status and wait....\n");
    Serial.printf("\t     at this time you can release the pin to previous status\n");
    Serial.printf("\t2. read/display new status and go next pin\n");

    for (int i = 0; i < count; ++i) {
        uint8_t pin = my_pins[i];
        _checkInputPin(pin);
    }
}

// pin 1 si blocca esp32
// pin 6-11 va in dump evitarli....

uint8_t all_pins[] =    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};




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

        pins->input_count = sizeof(i_pins);
        len = pins->input_count;
        for (int i = 0; i < len; i++) {
            pins->input[i] = i_pins[i];
        }

        // len = sizeof(o_pins);
        uint8_t o_pins[] =  { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
        pins->output_count = sizeof(o_pins);
        len = pins->output_count;
        for (int i = 0; i < len; i++) {
            pins->output[i] = o_pins[i];
        }

    }
    else if (choice == '2') {
        Serial.printf("testing pins of board: ESP32_WROOM_32E_2_RELAYs\n");

        uint8_t i_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
        pins->input_count = sizeof(i_pins);
        len = pins->input_count;
        for (int i = 0; i < len; i++) {
            pins->input[i] = i_pins[i];
        }

        uint8_t o_pins[] =  { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
        pins->output_count = sizeof(o_pins);
        len = pins->output_count;
        for (int i = 0; i < len; i++) {
            pins->output[i] = o_pins[i];
        }
    }



}







void loop() {
    bool found=false;
    Serial.printf("\n\tplease enter pin type i[nput] o[utput]: ");
    char choice = waitForAnyChar("io");

    Serial.printf("\n\n");
    if (choice == 'i') {
        Serial.printf("Following available:\n\t");
        for (int i = 0; i < pins->input_count; i++) {
            Serial.printf("%d, ", pins->input[i]);
        }

        Serial.printf("\n\n");
        Serial.printf("\n\tplease enter pin number: ");
        int32_t pin_nr = readSerialInt();

        for (int i = 0; i < pins->input_count; ++i) {
            if (pins->input[i] == pin_nr) {
                _checkInputPin(pin_nr);
                found=true;
                break;
            }
        }

        if (! found) {
            Serial.printf("\n\tERROR pin %d not in list!\n\n", pin_nr);
        }
    }


    else if (choice == 'o') {
        Serial.printf("Following available:\n\t");
        for (int i = 0; i < pins->output_count; i++) {
            Serial.printf("%d, ", pins->output[i]);
        }

        Serial.printf("\n\tplease enter pin number: ");
        int32_t pin_nr = readSerialInt();

        for (int i = 0; i < pins->output_count; ++i) {
            if (pins->output[i] == pin_nr) {
                _checkOutputPin(pin_nr);
                found=true;
                break;
            }
        }

        if (! found) {
            Serial.printf("\n\tERROR pin %d not in list!\n\n", pin_nr);
        }

    }

}
