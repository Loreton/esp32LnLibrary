//
// updated by ...: Loreto Notarantonio
// Date .........: 04-07-2025 13.43.57
//

#pragma once

#include <Arduino.h>    // in testa anche per le definizioni dei type

    void myButtonHandler(ButtonLongPress_Struct* btn);

    void startButtonNotificationHandlerCB(ButtonLongPress_Struct* p);
    void pumpStateNotificationHandlerCB(ButtonLongPress_Struct* p);
    void beepNotification(ButtonLongPress_Struct *p, uint16_t beep_duration);
    void pumpStateHandlerCB(ButtonLongPress_Struct *p);
    void startButtonHandlerCB(ButtonLongPress_Struct *p);
    void showStatusCB(ButtonLongPress_Struct *p);