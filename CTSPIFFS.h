/**
* \file CTSPIFFS.h
* \page CTSPIFFS CTSPIFFS
* Gestion du système de fichiers SPIFFS de la carte ESP32.
*
* Plusieurs fonctions sont disponibles pour lire et écrire dans le système de fichiers SPIFFS notamment des fichiers JSONs. 
*
*\ref CTSPIFFS.h
*/

#pragma once


#include <ArduinoJson.h>   //Arduino JSON by Benoit Blanchon : https://github.com/bblanchon/ArduinoJson
#include <SPIFFS.h>
#include "CTDebug.h"



typedef const std::function<void(JsonDocument&, File&)> FileReadFunction;




/**
 * @brief Permet de lire dans un fichier SPIFFS.\n
 * 
 * @remarks Le fichier est ouvert en mode lecture.\n
 * Les contenus du fichier sont passés à la fonction readFunction sous forme de JsonDocument.\n
 * 
 * @param readFunction La fonction qui va lire dans le fichier de config.
*/
void read_file(const char* fileName, FileReadFunction& readFunction) {
  if (!SPIFFS.begin(true)) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible de monter le système de fichiers");
    return;
  }

  File jsonFile = SPIFFS.open(fileName, FILE_READ);

  if ( !jsonFile ) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode lecture");
    jsonFile.close();
    SPIFFS.end();
    return;
  }

  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, jsonFile);

  if (error){
    MYDEBUG_PRINT("-SPIFFS : [ERREUR : ");
    MYDEBUG_PRINT(error.c_str());
    MYDEBUG_PRINT("] Impossible de parser le JSON dans le fichier : ");
    MYDEBUG_PRINTLN(jsonFile.name());
  }

  readFunction(jsonDocument, jsonFile);

  jsonFile.close();
  SPIFFS.end();  
}

/**
 * @brief Permet de lire et écrire dans un fichier SPIFFS.\n
 * 
 * @remarks Le fichier est ouvert en mode lecture et écriture.\n
 * Les contenus du fichier sont passés à la fonction writeFunction sous forme de JsonDocument.\n
 * 
 * @param fileName Le nom du fichier à lire et écrire.
 * @param writeFunction La fonction qui va écrire dans le fichier de config.
*/
void read_write_file(const char* fileName, FileReadFunction writeFunction) {
    
  read_file(fileName, [fileName, writeFunction](JsonDocument& jsonDocument, File& jsonFile) {
    jsonFile.close();
    jsonFile = SPIFFS.open(fileName, FILE_WRITE);

    if ( !jsonFile ) {
      MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode écriture");
      jsonFile.close();
      SPIFFS.end();
      return;
    }


    writeFunction(jsonDocument, jsonFile); 
    

    if (serializeJson(jsonDocument, jsonFile) == 0) {
      MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
    }
  });

}


/**
 * @brief Permet d'afficher les contenus du fichier passé en paramètre de façon formattée.\n
 * 
 * @remark Cette fonction ouvre un document JSON.
 * @warning NE PAS UTILISER CETTE FONCTION AVEC UN DOCUMENT JSON DEJA OUVERT.
 *
 * @param fileName 
 */
void display_file(const char* fileName) {
  read_file(fileName, [](JsonDocument& jsonDocument, File& jsonFile) {
    String json_text;
    serializeJsonPretty(jsonDocument, json_text);
    MYDEBUG_PRINT("-SPIFFS : Contenu du fichier ");
    MYDEBUG_PRINT(jsonFile.name());
    MYDEBUG_PRINTLN(" :");
    MYDEBUG_PRINTLN(json_text);
  });
}

/**
 * @brief Permet de réinitialiser le fichier de configuration.\n
 * 
 * Cette fonction supprime le fichier de configuration présent dans la mémoire (SPIFFS) de la carte.\n
*/
void reset_file(const char* filePath) {
  if (!SPIFFS.begin(true))
    return;
  
  SPIFFS.remove(filePath);

  SPIFFS.end();
}

/**
 * Configuration du système de fichiers SPIFFS
 * - Formatage du système de fichiers (OPTIONNEL)
 * - Lecture du fichier de configuration
*/
void setupSPIFFS(){

  // SPIFFS.format(); /// ------------- Au besoin, pour formatter le système de fichiers

  // get_config();
}
