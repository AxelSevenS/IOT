#pragma once


#include "Events.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>   //Arduino JSON by Benoit Blanchon : https://github.com/bblanchon/ArduinoJson

/*************************** Files ************************************/
#define CONFIG_FILE_PATH            "/config.json"
#define MONITORING_FILE_PATH            "/monitoring.json"


/*************************** Default Values ************************************/
#define DEFAULT_PASSWORD            "contacttracer"
#define DEFAULT_NET_SSID            "Hamburger"
#define DEFAULT_NET_PASSWORD        "YoLesKids"

#define DEFAULT_MONITORING_TIME     5.0
#define DEFAULT_MONITORING_DISTANCE 5.0

#define DEFAULT_USER_STATUS         "HEALTHY"



typedef const std::function<void(JsonDocument&, File&)> FileReadFunction;


/*************************** Configuration Variables ************************************/
String device_name;
String device_password;

String net_ssid;
String net_password;


HealthStatus user_status = HEALTHY;

float monitoring_time = 5.0;
float monitoring_distance = 5.0;



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

void update_device_config( const char* new_device_name, const char* new_device_password, JsonDocument& jsonDocument ) {

  bool deviceCredentialsChanged = false;

  if ( strcmp(new_device_name, device_name.c_str()) != 0 ) {
    deviceCredentialsChanged = true;
  }
  device_name = String(new_device_name);
  jsonDocument["device"]["name"] = device_name;

  if ( strcmp(new_device_password, device_password.c_str()) != 0 ) {
    deviceCredentialsChanged = true;
  }
  device_password = String(new_device_password);
  jsonDocument["device"]["password"] = device_password;

  if ( deviceCredentialsChanged ) {
    // onDeviceCredentialsChanged(device_name.c_str(), device_password.c_str());
    InvokeDeviceCredentialsChangedCallbacks(device_name.c_str(), device_password.c_str());
  }

}


/**
 * @brief Permet de mettre à jour le SSID et le mot de Passe de la Contact Tracer, en tant que point d'accès réseau, dont on se sert pour la configurer.\n
 * Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le SSID est alors stocké dans la variable @ref device_name;\n
 * et le mot de Passe dans @ref device_password.
 * 
 * @remark Cette fonction ouvre un document JSON.
 * @warning NE PAS UTILISER CETTE FONCTION AVEC UN DOCUMENT JSON DEJA OUVERT.
 * 
 * @param new_device_name Le nouveau que la carte va utiliser pour son Access Point et son OTA.
 * @param new_device_password Le nouveau mot de Passe que la carte va utiliser pour son Access Point et son OTA.
 */
void update_device_config( const char* new_device_name, const char* new_device_password ) {

  read_write_file(CONFIG_FILE_PATH, [&new_device_name, &new_device_password](JsonDocument& jsonDocument, File& jsonFile) {

    update_device_config(new_device_name, new_device_password, jsonDocument);

  });

}

/**
 * @brief Permet de mettre à jour le SSID et le mot de Passe de la Contact Tracer, en tant que point d'accès réseau, dont on se sert pour la configurer.\n
 * Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le SSID est alors stocké dans la variable @ref device_name;\n
 * et le mot de Passe dans @ref device_password.
 * 
 * @remark Cette fonction n'ouvre pas de document JSON, elle utilise celui passé en paramètre.
 * 
 * @param new_device_name Le nouveau que la carte va utiliser pour son Access Point et son OTA.
 * @param new_device_password Le nouveau mot de Passe que la carte va utiliser pour son Access Point et son OTA.
 * @param jsonDocument Le document JSON qui contient la configuration Réseau.
 */
void update_network_config( const char* new_net_ssid, const char* new_net_password, JsonDocument& jsonDocument ) {
  bool networkCredentialsChanged = false;

  if ( strcmp(new_net_ssid, net_ssid.c_str()) != 0 ) {
    networkCredentialsChanged = true;
  }
  net_ssid = String(new_net_ssid);
  jsonDocument["wifi"]["net_ssid"] = net_ssid;

  if ( strcmp(new_net_password, net_ssid.c_str()) != 0 ) {
    networkCredentialsChanged = true;
  }
  net_password = String(new_net_password);
  jsonDocument["wifi"]["net_password"] = net_password;

  if ( networkCredentialsChanged ) {
    // onNetworkCredentialsChanged(net_ssid.c_str(), net_password.c_str());
    InvokeNetworkCredentialsChangedCallbacks(net_ssid.c_str(), net_password.c_str());
  }
}

