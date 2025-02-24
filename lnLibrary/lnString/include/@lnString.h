/*
// updated by ...: Loreto Notarantonio
// Date .........: 23-02-2025 19.00.17
vedere test_file: /home/loreto/lnProfile/appls/Arduino-ESP32/Esp32TEST/LnFunctions/src/splitString_ok.cpp
*/

#include <Arduino.h>

uint8_t splitString(char *msg, const char *delim, bool safeInputString); // safeInputString=truen NON modifica l'originale
char   *getWord(char *msg, const char *delim, int8_t word_nr);             // non modifica l'originale

bool ln_isString(const char *);  // esistono altri isString