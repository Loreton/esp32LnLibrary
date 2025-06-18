#!/usr/bin/bash
#
###############################
#
###############################

# export ln_esp32_WROOM_32E_MODULE_2RELAY=1
# export ln_esp32_WROOM_32E_MODULE=2
# export ln_esp32_BOARD_TYPE=$ln_esp32_WROOM_32E_MODULE

# export ln_esp32_PRODUCTION=1
# export ln_esp32_DEVEL=2
# export ln_esp32_RELEASE_TYPE=$ln_esp32_DEVEL
# export ln_esp32_LORETO='LORET'
# echo "ln_esp32_RELEASE_TYPE:    $ln_esp32_RELEASE_TYPE"
# echo "ln_esp32_BOARD_TYPE:      $ln_esp32_BOARD_TYPE"

myBF=""
myBF="${myBF} -Dln_esp32_WROOM_32E_MODULE_2RELAY=1"
myBF="${myBF} -Dln_esp32_WROOM_32E_MODULE=2"
myBF="${myBF} -Dln_esp32_PRODUCTION=1"
myBF="${myBF} -Dln_esp32_DEVEL=2"
myBF="${myBF} -Dln_esp32_LORETO='LORETO'"

myBF="${myBF} -Dln_esp32_RELEASE_TYPE=${ln_esp32_DEVEL}"
myBF="${myBF} -Dln_esp32_BOARD_TYPE=${ln_esp32_WROOM_32E_MODULE}"



# export PLATFORMIO_BUILD_FLAGS=-DRELEASE_TYPE=$DEVEL -DESP32_BOARD_TYPE=$ESP32_WROOM_32E_MODULE
export PLATFORMIO_BUILD_FLAGS=$myBF
echo $PLATFORMIO_BUILD_FLAGS



