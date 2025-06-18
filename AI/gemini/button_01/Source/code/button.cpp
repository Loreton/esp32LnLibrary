// Funzione per inizializzare il pin del pulsante
void setupButton(ButtonState &btn, int pin, const char* name, int pressedLogicLevel) {
  btn.pin = pin;
  btn.name = name;
  btn.pressedLogicLevel = pressedLogicLevel;

  // Configura il pin in base al pressedLogicLevel
  if (pressedLogicLevel == LOW) {
    pinMode(btn.pin, INPUT_PULLUP); // Se premuto porta a LOW, usa pull-up interno
  } else {
    pinMode(btn.pin, INPUT);        // Se premuto porta a HIGH, non usare pull-up interno (richiede pull-down esterno se flottante)
  }

  btn.lastButtonState = !pressedLogicLevel; // Stato iniziale: non premuto
  btn.lastDebounceTime = 0;
  btn.buttonPressed = false;
  btn.pressStartTime = 0;
  btn.pressDuration = 0;
  btn.currentPressLevel = NO_PRESS;
  btn.maxLevelReachedAndNotified = false;
}

// Funzione per leggere un pulsante con debounce e rilevamento dei livelli di pressione
// Restituisce TRUE solo quando il pulsante viene RILASCIATO.
// I livelli di pressione raggiunti durante la pressione e la durata sono disponibili nella struttura ButtonState.
//
// Parametri:
//   btn: Riferimento alla struttura ButtonState del pulsante.
//   debounceDelay: Il tempo di debounce in millisecondi.
//   pressThresholds: Un array di unsigned long contenente le soglie di tempo per i livelli di pressione.
//                    L'indice 0 per il primo livello (es. SHORT_PRESS). Le soglie devono essere ordinate in modo crescente.
//                    Assicurati che `numThresholds` sia coerente con l'enum (es. se hai 4 livelli, `numThresholds` = 4).
//   numThresholds: Il numero di elementi nell'array pressThresholds. Questo dovrebbe essere uguale al numero di livelli validi (es. MAX_DEFINED_PRESS_LEVELS - 1 se NO_PRESS non è considerato un livello).
bool readButton(ButtonState &btn, unsigned long debounceDelay, const unsigned long pressThresholds[], size_t numThresholds) {

  // Legge lo stato attuale del pulsante (RAW)
  bool reading = digitalRead(btn.pin);

  // Controlla se lo stato RAW del pulsante è cambiato
  if (reading != btn.lastButtonState) {
    // Resetta il timer di debounce
    btn.lastDebounceTime = millis();
  }

  // Se è trascorso il tempo di debounce
  if ((millis() - btn.lastDebounceTime) > debounceDelay) {
    // Se lo stato RAW è stabile e diverso dallo stato debounced
    if (reading != btn.buttonPressed) {
      btn.buttonPressed = reading; // Aggiorna lo stato debounced

      // Se il pulsante è stato premuto
      if (btn.buttonPressed == btn.pressedLogicLevel) {
        btn.pressStartTime = millis(); // Registra il momento della pressione
        btn.currentPressLevel = NO_PRESS; // Reset del livello di pressione
        btn.maxLevelReachedAndNotified = false; // Reset del flag di notifica max livello
      }
      // Se il pulsante è stato rilasciato
      else {
        // Calcola la durata della pressione
        if (btn.pressStartTime != 0) { // Assicurati che fosse stato premuto
          btn.pressDuration = millis() - btn.pressStartTime;
          btn.pressStartTime = 0; // Resetta per la prossima pressione

          // Reset del livello di pressione e flag max level
          btn.currentPressLevel = NO_PRESS;
          btn.maxLevelReachedAndNotified = false;

          return true; // Il pulsante è stato rilasciato
        }
      }
    }
  }

  // Se il pulsante è attualmente premuto e stabile, aggiorna il livello di pressione
  if (btn.buttonPressed == btn.pressedLogicLevel) {
    if (btn.pressStartTime != 0) { // Assicurati che il timer sia partito
      unsigned long currentPressTime = millis() - btn.pressStartTime;

      ButtonPressType newLevel = NO_PRESS;
      // Itera le soglie dall'ultima (più grande) alla prima (più piccola)
      for (int i = numThresholds - 1; i >= 0; i--) {
        if (currentPressTime >= pressThresholds[i]) {
          newLevel = static_cast<ButtonPressType>(i + 1); // +1 perché SHORT_PRESS è 1, indice 0
          break; // Trovato il livello più alto raggiunto
        }
      }

      // Aggiorna il livello solo se è cambiato e non è stato raggiunto il massimo possibile
      if (newLevel > btn.currentPressLevel) {
        if (newLevel <= numThresholds) { // Assicurati di non superare i livelli definiti dall'array
          btn.currentPressLevel = newLevel;
        }

        // Verifica se è stato raggiunto il massimo livello e non è stato ancora notificato
        if (btn.currentPressLevel == static_cast<ButtonPressType>(numThresholds) && !btn.maxLevelReachedAndNotified) {
          // Questo è il punto in cui puoi eseguire un'azione o stampare che il massimo livello è stato raggiunto
          // Questa logica di notifica qui è opzionale e potresti gestirla nel loop() principale
          // In questo esempio, non ritorna true, ma imposta un flag per non notificare più volte
          btn.maxLevelReachedAndNotified = true;
        }
      }
    }
  }

  // Salva la lettura attuale per la prossima iterazione
  btn.lastButtonState = reading;

  return false; // Il pulsante non è stato rilasciato in questo ciclo
}