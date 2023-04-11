/**
 * \file MySPIFFS.h
 * \page spiffs SPIFFS
 * \brief (S)erial (P)eripheral (I)nterface (F)lash (F)ile (S)ystem 
 * La structure mémoire de l'ESP32 se découpe en plusieurs "zones":
 * \verbatim
|--------------|-------|---------------|--|--|--|--|--|
^              ^       ^               ^     ^
Sketch    OTA update   File system   EEPROM  WiFi config (SDK)
\endverbatim
 * Même si l'espace "File System" est situé sur le même chip que le programme,
 * le fait d'installer un nouveau programme n'affectera pas le contenu du File System. Cela peut nous permettre
 * de stocker des fichiers de données (type historique), de configuration ou du contenu pour un serveur web (feuille de
 * style, javascript ...) par exemple.
 * 
 * - Sketch : Cette zone est lue à chaque démarrage , les variables sont donc réinitialisées avec les valeurs par défaut.
 * - OTA Update : L’espace OTA est un dossier temporaire pour les mises à jour via OTA (Over The Air, cf. Device Management).
 * - EEPROM : Permet de stocker des infos, qui pourront être lues après redémarrage. Nous sommes sur une mémoire plus lente 
 *   (que la mémoire dédiée au sketch et qui dispose d’une durée de vie limitée en écriture . Ne pas en abuser).
 * - File system (SPIFFS) est un espace de stockage &quivalent à celui d'une carte SD. Selon les modules utilisés, la taille 
 *   du fichier systéme peut varier de 4MB/32Mb à 16MB/128Mb.
 *   
 * ESP8266 Sketch Data Upload 
 * Il s'agit d'un plugin Arduino qui permet d’uploader des fichiers dans la mémoire SPIFFS depuis 
 * l’interface Arduino. Rendez-vous sur la page GitHub du plugin pour la procédure d'installation : https://github.com/me-no-dev/arduino-esp32fs-plugin
 * - Télécharger https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/download/1.0/ESP32FS-1.0.zip
 * - Dans le dossier arduino (emplacement du carnet de croquis défini dans les préférences), créér un dossier "tools", s'il n'est pas déjà présent.
 * - Décompresser le fichier zip téléchargé dans ce dossier "tools".
 * - Re-démarrer l'IDE Arduino pour qu'il prenne en compte le nouveau plugin au redémarrage.
 * - Dans le dossier de votre Sketch, créér un dossier "data". Déposer un ou plusieurs petits fichiers format txt dans ce dossier; ils seront 
 *   transférés dans la mémoire SPIFFS par le plugin (le fichier spiffs_test.txt est a priori déjà présent).
 * - Ouvrez vos Sketch Arduino dans l'IDE et sélectionner dans le menu "Outils" : ESP32 Sketch data upload.
 * Le contenu du dossier data sera transféré dans la mémoire SPIFFS. Le transfert sera plus ou moins long en fonction de la 
 * vitesse d’upload sélectionnée.
 *  
 * Comment accéder au File System :
 * Pour pouvoir utiliser les fonctions du File System il faut inclure la bibiolthèque SPIFFS.h
 * #include "SPIFFS.h"
 * Documentation complète : https://github.com/esp8266/Arduino/blob/master/doc/filesystem.rst
 * 
 * Dans cet exemple nous utilisons le format JSON pour lire et écrire dans le fichier de configuration.
 * Nous utilisons la bibliothèque Arduino JSON de Benoît Blanchon. A télécharger dans le gestionnaire de bibliothèque.
 * 
 * Fichier \ref MySPIFFS.h
 */

#include "SPIFFS.h"
#include <ArduinoJson.h>   //Arduino JSON by Benoit Blanchon : https://github.com/bblanchon/ArduinoJson

String strConfigFile("/config.json");
String strTestFile("/spiffs_test.txt");

void setupSPIFFS(){

  MYDEBUG_PRINTLN("-SPIFFS : Montage du système de fichier");

  if (SPIFFS.begin(true)) {
    MYDEBUG_PRINTLN("-SPIFFS : MONTE");

    //SPIFFS.format(); // ------------- Au besoin, pour formatter le système de fichiers

    // Fichier de test
    if (SPIFFS.exists(strTestFile)) {  // ------------------------- Le fichier existe
      File file = SPIFFS.open(strTestFile, "r");
      if (file){
        MYDEBUG_PRINTLN("-SPIFFS : Lecture du fichier spiffs_test.txt");
        while(file.available()){ 
          Serial.write(file.read());        
        }
        MYDEBUG_PRINTLN("");
      }
    }

    if (SPIFFS.exists(strConfigFile)) {  // ------------------------- Le fichier existe
      //file exists, reading and loading
      MYDEBUG_PRINTLN("-SPIFFS : Lecture du fichier de configuration");
      File configFile = SPIFFS.open(strConfigFile, "r");
      if (configFile) {
        MYDEBUG_PRINTLN("-SPIFFS : Fichier ouvert");
        DynamicJsonDocument jsonDocument(512);
        // Désérialisation du document JSON
        DeserializationError error = deserializeJson(jsonDocument, configFile);
        if (error){
          MYDEBUG_PRINTLN("-SPIFFS : Impossible de parser le JSON");          
        } else {
          MYDEBUG_PRINTLN("-JSON: Fichier parsé");
          String parametre1 = jsonDocument["parametre1"];
          String parametre2 = jsonDocument["parametre2"];
          String parametre3 = jsonDocument["parametre3"];
          MYDEBUG_PRINT("-JSON [parametre1] : ");
          MYDEBUG_PRINTLN(parametre1);
          MYDEBUG_PRINT("-JSON [parametre2] : ");
          MYDEBUG_PRINTLN(parametre2);
          MYDEBUG_PRINT("-JSON [parametre3] : ");
          MYDEBUG_PRINTLN(parametre3);
        }
      }
      configFile.close();
      MYDEBUG_PRINTLN("-SPIFFS: Fichier fermé");
    } else {                              // ------------------- Le fichier n'existe pas
      MYDEBUG_PRINTLN("-SPIFFS: Le fichier de configuration n'existe pas");
      File configFile = SPIFFS.open(strConfigFile, "w");
      if (configFile) {
        MYDEBUG_PRINTLN("-SPIFFS: Fichier créé");
        DynamicJsonDocument jsonDocument(512);
        // Exemple de 3 paramètres
        jsonDocument["parametre1"] = String("127.0.0.1");
        jsonDocument["parametre2"] = String("0123456789");
        jsonDocument["parametre3"] = String("1A2B3C4D5E6F");
        // Sérialisation du JSON dans le fichier de configuration
        if (serializeJson(jsonDocument, configFile) == 0) {
          MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
        }
        // Fermeture du fichier
        configFile.close();
        MYDEBUG_PRINTLN("-SPIFFS : Fichier fermé");
      } else{
        MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en ecriture");
      }
    }
    SPIFFS.end();  
  } else {
    MYDEBUG_PRINT("-SPIFFS : Impossible de monter le système de fichier");
  }
}
