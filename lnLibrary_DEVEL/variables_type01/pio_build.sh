#!/usr/bin/bash
#
###############################
#
###############################
function __platformio_upload() {
    local number="${1:-0}" # default 0
    local number="${1}" # default nessuno
    [[ $number =~ ^[0-2]+$ ]] || { echo "Enter a valid number [0-2]"; return; }
    local usb_port="/dev/ttyUSB${number}"
    pio run -j1 -t upload --upload-port ${usb_port}
}


function __platformio_monitor() {
    local number="${1:-0}" # default 0
    local number="${1}" # default nessuno
    [[ $number =~ ^[0-2]+$ ]] || { echo "Enter a valid number [0-2]"; return; }
    local usb_port="/dev/ttyUSB${number}"
    pio device monitor --port  ${usb_port} --baud 115200
}


export VARIABILE_01=1

export ESP32_WROOM_32E_2RELAY_MODULE=1
export ESP32_WROOM_32E_MODULE=2
export ESP32_BOARD_TYPE=$ESP32_WROOM_32E_MODULE

export PRODUCTION=1
export DEVEL=2
export RELEASE_TYPE=$DEVEL


echo "RELEASE_TYPE:     $RELEASE_TYPE"
echo "ESP32_BOARD_TYPE: $ESP32_BOARD_TYPE"

# export PLATFORMIO_BUILD_FLAGS=-DRELEASE_TYPE=$DEVEL -DESP32_BOARD_TYPE=$ESP32_WROOM_32E_MODULE

# pio run -j1
# __platformio_upload 0 && __platformio_monitor 0

