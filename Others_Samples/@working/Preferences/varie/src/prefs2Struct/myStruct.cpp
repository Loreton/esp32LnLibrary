/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>
#define _MYSTRUCT_CPP
#include "myStruct.h"
#include "main.h"




structScheduler *myStructSetup(bool fClear) {

    const char *nameSpace="myStruct";
    const char *key_name="WSched";
    size_t array_size = MAX_WEEKLY_SCHEDULER * sizeof(structScheduler);
    size_t schLen = 0;

    // for (int i = 0; i < MAX_WEEKLY_SCHEDULER; ++i) {
    //     weeklySched[i].nr = i;
    //     weeklySched[i].relay_nr = i+1;
    // }

    Serial.printf("structScheduler:         %d\n", sizeof(structScheduler));
    Serial.printf("weeklySched single:      %d\n", sizeof(weeklySched[0]));
    Serial.printf("weeklySched array:       %d\n", sizeof(weeklySched));
    Serial.printf("weeklySched array:       %d\n", array_size);

    Preferences prefs;

    prefs.begin(nameSpace, RO_MODE);
        schLen = prefs.getBytesLength(key_name); // bytes del key
        Serial.printf("schLen           :       %d\n", schLen);
    prefs.end();


    // --------------------------------
    // - Write data
    // --------------------------------
    if (schLen == 0) { // namespace is empty
        prefs.begin(nameSpace, RW_MODE);
        Serial.printf("Storing key %s into the Preferences namespace %s\n", key_name, nameSpace);
        schLen = prefs.putBytes(key_name, weeklySched, array_size);
        size_t schLen1 = prefs.getBytesLength(key_name);
        prefs.end();

        Serial.printf("schLen after write:       %d\n", schLen);
        Serial.printf("schLen1 after write:      %d\n", schLen1);

    }

    // --------------------------------
    // controlla il valore sia se ha fatto la write sia che i dati esistano di già
    // --------------------------------
    if (schLen % array_size) {  // simple check that data fits
        Serial.printf("ERROR1 - data len [%d] is different from origin [%d]\n", schLen, array_size);
        return NULL;
    }


    // --------------------------------
    // - read data
    // --------------------------------
    // schLen = prefs.getBytesLength(key_name); // per sicurezza
    // char buffer[array_size];  // prepare a buffer for the data
    // structScheduler  buffer[MAX_WEEKLY_SCHEDULER];

    prefs.begin(nameSpace, RO_MODE);
        schLen = prefs.getBytes(key_name, ee_weeklySched, array_size);
        // schLen = prefs.getBytes(key_name, ee_weeklySched, array_size);
    prefs.end();

    if (schLen % array_size) {  // simple check that data fits
        Serial.printf("ERROR2 - read len [%d] is different from origin [%d]\n", schLen, array_size);
        return NULL;
    }

    // structScheduler *wSched = (structScheduler *)buffer;  // cast the bytes into a struct ptr
    for (int i = 0; i < MAX_WEEKLY_SCHEDULER; ++i) {
        Serial.println(weeklySched[i].nr);
        Serial.println(weeklySched[i].day_map);
        Serial.println(ee_weeklySched[i].nr);
        Serial.println(ee_weeklySched[i].day_map);
    }

    Serial.println("completed!");
    return ee_weeklySched;
}


