/*
// updated by ...: Loreto Notarantonio
// Date .........: 13-02-2025 09.38.05
*/


#include <Arduino.h>

// char msg[] = "1,20,300,4000,50000";

#define printf Serial.printf
#define println Serial.println


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
    printf("sString3: %s\n", sString.c_str());
    printf("cString3: %s\n", cString);
}



#if 0

int  main() {
    StringToChar();
    println();
    charToString();




    printf("----- completed ---- \n");

}

void setup() {
    Serial.begin(115200);
    delay(2000);
    main();
}

void loop() {}
#endif