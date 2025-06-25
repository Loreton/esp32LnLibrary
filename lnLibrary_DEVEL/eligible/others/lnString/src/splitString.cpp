/*
// updated by ...: Loreto Notarantonio
// Date .........: 26-02-2025 14.26.13
*/


#include <Arduino.h>

// --- correlati
#include "@lnString.h"
#include "@lnMacros.h"

// char msg[] = "1,20,300,4000,50000";

#define printf0_FN       lnPrintF_FN
#define print_ln         lnPrintLN


/*
####################################################
#    input; char *
#    modifica input string
####################################################
*/

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




/*
####################################################
#    input; char *
#    NON modifica input string
#    creaa una string di lavoro
####################################################
*/
uint8_t splitSaveString(char *input_str, const char *delim, bool safe) {
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
            printf0_FN("%d - '%s'\n", curr_word, ptr);
            ptr = strtok(NULL, delim);
            curr_word++;
        }
        free(working_string);
    }
    return ptr;
}





int  main() {
    char myString[] ="Loreto  ciao  300  4000       50000";
    const char delim[] = " ";
    int words = splitString(myString, delim);
    for (int j=0; j<words; j++) {
        printf0_FN("%d - %s\n", j, splittedResult[j]);
    }
}


void setup() {
    Serial.begin(115200);
    delay(2000);
    main();
    printf0_FN("program completed\n");

}

void loop() {}

