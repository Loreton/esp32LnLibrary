/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-05-2025 08.05.27
*/


#include <Arduino.h>

// char msg[] = "1,20,300,4000,50000";


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0x
#include "@logMacros.h"


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
        printf1_NFN("null\n");
    }
    else if (str[0] == '\0') {
        printf1_NFN("empty\n");
    }
    else if (str ) {
        printf1_NFN("%s\n", str);
        isValid=true;
    }
    return isValid;
}




