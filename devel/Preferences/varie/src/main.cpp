/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>

#define MAIN_CPP
#include "main.h"
#include "wifiKeyValue.h"
#include "wifiMultiSSID.h"
#include "Prefs2Struct.h"
#include "myStruct.h"



// ##########################################################
// #
// ##########################################################
void setup() {
    Serial.begin(115200);
    Serial.println("waiting 2 seconds ..");
    delay(2000);

    // readWiFi_keyValue();
    // setupMultiSSID_OK(false);
    // setupMultiSSID0(false);
    structScheduler *wSched = myStructSetup();
    // setupStruct();
    if (!wSched) {
        Serial.println("ERRORE - ERRORE - ERRORE - ERRORE - ERRORE - ERRORE - ");
        return;
    }
    Serial.println("sono in main");
    for (int i = 0; i < MAX_WEEKLY_SCHEDULER; ++i) {
        // Serial.println(weeklySched[i].nr);
        // Serial.println(weeklySched[i].day_map);
        Serial.println(wSched[i].nr);
        Serial.println(wSched[i].day_map);
    }


}



// ##########################################################
// #
// ##########################################################
void loop() {
  // put your main code here, to run repeatedly:
}


