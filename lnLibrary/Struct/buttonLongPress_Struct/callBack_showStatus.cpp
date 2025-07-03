//
// updated by ...: Loreto Notarantonio
// Date .........: 03-07-2025 18.46.12
//
#ifdef __ln_MODULE_DEBUG_TEST__

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()
#include "@globalVars.h" //



#include "@buttonLongPress_Struct.h"
// #include "@pinController_Struct.h" // per l'active buzzer per inviare un beep durante la pressione del tasto
#include "callBackFunctions.h" // per functions protoype





//###########################################################################
//#
//###########################################################################
void showStatusCB(ButtonLongPress_Struct *p) {
    // printf0_FN("\t%-18s: %2d - (%d)\n" , "pin nr"         , p->pin                   , p->mode);
    printf99_FN("%s\n", p->m_pinID);
    printf99_FN("\t%-18s: %2d\n",   "pressedLogicLevel",          p->m_pressedLogicLevel); //,           str_pinLevel[p->m_pressedLogicLevel]);
    printf99_FN("\t%-18s: %2d\n",   "buttonPressed",              p->m_buttonPressed); //,               str_TrueFalse[p->m_buttonPressed]);
    printf99_FN("\t%-18s: %2d\n",   "maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified); //,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);

    printf99_FN("\t%-18s: %2d - %s\n",   "pressedLogicLevel",          p->m_pressedLogicLevel,           str_pinLevel[p->m_pressedLogicLevel]);
    printf99_FN("\t%-18s: %2d - %s\n",   "buttonPressed",              p->m_buttonPressed,               str_TrueFalse[p->m_buttonPressed]);
    printf99_FN("\t%-18s: %2d - %s\n",   "maxLevelReachedAndNotified", p->m_maxLevelReachedAndNotified,  str_TrueFalse[p->m_maxLevelReachedAndNotified]);

    printf99_FN("\t%-18s: nr:%2d --> [ ", "threshold"      , p->m_numThresholds);
    for (int8_t j=0; j < p->m_numThresholds; j++) {
        printf99_FN("%2d, ", p->m_pressThresholds[j]);
    }
    printf99(" ]\n"); // close threshold line

    printf99("\n"); // blank line

}




#endif