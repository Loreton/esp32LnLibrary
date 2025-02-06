/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>
#include "main.h"






const char * PROGMEM mySSID [] = {"Casetta",      "cpe210",        "OpenWrt",      "LoretoHotSpot", "Loreto.net",   "WebPocket-4545"};
const char * PROGMEM myPASS [] = {"CasettaPassw", "cpe210Passw", "OpenWrtPassw", "LoretohsPassw",      "LoretonetPassw",    "WebSockPassw"};
const uint8_t SSID_COUNT = sizeof(mySSID) / sizeof(mySSID[0]);
const size_t mySSID_size = sizeof(mySSID);
const size_t myPASS_size = sizeof(myPASS);


const char PROGMEM *wifiMultiSSID_ns = "wifiMultiSSID"; // namespace


// preparazioni variabili
const char *ssid_key_name="SSID";
const char *pass_key_name="PASSW";
const char *mySSID_buffer[mySSID_size] = {}; // Just making a buffer (array) big enough.
size_t read_len=0;
size_t write_len=0;


#if 0
NON FUNZIONA
// ##########################################################
// #
// ##########################################################
size_t readArray(const char *nameSpace, const char* key_name, const char *array_buffer[], size_t array_size) {
// size_t readArray(const char *nameSpace, const char* key_name, void *array_buffer, size_t array_size) {
    Preferences preferences;
    // Serial.printf("Retrieving key %s from the Preferences namespace\n", key_name);

    preferences.begin(nameSpace, RO_MODE);

    read_len = preferences.getBytes(key_name, &array_buffer, array_size);
    preferences.end();

    if (read_len != array_size) {
        Serial.printf("ERROR - read len [%d] is different from origin [%d]\n", read_len, array_size);
    } else {
        Serial.printf("read len [%d] was as expected [%d]\n", read_len, array_size);
    }


    return read_len;
}

// ##########################################################
// #
// ##########################################################
size_t writeArray(const char * nameSpace, const char *key_name, const char *array_buffer[],  size_t array_size) {
// size_t writeArray(const char * nameSpace, const char *key_name, void *array_buffer,  size_t array_size) {
    Preferences preferences;

    preferences.begin(nameSpace, RW_MODE);
    Serial.printf("Storing key %s into the Preferences namespace %s\n", key_name, nameSpace);
    write_len = preferences.putBytes(key_name, &array_buffer, array_size );
    preferences.end();


    if (write_len != array_size) {
        Serial.printf("ERROR - write len [%d] is different from expected [%d]\n", write_len, array_size);
    } else {
        Serial.printf("OK - write len [%d] was as expected [%d]\n", write_len, array_size);
    }

    return write_len;

}

// ##########################################################
// #
// ##########################################################
void setupMultiSSID1(bool fClear) {
    Preferences preferences;

    // if (fClear) {
    //     Serial.printf("\nClearing preferences area\n");
    //     preferences.clear();                     // delete any previous keys in this namespace
    // }

    Serial.printf("\n\nReading array\n");

    // allocate buffer to retrieve data
    const char *mySSID_buffer[mySSID_size] = {}; // Just making a buffer (array) big enough.
    size_t read_len = readArray(wifiMultiSSID_ns, ssid_key_name, mySSID_buffer, mySSID_size);
    if (read_len != mySSID_size) {
        Serial.printf("ERROR - read len [%d] is different from origin [%d]\n", read_len, mySSID_size);
        size_t write_len = writeArray(wifiMultiSSID_ns, ssid_key_name, mySSID, mySSID_size);
        if (write_len) {
            read_len = readArray(wifiMultiSSID_ns, ssid_key_name, mySSID_buffer, mySSID_size);
        }
    }

    if (read_len) {
        const char *ptr;
        Serial.print("original: ");
        Serial.println(mySSID[0]);
        Serial.print("read: ");
        Serial.println(mySSID_buffer[0]);
        ptr = &mySSID_buffer[0];
        Serial.print("read: ");
        Serial.println(ptr);
    }

}
#endif





