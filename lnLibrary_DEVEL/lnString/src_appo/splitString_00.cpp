/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-02-2025 16.41.14
*/


#include <Arduino.h>


struct splitTokens {
   uint8_t lun;
   const char *token;
};

// String stringSplitResponse[10];

// struct words splittedWords;
const char *splitted[10];
struct splitTokens tokens[10];


/*
char cstring[30];
  char * cptr = &cstring[0];
  String Str = "Hi There !";
  StringToChar(cptr, Str);
*/
void StringToChar(String str, char * &cstr) {
    str.toCharArray(cstr, str.length()+1);
}


int ReadDec(const char * & cp, const char term) {
  int t = 0;
  while ((*cp) && (*cp != term) && (*cp >= '0') && (*cp <= '9')) {
    t = (t * 10) + ((*cp++) - '0');
  }
  if ((*cp) && (*cp == term)) cp++;
  return t;
}








/* ###################################################
split type String
#######################################################*/
void splitString_OK(String input, const char *delim) {
    // convertione string to cString
    Serial.println("");
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
            Serial.println(ptr);
            ptr = &cStr[i+1];
        }
    }
    Serial.println(ptr);

}




uint8_t stringCopy(const char* data, const char *dest) {
    // - calcolo len
    byte len=0;
    const char *ptr = data;

    while (*ptr++) {
        *dest++ = *ptr++;
        len++;
    }
    return len;
}

// - calcolo len
byte stringLen(const char* data) {
    byte len=0;
    const char *ptr = data;
    while (*ptr++) {
        len++;
    }
    return len;
}


/* ###################################################
split cType String
inserisce un EOS al posto del delim
#######################################################*/
void split_cStringType(char *inputStr, char delim) {
    // convertione string to cString
    Serial.println("\n\nc type");
     char *cStr = &inputStr[0];
     char *ptr = &inputStr[0];
    uint8_t lun = strlen(inputStr);


    for (int8_t i; i<=lun; i++) {
        // Serial.print(*ptr);
        if (cStr[i] == delim) {
            cStr[i]='\0';
            Serial.println(ptr);
            ptr = &cStr[i+1];
        }
    }
    Serial.println(ptr);

}


dddd







void setup() {
    Serial.begin(115200);
    Serial.println();
    delay(1000);

    String xxx = "Here; is some; sample;100;data;1.414;1020";
    splitString_OK(xxx, ";");
    // splitString_type01a(xxx, ";");
    char myString[] = "Here; is some; sample;100;data;1.414;1020";
    // split_StringType(xxx, ';');
    // split_cStringType(myString, ';');
    split_anyString(xxx, ';');
    split_anyString(myString, ';');

    Serial.println( myString);
    // split_cStringType(myString, ';');
    // splitString_type01(&myString[0]);
    // splitString_type03(xxx);
    // for (int8_t i; i<sizeof(stringSplitResponse); i++) {
    //     Serial.printf("token #%d: %s", i, stringSplitResponse[i]);
    // }

}






void loop()
{
}

