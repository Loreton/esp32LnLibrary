/*
// updated by ...: Loreto Notarantonio
// Date .........: 02-07-2025 12.19.06
vedere test_file: /home/loreto/lnProfile/appls/Arduino-ESP32/Esp32TEST/LnFunctions/src/splitString_ok.cpp
*/

#pragma once


    uint8_t splitSavingString(char *input_str, const char *delim, bool safe);
    uint8_t splitString(char *msg, const char *delim);

    char   *getWord(char *msg, const char *delim, int8_t word_nr);             // non modifica l'originale

    bool    ln_isString(const char *);  // esistono altri isString
    int32_t stringToSeconds(char *input_str, const char *delim);

    // void setPinID(char buffer[], int MAXLENGTH, const char *name, int pin);
    // const char*  setFnameLine(char buffer[], int MAXLENGTH, const char *path, int lineNo);