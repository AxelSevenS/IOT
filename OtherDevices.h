
#pragma once

#include "MySPIFFS.h"
#include "MyNTP.h"

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