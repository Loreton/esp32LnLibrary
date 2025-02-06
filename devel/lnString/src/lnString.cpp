/*
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 17.51.49
vedere test_file: /home/loreto/lnProfile/appls/Arduino-ESP32/Esp32TEST/LnFunctions/src/splitString_ok.cpp
*/

#include <Arduino.h>
#include "@lnMacros.h"
#define printf Serial.printf


    // Serial.print(F("\"\t"));
    // Serial.println(atol(ptr)); // atol(ptr) will be your long int you could store in your array at position i.
    // atol() info at http://www.cplusplus.com/reference/cstdlib/atol

// https://forum.arduino.cc/t/split-string-to-array/593683/9


/*
####################################################
#    input; char *
#    modifica input string
####################################################
*/

uint8_t splitProcess(char *input_str, const char *delim) {

    printf("Parsing String: %s\n", input_str);

    char* ptr = strtok(input_str, delim); // get first word
    uint8_t i = 1;
    printf("index\ttext\n");
    while (ptr) {
        printf("%d - \"%s\"\n", i, ptr);  // this is the ASCII text we want to transform into an integer
        ptr = strtok(NULL, delim);
        i++;
    }
    return i;
}




/*
####################################################
#    input; char *
#    NON modifica input string
#    creaa una string di lavoro
####################################################
*/
uint8_t splitString(char *input_str, const char *delim, bool safe) {
    uint8_t words = 0;
    if (safe) {
        char *working_string = strdup(input_str); // se non vogliamo modificare l'originale
        words = splitProcess(working_string, delim);
        free(working_string);
    }
    else {
        words = splitProcess(input_str, delim);
    }


    return words;
}


/*
####################################################
#    input; String object
#    NON modifica input string
#    creaa una string di lavoro
####################################################
*/
uint8_t splitString(String input_str, const char *delim) {
    uint8_t lun = input_str.length();
    char  cString[lun+1];
    input_str.toCharArray(cString, lun+1);
    return splitProcess(&cString[0], delim);
}



/*
####################################################
#    input; char *
#    NON modifica input string
#    creaa una string di lavoro
####################################################
*/
char * getWord(char *input_str, const char *delim, uint8_t word_nr) {
    char *working_string = strdup(input_str); // se non vogliamo modificare l'originale

    char* ptr = strtok(working_string, delim); // get first word
    uint8_t words = 1;
    while (ptr) {
        if (words == word_nr) {
            break;
        }
        printf("%d - \"%s\"\n", words, ptr);
        ptr = strtok(NULL, delim);
        words++;
    }
    free(working_string);
    return ptr;
}


int main() {

    String xxx = "Here; is some; sample;100;data;1.414;1020";
    splitString_OK(xxx, ";");
    // splitString_type01a(xxx, ";");
    char myString[] = "Here; is some; sample;100;data;1.414;1020";
    // split_StringType(xxx, ';');
    // split_cStringType(myString, ';');
    split_anyString(xxx, ';');
    split_anyString(myString, ';');
}


void setup() {
    Serial.begin(115200);
    delay(2000);
    main()

}



void loop() {}
