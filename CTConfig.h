/**
 * \file CTConfig.h
 * \page CTConfig.h CTConfig.h 
 * 
 * Fichier de configuration de la carte esp32.
 * La connexion 
 
 * 
 * 
 *\ref CTConfig.h   
 */

#pragma once

#include "CTDebug.h"
#include "CTSPIFFS.h"


#define CONFIG_FILE_PATH            "/config.json"


/*************************** Default Values ************************************/
#define DEFAULT_PASSWORD            "contacttracer"


/*************************** Configuration Variables ************************************/

typedef void(*CredentialsCallback)(const char* name, const char* password);


String device_name;
String device_password;

/**
 * @brief AddNetworkCredentialsChangedCallback => Permet de récupérer les informations de connexion du réseau wifi et s'y connecter
 */
std::vector<CredentialsCallback> onDeviceCredentialsChangedCallbacks;
void AddDeviceCredentialsChangedCallback(CredentialsCallback callback) {
    onDeviceCredentialsChangedCallbacks.push_back(callback);
}

/**
 * @brief InvokeDeviceCredentialsChangedCallbacks => Permet de récupérer les informations de connexion de l'appareil changées.
 */
void InvokeDeviceCredentialsChangedCallbacks(const char* name, const char* password) {
    for (auto callback : onDeviceCredentialsChangedCallbacks) {
        callback(name, password);
    }
}

/**
* @brief Permet de mettre à jour le SSID et le mot de Passe de la Contact Tracer, en tant que point d'accès réseau, dont on se sert pour la configurer.
* Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte (fichier Json).
* @param new_device_name Le nouveau ssid que la carte va utiliser pour son Access Point.
* @param new_device_password Le nouveau mot de Passe que la carte va utiliser pour son Access Point.
* @param jsonDocument Document JSON contenant les données du fichier de configuration
*/
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
*@brief Recupere les données du fichier de configuration et les stocke dans les variables globales, Si une donnée n'est pas présente dans le fichier, on lui attribue une valeur par défaut.
*renvoie un message d'erreur si le fichier de configuration est vide.
*@param jsonDocument Document JSON contenant les données du fichier de configuration
*
*/
void get_device_config( JsonDocument& jsonDocument ) {
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
void get_device_config() {
  
  read_write_file(CONFIG_FILE_PATH, [](JsonDocument& jsonDocument, File& jsonFile) {

    get_device_config(jsonDocument);

  });

}