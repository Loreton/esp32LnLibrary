//
// updated by ...: Loreto Notarantonio
// Date .........: 03-07-2025 17.51.58
//

#pragma once

#include <Arduino.h>    // in testa anche per le definizioni dei type

    void myButtonHandler(ButtonLongPress_Struct* btn);

    void startButtonNotificationHandlerCB(ButtonLongPress_Struct* p);
    void pumpStateNotificationHandlerCB(ButtonLongPress_Struct* p);
    void pumpStateHandlerCB(ButtonLongPress_Struct *p);
    void startButtonHandlerCB(ButtonLongPress_Struct *p);
    void showStatusCB(ButtonLongPress_Struct *p);