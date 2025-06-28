//
// updated by ...: Loreto Notarantonio
// Date .........: 20-06-2025 16.29.07
//

#pragma once

#include <Arduino.h>


extern const byte MAX_NUMCHARS;

int32_t     readSerialInt(void);
bool        waitForEnter(const char *descr=NULL);
bool        waitForChar(const char *descr, char chr);
char        waitForAnyChar(const char *chars);
uint8_t     readSerialData(const char *chars="", uint8_t max_chars=MAX_NUMCHARS, const char exits='x');
uint8_t     readSerialData2(const char *chars="", char *buffer={}, uint8_t max_chars=MAX_NUMCHARS, const char exits='x');

