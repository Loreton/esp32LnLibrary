//
// updated by ...: Loreto Notarantonio
// Date .........: 30-03-2025 19.53.01
//

#include <Arduino.h> // in testa anche per le definizioni dei type


// Example 2 - Receive with an end-marker

const byte numChars = 20;
char receivedChars[numChars+1];   // an array to store the received data

boolean newData = false;


void readSerialData(void) {
    int8_t ndx = 0;
    char endMarker = 13; // char endMarker = '\n';
    char rc;

    // Flash buffer
    while (Serial.available() > 0) {
        Serial.read();
    }

    while(true) { // remain here until told to break
        if (Serial.available() > 0) {
            rc = Serial.read();
            int asciiVal = (int) rc;

            if (asciiVal == endMarker) {
                receivedChars[ndx] = '\0'; // terminate the string
                break;
            }

            else {// END char
                receivedChars[ndx] = rc;
                ndx++;
            }

            // if numchar reached exit
            if (ndx >= numChars) {
                receivedChars[numChars] = '\0'; // terminate the string
                ndx = numChars;
                break;
            }
        }

    }
}



int8_t recvWithEndMarker() {
    Serial.printf("Waiting for 2 chars [ENTER to confirm]\n");
    return readSerialData();
}

int32_t ReadSerialInt() {
    Serial.printf("Please enter integer value [ENTER to confirm]\n");
    int8_t count = readSerialData();
    return atoi(receivedChars);
}
















void setup() {
    Serial.begin(115200);
    delay(2200);
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvWithEndMarker();
    Serial.printf("[%s] - lenght0: %d\n", receivedChars, strlen(receivedChars));

    int32_t value = ReadSerialInt();
    Serial.println(value);
}