// ##########################################################
// #
// ##########################################################
void setupMultiSSID_OK(bool fClear) {
    Preferences preferences;

    const char *nameSpace = wifiMultiSSID_ns;
    preferences.begin(nameSpace, RW_MODE);

    if (fClear) {
        preferences.clear();                     // delete any previous keys in this namespace
    }

    // preparazioni variabili
    const char *ssid_key_name="SSID";
    const char *pass_key_name="PASSW";
    const char *mySSID_buffer[mySSID_size] = {}; // Just making a buffer (array) big enough.
    size_t read_len=0;
    size_t write_len=0;


    // ------------------------------
    // - retrieving data into buffer
    // ------------------------------
    Serial.printf("\n\nReading array\n");
    Serial.printf("Retrieving key %s from the Preferences namespace %s\n", ssid_key_name, nameSpace);
    read_len = preferences.getBytes(ssid_key_name, &mySSID_buffer, mySSID_size);


    // ------------------------------
    // - controllo lettura
    // ------------------------------
    if (read_len != mySSID_size) {
        Serial.printf("ERROR - read len [%d] is different from origin [%d]\n", read_len, mySSID_size);

        // - scrivi
        Serial.printf("Storing key %s into the Preferences namespace %s\n", ssid_key_name, nameSpace);
        write_len = preferences.putBytes(ssid_key_name, &mySSID, mySSID_size);
        if (write_len) { // re-read
            read_len = preferences.getBytes(ssid_key_name, &mySSID_buffer, mySSID_size);
        }

    } else {
        Serial.printf("OK - read len [%d] was as expected [%d]\n", read_len, mySSID_size);
    }

    preferences.end();



    if (read_len) {
        Serial.print("original: ");
        Serial.println(mySSID[0]);
        Serial.println(mySSID[4]);
        Serial.print("read: ");
        Serial.println(mySSID_buffer[0]);
        Serial.println(mySSID_buffer[4]);
    }

}

#if 0
// ##########################################################
// #
// ##########################################################
void setupMultiSSID2(bool fClear, const char *key_name, void *buffer1) {
    Preferences preferences;

    const char *nameSpace = wifiMultiSSID_ns;
    preferences.begin(nameSpace, RW_MODE);

    if (fClear) {
        preferences.clear();                     // delete any previous keys in this namespace
    }

    const char buffer[mySSID_size] = {}; // Just making a buffer (array) big enough.
    const char *ptr = &buffer[0]; // Just making a buffer (array) big enough.
    // ------------------------------
    // - retrieving data into buffer
    // ------------------------------
    Serial.printf("\n\nReading array\n");
    Serial.printf("Retrieving key %s from the Preferences namespace %s\n", key_name, nameSpace);
    read_len = preferences.getBytes(key_name, &buffer[0], mySSID_size);


    // ------------------------------
    // - controllo lettura
    // ------------------------------
    if (read_len != mySSID_size) {
        Serial.printf("ERROR - read len [%d] is different from origin [%d]\n", read_len, mySSID_size);

        // - scrivi
        Serial.printf("Storing key %s into the Preferences namespace %s\n", key_name, nameSpace);
        write_len = preferences.putBytes(key_name, &mySSID, mySSID_size);
        if (write_len) { // re-read
            read_len = preferences.getBytes(key_name, &buffer, mySSID_size);
        }

    } else {
        Serial.printf("OK - read len [%d] was as expected [%d]\n", read_len, mySSID_size);
    }

    preferences.end();



    if (read_len) {
        Serial.print("original: ");
        Serial.println(mySSID[0]);
        Serial.println(mySSID[4]);
        Serial.print("read: ");
        // Serial.println(ptr[0]);
        // Serial.println(ptr[4]);
        Serial.println(buffer[0]);
        Serial.println(buffer[4]);
    }

}

// ##########################################################
// #
// ##########################################################
void setupMultiSSID0(bool fClear) {

    // preparazioni variabili
    const char *ssid_key_name1="SSID";
    const char *pass_key_name1="PASSW";
    const char PROGMEM *mySSID_buffer1[mySSID_size] = {}; // Just making a buffer (array) big enough.
    size_t read_len=0;
    size_t write_len=0;

    setupMultiSSID2(fClear, ssid_key_name1, mySSID_buffer1);

    // ------------------------------
    // - controllo lettura
    // ------------------------------
}

