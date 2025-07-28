/*
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 14.52.14
*/


#include <Arduino.h>

#include "lnLogger_Class.h"


void setPinID(char buffer[], int MAXLENGTH, const char *name, int pin) {
    //int pinLen = snprintf(NULL, 0, ".%02d", pin); // utile per il calcolo lunghezza

    static char pinSuffix[8];
    int pinLen = snprintf(pinSuffix, sizeof(pinSuffix), ".%02d", pin);

    int nameMaxLen = MAXLENGTH - (pinLen);
    LOG_TRACE("MAXLENGTH: %d", (int)MAXLENGTH);
    LOG_TRACE("nameMaxLen: %d", (int)nameMaxLen);
    LOG_TRACE("pinLen: %d", (int)pinLen);

    //-- manca un carattere non capisco!!!
    // snprintf(buffer, MAXLENGTH, "[%.*s.%2d]", (int)nameMaxLen-2, name, pin);

    //... allora spezziamo il processo... prima i nome
    int index = snprintf(buffer, MAXLENGTH, "%.*s", (int)nameMaxLen, name);
    LOG_TRACE("buffer: <%s> len: %d", buffer, (int)strlen(buffer));

    //... poi il pin
    index = snprintf(buffer+index, pinLen+1, pinSuffix);
    LOG_TRACE("buffer: <%s> len: %d", buffer, (int)strlen(buffer));

}

#if 0
int main() {
    printf("Hello World");
    char pinID[16]; // name_len + 1 + 6+2
    const size_t MAXLENGTH = sizeof(pinID)-1;
    printf("MAXLENGTH: %d", (int)MAXLENGTH);
    setPinID(pinID, MAXLENGTH, "123456789012345678901234567890", 10, false);
    printf("pinID: <%s> len: %d", pinID, (int)strlen(pinID));
    if (strlen(pinID) > MAXLENGTH) {
        printf("ERRORE: lunghezza superata");
    }


    return 0;
#endif

