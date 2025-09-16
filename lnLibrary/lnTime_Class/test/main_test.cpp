//
// updated by ...: Loreto Notarantonio
// Date .........: 09-09-2025 16.17.56
//


// #define  LOG_MODULE_LEVEL LOG_DEFAULT_TRACE
#include "lnLogger_Class.h"


#include "LnTime_Class.h"


// ### prototypes for testing
void onSecondModulo(void);
void atSecond(void);
void atSecondVector(void);


void onMinuteModulo(void);
void atMinute(void);
void atMinuteVector(void);




void setup() {
    Serial.begin(115200);
    delay(2000);
    lnLog.init();

    lnTime.setup(2 * 60);
    lnTime.set_dhmCustomUpdate(false);
    Serial.println("Sono lnTime TEST");
}



bool    first_run=true;




// #############################################################
// #
// #############################################################
int8_t counter=0;
void loop() {
    uint16_t value;
    uint16_t minute;
    uint16_t second;
    lnTime.update();


    if (lnTime.onSecond()) {
        // {;i("Current time: %s", lnTime.nowTime());
        // Stampa il tempo completo usando la funzione dedicata
        // struct tm current_time_info = lnTime.getTimeStruct();
        // printLocalTime(&current_time_info); // Chiamata alla funzione esterna
        LOG_TRACE("It's a new second!");
        // print_rtc_time(&lnTime); // Chiamata alla funzione esterna, passando il puntatore
    }

    // atSecond();
    // onSecondModulo();
    atSecondVector();

    atMinute();
    onMinuteModulo();
    atMinuteVector();


    if (first_run) {
        first_run=false;
        LOG_INFO("fine primo loop....\n\n");
    }
    delay(100); // Piccolo ritardo per non sovraccaricare la CPU
}


