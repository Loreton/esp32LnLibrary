/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-08-2025 10.57.44
*/


#include <Arduino.h>

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
// #define  LOG_MODULE_LEVEL LOG_LEVEL_DEBUG
#include <lnLogger_Class.h>





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
    LOG_INFO("sString1: %s\n", sString.c_str());
    LOG_INFO("cString1: %s\n", cString);
    sString="loreto";
    LOG_INFO("sString2: %s\n", sString.c_str());
    LOG_INFO("cString2: %s\n", cString);
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

    LOG_INFO("sString3: %s\n", sString.c_str());
    LOG_INFO("cString3: %s\n", cString);
}


bool ln_isString(const char *str) {
    bool isValid = false;

    if (!str) {
        LOG_INFO("null\n");
    }
    else if (str[0] == '\0') {
        LOG_INFO("empty\n");
    }
    else if (str ) {
        LOG_INFO("%s\n", str);
        isValid=true;
    }
    return isValid;
}




