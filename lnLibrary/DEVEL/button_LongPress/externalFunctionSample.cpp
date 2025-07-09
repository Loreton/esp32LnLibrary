//
// updated by ...: Loreto Notarantonio
// Date .........: 23-06-2025 11.30.51
// ref: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
//

#include <Arduino.h>    // in testa anche per le definizioni dei type

#define LOG_LEVEL_0
#define LOG_LEVEL_99
#include "@logMacros.h" // printf:XFN()
#include "@button_LongPress.h"





#ifdef __ln_MODULE_DEBUG_TEST__
#include "@SerialRead.h" // waitForEnter()





// Notifica continua del livello raggiunto mentre il pulsante è premuto (opzionale)
//###########################################################################
//#
//###########################################################################
void notifyCurrentButtonLevel(ButtonState_t *btn, uint8_t buzzer_pin) {
    if (btn->buttonPressed == btn->pressedLogicLevel) {
        if (btn->currentPressLevel != NO_PRESS) {
            if (btn->currentPressLevel != btn->lastPrintedLevel) {
                printf0_FN("[%s] Pressione in corso (ms:%06ld)\n", btn->name, (millis() - btn->pressStartTime));
                switch (btn->currentPressLevel) {
                    case PRESSED_LEVEL_1:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    case PRESSED_LEVEL_2:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    case PRESSED_LEVEL_3:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    case PRESSED_LEVEL_4:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    case PRESSED_LEVEL_5:
                        printf99_FN("notify PRESSED_LEVEL: %d\n", btn->currentPressLevel);
                        notifyBuzzer(buzzer_pin, 300);
                        break;

                    default:
                        break;

                }
                btn->lastPrintedLevel = btn->currentPressLevel;
            }
            // --- LOGICA DEL BEEP OGNI 5 SECONDI ---
            if (btn->maxLevelReachedAndNotified) {
                if (millis() - lastBeepTime >= BEEP_INTERVAL) {
                    attachBuzzer(passiveBuzzer_pin, 10, 0);
                    playTone(1000, 500, 0);
                    lastBeepTime = millis();
                    printf99_FN("  [BEEP] Livello massimo ancora raggiunto!\n");
                }
            }
        }

    } else { // (btn->buttonPressed != btn->pressedLogicLevel)
        btn->lastPrintedLevel = NO_PRESS; // Reset per il prossimo ciclo di pressione
        lastBeepTime = 0; // Resetta il timer del beep quando il pulsante non è più premuto al livello massimo
        noTone(0);
    }


}


//###########################################################################
//#
//###########################################################################
bool relayState = false;
void pumpState_Action(ButtonState_t *btn) {
    switch (btn->currentPressLevel) {
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
                digitalWrite(RELAY_PIN, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(RELAY_PIN, HIGH);
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
void startButton_Action(ButtonState_t *btn) {
    switch (btn->currentPressLevel) {
        case PRESSED_LEVEL_1:
            printf99_FN("PRESSED_LEVEL_1\n");
            break;

        case PRESSED_LEVEL_2:
            printf99_FN("PRESSED_LEVEL_2\n");
            relayState = !relayState;
            if (relayState) {
                digitalWrite(RELAY_PIN, LOW);
                printf0_FN("  --> Relè ACCESO!\n");
            } else {
                digitalWrite(RELAY_PIN, HIGH);
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
void processButton(ButtonState_t *btn) {

    printf0_FN("[%s[  Rilasciato! Durata: %ld ms\n", btn->name, btn->pressDuration);
    Serial.print("Livello finale raggiunto: ");

    if (btn->pin == startButton_pin) {
        startButton_Action(btn);
    }
    else if (btn->pin == pumpState_pin) {
        pumpState_Action(btn);
    }


    // *** RESET DEI PARAMETRI DI LIVELLO NELLA FUNZIONE CHIAMANTE ***
    // Dopo aver processato i dati, li resettiamo per la prossima pressione.
    btn->currentPressLevel = NO_PRESS;
    btn->lastPrintedLevel = NO_PRESS;
    btn->pressDuration = 0;
    btn->maxLevelReachedAndNotified = false;
    // .pressStartTime non ha bisogno di essere resettato qui, è già fatto in readButton quando rilascia.
}



//###########################################################################
//#
//###########################################################################
void buttonLP_pinStatus(ButtonState_t *p, bool prompt) {
    // printf0_FN("\t%-18s: %2d - (%d)\n" , "pin nr"         , p->pin                   , p->mode);
    printf0_FN("%s\n", p->pinID);
    printf0_FN("\t%-18s: %2d - %s\n",   "pressedLogicLevel",          p->pressedLogicLevel,           str_pinLevel[p->pressedLogicLevel]);
    printf0_FN("\t%-18s: %2d - %s\n",   "buttonPressed",              p->buttonPressed,               str_TrueFalse[p->buttonPressed]);
    printf0_FN("\t%-18s: %2d - %s\n",   "maxLevelReachedAndNotified", p->maxLevelReachedAndNotified,  str_TrueFalse[p->maxLevelReachedAndNotified]);

    printf0_FN("\t%-18s: %2d\n",        "currentPressLevel",          p->currentPressLevel);
    printf0_FN("\t%-18s: %2d\n",        "lastPrintedLevel",           p->lastPrintedLevel);
    printf0_FN("\t%-18s: %2d\n",        "numThresholds",              p->numThresholds);

    printf0_FN("\t%-18s: nr:%2d --> [ ", "threshold"      , p->numThresholds);
    for (int8_t j=0; j<p->numThresholds; j++) {
        printf0("%2d, ", p->pressThresholds[j]);
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