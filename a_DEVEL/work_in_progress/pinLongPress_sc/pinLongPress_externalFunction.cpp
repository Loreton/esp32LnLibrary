//
// updated by ...: Loreto Notarantonio
// Date .........: 23-06-2025 18.07.20
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#ifdef __ln_MODULE_DEBUG_TEST__
#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()
#include "@pinLongPress_sc.h"
#include "@pinController_sc.h" // per l'active buzzer per inviare un beep durante la pressione del tasto

#include "@main_test.h"

extern pinController_sc activeBuzzer;



#include "@SerialRead.h" // waitForEnter()

uint32_t lastBeepTime;
#define BEEP_INTERVAL 5

void notifyBuzzer(pinController_sc *buzzer, uint8_t pulses=0) {
    if (buzzer) {
        if (pulses > 0) {
            buzzer->blinking_dc(300, .7, pulses);
        } else{
            buzzer->pulse(300);
        }

    }
    else {
        printf99_FN("BEEP\n");
    }
}




// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
//###########################################################################
//#
//###########################################################################
void notifyCurrentButtonLevel(pinLongPress_sc *p, pinController_sc *buzzer) {
    if (p->_buttonPressed == p->_pressedLogicLevel) {
        if (p->_currentPressLevel != NO_PRESS) {
            if (p->_currentPressLevel != p->_lastPrintedLevel) {
                printf0_FN("[%s] Pressione in corso (ms:%06ld)\n", p->_name, (millis() - p->_pressStartTime));
                switch (p->_currentPressLevel) {
                    case PRESSED_LEVEL_1:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->_currentPressLevel);
                        notifyBuzzer(buzzer);
                        break;


                    case PRESSED_LEVEL_2:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->_currentPressLevel);
                        notifyBuzzer(buzzer, p->_currentPressLevel);
                        break;

                    case PRESSED_LEVEL_3:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->_currentPressLevel);
                        notifyBuzzer(buzzer, p->_currentPressLevel);
                        break;

                    case PRESSED_LEVEL_4:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->_currentPressLevel);
                        notifyBuzzer(buzzer, p->_currentPressLevel);
                        break;

                    case PRESSED_LEVEL_5:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", p->_currentPressLevel);
                        notifyBuzzer(buzzer, p->_currentPressLevel);
                        break;

                    default:
                        break;

                }
                p->_lastPrintedLevel = p->_currentPressLevel;
            }
            // --- LOGICA DEL BEEP OGNI 5 SECONDI ---
            if (p->_maxLevelReachedAndNotified) {
                if (millis() - lastBeepTime >= BEEP_INTERVAL) {
                    buzzer->blinking_dc(1000, .60);
                    lastBeepTime = millis();
                    printf99_FN("  [BEEP] Livello massimo ancora raggiunto!\n");
                }
            }
        }

    } else { // (p->_buttonPressed != p->_pressedLogicLevel)
        p->_lastPrintedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
        lastBeepTime = 0; // Resetta il timer del beep quando il pulsante non è più premuto al livello massimo
        buzzer->off();
    }


}


//###########################################################################
//#
//###########################################################################
bool relayState = false;
void pumpState_Action(pinLongPress_sc *p) {
    switch (p->_currentPressLevel) {
        case PRESSED_LEVEL_1:
            printf99_FN("PRESSED_LEVEL_1\n");
            break;

        case PRESSED_LEVEL_2:
            printf99_FN("PRESSED_LEVEL_2\n");
            break;

        case PRESSED_LEVEL_3:
            printf99_FN("PRESSED_LEVEL_3\n");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(pressControlRelay_pin, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(pressControlRelay_pin, HIGH);
                printf0_FN("  --> Relè SPENTO!\n");
            }
            break;

        case PRESSED_LEVEL_4:
            printf99_FN("PRESSED_LEVEL_4\n");
            break;

        default:
            printf99_FN("Sconosciuto/Non Qualificato\n");
            break;
    }
}

