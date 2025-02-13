/*
// updated by ...: Loreto Notarantonio
// Date .........: 13-02-2025 11.38.54
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

const char *result[20];

uint8_t splitProcess(char *input_str, const char *delim) {
    printf0_FN("Parsing String: %s\n", input_str);


    char* ptr = strtok(input_str, delim); // get first word
    printf0_FN("index\ttext\n");
    int8_t i = 0;
    while (ptr) {
        result[i]=ptr;
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







#if 0
void printResult(int8_t n_words) {
    print_ln("-------------------");
    print_ln("- printing results");
    print_ln("-------------------");
    for (int8_t i = 0; i < n_words; ++i) {
        printf0_FN("%d -  %s\n", i, result[i]);
    }
    print_ln();
}





int  main() {


    uint8_t words;
    // char *delim = ", ";
    const char *blankDelim = " ";

    printf0_FN("----- split char *String NO-CHANGES (multiple blanks are ignored)---- \n");
    char msg_BLANK[] = "Loreto  ciao  300  4000       50000";
    words = splitString(msg_BLANK, blankDelim, true);
    printf0_FN("words: %d\n", words);
    printf0_FN("%s\n\n", msg_BLANK);
    printResult(words);



    printf0_FN("----- split String object  NO-CHANGES ---- \n");
    String msgStr = "Loreto, ciao, 300, 4000, 50000";

    // devo prima convertire la String to char é
    uint8_t lun = msgStr.length();
    char  cString[lun+1];
    msgStr.toCharArray(cString, lun+1);
    words = splitString(cString, ", ", false);
    printf0_FN("words: %d\n", words);
    printResult(words);





    char msg[] = "Loreto   ciao    300     4000     50000";
    printf0_FN("----- Word number 3 NO-CHANGES ---- \n");
    char *word = Word(msg, ", ", 3);
    printf0_FN("word: %s\n", word);
    uint32_t value=atol(word); // convert to long integer
    printf0_FN("%ld\n\n", value);

    printf0_FN("----- Word number 6 NO-CHANGES  (NOT EXISTS) ---- \n");
    word = Word(msg, ", ", 6);
    if (word) {
        printf0_FN("word: %s\n", word);
        value=atol(word); // convert to long integer
        printf0_FN("%ld\n\n", value);
    } else {
        printf0_FN("Word 6 NOT found\n");
    }




    printf0_FN("----- split char *String CHANGES CHANGES CHANGES CHANGES CHANGES on input string ---- \n");
    words = splitString(msg, ", ", false);
    printf0_FN("words: %d\n", words);
    printResult(words);



    printf0_FN("----- completed ---- \n");

}

void setup() {
    Serial.begin(115200);
    delay(2000);
    main();
}

void loop() {}
#endif