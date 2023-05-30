
#include "SPIFFS.h"
#include <ArduinoJson.h>   //Arduino JSON by Benoit Blanchon : https://github.com/bblanchon/ArduinoJson

char* strConfigFile = "/config.json";


void get_config() {
  if (!SPIFFS.begin(true))
    return;

  File configFile = SPIFFS.open(strConfigFile, "r");

  if ( !configFile ) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en lecture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  MYDEBUG_PRINTLN("-SPIFFS : Lecture du fichier de configuration");
  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, configFile);

  if (error){
    MYDEBUG_PRINTLN("-SPIFFS : Impossible de parser le JSON, création d'un nouveau fichier");
  }

  const char* json_ap_ssid = jsonDocument["wifi"]["ap_ssid"];
  if ( !json_ap_ssid ) {
    const char* default_ap_ssid = "ESP32_Axel";
    jsonDocument["wifi"]["ap_ssid"] = default_ap_ssid;
  }
  ap_ssid = jsonDocument["wifi"]["ap_ssid"].as<String>();

  const char* json_ap_password = jsonDocument["wifi"]["ap_password"];
  if ( !json_ap_password ) {
    const char* default_ap_password = "AdminSex";
    jsonDocument["wifi"]["ap_password"] = default_ap_password;
  }
  ap_password = jsonDocument["wifi"]["ap_password"].as<String>();

  const char* json_net_ssid = jsonDocument["wifi"]["net_ssid"];
  if ( !json_net_ssid ) {
    const char* default_net_ssid = "Hamburger";
    jsonDocument["wifi"]["net_ssid"] = default_net_ssid;
  }
  net_ssid = jsonDocument["wifi"]["net_ssid"].as<String>();

  const char* json_net_password = jsonDocument["wifi"]["net_password"];
  if ( !json_net_password ) {
    const char* default_net_password = "YoLesKids";
    jsonDocument["wifi"]["net_password"] = default_net_password;
  }
  net_password = jsonDocument["wifi"]["net_password"].as<String>();


  configFile.close();
  configFile = SPIFFS.open(strConfigFile, "w");

  if ( !configFile ) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en ecriture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  
  if (serializeJson(jsonDocument, configFile) == 0) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
  }

  // Fermeture du fichier
  configFile.close();
  SPIFFS.end();  
}

// String readFile(const char* path) {
//   MYDEBUG_PRINTLN("-SPIFFS : Lecture du fichier " + String(path));
//   File file = SPIFFS.open(path, "r");
//   if (!file) {
//     MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier");
//     return "";
//   }
//   MYDEBUG_PRINTLN("-SPIFFS : Lecture du fichier");
//   String fileContent;
//   while (file.available()) {
//     fileContent += String((char)file.read());
//   }
//   MYDEBUG_PRINTLN("-SPIFFS : Lecture terminée");
//   file.close();
//   return fileContent;
// }

// String writeFile(const char* path, const char* message) {
//   MYDEBUG_PRINTLN("-SPIFFS : Ecriture du fichier " + String(path));
//   File file = SPIFFS.open(path, "w");
//   if (!file) {
//     MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier");
//     return "";
//   }
//   MYDEBUG_PRINTLN("-SPIFFS : Ecriture du fichier");
//   if (file.print(message)) {
//     MYDEBUG_PRINTLN("-SPIFFS : Ecriture terminée");
//   } else {
//     MYDEBUG_PRINTLN("-SPIFFS : Erreur d'écriture");
//   }
//   file.close();
//   return message;
// }

void reset_config() {
  // delete the config file
  SPIFFS.remove(strConfigFile);
}

void setupSPIFFS(){

    // SPIFFS.format(); // ------------- Au besoin, pour formatter le système de fichiers

  // MYDEBUG_PRINTLN("-SPIFFS : Montage du système de fichier");

  // SPIFFS.begin(true);
}
