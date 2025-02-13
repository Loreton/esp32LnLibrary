/*
// updated by ...: Loreto Notarantonio
// Date .........: 13-02-2025 09.43.43
*/

// #define PIPPO
// #ifdef PIPPO
    // #include <Arduino.h>
    // #define printf Serial.printf
// #else
    #include <stdio.h>
    #include <string.h>
// #endif

// char msg[] = "1,20,300,4000,50000";




#if 0
// https://stackoverflow.com/questions/60891646/trim-leading-and-trailing-whitespace-from-string-in-c
char *trim_02(char *s) {
    char *start;
    char *end;
    int len = strlen(s);
    start = s;
    end   = s + len - 1;

    while (*start ==' ' || *start =='\t' || *start =='\n') {start++; }
    // printf("'%s'\n", start);


    while (*end ==' '   || *end =='\t'   || *end =='\n')   {end--; }
    // printf("lastchar: '%c'\n", *end); // verifica
    *(++end)=0;
    bool new_string=false;
    if (new_string) {
        char temp[length + 1];
        int index = 0;

        while (start <= end) {
            temp[index++] = *start;
            start++;
        }

        temp[index] = '\0';

        char *result = malloc(strlen(temp) + 1);
        if (result == NULL)  // check if malloc failed
            return NULL;

        strcpy(result, temp);
        return result;
    }

}
#endif




// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.
// The return value must NOT be deallocated using free() etc.
void ltrim_ok(char *s) {
    printf("---------- ltrim --------\n");
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
    printf("\t1. lastchar: '%c'\n", *s); // verifica
    printf("\t2. '%s' lun: %d (strlen: %d)\n", s, lun, len);
}


// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.
// The return value must NOT be deallocated using free() etc.

void ltrim(char *s) {
    printf("---------- ltrim --------\n");
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
    printf("\t1. first char: '%c'\n", *s); // verifica
    printf("\t2. last  char: '%c'\n", *(--ptr)); // verifica
    printf("\t3. '%s' lun: %d (strlen: %d) -non ho capito perché diverso ma non importa-\n", s, lun, (int) strlen(s));
}


void rtrim(char *s) {
    printf("---------- rtrim --------\n");

    char *p = s;
    while (*p++); // goto end of string
    p--; // 1 per l'ultimo ++
    int lun=p-s;
    int len=strlen(s);
    printf("\t1. '%s' lun: %d (strlen: %d)\n", s, lun, len);
    // printf("lun: %d\n", lun);
    // printf("after rtrim: '%s' lun: %d (strlen: %d)\n", s, lun, strlen(s));
    printf("\t2. lastchar: '%c'\n", *p); // verifica

   // move backward and Skip trailing spaces
    p--; // skip  '\0'
    while (*p ==' ' || *p =='\t' || *p =='\n') {
        p--;
    }
    *(++p)=0;
    lun=p-s;
    len=strlen(s);
    printf("\t3. lastchar: '%c'\n", *(--p)); // verifica
    printf("\t4. '%s' lun: %d (strlen: %d)\n", s, lun, len);

}


#if 0
int main() {
    char s[] = " \t  Hello, Geeks!    x";
    printf("original: '%s'\n", s);
    int lun=strlen(s);
    printf("lun0: %d\n", lun);

    ltrim(s);
    printf("\n\n");
    rtrim(s);
    int len=strlen(s);
    printf("final: '%s' lun: %d\n", s, len);
    return 0;
}

void setup() {
    // Serial.begin(115200);
    // delay(2000);
    main();
}

 

void loop() {}


#endif