/*
# updated by ...: Loreto Notarantonio
# Date .........: 22-07-2024 08.57.18
*/
#include "FS.h"
#include "LittleFS.h"

#ifndef __FILE_SYSTEM_H__
    #define __FILE_SYSTEM_H__




    // --------------------------------------------------
    // --- functions prototypes
    // --------------------------------------------------
    bool            openLittleFS();
    const char *    readTextFile(fs::FS &fs,     const char * path);
    bool            writeTextFile(fs::FS &fs,  const char * path, const char * message);
    void            writeBinData(fs::FS &fs,   const char * path, byte *data, size_t size);
    void            appendFile(fs::FS &fs,     const char * path, const char * message);
    void            renameFile(fs::FS &fs,     const char * path1, const char * path2);
    void            deleteFile(fs::FS &fs,     const char * path);

    void            listDir(fs::FS &fs, const char * dirname, uint8_t levels);

    // String          readTextFile(fs::FS &fs,     const char * path);
    // const char * readTextFile(fs::FS &fs,     const char * path);
#endif