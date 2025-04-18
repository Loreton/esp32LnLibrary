/*
// updated by ...: Loreto Notarantonio
// Date .........: 18-04-2025 11.36.27
*/



// ---------------------------------
// lnLibrary headers files
// ---------------------------------
#define LOG_LEVEL_0
#include "@logMacros.h"
#include "@mainStructures.h"
#include "@pinOperations.h"




void displayPinStatus(io_input_pin_struct_t *p) {
    // printf2_NFN("\t%-18s: %2d - (%d)\n" , "pin nr"         , p->pin                   , p->mode);
    printf2_NFN("[%s mode %d:\n", p->pinID, p->mode);
    printf2_NFN("\t%-18s: %2d . %s\n" , "curr phys state" , p->ph_state     , str_pinLevel[p->ph_state]);
    printf2_NFN("\t%-18s: %2d - %s\n" , "active level"    , p->active_level , str_pinLevel[p->active_level]);
    printf2_NFN("\t%-18s: %2d . %s\n" , "is_ON"           , p->is_ON        , str_action[p->is_ON] );
    printf2_NFN("\t%-18s: %2d . %s\n" , "isPressed"       , p->isPressed    , str_action[p->isPressed] );
    printf2_NFN("\t%-18s: %2d . %s\n" , "isReleased"      , p->isReleased   , str_action[p->isReleased]);
    printf2_NFN("\t%-18s: %2d . %s\n" , "ON level"        , p->ON           , str_pinLevel[p->ON]);
    printf2_NFN("\t%-18s: %2d . %s\n" , "OFF level"       , p->OFF          , str_pinLevel[p->OFF]);
    printf2_NFN("\t%-18s: %2d . %s\n" , "last level"      , p->lastState    , str_pinLevel[p->lastState]);
    printf2_NFN("\t%-18s: %2d . %s\n" , "pressedLevel"    , p->pressedLevel , THRESHOLD_LEVEL_TYPES[p->pressedLevel]);


    printf2_NFN("\t%-18s: nr:%2d --> [ ", "threshold"      , p->n_thresholds);
    for (int8_t j=0; j<p->n_thresholds; j++) {
        printf2("%2d, ", p->thresholds[j]);
    }
    printf2(" ]\n"); // close threshold line

    printf2_NFN("\t%-18s: lvl: %s\n",      "default pressedLevel",     THRESHOLD_LEVEL_TYPES[p->pressedLevel]);
    printf2_NFN("%s is %s\n", p->pinID, str_OffOn[p->is_ON]);
    printf2("\n"); // blank line
}




void displayPinStatus(io_output_pin_struct_t *p) {
    printf2_NFN("[%s mode %d:\n", p->pinID, p->mode);
    printf2_NFN("\t%-18s: %2d . %s\n" , "curr phys state" , p->ph_state     , str_pinLevel[p->ph_state]);
    printf2_NFN("\t%-18s: %2d - %s\n" , "active level"    , p->active_level , str_pinLevel[p->active_level]);
    printf2_NFN("\t%-18s: %2d . %s\n" , "is_acted"        , p->is_acted     , str_OffOn[p->is_acted]);
    printf2_NFN("\t%-18s: %2d . %s\n" , "ON level"        , p->ON           , str_pinLevel[p->ON]);
    printf2_NFN("\t%-18s: %2d . %s\n" , "OFF level"       , p->OFF          , str_pinLevel[p->OFF]);
    printf2_NFN("%s is %s\n", p->pinID, str_OffOn[p->is_acted]);
    printf2("\n"); // blank line
}



void checkPin_forDebug(io_input_pin_struct_t *p, bool first_run) {
    uint8_t is_ON = readInputPin(p);
    if (p->changedState || first_run) {
        p->changedState = false;
        printf1_NFN("%s ph_state %d - isActed: %d [%s]\n", p->pinID, p->ph_state, p->is_ON, str_OffOn[p->is_ON]);
        // displayPinStatus(p);
        // printf0("\n");
    }
}

void checkPin_forDebug(io_output_pin_struct_t *p, bool first_run) {
    uint8_t is_acted = readOutputPin(p);

    if (p->changedState || first_run) {
        p->changedState = false;
        printf1_NFN("%s ph_state %d - isActed: %d [%s]\n", p->pinID, p->ph_state, p->is_acted, str_OffOn[p->is_acted]);
        // displayPinStatus(p);
        // printf0("\n");
    }
}


