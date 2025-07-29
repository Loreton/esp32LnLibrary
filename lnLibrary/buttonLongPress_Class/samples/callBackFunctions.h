//
// updated by ...: Loreto Notarantonio
// Date .........: 28-07-2025 16.48.22
//
#ifdef __ln_MODULE_DEBUG_TEST__
    #pragma once

    #include <Arduino.h>    // in testa anche per le definizioni dei type

    void myButtonHandler(ButtonLongPress_Class* btn);

    void startButtonNotificationHandlerCB(ButtonLongPress_Class* p);
    void pumpStateNotificationHandlerCB(ButtonLongPress_Class* p);
    void beepNotification(ButtonLongPress_Class *p, uint16_t beep_duration);
    void pumpStateHandlerCB(ButtonLongPress_Class *p);
    void startButtonHandlerCB(ButtonLongPress_Class *p);
    void showStatusCB(ButtonLongPress_Class *p);

#endif