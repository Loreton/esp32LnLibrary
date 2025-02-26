/*
// updated by ...: Loreto Notarantonio
// Date .........: 26-02-2025 18.02.21
*/


#include <Arduino.h>

// char msg[] = "1,20,300,4000,50000";




// ---------------------------------
#include "@lnMacros.h"
// macros Aliases
// ---------------------------------
#define printf0_NFN          lnPrintF_NowFN
#define x_printf0_NFN


/*
####################################################
#    input; char *
#    modifica input string
####################################################
*/



/*
####################################################
#    input; String object
#    NON modifica input string
#    creaa una string di lavoro
####################################################
*/
void charToString() {
    char cString[] = "char to String";
    String sString;
    sString = cString;
    printf("sString1: %s\n", sString.c_str());
    printf("cString1: %s\n", cString);
    sString="loreto";
    printf("sString2: %s\n", sString.c_str());
    printf("cString2: %s\n", cString);
}


// SAMPLE
void StringToChar() {
    // Define
    String sString = "String to char";

    // Length (with one extra character for the null terminator)
    int str_len = sString.length() + 1;

    // Prepare the character array (the buffer)
    char cString[str_len];

    // Copy it over
    sString.toCharArray(cString, str_len);
    // oppure
    sprintf(cString, "%s", sString.c_str());

    printf("sString3: %s\n", sString.c_str());
    printf("cString3: %s\n", cString);
}


bool ln_isString(const char *str) {
    bool isValid = false;

    if (!str) {
        x_printf0_NFN("null\n");
    }
    else if (str[0] == '\0') {
        x_printf0_NFN("empty\n");
    }
    else if (str ) {
        x_printf0_NFN("%s\n", str);
        isValid=true;
    }
    return isValid;
}





// ######################################################################
// #  Example:
// #    char myString[]="12:34:55";
// #    char delim[] = ":";
// #    int32_t seconds = stringToSeconds(myString, delim);
// #    printf0_FN("seconds: %d\n", seconds);
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
    printf0_FN("seconds [switch]: %d\n", seconds);
}