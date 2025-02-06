//
// updated by ...: Loreto Notarantonio
// Date .........: 06-02-2025 08.55.46
//

// Adapted from: https://github.com/espressif/arduino-esp32/blob/master/libraries/LittleFS/examples/LITTLEFS_test/LITTLEFS_test.ino
// Project details: https://RandomNerdTutorials.com/esp32-write-data-littlefs-arduino/

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>


// #include <../lnMacros/@lnMacros.h>    // per debug0_nowfn()
#include "@lnMacros.h"
#include "@fileSystem.h"    // per Lisdir() di openFilesystem()




//  You only need to format LittleFS the first time you run a
//  test or else use the LITTLEFS plugin to create a partition
//  https://github.com/lorol/arduino-esp32littlefs-plugin
#define FORMAT_LITTLEFS_IF_FAILED true



// --- essentials
#define debug0_nowfn                //  ln_printf_NowFN
#define debug0_fn                 ln_printf_FN


// #############################################################
// --- apriamo il filesystem
// #############################################################
bool openLittleFS() {

    // if (!fs.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    //     debug0_nowfn("LittleFS Mount Failed\n");
    //     return false;
    // }

    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
        debug0_fn("LittleFS Mount Failed\n");
        return false;
    }
    listDir(LittleFS, "/", 1); // List the directories up to one level beginning at the root directory
    return true;
}



// #############################################################
// #  list dir
// #############################################################
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    debug0_nowfn("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        debug0_fn("- failed to open directory\n");
        return;
    }
    if(!root.isDirectory()){
        debug0_fn(" - not a directory\n");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            debug0_nowfn("  DIR : %s\n", file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            debug0_nowfn("  FILE: %s\tSIZE: %d\n", file.name(), file.size());
        }
        file = root.openNextFile();
    }
}


// #############################################################
// #  Read text file
// #  Converts to a c style String (add '\0') to be orrectly used.
// #############################################################
const char * readTextFile(fs::FS &fs, const char * path){
    const char *fileContent;

    debug0_nowfn("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        debug0_fn("- failed to open file for reading");
        return "";
    }

    size_t filesize = file.size();
    debug0_nowfn(" [%d bytes]\n", filesize);
    debug0_nowfn("- read from file:\n");
    String data;
    if (file.available()) {
        data = file.readString();
        fileContent = data.c_str(); // Converts the contents of a String as a C-style, null-terminated string.
        // debug0_nowfn("type of data: %s - %s\n", typeStr(fileContent), fileContent);
        /*
        debug0_nowfn("type of data: %s - %s\n", typeStr(data), data);
        */
    }
    file.close();
    return fileContent;
}



bool writeTextFile(fs::FS &fs, const char * path, const char * message) {
    Serial.printf("Writing file: %s\n", path);
    bool rcode = false;
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("- failed to open file for writing");
        return rcode;
    }
    // message += '\0';
    if (file.println(message)) {
        Serial.println("- file written");
        rcode = true;
    } else {
        Serial.println("- write failed");
    }
    file.close();
    return rcode;
}



// ##################################################################
// #
// ##################################################################
void writeBinData(fs::FS &fs, const char * path, byte *data, size_t size) {
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("- failed to open file for writing");
        return;
    }

    const size_t bytesWritten = file.write(data, size);
    file.close();

    if (bytesWritten < size) {
        Serial.println("Write error");
    } else {
        Serial.println("Write OK");
    }
}


void renameFile(fs::FS &fs, const char * path1, const char * path2) {
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        debug0_nowfn("- file renamed\n");
    } else {
        debug0_fn("- rename failed\n");
    }
}

void deleteFile(fs::FS &fs, const char * path) {
    debug0_nowfn("Deleting file: %s\n", path);
    if (fs.remove(path)) {
        debug0_nowfn("- file deleted\n");
    } else {
        debug0_fn("- delete failed\n");
    }
}


#if 0
void testFileSystem() {
    Serial.begin(115200);

    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    listDir(LittleFS, "/", 1); // List the directories up to one level beginning at the root directory
    createDir(LittleFS, "/mydir"); // Create a mydir folder
    writeFile(LittleFS, "/mydir/hello1.txt", "Hello1"); // Create a hello1.txt file with the content "Hello1"
    listDir(LittleFS, "/", 1); // List the directories up to one level beginning at the root directory
    deleteFile(LittleFS, "/mydir/hello1.txt"); //delete the previously created file
    removeDir(LittleFS, "/mydir"); //delete the previously created folder
    listDir(LittleFS, "/", 1); // list all directories to make sure they were deleted

    writeFile(LittleFS, "/hello.txt", "Hello "); //Create and write a new file in the root directory
    appendFile(LittleFS, "/hello.txt", "World!\n"); //Append some text to the previous file
    readFile(LittleFS, "/hello.txt"); // Read the complete file
    renameFile(LittleFS, "/hello.txt", "/foo.txt"); //Rename the previous file
    readFile(LittleFS, "/foo.txt"); //Read the file with the new name
    deleteFile(LittleFS, "/foo.txt"); //Delete the file
    testFileIO(LittleFS, "/test.txt"); //Testin
    deleteFile(LittleFS, "/test.txt"); //Delete the file

    Serial.println( "Test complete" );
}

#endif
