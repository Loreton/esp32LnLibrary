/*
// updated by ...: Loreto Notarantonio
// Date .........: 26-02-2025 18.03.33
vedere test_file: /home/loreto/lnProfile/appls/Arduino-ESP32/Esp32TEST/LnFunctions/src/splitString_ok.cpp
*/



#ifndef __LN_STRING_H__
    #define __LN_STRING_H__

    uint8_t splitString(char *msg, const char *delim, bool safeInputString); // safeInputString=truen NON modifica l'originale
    char   *getWord(char *msg, const char *delim, int8_t word_nr);             // non modifica l'originale

    bool ln_isString(const char *);  // esistono altri isString
    int32_t stringToSeconds(char *input_str, const char *delim);

#endif