//
// updated by ...: Loreto Notarantonio
// Date .........: 07-07-2025 09.38.00
//
#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>    // in testa anche per le definizioni dei type

#include "lnLogger.h" // printf:XFN()
#include "lnGlobalVars.h" //



#include "ButtonLongPress_Struct.h"
// #include "pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "callBackFunctions.h" // per functions protoype





//###########################################################################
//#
//###########################################################################
void showStatusCB(ButtonLongPress_Struct *p) {
    // printf0_FN("\t%-18s: %2d - (%d)" , "pin nr"         , p->pin                   , p->mode);
    LOG_DEBUG("%s", p->m_pinID);
    LOG_DEBUG("\t%-18s: %2d",   "pressedLogicLevel",          p->m_pressedLogicLevel); //,           str_pinLevel[p->m_pressedLogicLevel]);
    LOG_DEBUG("\t%-18s: %2d",   "buttonPressed",              p->m_buttonPressed); //,               str_TrueFalse[p->m_buttonPressed]);
    LOG_DEBUG("\t%-18s: %2d",   "maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified); //,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);

    LOG_DEBUG("\t%-18s: %2d - %s",   "pressedLogicLevel",          p->m_pressedLogicLevel,           str_pinLevel[p->m_pressedLogicLevel]);
    LOG_DEBUG("\t%-18s: %2d - %s",   "buttonPressed",              p->m_buttonPressed,               str_TrueFalse[p->m_buttonPressed]);
    LOG_DEBUG("\t%-18s: %2d - %s",   "maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);

    LOG_DEBUG("\t%-18s: nr:%2d --> [ ", "threshold"      , p->m_numThresholds);
    for (int8_t j=0; j < p->m_numThresholds; j++) {
        LOG_DEBUG("%2d, ", p->m_pressThresholds[j]);
    }
    LOG_DEBUG(" ]"); // close threshold line

    LOG_DEBUG(""); // blank line

}




#endif