/*
// updated by ...: Loreto Notarantonio
// Date .........: 03-02-2025 15.40.18
*/


#include <Arduino.h>

// char msg[] = "1,20,300,4000,50000";
char msg[] = "Loreto , ciao ,300 ,4000,50000";

#define printf Serial.printf




// https://forum.arduino.cc/t/split-string-to-array/593683/9
void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.print(F("Parsing String: "));
  Serial.println(msg);
  char* ptr = strtok(msg, ",");
  byte i = 0;
  Serial.println(F("index\ttext\tNumeric Value"));
  while (ptr) {
    Serial.print(i);
    Serial.print(F("\t\""));
    Serial.println(ptr); // this is the ASCII text we want to transform into an integer
    // Serial.print(F("\"\t"));
    // Serial.println(atol(ptr)); // atol(ptr) will be your long int you could store in your array at position i. atol() info at http://www.cplusplus.com/reference/cstdlib/atol
    ptr = strtok(NULL, ",");
    i++;
  }
}

void loop() {}