/*
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 18.06.48
*/


#include <Arduino.h>


#define printf  Serial.printf
#define println println



/* ###################################################
split type String
#######################################################*/
void splitString_OK(String input, const char *delim) {
    // convertione string to cString
    println("");
    uint8_t lun = input.length();
    char  cString[lun+1];
    char *cStr = &cString[0];
    char *ptr = &cString[0];
    // char *ptr0;
    input.toCharArray(cStr, lun+1);

    for (int8_t i; i<=lun; i++) {
        // Serial.print(*ptr);
        if (cStr[i] == ';') {
            cStr[i]='\0';
            println(ptr);
            ptr = &cStr[i+1];
        }
    }
    println(ptr);

}







/* ###################################################
split cType String polimorfismo
#######################################################*/
void split_anyString(char *inputStr, char delim) {
    println("\n\nc type");
     char *cStr = &inputStr[0];
     char *ptr = &inputStr[0];
    uint8_t lun = strlen(inputStr);

    for (int8_t i; i<=lun; i++) {
    // for (int8_t i=0; *ptr != '\0'; i++) {
        if (cStr[i] == delim) {
            cStr[i]='\0';
            println(ptr);
            ptr = &cStr[i+1];
        }
    }
    println(ptr);
}



/* ###################################################
split type String polimorfismo
#######################################################*/
void split_anyString(String input, char delim) {
    println("\n\nString type");
    uint8_t lun = input.length();
    char  cString[lun+1];
    input.toCharArray(&cString[0], lun+1);
    split_anyString(&cString[0], delim);

}





int main() {
    String xxx = "Here; is some; sample;100;data;1.414;1020";
    splitString_OK(xxx, ";");

    char myString[] = "Here; is some; sample;100;data;1.414;1020";
    split_anyString(xxx, ';');
    split_anyString(myString, ';');

    String myString = "Here  is some  sample   100   data   1.414    1020";
    split_anyString(xxx, ';');
    split_anyString(myString, ';');
}


void setup() {
    Serial.begin(115200);
    delay(2000);
    main();
}



void loop() {}


