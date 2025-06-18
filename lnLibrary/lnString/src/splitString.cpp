/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-05-2025 08.05.32
*/


#include <Arduino.h>


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0x
#include "@logMacros.h"
#include "@lnString.h"

/*
####################################################
#    input; char *
#    modifica input string
####################################################
*/

const PROGMEM char *splittedResult[100];

uint8_t splitString(char *input_str, const char *delim) {
    printf1_NFN("Parsing String: %s\n", input_str);


    char* ptr = strtok(input_str, delim); // get first word
    printf1_NFN("index\ttext\n");
    int8_t i = 0;
    while (ptr) {
        splittedResult[i]=ptr;
        printf1_NFN("%d - '%s'\n", i, ptr);  // this is the ASCII text we want to transform into an integer
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
uint8_t splitSavingString(char *input_str, const char *delim, bool safe) {
    uint8_t words = 0;
    if (safe) {
        char *working_string = strdup(input_str); // se non vogliamo modificare l'originale
        words = splitString(working_string, delim);
        free(working_string);
    }
    else {
        words = splitString(input_str, delim);
    }


    return words;
}



/*
####################################################
#    input; char *
#    NON modifica input string
#    creaa una string di lavoro
####################################################
*/
char *getWord(char *input_str, const char *delim, int8_t word_nr) {
    char *working_string = strdup(input_str); // se non vogliamo modificare l'originale
    char *ptr;

    if (word_nr > 0) {
        ptr = strtok(working_string, delim); // get first word
        uint8_t curr_word = 1;
        while (ptr) {
            if (curr_word == word_nr) {
                break;
            }
            printf1_NFN("%d - '%s'\n", curr_word, ptr);
            ptr = strtok(NULL, delim);
            curr_word++;
        }
        free(working_string);
    }
    return ptr;
}


// ######################################################################
// #  Example:
// #    char myString[]="12:34:55";
// #    char delim[] = ":";
// #    int32_t seconds = stringToSeconds(myString, delim);
// #    printf1_NFN("seconds: %d\n", seconds);
// ######################################################################
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
    printf1_NFN("seconds [switch]: %d\n", seconds);
    return seconds;
}


char *getSubString(char *source, char *dest, char left, char right) {
    char *myFrom = strrchr(source, left) ? strrchr(source, left)+1 : source;
    char *myTo = strrchr(myFrom, right);
    int len = strlen(myFrom);
    if (myTo != NULL) {
        len=myTo - myFrom;
    }
    printf("len: %d\n", len);
    snprintf(dest, len+1, "%s", myFrom);
    printf("From: %s\n", dest);
    return dest;
}
