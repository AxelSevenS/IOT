#pragma once


#include "SPIFFS.h"
#include <ArduinoJson.h>   //Arduino JSON by Benoit Blanchon : https://github.com/bblanchon/ArduinoJson

char* strConfigFile = "/config.json";


enum Status {
  SAFE = 2,
  CONTACT = 1,
  SICK = 0
};

static Status userStatus = SAFE;

String ap_ssid;
String ap_password;

String net_ssid;
String net_password;

float monitoring_time = 5.0;
float monitoring_distance = 5.0;


/**
 * @brief Permet de mettre à jour le SSID et le mot de Passe de la Contact Tracer, en tant que point d'accès réseau, dont on se sert pour la configurer.\n
 * Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le SSID est alors stocké dans la variable @ref ap_ssid;\n
 * et le mot de Passe dans @ref ap_password.
 * 
 * @param new_ap_ssid Le nouveau SSID que la carte va utiliser pour son Access Point.
 * @param new_ap_password Le nouveau mot de Passe que la carte va utiliser pour son Access Point.
 *
 */
void update_AP_config( const char* new_ap_ssid, const char* new_ap_password ) {
  
  if (!SPIFFS.begin(true))
    return;

  File configFile = SPIFFS.open(strConfigFile, "r");

  if ( !configFile ) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode lecture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, configFile);

  if (error){
    MYDEBUG_PRINTLN("-SPIFFS : Impossible de parser le JSON, création d'un nouveau fichier");
  }
  configFile.close();
  configFile = SPIFFS.open(strConfigFile, "w");

  if ( !configFile ) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode écriture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  jsonDocument["wifi"]["ap_ssid"] = new_ap_ssid;
  ap_ssid = String(new_ap_ssid);

  jsonDocument["wifi"]["ap_password"] = new_ap_password;
  ap_password = String(new_ap_password);

  if (serializeJson(jsonDocument, configFile) == 0) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
  }

  // Fermeture du fichier
  configFile.close();
  SPIFFS.end();  

}

/**
 * 
 * @brief Permet de mettre à jour le SSID et le mot de Passe du point d'accès réseau que va utiliser Le Contact Tracer.\n
 * Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte.
 * 
 * Le SSID est alors stocké dans la variable @ref net_ssid;\n
 * et le mot de Passe dans @ref net_password.
 * 
 * @param new_net_ssid Le nouveau SSID réseau que la carte va utiliser.
 * @param new_net_password Le nouveau mot de Passe réseau que la carte va utiliser
 */
void update_network_config( const char* new_net_ssid, const char* new_net_password ) {
  
  if (!SPIFFS.begin(true))
    return;

  File configFile = SPIFFS.open(strConfigFile, "r");

  if ( !configFile ) { // ------------------------- File can't be read
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode lecture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, configFile);

  if (error){
    MYDEBUG_PRINTLN("-SPIFFS : Impossible de parser le JSON, création d'un nouveau fichier");
  }
  configFile.close();
  configFile = SPIFFS.open(strConfigFile, "w");

  if ( !configFile ) { // ------------------------- File can't be read
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode écriture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  jsonDocument["wifi"]["net_ssid"] = new_net_ssid;
  net_ssid = String(new_net_ssid);

  jsonDocument["wifi"]["net_password"] = new_net_password;
  net_password = String(new_net_password);

  if (serializeJson(jsonDocument, configFile) == 0) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
  }

  // Fermeture du fichier
  configFile.close();
  SPIFFS.end();  

}


/**
 * @brief Permet de mettre à jour le SSID et le mot de Passe de la Contact Tracer, en tant que point d'accès réseau, dont on se sert pour la configurer.\n
 * Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le SSID est alors stocké dans la variable @ref ap_ssid;\n
 * et le mot de Passe dans @ref ap_password.
 * 
 * @param new_ap_ssid Le nouveau SSID que la carte va utiliser pour son Access Point.
 * @param new_ap_password Le nouveau mot de Passe que la carte va utiliser pour son Access Point.
 *
 */
