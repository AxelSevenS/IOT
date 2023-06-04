/**
 * \file CTHealth.h
 * \page CTHealth CTHealth
 * CTHealth est un fichier qui permet de gérer le Status de Santé de l'Utilisateur 
 * le changement de statut s'effectue via la fonction :
 * @ref update_health_status
 * L'historique de rencontre des statuts des autres esp seront stocké dans un fichier JSON qui sera stockés dans la memoire (SPIFFS),
 * ainsi que le statut de santé de l'Esp.
 *
 * 
 * \ref CTHealth.h
 */
#pragma once

#include <vector>
#include "CTSPIFFS.h"
#include "CTConfig.h"


#define DEFAULT_USER_STATUS         "HEALTHY"



enum HealthStatus {
  HEALTHY = 2,
  CONTACT = 1,
  SICK = 0
};


HealthStatus user_status = HEALTHY;


/**
 * @brief AddDeviceCredentialsChangedCallback => Permet de récupérer les informations de connexion de l'appareil
 */
typedef void(*HealthCallback)(HealthStatus status);


std::vector<HealthCallback> onHealthStatusChangedCallbacks;
void AddHealthStatusChangedCallback(HealthCallback callback) {
  onHealthStatusChangedCallbacks.push_back(callback);
}

void InvokeHealthStatusChangedCallbacks(HealthStatus status) {
  for (auto callback : onHealthStatusChangedCallbacks) {
    callback(status);
  }
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

// void get_health_status( JsonDocument& jsonDocument ) {
//   auto json_status = jsonDocument["status"];
//   if ( json_status.isNull() || json_status == "" ) {
//     MYDEBUG_PRINTLN("-SPIFFS : Pas de statut dans le fichier de configuration");
//     json_status = DEFAULT_USER_STATUS;
//   }

//   HealthStatus new_health_status = json_status == "HEALTHY" ? HEALTHY : json_status == "CONTACT" ? CONTACT : SICK;

//   update_health_status(new_health_status, jsonDocument);
// }

// void get_health_status() {
//   read_write_file( CONFIG_FILE_PATH, [](JsonDocument& jsonDocument, File& jsonFile) {
//     get_health_status(jsonDocument);
//   } );
// }