//###########################################################################
//#
//###########################################################################
void startButton_Action(pinLongPress_sc *p) {
    // printf99_FN("sono qui p->_currentPressLevel %d\n", p->_currentPressLevel);
    switch (p->_currentPressLevel) {
        case PRESSED_LEVEL_1:
            printf99_FN("PRESSED_LEVEL_1\n");
            break;

        case PRESSED_LEVEL_2:
            printf99_FN("PRESSED_LEVEL_2\n");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(pressControlRelay_pin, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(pressControlRelay_pin, HIGH);
                printf0_FN("  --> Relè SPENTO!\n");
            }
            break;

        case PRESSED_LEVEL_3:
            printf99_FN("PRESSED_LEVEL_3\n");
            break;

        case PRESSED_LEVEL_4:
            printf99_FN("PRESSED_LEVEL_4\n");
            break;

        default:
            printf99_FN("Sconosciuto/Non Qualificato\n");
            break;
    }
}




//###########################################################################
//#
//###########################################################################
void processButton(pinLongPress_sc *p) {
    printf0_FN("[%s[  Rilasciato! Durata: %ld ms\n", p->_name, p->_pressDuration);
    // Serial.print("Livello finale raggiunto: ");

    if (p->_pin == startButton_pin) {
        Serial.print("start button action\n");
        startButton_Action(p);
    }
    else if (p->_pin == pumpState_pin) {
        pumpState_Action(p);
    }


    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    p->_currentPressLevel = NO_PRESS;
    p->_lastPrintedLevel = NO_PRESS;
    p->_pressDuration = 0;
    p->_maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.
}



//###########################################################################
//#
//###########################################################################
void pinLongPressStatus(pinLongPress_sc *p, bool prompt) {
    // printf0_FN("\t%-18s: %2d - (%d)\n" , "pin nr"         , p->pin                   , p->mode);
    printf0_FN("%s\n", p->_pinID);
    printf0_FN("\t%-18s: %2d\n",   "pressedLogicLevel",          p->_pressedLogicLevel); //,           str_pinLevel[p->_pressedLogicLevel]);
    printf0_FN("\t%-18s: %2d\n",   "buttonPressed",              p->_buttonPressed); //,               str_TrueFalse[p->_buttonPressed]);
    printf0_FN("\t%-18s: %2d\n",   "maxLevelReachedAndNotified", p->_maxLevelReachedAndNotified); //,  str_TrueFalse[p->_maxLevelReachedAndNotified]);

    // printf0_FN("\t%-18s: %2d - %s\n",   "pressedLogicLevel",          p->_pressedLogicLevel,           str_pinLevel[p->_pressedLogicLevel]);
    // printf0_FN("\t%-18s: %2d - %s\n",   "buttonPressed",              p->_buttonPressed,               str_TrueFalse[p->_buttonPressed]);
    // printf0_FN("\t%-18s: %2d - %s\n",   "maxLevelReachedAndNotified", p->_maxLevelReachedAndNotified,  str_TrueFalse[p->_maxLevelReachedAndNotified]);

    printf0_FN("\t%-18s: %2d\n",        "currentPressLevel",          p->_currentPressLevel);
    printf0_FN("\t%-18s: %2d\n",        "lastPrintedLevel",           p->_lastPrintedLevel);
    printf0_FN("\t%-18s: %2d\n",        "numThresholds",              p->_numThresholds);

    printf0_FN("\t%-18s: nr:%2d --> [ ", "threshold"      , p->_numThresholds);
    for (int8_t j=0; j < p->_numThresholds; j++) {
        printf0("%2d, ", p->_pressThresholds[j]);
    }
    printf0(" ]\n"); // close threshold line

    // printf0_FN("\t%-18s: lvl: %s\n",      "default pressedLevel",     THRESHOLD_LEVEL_TYPES[p->pressedLevel]);
    // printf0_FN("%s is %s\n", p->pinID, str_OffOn[p->is_ON]);
    printf0("\n"); // blank line
    if (prompt) {
        waitForEnter();
    }
}


#endif