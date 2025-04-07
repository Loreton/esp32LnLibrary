//
// updated by ...: Loreto Notarantonio
// Date .........: 07-04-2025 09.15.06
//

#include <Arduino.h> // in testa anche per le definizioni dei type


// Example 2 - Receive with an end-marker
const byte MAX_NUMCHARS = 20;
char receivedChars[MAX_NUMCHARS+1];   // an array to store the received data



uint8_t readSerialData(const char *chars="", uint8_t max_chars=MAX_NUMCHARS, const char exits='x') {
    if (max_chars > MAX_NUMCHARS) {max_chars = MAX_NUMCHARS; }

    char   endMarker = 13; // char endMarker = '\n';
    char   chr;
    int8_t ndx = 0;
    uint8_t len = strlen(chars);


    // Flash buffer
    while (Serial.available() > 0) {
        Serial.read();
    }


    while(true) { // remain here until told to break
        if (Serial.available() > 0) {
            chr = Serial.read();
            int asciiVal = (int) chr;

            if (asciiVal == endMarker) {
                receivedChars[ndx] = '\0'; // terminate the string
                break;
            }

            else if (exits == chr) {
                Serial.print(chr);
                receivedChars[0] = chr;
                receivedChars[1] = '\0'; // terminate the string
                ndx=0;
                break;
            }

            else if (len != 0) { // specific char
                for (int i = 0; i < len; i++) {
                    if (chars[i] == chr) {
                        receivedChars[ndx++] = chr;
                        Serial.print(chr);
                        break;
                    }
                }
            }

            else { // any char
                receivedChars[ndx++] = chr;
                Serial.print(chr);
            }


            // if numchar reached exit
            if (ndx >= max_chars) {
                receivedChars[max_chars] = '\0'; // terminate the string
                ndx = max_chars;
                break;
            }
        }

    }
    Serial.printf("\n");
    return ndx; // numero di caratteri catturati
}

// prototypes
// int32_t readSerialChar(void);
// int32_t readSerialInt(void);
// bool waitForChar(char chr);

int32_t readSerialInt() {
    uint8_t count = readSerialData("0123456789");
    if (count<1) {
        return -1;
    }
    return atoi(receivedChars); // convert data to integer
}


// #################################################
// # wait for 'chr' input
// #################################################
bool waitForChar(const char *descr, char chr) {
    const char appo[] = {chr};
    Serial.printf(descr);
    readSerialData(&appo[0], 1);
    if (receivedChars[0] == chr) {
        return true;
    }
    return false;
}
// #################################################
// # wait for 'chr' input
// #################################################
char waitForAnyChar(const char *chars) {
    uint8_t count = readSerialData(chars, 1);
    if (count > 0) {
        return receivedChars[0];
    }
    return 0;
}







void setup_() {
    Serial.begin(115200);
    delay(2200);
    Serial.println("<Arduino is ready>");
}

int first_run = 0;
void loop_() {
    if (first_run == 0) {
        // Serial.printf("Please enter 'c' char or ENTER to skip\n");
        waitForChar("Please enter 'c' char or ENTER to skip\n", 'c');
        Serial.printf("[%s] - lenght0: %d\n", receivedChars, strlen(receivedChars));
        first_run++;
    }


    Serial.printf("Please enter one of following chars [2468] or ENTER to skip\n");
    char choice = waitForAnyChar("abcdef");
    Serial.printf("choice: %c ascii: %d \n", char(choice), choice);
    Serial.printf("\n");

    Serial.printf("Please enter integer value [ENTER to confirm]\n");
    int32_t value = readSerialInt();
    Serial.printf("value: %li \n", value);
    Serial.printf("\n");

}
