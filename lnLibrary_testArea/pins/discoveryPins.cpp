//
// updated by ...: Loreto Notarantonio
// Date .........: 03-04-2025 15.29.56
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
// const char PROGMEM *str_input_output[] = {"INPUT", "OUTPUT"};
const char PROGMEM *TAB = "    ";

PINS myPins;
PINS *pins = &myPins;

// prototypes
int32_t readSerialChar(void);
int32_t readSerialInt(void);
bool waitForChar(char chr);


// ################################################################
// #
// ################################################################
void _checkOutputPin(uint8_t pin) {

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
void discovery_ESP32_Output_Pins( uint8_t my_pins[], uint8_t count) {
    Serial.printf("numero di pins %d\n", count);

    for (int i = 0; i < count; i++) {
        uint8_t pin = my_pins[i];
        _checkOutputPin(pin);
    }

}


// ################################################################
// #
// ################################################################
void _checkInputPin(uint8_t pin) {

    // Serial.printf("working on INPUT_PULLUP pin: %d\n", pin);
    pinMode(pin, INPUT_PULLUP);

    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]); Serial.printf(" - change pin setting ['c' to continue]: "); waitForChar('c');
    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]); Serial.printf(" - reset  pin setting ['c' to continue]: "); waitForChar('c');
    Serial.printf("\t[pin: %d] - status: %s", pin, str_pinLevel[digitalRead(pin)]);
    Serial.printf("\n\n");
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
        _checkInputPin(pin);
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

        uint8_t o_pins[] =  { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 99 };
        pins->output_count = copyArray(o_pins, pins->output, sizeof(o_pins));

    }
    else if (choice == '2') {
        Serial.printf("testing pins of board: ESP32_WROOM_32E_2_RELAYs\n");

        uint8_t i_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
        pins->input_count = copyArray(i_pins, pins->input, sizeof(i_pins));

        uint8_t o_pins[] =  { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 99};
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
        _checkOutputPin(pins[i]);
    }
}


// ################################################################
// #
// ################################################################
bool checkPin(uint8_t  *pins, uint8_t count, uint8_t match_pin, char pin_type) {
    bool found=false;
    for (int i = 0; i < count; i++) {
        if (pins[i] == match_pin) {
            if (pin_type == 'i') {
                _checkInputPin(pins[i]);
            }
            else {
                _checkOutputPin(pins[i]);
            }
            found=true;
            break;
        }
    }
    return found;
}




// ################################################################
// #
// ################################################################
void loop() {
    static char choice = 'x';
    bool found=false;
    // schegle input/output
    if (choice == 'x') {
        Serial.printf("\n\tplease enter pin type i[nput] o[utput]: ");
        choice = waitForAnyChar("io");
        if (choice == 'i') {
            Serial.printf("Following available:\n\t");
            for (int i = 0; i < pins->input_count; i++) {
                Serial.printf("%d, ", pins->input[i]);
            }
        }

        else if (choice == 'o') {
            Serial.printf("Following available:\n\t");
            for (int i = 0; i < pins->output_count; i++) {
                Serial.printf("%d, ", pins->output[i]);
            }
        }

    }

    Serial.printf("\n\n\tplease enter pin number (choice: %c): ", choice);
    int32_t pin_nr = readSerialInt();
    if (pin_nr < 0) { // return no pin
        choice = 'x';
        return; // loop
    }

    else if (choice == 'i') {
        if (! checkPin(pins->input, pins->input_count, pin_nr, choice)) {
            Serial.printf("\n\tERROR pin %d not in list!\n\n", pin_nr);
        }
    }
    else if (choice == 'o') {
        if (pin_nr == 99) {
            allOut(pins->output, pins->output_count-1);
        }
        else if (! checkPin(pins->output, pins->output_count, pin_nr, choice)) {
            Serial.printf("\n\tERROR pin %d not in list!\n\n", pin_nr);
        }
    }

}
