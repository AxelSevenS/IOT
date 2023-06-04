/**
* \file CTMonitoring.h
* \page CTMonitoring CTMonitoring
* CTMonitoring permet de gérer la surveillance des contacts entre les utilisateurs (monitoring). 
* 
* Plusieurs infos sont recupérés (Nom,Temps d'expositions ect...).
* Les données obtenus sont traités et stockés dans un fichier JSON sur la mémoire de la carte.
*
*\ref CTMonitoring.h
*/
#pragma once

#include "vector"
#include "CTSPIFFS.h"
// #include <ArduinoJson.h>
#include "CTConfig.h"
#include "CTNTP.h"


#define MONITORING_FILE_PATH        "/monitoring.json"

#define DEFAULT_MONITORING_TIME     5.0
#define DEFAULT_MONITORING_DISTANCE 5.0

typedef void(*MonitoringCallback)(float time, float distance);


float monitoring_time = 5.0;
float monitoring_distance = 5.0;


std::vector<MonitoringCallback> onMonitoringConfigChangedCallbacks;
void AddMonitoringConfigChangedCallback(MonitoringCallback callback) {
    onMonitoringConfigChangedCallbacks.push_back(callback);
}

void InvokeMonitoringConfigChangedCallbacks(float time, float distance) {
    for (auto callback : onMonitoringConfigChangedCallbacks) {
        callback(time, distance);
    }
}

/**
 * @brief Vérifie si le device est marqué comme malade dans le fichier de monitoring et met à jour le status de l'utilisateur si c'est le cas
 * 
 * @param monitoringDocument
 * @param monitoringFile
 * @param otherDeviceAddress
*/
void become_contact_if_device_is_sick(JsonDocument& monitoringDocument, File& monitoringFile, const char* otherDeviceAddress) {

  // si le device est marqué pour surveillance rapprochée et qu'il est malade, marquer l'utilisateur comme contact
  if ( monitoringDocument[otherDeviceAddress]["close_monitoring"] == true && monitoringDocument[otherDeviceAddress]["status"] == "SICK" && user_status == HEALTHY ) {
    //TODO : envoyer un message à l'utilisateur pour lui dire qu'il est en contact avec un malade
    update_health_status(CONTACT, monitoringDocument);
  }

}

/**
 * @brief Met à jour le status d'un device dans le fichier de monitoring
 * 
 * @param address
 * @param status
 * 
*/
void update_device_status(const char* address, const char* status) {
    
  read_write_file(MONITORING_FILE_PATH, [=](JsonDocument& monitoringDocument, File& monitoringFile) {
    auto otherDevice = monitoringDocument[address];

    MYDEBUG_PRINT("DEVICE : Updated status of ");
    MYDEBUG_PRINTLN(otherDevice.as<String>().c_str());
    MYDEBUG_PRINTLN(status);

    // mise à jour de l'état du device dans le fichier de config
    otherDevice["status"] = "TEST";

    become_contact_if_device_is_sick(monitoringDocument, monitoringFile, address);
  });

}

/**
 * @brief Effectue une "interaction" avec un device, c'est à dire met à jour son nom, son temps d'exposition et son statut dans le fichier de monitoring\n
 * Si le temps d'exposition est supérieur à la durée de surveillance, le device est marqué pour surveillance rapprochée
 * 
 * @param address
 * @param name
 * @param last_contact
 * 
*/
void device_interact(const char* address, const char* name, long last_contact) {
  
  MYDEBUG_PRINT("DEVICE : Interacted with ");
  MYDEBUG_PRINTLN(name);
    
  read_write_file(MONITORING_FILE_PATH, [=](JsonDocument& monitoringDocument, File& monitoringFile) {
    auto otherDevice = monitoringDocument[address];

    // mettre à jour le nom du device à chaque interaction, car il peut changer
    if ( !otherDevice["name"].isNull() != 0 ) {
        otherDevice["name"] = name;
    }

    // si c'est la première fois qu'on rencontre le device, assigner le temps de première rencontre
    auto first_contact = otherDevice["first_contact"];
    if ( first_contact.isNull() ) {
      first_contact = getNTP();
    }

    // mettre à jour le temps d'exposition
    auto total_exposure = otherDevice["total_exposure"];
    float exposure_time = total_exposure.as<float>();
    float time_since_last_contact = (millis() - last_contact) / 1000.0;

    if ( total_exposure.isNull() ) {
      total_exposure = time_since_last_contact;
    } else {
      total_exposure = exposure_time + time_since_last_contact;
    }
    exposure_time = total_exposure.as<float>();

    // marquer le device pour surveillance rapprochée si le temps d'exposition est supérieur à la durée de surveillance
    if (exposure_time > (float)monitoring_time * 60) {
      otherDevice["close_monitoring"] = true;
    }

    become_contact_if_device_is_sick(monitoringDocument, monitoringFile, address);

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

// void get_monitoring_config( JsonDocument& jsonDocument ) {

//   auto json_monitoring_time = jsonDocument["monitoring"]["time"];
//   if ( json_monitoring_time.isNull() ) {
//     MYDEBUG_PRINTLN("-SPIFFS : Pas d'intervalle de surveillance dans le fichier de configuration");
//     json_monitoring_time = DEFAULT_MONITORING_TIME;
//   }

//   auto json_monitoring_distance = jsonDocument["monitoring"]["distance"];
//   if ( json_monitoring_distance.isNull() ) {
//     MYDEBUG_PRINTLN("-SPIFFS : Pas de distance de surveillance dans le fichier de configuration");
//     json_monitoring_distance = DEFAULT_MONITORING_DISTANCE;
//   }


//   update_monitoring_config(json_monitoring_time.as<float>(), json_monitoring_distance.as<float>(), jsonDocument);
// }

// void get_monitoring_config() {
//   read_write_file( CONFIG_FILE_PATH, [](JsonDocument& jsonDocument, File& jsonFile) {
//     get_monitoring_config(jsonDocument);
//   } );
// }
