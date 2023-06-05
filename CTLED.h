/**
 * @file CTLED.h
 * @brief CTLED est un module qui sert à gérer la LED de l'appareil.
 * Lorsque l'Utilisateur est en bonne santé, la LED est allumée en permanence.
 * Lorsque l'Utilisateur est en contact avec une personne malade, la LED fluctue en luminosité de façon sinusoidale.
 * Lorsque l'Utilisateur est malade, la LED clignote.
 * 
 * @page CTLED LED d'indications Contact Tracer
 * @ref CTLED.h
 */

#define LED_PIN    23                  // PIN + de la LED

int PWMRANGE = 256;

int current_power = 0;
bool crescendo = true;

/**
 * @brief Initialise la LED
 */
void setupLED() {
  pinMode(LED_PIN, OUTPUT);
}

/**
 * @brief Met à jour la LED en fonction de l'état de santé de l'Utilisateur.
 * - HEALTHY : La LED est allumée en permanence.
 * - CONTACT : La LED fluctue en luminosité de façon sinusoidale.
 * - SICK : La LED clignote.
 */
void updateLED() {

  switch(user_status) {
    case HEALTHY:
      current_power = HIGH;
      break;
    case CONTACT:
      if (current_power == 0 || current_power == PWMRANGE ) {
        crescendo = !crescendo;
      }
      current_power += crescendo ? 1 : -1;
      break;
    case SICK:
      current_power = current_power == PWMRANGE ? 0 : PWMRANGE;
      break;
  }

  digitalWrite(LED_PIN, current_power);
}