/**
 * @brief Permet de mettre à jour le SSID et le mot de Passe du point d'accès réseau que va utiliser Le Contact Tracer.\n
 * Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte.
 * 
 * Le SSID est alors stocké dans la variable @ref net_ssid;\n
 * et le mot de Passe dans @ref net_password.
 * 
 * @remark Cette fonction ouvre un document JSON.
 * @warning NE PAS UTILISER CETTE FONCTION AVEC UN DOCUMENT JSON DEJA OUVERT.
 * 
 * @param new_net_ssid Le nouveau SSID réseau que la carte va utiliser.
 * @param new_net_password Le nouveau mot de Passe réseau que la carte va utiliser
 */
void update_network_config( const char* new_net_ssid, const char* new_net_password ) {

  read_write_file(CONFIG_FILE_PATH, [new_net_ssid, new_net_password](JsonDocument& jsonDocument, File& jsonFile) {

    update_network_config(new_net_ssid, new_net_password, jsonDocument);

  }); 

}

/**
 * @brief Permet de mettre à jour les variables de monitoring.\n
 * Les nouvelles variables sont stockées dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le temps de monitoring est alors stocké dans la variable @ref monitoring_time;\n
 * et la distance de monitoring dans @ref monitoring_distance.
 * 
 * @remark Cette fonction n'ouvre pas de document JSON, elle utilise celui passé en paramètre.
 * 
 * @param new_time Le nouveau temps de monitoring.
 * @param new_distance La nouvelle distance de monitoring.
 * @param jsonDocument Le document JSON qui contient les données de Surveillance.
 */
void update_monitoring_config( float new_time, float new_distance, JsonDocument& jsonDocument ) {

  bool monitoringConfigChanged = false;

  if ( monitoring_time != new_time ) {
    monitoringConfigChanged = true;
  }
  monitoring_time = new_time;
  jsonDocument["monitoring"]["time"] = monitoring_time;

  if ( monitoring_distance != new_distance ) {
    monitoringConfigChanged = true;
  }
  monitoring_distance = new_distance;
  jsonDocument["monitoring"]["distance"] = monitoring_distance;

  if ( monitoringConfigChanged ) {
    // onMonitoringConfigChanged(monitoring_time, monitoring_distance);
    InvokeMonitoringConfigChangedCallbacks(monitoring_time, monitoring_distance);
  }

}

/**
 * @brief Permet de mettre à jour les variables de monitoring.\n
 * Les nouvelles variables sont stockées dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le temps de monitoring est alors stocké dans la variable @ref monitoring_time;\n
 * et la distance de monitoring dans @ref monitoring_distance.
 * 
 * @remark Cette fonction ouvre un document JSON.
 * @warning NE PAS UTILISER CETTE FONCTION AVEC UN DOCUMENT JSON DEJA OUVERT.
 * 
 * @param new_time Le nouveau temps de monitoring.
 * @param new_distance La nouvelle distance de monitoring.
 */
void update_monitoring_config( float new_time, float new_distance ) {

  read_write_file(CONFIG_FILE_PATH, [new_time, new_distance](JsonDocument& jsonDocument, File& jsonFile) {

    update_monitoring_config(new_time, new_distance, jsonDocument);

  });

}

/**
 * @brief Permet de mettre à jour le Status de Santé de l'Utilisateur.\n
 * Le nouveau Status est stocké dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le Status de Santé est alors stocké dans la variable @ref user_status.
 * 
 * @remark Cette fonction n'ouvre pas de document JSON, elle utilise celui passé en paramètre.
 * 
 * @param new_health_status Le nouveau Status de Santé de l'Utilisateur.
 * @param jsonDocument Le document JSON qui contient les données de Surveillance.
 */
void update_health_status( HealthStatus new_health_status, JsonDocument& jsonDocument ) {

  bool healthStatusChanged = false;

  if ( user_status != new_health_status ) {
    healthStatusChanged = true;
  }
  jsonDocument["status"] = new_health_status == HEALTHY ? "HEALTHY" : new_health_status == CONTACT ? "CONTACT" : "SICK";
  user_status = new_health_status;

  if ( healthStatusChanged ) {
    // onHealthStatusChanged(health_status);
    InvokeHealthStatusChangedCallbacks(user_status);
  }

}

