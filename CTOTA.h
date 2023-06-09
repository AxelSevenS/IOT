/**
* \file CTOTA.h
* \page CTOTA CTOTA
* Gestion des mises à jour OTA.
* Dans le but de pouvoir mettre à jour le firmware de l'ESP32 à distance.
* (Non fonctionnel)
*
* \ref CTOTA.h
*/

#pragma once

#include <ArduinoOTA.h>
// #include <RemoteDebug.h>
#include "CTTicker.h"
#include "CTSPIFFS.h"
#include "CTConfig.h"


// RemoteDebug Debug;
// Ticker debugTicker;

// /**
//  * Fonction appelée par un ticker pour générer des messages de debug à différents niveaux
//  */
// void generateDebugLog(){
//   rdebugVln("-Remote DEBUG : Message VERBOSE");
//   rdebugDln("-Remote DEBUG : Message DEBUG");
//   rdebugIln("-Remote DEBUG : Message INFO");
//   rdebugWln("-Remote DEBUG : Message WARNING");
//   rdebugEln("-Remote DEBUG : Message ERROR");
// }

void start_OTA(const char* hostname, const char* password){
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPassword(password);
  ArduinoOTA.begin();

  // Debug.begin(hostname);
  // Debug.setPassword(password);
}

/**
 * Configuration et démarrage des services OTA & Remote Debug
 */
void setupOTA(){
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  read_file(CONFIG_FILE_PATH, [](JsonDocument& configDocument, File& configFile) {

    String json_device_name = (configDocument["device"]["name"]).as<String>();
    String json_device_password = (configDocument["device"]["password"]).as<String>();

    start_OTA(json_device_name.c_str(), json_device_password.c_str());

  });

  AddDeviceCredentialsChangedCallback([](const char* name, const char* password) {
    MYDEBUG_PRINTLN("OTA : Nouveaux identifiants reçus");
    start_OTA(name, password);
  });
  
  // onDeviceCredentialsChanged += [](const char* ssid, const char* password) {
  //   MYDEBUG_PRINTLN("OTA : Nouveaux identifiants reçus");
  //   start_OTA(ssid, password);
  // };

  // Debug.setResetCmdEnabled(true);
  // Debug.showColors(true);
  // Debug.setSerialEnabled(true);

  // myTicker.attach(2, generateDebugLog);
}

/**
 * A chaque itération, on verifie si une mise a jour nous est envoyée.
 * Si tel est cas, la bibliothèque ArduinoOTA se charge de tout !
 */
void loopOTA(){
  ArduinoOTA.handle();
  // Debug.handle();
}
