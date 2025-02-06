/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
*/


/*
Remove a Namespace
In the Arduino implementation of Preferences, there is no method of completely removing a namespace.
As a result, over the course of several projects, the ESP32 non-volatile storage (nvs)
Preferences partition may become full. To completely erase and reformat the NVS memory used by Preferences,
create a sketch that contains:
*/


#include <nvs_flash.h>

void setup() {
  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init(); // initialize the NVS partition.
  while(true);
}

void loop() {

}

// You should download a new sketch to your board immediately after running the above,
// or it will reformat the NVS partition every time it is powered up.