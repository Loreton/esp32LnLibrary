/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 16.18.59
*/

// ---------------------------------
// lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0x
#define LOG_LEVEL_99x
#include "../lnMainInclude/@logMacros.h"

/*
// Online C compiler to run C program online
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define printf99_FN printf
*/
void setPinID(char buffer[], int MAXLENGTH, const char *name, int pin) {
    //int pinLen = snprintf(NULL, 0, ".%02d", pin); // utile per il calcolo lunghezza

    static char pinSuffix[8];
    // const char *pinFmt=".%02d]";
    int pinLen = snprintf(pinSuffix, sizeof(pinSuffix), ".%02d", pin);

    int nameMaxLen = MAXLENGTH - (pinLen);
    printf99_FN("MAXLENGTH: %d\n", (int)MAXLENGTH);
    printf99_FN("nameMaxLen: %d\n", (int)nameMaxLen);
    printf99_FN("pinLen: %d\n", (int)pinLen);

    //-- manca un carattere non capisco!!!
    // snprintf(buffer, MAXLENGTH, "[%.*s.%2d]", (int)nameMaxLen-2, name, pin);

    //... allora spezziamo il processo... prima i nome
    int index = snprintf(buffer, MAXLENGTH, "%.*s", (int)nameMaxLen, name);
    printf99_FN("buffer: <%s> len: %d\n", buffer, (int)strlen(buffer));

    //... poi il pin
    index = snprintf(buffer+index, pinLen+1, pinSuffix);
    printf99_FN("buffer: <%s> len: %d\n", buffer, (int)strlen(buffer));

}

#if 0
int main() {
    printf("Hello World\n");
    char pinID[16]; // name_len + 1 + 6+2
    const size_t MAXLENGTH = sizeof(pinID)-1;
    printf("MAXLENGTH: %d\n", (int)MAXLENGTH);
    setPinID(pinID, MAXLENGTH, "123456789012345678901234567890", 10, false);
    printf("pinID: <%s> len: %d\n", pinID, (int)strlen(pinID));
    if (strlen(pinID) > MAXLENGTH) {
        printf("ERRORE: lunghezza superata\n");
    }


    return 0;
#endif

