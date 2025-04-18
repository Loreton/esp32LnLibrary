/*
// updated by ...: Loreto Notarantonio
// Date .........: 16-04-2025 18.40.10
*/

#include "Arduino.h"
#include "time.h"
#include "esp_sntp.h"
#include <ESP32Time.h> // ESP32Time.cpp


// ---------------------------------
// loreto headers files
// ---------------------------------
#include "@ln_time.h"


// ---------------------------------
// macros Aliases for LOG
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"




#define EUROPE_ROME_TZ "CET-1CEST,M3.5.0,M10.5.0/3" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

// const char* ntpServers [] = {"pool.ntp.org", "br.pool.ntp.org", "time.nist.gov","2.br.pool.ntp.org"};
const char* ntpServer = "pool.ntp.org";

const char* PROGMEM sntp_status[] = {   "SNTP_SYNC_STATUS_RESET",
                                        "SNTP_SYNC_STATUS_COMPLETED",
                                        "SNTP_SYNC_STATUS_IN_PROGRESS"
                                    };

// #####################################################################
// # https://werner.rothschopf.net/microcontroller/202103_arduino_esp32_ntp_en.htm
// #####################################################################
void cbSyncTime(struct timeval *tv)  { // callback function to show when NTP was synchronized
    uint8_t status = sntp_get_sync_status();
    printf1_NFN("NTP time synched: %d [%s]\n", status, sntp_status[status]);
        // * sntp_get_sync_status(void) //
        // *    0  SNTP_SYNC_STATUS_RESET: Reset status.
        // *    1  SNTP_SYNC_STATUS_COMPLETED: Time is synchronized.
        // *    2  SNTP_SYNC_STATUS_IN_PROGRESS: Smooth time sync in progress.
}



// #####################################################################
// # https://forum.arduino.cc/t/using-ntp-server-to-set-time-possible-misunderstanding/1000465/10
// #####################################################################
void NTPInit(void) { // ntp time sync, it may have an error were it freezes code if wifi failed. yet to work with pointers.
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    sntp_set_time_sync_notification_cb(cbSyncTime);  // set a Callback function for time synchronization notification
    sntp_set_sync_interval(12 * 60 * 60 * 1000UL); // 12 hours
    sntp_set_sync_interval(1 * 2 * 60 * 1000UL); // 2 minuti

    bool method1 = false;
    if (method1) {
        configTime(3600, 3600, ntpServer);
    }
    else {
        configTime(0, 0, ntpServer);    // 0, 0 because we will use TZ in the next line
    }
    setenv("TZ", EUROPE_ROME_TZ, 1);    // Set environment variable with your time zone
    tzset();


    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        rtc.setTimeStruct(timeinfo);
        // Serial.printf("rtc set with NTP time\n");
        // printf0_Now("rtc set with NTP time\n");
    }

    return;
}

