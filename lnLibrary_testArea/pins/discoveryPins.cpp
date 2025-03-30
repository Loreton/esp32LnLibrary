//
// updated by ...: Loreto Notarantonio
// Date .........: 30-03-2025 17.27.45
//

#include <Arduino.h> // in testa anche per le definizioni dei type



// const uint8_t my_pins[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
// const uint8_t my_pins_[] = { 0, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};

typedef struct {
    uint8_t input_pins[40];
    uint8_t output_pins[40];
    uint8_t input_count;
    uint8_t output_count;
} PINS;



void waitForInput(uint8_t chr) {
    while(true) { // remain here until told to break
        if (Serial.available() > 0) {
            if(Serial.read() == chr) { // is that something the char G?
                break;
            }
        }
    }
}
uint8_t waitForInput(uint8_t *pins, ) {
    while(true) { // remain here until told to break
        if (Serial.available() > 0) {
            if(Serial.read() == chr) { // is that something the char G?
                break;
            }
        }
    }
}

uint8_t inputFor2Choice(uint8_t chr1, uint8_t chr2) {
    uint8_t choice = '0';
    while(true) { // remain here until told to break
        if (Serial.available() > 0) {
            if(Serial.read() == chr1) { // is that something the char G?
                choice = chr1;
                break;
            }
            else if(Serial.read() == chr2) { // is that something the char G?
                choice = chr2;
                break;
            }
        }
    }
    return choice;
}


void _checkOutputPin(uint8_t pin) {

    Serial.printf("setting pin: %d for output --> ", pin);
    digitalWrite(pin, LOW); pinMode(pin, OUTPUT);

    digitalWrite(pin, LOW);
    delay(1000); Serial.printf(" - LOW: %d", digitalRead(pin));

    digitalWrite(pin, HIGH);
    delay(1000); Serial.printf(" - HIGH: %d", digitalRead(pin));

    digitalWrite(pin, LOW);
    delay(1000); Serial.printf(" - LOW: %d", digitalRead(pin));

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

    Serial.printf("working on INPUT_PULLUP pin: %d\n", pin);
    pinMode(pin, INPUT_PULLUP);

                    Serial.printf("    status 1: %d\n", digitalRead(pin));
    waitForInput('c'); Serial.printf("    status 2: %d\n", digitalRead(pin));

    waitForInput('c'); Serial.printf("    status 3: %d\n", digitalRead(pin));

    Serial.printf("\n");
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

// #define AZ_DELIVERY_ESP32_WROOM_32_
// #define ESP32_WROOM_32E_2_RELAY

// #ifdef AZ_DELIVERY_ESP32_WROOM_32
//     uint8_t out_pins[] =    { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
//     uint8_t input_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
//     // pin 34, 35, 36, 39 hanno un valore di 0 - NOT usable
// #elif defined ESP32_WROOM_32E_2_RELAYs
//     uint8_t out_pins[] =    { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
//     uint8_t input_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
// #endif


void setup_ok() {
    Serial.begin(115200);
    delay(2200);

    Serial.printf("please enter board to be checked:\n");
    Serial.printf("1. AZ_DELIVERY_ESP32_WROOM_32\n");
    Serial.printf("2. ESP32_WROOM_32E_2_RELAYs\n");

    uint8_t choice = inputFor2Choice('1', '2');

    if (choice == '1') {
        Serial.printf("testing pins of board: AZ_DELIVERY_ESP32_WROOM_32\n")
        uint8_t out_pins[] =    { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
        uint8_t input_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
        discovery_ESP32_Output_Pins(out_pins, sizeof(out_pins));
        discovery_ESP32_Input_Pins(input_pins, sizeof(input_pins));
    }
    else if (choice == '2') {
        Serial.printf("testing pins of board: ESP32_WROOM_32E_2_RELAYs\n")
        uint8_t out_pins[] =    { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
        uint8_t input_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
        discovery_ESP32_Output_Pins(out_pins, sizeof(out_pins));
        discovery_ESP32_Input_Pins(input_pins, sizeof(input_pins));
    }
    // else {
    //     uint8_t out_pins[] =    {};
    //     uint8_t input_pins[] =  {};

    // }



    Serial.printf("test completed.\n");
}

void setup() {
    Serial.begin(115200);
    delay(2200);

    Serial.printf("please enter board to be checked:\n");
    Serial.printf("1. AZ_DELIVERY_ESP32_WROOM_32\n");
    Serial.printf("2. ESP32_WROOM_32E_2_RELAYs\n");

    uint8_t choice = inputFor2Choice('1', '2');

    PINS *azDelivery_Pins = PINS;
    PINS *twoRelays_Pins = PINS;

    if (choice == '1') {
        Serial.printf("testing pins of board: AZ_DELIVERY_ESP32_WROOM_32\n")
        uint8_t out_pins[] =    { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
        uint8_t input_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};

        myPins->input_count = sizeof(input_pins);
        myPins->output_count = sizeof(output_pins);
        for (int i = 0; i < myPins->input_count; ++i) {
            myPins->input_pins[i] = input_pins[i];
        }
        for (int i = 0; i < myPins->output_count; ++i) {
            myPins->output_pins[i] = output_pins[i];
        }

    }
    else if (choice == '2') {
        Serial.printf("testing pins of board: ESP32_WROOM_32E_2_RELAYs\n")
        uint8_t out_pins[] =    { 0,    2, 3, 4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
        uint8_t input_pins[] =  { 0,    2,    4, 5,                     12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39};
    }



    Serial.printf("test completed.\n");
}

void loop() {}