#endif


#if 0

    preferences.begin(nameSpace, RW_MODE);
    Serial.printf("Storing key %s into the Preferences namespace %s", key_name, nameSpace);
    size_t write_len = preferences.putBytes(nameSpace, &array_buffer, array_size );
    preferences.end();


    if (write_len != array_size) {
        Serial.printf("ERROR - write len [%d] is different from expected [%d]\n", write_len, array_size);
    }

    return write_len;



    size_t read_len = readArray(wifiMultiSSID_ns, "mySSID", mySSID_buffer, mySSID_size);
    if (read_len != mySSID_size) {
        Serial.printf("ERROR - read len [%d] is different from origin [%d]\n", read_len, mySSID_size);
        size_t write_len = writeArray(wifiMultiSSID_ns, "mySSID", mySSID, mySSID_size);
    }

}
#endif




void setup___() {

#if 0
    Preferences mySketchPrefs;
    Serial.begin(115200);
    delay(250);

    mySketchPrefs.begin("myPrefs", RW_MODE);   // open (or create) the namespace
                                               //  "myPrefs" in RW mode
    mySketchPrefs.clear();                     // delete any previous keys in this namespace

    // Create an array of test values. We're using hex numbers
    //  throughout to better show how the bytes move around.
    int16_t myArray[] = { 0x1112, 0x2122, 0x3132, 0x4142, 0x5152, 0x6162, 0x7172 };

    Serial.println("Printing myArray...");
    for (int i = 0; i < sizeof(myArray) / sizeof(int16_t); i++) {
        Serial.print(myArray[i], HEX); Serial.print(", ");
    }
    Serial.println("\n");
    // In the next statement, the second sizeof() needs
    //  to match the data type of the elements of myArray
    Serial.print("The number of elements in myArray is: ");
    Serial.println( sizeof(myArray) / sizeof(int16_t) );
    Serial.print("But the size of myArray in bytes is: ");
    Serial.println( sizeof(myArray) );
    Serial.println("");

    Serial.println("Storing myArray into the Preferences namespace 'myPrefs' against the key 'myPrefsBytes'.");
    // Note: in the next statement, to store the entire array, we must use the
    //  size of the array in bytes, not the number of elements in the array.
    mySketchPrefs.putBytes( "myPrefsBytes", myArray, sizeof(myArray) );
    Serial.print("The size of \"myPrefsBytes\" is (in bytes): ");
    Serial.println( mySketchPrefs.getBytesLength("myPrefsBytes") );
    Serial.println("");

    int16_t myIntBuffer[20] = {}; // No magic about 20. Just making a buffer (array) big enough.
    Serial.println("Retrieving the value of myPrefsBytes into myIntBuffer.");
    Serial.println("   - Note the data type of myIntBuffer matches that of myArray");
    mySketchPrefs.getBytes("myPrefsBytes", myIntBuffer,
                           mySketchPrefs.getBytesLength("myPrefsBytes"));

    Serial.println("Printing myIntBuffer...");
    // In the next statement, sizeof() needs to match the data type of the elements of myArray
    for (int i = 0; i < mySketchPrefs.getBytesLength("myPrefsBytes") / sizeof(int16_t); i++) {
       Serial.print(myIntBuffer[i], HEX); Serial.print(", ");
    }
    Serial.println("\n");

    Serial.println(
      "We can see how the data from myArray is actually stored in the namespace as follows.");
    uint8_t myByteBuffer[40] = {}; // No magic about 40. Just making a buffer (array) big enough.
    mySketchPrefs.getBytes("myPrefsBytes", myByteBuffer, mySketchPrefs.getBytesLength("myPrefsBytes"));

    Serial.println("Printing myByteBuffer...");
    for (int i = 0; i < mySketchPrefs.getBytesLength("myPrefsBytes"); i++) {
       Serial.print(myByteBuffer[i], HEX); Serial.print(", ");
    }
    Serial.println("");
#endif

}
