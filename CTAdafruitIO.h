/**
 * @brief Gestion de la connexion au broker Adafruit IO et des données de télémétrie .
 * Adafruit nous permet de recuperer les états de santé des utilisateurs et de les afficher sur une interface web.
 * 
 * @page AdafruitIO Adafruit IO MQTT
 * @ref CTAdafruitIO.h
 * @file CTAdafruitIO.h
 */
 
#pragma once

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "CTTicker.h"
#include "CTBLE.h"
#include "CTMonitoring.h"

/************************* Configuration *************************************/

#define IO_SERVER         "io.adafruit.com"
#define IO_SERVERPORT     1883
#define IO_USERNAME       "AxelSeven"
#define IO_KEY            "aio_ABSF03axiYmBxXpX1rPa1FqKA1sF"

#define FEED_FREQ           10
#define PROCESS_PACKETS_MS  10000

#define FEED_HEALTH           "/feeds/health"


/************************** Variables ****************************************/
// Instanciation du client WiFi qui servira à se connecter au broker Adafruit
WiFiClient client;
Adafruit_MQTT_Client MyAdafruitMqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);
uint32_t uiSliderValue=0;


/****************************** Feeds ****************************************/
Adafruit_MQTT_Publish healthPublish = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_HEALTH);
Adafruit_MQTT_Subscribe health = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_HEALTH, MQTT_QOS_1);



/**
 * @brief Callback de réception des données de santé\n
 * Va mettre à jour le statut de l'utilisateur en fonction des données reçues
 */
void healthCallback(char *data, uint16_t len){


  MYDEBUG_PRINT("-Adafruit : Réception des Données de Santé Utilisateur : ");
  MYDEBUG_PRINTLN(data);

  char* chars_array = strtok(data, "=");
  String address = String(chars_array);

  chars_array = strtok(NULL, "=");
  const char* status = chars_array;

  // Do nothing if the callback is from this device
  if ( String(BLEDevice::getAddress().toString().c_str()) == address ) {
    return;
  }


  update_device_status(address.c_str(), status);

}

/**
 * @brief Connexion au broker Adafruit
 * 
 * @param cycles Nombre de tentatives de connexion
*/
bool connectAdafruitIO(int cycles = 50) {

  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  if (MyAdafruitMqtt.connected()) {
    return true; 
  }

  MYDEBUG_PRINT("-AdafruitIO : Connexion au broker");

  int8_t ret = MyAdafruitMqtt.connect();
  if (ret == 0) {
    MYDEBUG_PRINTLN("\n-AdafruitIO : Connecté au Broker.");
    return true;
  }
  // for (int i = 0; i < cycles; i++) {

  //   if ((ret = ) == 0) {
  //     MYDEBUG_PRINTLN("\n-AdafruitIO : Connecté au Broker.");
  //     return true;
  //   }
  //   MYDEBUG_PRINT(".");
  //   // delay(100);
  // }

  MYDEBUG_PRINT("\n-AdafruitIO : [ERREUR : ");
  MYDEBUG_PRINT(MyAdafruitMqtt.connectErrorString(ret));
  MYDEBUG_PRINT("] Impossible de se connecter au broker\n");

  MyAdafruitMqtt.disconnect();

  return false;
}


/**
 * @brief Envoi des données de télémétrie au broker Adafruit
*/
void sendData(){
  
  if ( !connectAdafruitIO() ) {
    return;
  }
  

  char* healthStatePublish;
  const char* address = BLEDevice::getAddress().toString().c_str();
  const char* statusDisplay = user_status == HEALTHY ? "HEALTHY" : user_status == CONTACT ? "CONTACT" : "SICK";
  asprintf(&healthStatePublish, "%s=%s", address, statusDisplay);

  MYDEBUG_PRINT("-AdafruitIO TICKER : Envoi des données : ");
  MYDEBUG_PRINTLN(healthStatePublish);
  
  bool healthPublishState = healthPublish.publish(healthStatePublish);
  if ( !healthPublishState ) {
    MYDEBUG_PRINTLN("-AdafruitIO : [ERREUR] Impossible d'envoyer les données de santé");
  }
}

/**
 * Configuration de la connexion au broker Adafruit IO
 * - Connexion WiFi
 * - Configuration de l'actuateur
 * - Configuration des callbacks
 */
void setupAdafruitIO() {

  health.setCallback(healthCallback);
  MyAdafruitMqtt.subscribe(&health);

  AddHealthStatusChangedCallback([](HealthStatus status) {
    sendData();
  });

  myTicker.attach(FEED_FREQ, sendData);
}

/**
 * Boucle Adafruit IO
 * - Vérification de l'état de la connexion
 * - Traitement des messages reçus
 * - Maintien de la connexion en vie avec un Ping si on aucun publish télémétrie n'est fait
 */
void loopAdafruitIO() {
  if ( !MyAdafruitMqtt.connected() ) {
    return; 
  }
  
  MyAdafruitMqtt.processPackets(PROCESS_PACKETS_MS);
  if( !MyAdafruitMqtt.ping() ) {
    MyAdafruitMqtt.disconnect();
  }
}