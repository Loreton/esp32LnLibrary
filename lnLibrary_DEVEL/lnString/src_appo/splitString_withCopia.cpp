/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-02-2025 16.44.56
*/


#include <Arduino.h>


struct splitTokens {
   uint8_t lun = 0;
   const char *token = "\0";
};

// struct splitTokens tokens[10];
char *token[10];








/* ###################################################
split cType String
inserisce un EOS al posto del delim
"word1; wordd2;,,,"
#######################################################*/
void split_cStringType(char *inputStr, char delim) {
    // convertione string to cString
    Serial.println("\n\nc type");
    char *cStr = &inputStr[0];

    // cacolo area da allocare ed allocazione
    uint8_t lun = strlen(inputStr);
    char buffer[lun+1];
    char *dest = &buffer[0];


    char *ptr = cStr;
    int8_t word_nr=0;
    int8_t start_word=0;

    token[word_nr] = &dest[0];        // first token
    for (int8_t i; i<=lun; i++) {
        dest[i] = cStr[i]; // copy char

        // closing token
        Serial.printf("i: %d \n", i);
        if (cStr[i] == delim) {
            dest[i]='\0';
            Serial.printf("word nr: %d - starting %d\n", word_nr, start_word);
            Serial.printf("word: %s \n", &dest[start_word]);



            token[word_nr] = &dest[start_word]; // save token
            Serial.printf("token word: %s \n", &token[word_nr]);

            // Serial.printf("word nr: %d - %s - %s\n", word_nr, token[word_nr], dest[start_word]);

            // prepare next word
            start_word = i+1;                  // next starting word
            word_nr += 1;                  // next starting word
            Serial.printf("word nr: %d - starting %d\n", word_nr, start_word);
            Serial.printf("\n");

        }
    }
    Serial.println(ptr);

}










void setup() {
    Serial.begin(115200);
    Serial.println();
    delay(1000);

    char myString[] = "Here; is some; sample;100;data;1.414;1020";
    split_cStringType(myString, ';');

    Serial.println( myString);

}






void loop()
{
}

