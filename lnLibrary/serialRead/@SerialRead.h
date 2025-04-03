//
// updated by ...: Loreto Notarantonio
// Date .........: 02-04-2025 17.13.48
//

#ifndef __LN_SERIAL_READ_H__
    #define __LN_SERIAL_READ_H__


    extern const byte MAX_NUMCHARS;

    int32_t     readSerialInt(void);
    bool        waitForChar(char chr);
    char        waitForAnyChar(const char *chars);
    uint8_t     readSerialData(const char *chars="", uint8_t max_chars=MAX_NUMCHARS);

#endif