/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-07-2025 12.10.43
*/


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
#include "lnLogger.h"



// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.
// The return value must NOT be deallocated using free() etc.
void ltrim_ok(char *s) {
    LOG_INFO("---------- ltrim --------\n");
    // Pointer to the beginning of the trimmed string
    char *ptr = s;
    char *ptr0 = s;
    // Skip leading spaces
    while (*s ==' ' || *s =='\t' || *s =='\n') {s++; }

    // Shift remaining characters to the beginning of string so th pointer of string is unchanged
    while (*ptr++ = *s++);
    s-=2;
    int lun=s-ptr0;
    int len=strlen(s);
    LOG_INFO("\t1. lastchar: '%c'\n", *s); // verifica
    LOG_INFO("\t2. '%s' lun: %d (strlen: %d)\n", s, lun, len);
}


// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.
// The return value must NOT be deallocated using free() etc.

void ltrim(char *s) {
    LOG_INFO("---------- ltrim --------\n");
    // Pointer to the beginning of the trimmed string
    char *dest = s;
    char *ptr = s;
    char *begin = s;

    // Skip leading spaces
    while (*ptr ==' ' || *ptr =='\t' || *ptr =='\n') {ptr++; }

    // Shift remaining characters to the beginning of string so th pointer of string is unchanged
    while (*dest++ = *ptr++);
    ptr--; // points to '\0'
    int lun=ptr-s;
    LOG_INFO("\t1. first char: '%c'\n", *s); // verifica
    LOG_INFO("\t2. last  char: '%c'\n", *(--ptr)); // verifica
    LOG_INFO("\t3. '%s' lun: %d (strlen: %d) -non ho capito perché diverso ma non importa-\n", s, lun, (int) strlen(s));
}


void rtrim(char *s) {
    LOG_INFO("---------- rtrim --------\n");

    char *p = s;
    while (*p++); // goto end of string
    p--; // 1 per l'ultimo ++
    int lun=p-s;
    int len=strlen(s);
    LOG_INFO("\t1. '%s' lun: %d (strlen: %d)\n", s, lun, len);
    // LOG_INFO("lun: %d\n", lun);
    // LOG_INFO("after rtrim: '%s' lun: %d (strlen: %d)\n", s, lun, strlen(s));
    LOG_INFO("\t2. lastchar: '%c'\n", *p); // verifica

   // move backward and Skip trailing spaces
    p--; // skip  '\0'
    while (*p ==' ' || *p =='\t' || *p =='\n') {
        p--;
    }
    *(++p)=0;
    lun=p-s;
    len=strlen(s);
    LOG_INFO("\t3. lastchar: '%c'\n", *(--p)); // verifica
    LOG_INFO("\t4. '%s' lun: %d (strlen: %d)\n", s, lun, len);

}


#if 0
int main() {
    char s[] = " \t  Hello, Geeks!    x";
    LOG_INFO("original: '%s'\n", s);
    int lun=strlen(s);
    LOG_INFO("lun0: %d\n", lun);

    ltrim(s);
    LOG_INFO("\n\n");
    rtrim(s);
    int len=strlen(s);
    LOG_INFO("final: '%s' lun: %d\n", s, len);
    return 0;
}

void setup() {
    // Serial.begin(115200);
    // delay(2000);
    main();
}

 

void loop() {}


#endif