/**
 * @brief Permet de mettre à jour le Status de Santé de l'Utilisateur.\n
 * Le nouveau Status est stocké dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le Status de Santé est alors stocké dans la variable @ref user_status.
 * 
 * @remark Cette fonction ouvre un document JSON.
 * @warning NE PAS UTILISER CETTE FONCTION AVEC UN DOCUMENT JSON DEJA OUVERT.
 * 
 * @param new_health_status Le nouveau Status de Santé de l'Utilisateur.
 */
void update_health_status( HealthStatus new_health_status ) {

  read_write_file(CONFIG_FILE_PATH, [new_health_status](JsonDocument& jsonDocument, File& jsonFile) {

    update_health_status(new_health_status, jsonDocument);

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
 * @brief Permet de récupérer les variables de configuration.\n
 * variables de configuration stockées dans la mémoire (SPIFFS) de la carte.\n\n
 * Lors de l'appel de cette fonction, les variables globales sont mises à jour avec les valeurs présentes dans le fichier de configuration.\n\n
 * 
 * Le nom de la carte est alors stocké dans la variable @ref device_name;\n
 * le mot de Passe de la carte est stocké dans @ref device_password;\n
 * le SSID du réseau est stocké dans @ref net_ssid;\n
 * le mot de Passe du réseau est stocké dans @ref net_password;\n
 * le statut de l'utilisateur est stocké dans @ref user_status.
 * le temps de monitoring est stocké dans @ref monitoring_time.
 * la distance de monitoring est stockée dans @ref monitoring_distance.
 */
void get_config() {
  
  read_write_file(CONFIG_FILE_PATH, [](JsonDocument& jsonDocument, File& jsonFile) {


    // On récupère les données du fichier de configuration et on les stocke dans les variables globales
    // Si une donnée n'est pas présente dans le fichier, on lui attribue une valeur par défaut


    auto json_name = jsonDocument["device"]["name"];
    if ( json_name.isNull() ) {
      MYDEBUG_PRINTLN("-SPIFFS : Pas de nom de device dans le fichier de configuration");
      json_name = "ContactTracer " + String(random(0xffff), HEX);
    }

    auto json_password = jsonDocument["device"]["password"];
    if ( json_password.isNull() ) {
      MYDEBUG_PRINTLN("-SPIFFS : Pas de mot de passe dans le fichier de configuration");
      json_password = DEFAULT_PASSWORD;
    }

    update_device_config(json_name.as<const char*>(), json_password.as<const char*>(), jsonDocument);



    auto json_status = jsonDocument["status"];
    if ( json_status.isNull() || json_status == "" ) {
      MYDEBUG_PRINTLN("-SPIFFS : Pas de statut dans le fichier de configuration");
      json_status = DEFAULT_USER_STATUS;
    }

    HealthStatus new_health_status = json_status == "HEALTHY" ? HEALTHY : json_status == "CONTACT" ? CONTACT : SICK;

    update_health_status(new_health_status, jsonDocument);



    auto json_net_ssid = jsonDocument["wifi"]["net_ssid"];
    if ( json_net_ssid.isNull() ) {
      MYDEBUG_PRINTLN("-SPIFFS : Pas de SSID NET dans le fichier de configuration");
      json_net_ssid = DEFAULT_NET_SSID;
    }

    auto json_net_password = jsonDocument["wifi"]["net_password"];
    if ( json_net_password.isNull() ) {
      MYDEBUG_PRINTLN("-SPIFFS : Pas de mot de passe NET dans le fichier de configuration");
      json_net_password = DEFAULT_NET_PASSWORD;
    }

    update_network_config(json_net_ssid.as<const char*>(), json_net_password.as<const char*>(), jsonDocument);

    

    auto json_monitoring_time = jsonDocument["monitoring"]["time"];
    if ( json_monitoring_time.isNull() ) {
      MYDEBUG_PRINTLN("-SPIFFS : Pas d'intervalle de surveillance dans le fichier de configuration");
      json_monitoring_time = DEFAULT_MONITORING_TIME;
    }

    auto json_monitoring_distance = jsonDocument["monitoring"]["distance"];
    if ( json_monitoring_distance.isNull() ) {
      MYDEBUG_PRINTLN("-SPIFFS : Pas de distance de surveillance dans le fichier de configuration");
      json_monitoring_distance = DEFAULT_MONITORING_DISTANCE;
    }


    update_monitoring_config(json_monitoring_time.as<float>(), json_monitoring_distance.as<float>(), jsonDocument);

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

  get_config();
}
