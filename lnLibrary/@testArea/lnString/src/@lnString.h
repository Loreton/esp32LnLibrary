/*
// updated by ...: Loreto Notarantonio
// Date .........: 05-02-2025 19.24.14
vedere test_file: /home/loreto/lnProfile/appls/Arduino-ESP32/Esp32TEST/LnFunctions/src/splitString_ok.cpp
*/

#include <Arduino.h>

uint8_t splitString(char *msg, const char *delim, bool safeInputString); // safeInputString=truen NON modifica l'originale
uint8_t splitString(String msg, const char *delim);                      // non modifica l'originale
char   *getWord(char *msg, const char *delim, uint8_t word_nr);             // non modifica l'originale

