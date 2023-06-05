/**
 * @file CTDebug.h
 * @brief CTDebug est un module qui permet d'activer ou de désactiver les messages de debug.
 * Lorsque MYDEBUG est défini, les messages de debug sont affichés sur le port série.
 * 
 * @page CTDebug Debug Contact Tracer
 * @ref CTDebug.h
 */

#ifdef MYDEBUG
 #define MYDEBUG_PRINT(x)     Serial.print (x)
 #define MYDEBUG_PRINTDEC(x)  Serial.print (x, DEC)
 #define MYDEBUG_PRINTHEX(x)  Serial.print (x, HEX)
 #define MYDEBUG_PRINTLN(x)   Serial.println (x)
 #define MYDEBUG_PRINTF    Serial.printf
#else
 #define MYDEBUG_PRINT(x)
 #define MYDEBUG_PRINTDEC(x)
 #define MYDEBUG_PRINTHEX(x)
 #define MYDEBUG_PRINTLN(x)
 #define MYDEBUG_PRINTF(x)
#endif

/**
 * @brief Fonction qui permet d'initialiser le port série pour le debug.
 */
void setupDebug(){
#ifdef MYDEBUG 
  Serial.begin(230400);
  Serial.println("Ouverture du port série");
#endif  
}