void update_monitoring_config( float new_time, float new_distance ) {
  
  if (!SPIFFS.begin(true))
    return;

  File configFile = SPIFFS.open(strConfigFile, "r");

  if ( !configFile ) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode lecture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, configFile);

  if (error){
    MYDEBUG_PRINTLN("-SPIFFS : Impossible de parser le JSON, création d'un nouveau fichier");
  }
  configFile.close();
  configFile = SPIFFS.open(strConfigFile, "w");

  if ( !configFile ) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode écriture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  jsonDocument["monitoring"]["time"] = new_time;
  monitoring_time = new_time;

  jsonDocument["monitoring"]["distance"] = new_distance;
  monitoring_distance = new_distance;

  if (serializeJson(jsonDocument, configFile) == 0) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
  }

  // Fermeture du fichier
  configFile.close();
  SPIFFS.end();  

}

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

  // print the config file
  while (configFile.available()) {
    MYDEBUG_PRINT((char)configFile.read());
  }
  MYDEBUG_PRINTLN("");
  configFile.seek(0);
  

  MYDEBUG_PRINTLN("-SPIFFS : Lecture du fichier de configuration");
  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, configFile);

  if (error){
    MYDEBUG_PRINT("-SPIFFS : [ERREUR : ");
    MYDEBUG_PRINT(error.c_str());
    MYDEBUG_PRINTLN("] Impossible de parser le JSON, création d'un nouveau fichier");
  }

  auto json_status = jsonDocument["status"];
  if ( json_status.isNull() ) {
    MYDEBUG_PRINTLN("-SPIFFS : Pas de statut dans le fichier de configuration");
    const char* default_status = "SAFE";
    json_status = default_status;
  }
  const char* status = jsonDocument["status"].as<String>().c_str();
  userStatus = status == "SAFE" ? SAFE : status == "CONTACT" ? CONTACT : SICK;

  auto json_ap_ssid = jsonDocument["wifi"]["ap_ssid"];
  if ( json_ap_ssid.isNull() ) {
    MYDEBUG_PRINTLN("-SPIFFS : Pas de SSID AP dans le fichier de configuration");
    const char* default_ap_ssid = "ESP32_Axel";
    json_ap_ssid = default_ap_ssid;
  }
  ap_ssid = json_ap_ssid.as<String>();

  auto json_ap_password = jsonDocument["wifi"]["ap_password"];
  if ( json_ap_password.isNull() ) {
    MYDEBUG_PRINTLN("-SPIFFS : Pas de mot de passe AP dans le fichier de configuration");
    const char* default_ap_password = "AdminSex";
    json_ap_password = default_ap_password;
  }
  ap_password = json_ap_password.as<String>();

  auto json_net_ssid = jsonDocument["wifi"]["net_ssid"];
  if ( json_net_ssid.isNull() ) {
    MYDEBUG_PRINTLN("-SPIFFS : Pas de SSID NET dans le fichier de configuration");
    const char* default_net_ssid = "Hamburger";
    json_net_ssid = default_net_ssid;
  }
  net_ssid = json_net_ssid.as<String>();

  auto json_net_password = jsonDocument["wifi"]["net_password"];
  if ( json_net_password.isNull() ) {
    MYDEBUG_PRINTLN("-SPIFFS : Pas de mot de passe NET dans le fichier de configuration");
    const char* default_net_password = "YoLesKids";
    json_net_password = default_net_password;
  }
  net_password = json_net_password.as<String>();



  auto json_monitoring_distance = jsonDocument["monitoring"]["distance"];
  if ( json_monitoring_distance.isNull() ) {
    MYDEBUG_PRINTLN("-SPIFFS : Pas de distance de surveillance dans le fichier de configuration");
    float default_monitoring_distance = 5.0;
    json_monitoring_distance = default_monitoring_distance;
  }
  monitoring_distance = json_monitoring_distance.as<float>();

  auto json_monitoring_time = jsonDocument["monitoring"]["interval"];
  if ( json_monitoring_time.isNull() ) {
    MYDEBUG_PRINTLN("-SPIFFS : Pas d'intervalle de surveillance dans le fichier de configuration");
    uint8_t default_monitoring_time = 5;
    json_monitoring_time = default_monitoring_time;
  }
  monitoring_time = json_monitoring_time.as<uint8_t>();



  // Add array to the JSON document if it doesn't exist
  auto json_monitered = jsonDocument["monitored"];
  if ( json_monitered.isNull() ) {
    MYDEBUG_PRINTLN("-SPIFFS : Pas de liste de capteurs en surveillance dans le fichier de configuration");
    jsonDocument.createNestedObject("monitored");
  }

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
  if (!SPIFFS.begin(true))
    return;
  
  SPIFFS.remove(strConfigFile);

  SPIFFS.end();
}

void setupSPIFFS(){

  // SPIFFS.format(); /// ------------- Au besoin, pour formatter le système de fichiers

  get_config();
}
