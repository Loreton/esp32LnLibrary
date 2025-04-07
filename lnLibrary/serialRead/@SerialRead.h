//
// updated by ...: Loreto Notarantonio
// Date .........: 07-04-2025 09.13.35
//

#ifndef __LN_SERIAL_READ_H__
    #define __LN_SERIAL_READ_H__


    extern const byte MAX_NUMCHARS;

    int32_t     readSerialInt(void);
    // bool        waitForChar(char chr);
    // bool        waitForChar(char chr, const char *descr="");
    bool        waitForChar(const char *descr, char chr);
    char        waitForAnyChar(const char *chars);
    // uint8_t     readSerialData(const char *chars="", uint8_t max_chars=MAX_NUMCHARS);
    uint8_t     readSerialData(const char *chars="", uint8_t max_chars=MAX_NUMCHARS, const char exits='x');

#endif