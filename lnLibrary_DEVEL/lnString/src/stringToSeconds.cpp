/*
// updated by ...: Loreto Notarantonio
// Date .........: 26-02-2025 14.35.54
*/





#ifdef COMPILER_ONLINE
    #include "stdio.h"
    #include "string.h"

    #define printf0_FN printf
    #define uint8_t unsigned int
    #define int8_t int
    #define int16_t int

#else
    #include <Arduino.h>

    // --- correlati
    #include "@lnString.h"
    #include "@lnMacros.h"

    // char msg[] = "1,20,300,4000,50000";

    #define printf0_FN       lnPrintF_FN
    #define print_ln         lnPrintLN
#endif




const char *splittedResult[20];

uint8_t splitString(char *input_str, const char *delim) {
    printf0_FN("Parsing String: %s\n", input_str);


    char* ptr = strtok(input_str, delim); // get first word
    printf0_FN("index\ttext\n");
    int8_t i = 0;
    while (ptr) {
        splittedResult[i]=ptr;
        printf0_FN("%d - '%s'\n", i, ptr);  // this is the ASCII text we want to transform into an integer
        ptr = strtok(NULL, delim);
        i++;
    }
    return i;
}

int32_t stringToSeconds(char *input_str, const char *delim) {
    int words = splitString(input_str, delim);
    int32_t seconds=0;

    // TYPE 1
    switch (words) {
        case 1:
            seconds += atoi(splittedResult[0]);
            break;

        case 2:
            seconds += atoi(splittedResult[0])*60;
            seconds += atoi(splittedResult[1]);
            break;

        case 3:
            seconds += atoi(splittedResult[0])*3600;
            seconds += atoi(splittedResult[1])*60;
            seconds += atoi(splittedResult[2]);
            break;

        default:
            seconds = 0;

    }
    printf0_FN("seconds [switch]: %d\n", seconds);


    // TYPE 2
    seconds=0;
    if (words == 1) {
        seconds += atoi(splittedResult[0]);
    }
    else if (words == 2) {
        seconds += atoi(splittedResult[0])*60;
        seconds += atoi(splittedResult[1]);
    }
    else if (words == 3) {
        seconds += atoi(splittedResult[0])*3600;
        seconds += atoi(splittedResult[1])*60;
        seconds += atoi(splittedResult[2]);
    }
    else {
        seconds=0;
    }
    printf0_FN("seconds [if]: %d\n", seconds);
    return seconds;

}



int  main() {
    char myString[]="12:34:55"; char delim[] = ":";
    int32_t seconds = stringToSeconds(myString, delim);
    printf0_FN("seconds: %d\n", seconds);

}



void setup() {
    Serial.begin(115200);
    delay(2000);
    main();
    printf0_FN("program completed\n");
}

void loop() {}
