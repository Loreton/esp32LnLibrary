/*
// updated by ...: Loreto Notarantonio
// Date .........: 30-06-2025 16.18.40
*/


// ---------------------------------
// lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL 0
#include "../lnMainInclude/@logMacros.h"



// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.
// The return value must NOT be deallocated using free() etc.
void ltrim_ok(char *s) {
    printf1_NFN("---------- ltrim --------\n");
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
    printf1_NFN("\t1. lastchar: '%c'\n", *s); // verifica
    printf1_NFN("\t2. '%s' lun: %d (strlen: %d)\n", s, lun, len);
}


// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.
// The return value must NOT be deallocated using free() etc.

void ltrim(char *s) {
    printf1_NFN("---------- ltrim --------\n");
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
    printf1_NFN("\t1. first char: '%c'\n", *s); // verifica
    printf1_NFN("\t2. last  char: '%c'\n", *(--ptr)); // verifica
    printf1_NFN("\t3. '%s' lun: %d (strlen: %d) -non ho capito perché diverso ma non importa-\n", s, lun, (int) strlen(s));
}


void rtrim(char *s) {
    printf1_NFN("---------- rtrim --------\n");

    char *p = s;
    while (*p++); // goto end of string
    p--; // 1 per l'ultimo ++
    int lun=p-s;
    int len=strlen(s);
    printf1_NFN("\t1. '%s' lun: %d (strlen: %d)\n", s, lun, len);
    // printf1_NFN("lun: %d\n", lun);
    // printf1_NFN("after rtrim: '%s' lun: %d (strlen: %d)\n", s, lun, strlen(s));
    printf1_NFN("\t2. lastchar: '%c'\n", *p); // verifica

   // move backward and Skip trailing spaces
    p--; // skip  '\0'
    while (*p ==' ' || *p =='\t' || *p =='\n') {
        p--;
    }
    *(++p)=0;
    lun=p-s;
    len=strlen(s);
    printf1_NFN("\t3. lastchar: '%c'\n", *(--p)); // verifica
    printf1_NFN("\t4. '%s' lun: %d (strlen: %d)\n", s, lun, len);

}


#if 0
int main() {
    char s[] = " \t  Hello, Geeks!    x";
    printf1_NFN("original: '%s'\n", s);
    int lun=strlen(s);
    printf1_NFN("lun0: %d\n", lun);

    ltrim(s);
    printf1_NFN("\n\n");
    rtrim(s);
    int len=strlen(s);
    printf1_NFN("final: '%s' lun: %d\n", s, len);
    return 0;
}

void setup() {
    // Serial.begin(115200);
    // delay(2000);
    main();
}

 

void loop() {}


#endif