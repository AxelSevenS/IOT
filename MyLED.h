#define LED_PIN    23                  // PIN + de la LED

int PWMRANGE = 256;

 void setupLED() {
  pinMode(LED_PIN, OUTPUT);     // Configuration de la LED_PIN en sortie
  digitalWrite(LED_PIN, LOW);   // Initialisation de l'état de la LED

  MYDEBUG_PRINTLN("- PWM : clignote");
  for (int i=0; i<10; i++){     // On fait clignoter la LED
    MYDEBUG_PRINT("- LED : [");
    MYDEBUG_PRINT(i);
    MYDEBUG_PRINTLN("]");
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);    
    delay(500);
  }
}

void playWithLED(){
  // On augmente la fréquence PWN de 0 au Maximum
  // Pour la valeur maximale on utilisera PWMRANGE
  MYDEBUG_PRINTLN("- PWM : crescendo");
  for(int i=0; i<PWMRANGE; i++){
    analogWrite(LED_PIN, i);
    delay(10);
  }
  // On diminue la fréquence PWN du maximum à 0
  MYDEBUG_PRINTLN("- PWM : decrescendo");
  for(int i=PWMRANGE; i>0; i--){
    analogWrite(LED_PIN, i);
    delay(10);
  }
}
