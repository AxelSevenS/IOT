/**
 * \file MyAdafruitIO.h
 * \page adafruitio Adafruit IO
 * \brief Adafruit IO Platform
 * 
 * <H2>Adafruit IO</H2>
 * Nous allons utiliser la plateforme Adafruit IO en guise de première approche de MQTT.
 * Pour ce faire, installez la bibliothèque "Adafruit IO Arduino" ainsi que toutes ses dépendances.
 * - Rendez vous sur la plateforme : https://io.adafruit.com/
 * - Enregistrez vous : https://accounts.adafruit.com/users/sign_up
 * - Une fois connecté, récupérez votre Aadafruit Key en cliquant sur "My Key"
 * - Copier vos identifiants pour Arduino (IO_USERNAME et IO_KEY) et coller dans la section
 *   configuration ci-dessous
 * - Dans la page des Feeds (topics MQTT), créer les Feeds suivants, sans les rattacher à un groupe : 
 *   - slider 
 *   - onoff
 *   - temperature
 *   - humidity
 *   
 *   Adafruit a défini la notion de Feeds qui est utilisé indifféremment pour des attributs et
 *   des données de télémétrie MQTT.
 * - Dans la page des Dashboards, créer un dashboard et éditer le. Dans celui-ci, ajouter les "blocks" suivants:
 *   - Un Slider associé au feed slider
 *   - Un Toggle associé au feed onoff, en laissant les valeurs par défaut ("ON" et "OFF")
 *   - Un Indicator associé au feed onoff, en indiquant la condition "=ON"
 *   - un Line Chart associé aux feeds temperature et humidity
 * - Tester le code en le téléversant
 * - Installer l'application IFTTT sur votre téléphone mobile
 *   - Tester différentes actions possibles avec Adafruit IO
 *     - If this : Buton widget / Button press
 *     - Then that : Adafruit / Send data to Adafruit IO. Sélectionner votre feed et renseigner
 *       la valeur que vous voulez transmettre (ON par exemple)
 *       A noter que vous pouvez ajouter des "ingrédients" qui sont d'autres données qui sont à
 *       votre disposition (du téléphone ou depuis le service appelé en "If this".
 * 
 * Documentation sur l'API MQTT : https://learn.adafruit.com/adafruit-io/mqtt-api
 * Voici un exemple de clients qui utilisent l'API MQTT : https://learn.adafruit.com/desktop-mqtt-client-for-adafruit-io/overview
 * Vous pouvez aussi utiliser la REST API pour intéragir avec la plateforme Adafruit IO.
 * 
 * <H2>Bibliothèque à installer</H2>
 * Pour utiliser l'API MQTT d'Adafruit, installer :
 * - Adafruit MQTT library by Adafruit : https://github.com/adafruit/Adafruit_MQTT_Library
 * 
 * Fichier \ref MyAdafruitIO.h
 */
#pragma once

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "MyBLE.h"

/************************* Configuration *************************************/
// Connexion Adafruit
#define IO_SERVER         "io.adafruit.com"
#define IO_SERVERPORT     1883
#define IO_USERNAME       "feur25"
#define IO_KEY            "aio_woCH11XUSWVgHDhatnxPe8B9BcZz"
// Feeds
#define FEED_SLIDER           "/feeds/slider"
#define FEED_ONOFF            "/feeds/onoff"
#define FEED_TEMPERATURE      "/feeds/temperature"
#define FEED_HUMIDITY         "/feeds/humidity"
#define FEED_HEALTH           "/feeds/health"

// Frequence d'envoi des données
#define FEED_FREQ           10
#define PROCESS_PACKETS_MS  10000

/************************** Variables ****************************************/
// Instanciation du client WiFi qui servira à se connecter au broker Adafruit
WiFiClient client;
// Instanciation du client Adafruit avec les informations de connexion
Adafruit_MQTT_Client MyAdafruitMqtt(&client, IO_SERVER, IO_SERVERPORT, /* IO_USERNAME,  */IO_USERNAME, IO_KEY);
// Variable de stockage de la valeur du slider
uint32_t uiSliderValue=0;
Ticker MyAdafruitTicker;


/****************************** Feeds ****************************************/
// Création des Feed auxquels nous allons souscrire :
// Un FEED 'time' pour récupérer l'heure
// Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_TEMPERATURE);
// Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_HUMIDITY);
Adafruit_MQTT_Publish healthPublish = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_HEALTH);

// Adafruit_MQTT_Subscribe timefeed = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, "time/seconds");
// Adafruit_MQTT_Subscribe slider = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_SLIDER, MQTT_QOS_1);
// Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_ONOFF, MQTT_QOS_1);
Adafruit_MQTT_Subscribe health = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_HEALTH, MQTT_QOS_1);

//s'abonne un l'état de santé d'un utilisateur
// Adafruit_MQTT_Subscribe ownerHealth = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_OWNER_HEALTH, MQTT_QOS_1);
// Adafruit_MQTT_Publish ownerHealthPublish = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_OWNER_HEALTH);

/*************************** Sketch Code ************************************/
int sec;
int mini;
int hour;

int timeZone = 1; // GMT+1 pour la France



void healthCallback(char *data, uint16_t len){


  char* chars_array = strtok(data, "=");
  String address = String(chars_array);

  chars_array = strtok(NULL, "=");
  const char* status = chars_array;

  // Do nothing if the callback is from this device
  if ( String(BLEDevice::getAddress().toString().c_str()) == address ) {
    return;
  }

  MYDEBUG_PRINT("-Adafruit : Réception des Données de Santé Utilisateur : ");
  MYDEBUG_PRINTLN(data);


  SPIFFS.begin();

  File file = SPIFFS.open(strConfigFile, "r");
  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, file);
  
  if (error){
    MYDEBUG_PRINT("-SPIFFS : [ERREUR : ");
    MYDEBUG_PRINT(error.c_str());
    MYDEBUG_PRINTLN("] Impossible de parser le JSON, création d'un nouveau fichier");
  }

  auto monitored = jsonDocument["monitored"];
  auto updatedUser = monitored[address];
  if ( updatedUser["contact"] == true && status == "SICK" && updatedUser["status"] != "SICK" && userStatus == SAFE ) {
    //TODO : envoyer un message à l'utilisateur pour lui dire qu'il est en contact avec un malade
    MYDEBUG_PRINTLN("Envoi d'un message à l'utilisateur pour lui dire qu'il est en contact avec un malade");
    userStatus = CONTACT;
    jsonDocument["status"] = userStatus == SAFE ? "SAFE" : userStatus == CONTACT ? "CONTACT" : "SICK";
  }
  updatedUser["status"] = status;


  file.close();
  file = SPIFFS.open(strConfigFile, "w");

  serializeJson(jsonDocument, file);

  file.close();
  SPIFFS.end();

}

/**
 * Connexion au broker Adafruit IO
 */
bool connectAdafruitIO(int cycles = 50) {

  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  if (MyAdafruitMqtt.connected()) {                       // Si déjà connecté, alors c'est tout bon
    return true; 
  }

  MYDEBUG_PRINT("-AdafruitIO : Connexion au broker");

  int8_t ret;
  for (int i = 0; i < cycles; i++) {

    if ((ret = MyAdafruitMqtt.connect()) == 0) {
      MYDEBUG_PRINTLN("\n-AdafruitIO : Connecté au Broker.");
      return true;
    }
    MYDEBUG_PRINT(".");
    delay(100);
  }

  MYDEBUG_PRINT("\n-AdafruitIO : [ERREUR : ");
  MYDEBUG_PRINT(MyAdafruitMqtt.connectErrorString(ret));
  MYDEBUG_PRINT("] Impossible de se connecter au broker ...\n");

  MyAdafruitMqtt.disconnect();                                  // Deconnexion pour être propre

  return false;
}


/**
 * Récupération et envoi des données de télémétrie
 */
void getAndSendDataToAdafruit(){
  
  if ( !connectAdafruitIO() ) { // Connexion au broker Adafruit
    return;
  }
  // connectAdafruitIO();
  
  MYDEBUG_PRINTLN("-AdafruitIO TICKER : Envoi des données");

  // float myRandomTemp = 20+(float)random(-50,50)/10;
  // float myRandomHum = 50+(float)random(-100,100)/10;


  if (userStatus == SAFE) {
    return;
  }

  char* healthStatePublish;
  const char* address = BLEDevice::getAddress().toString().c_str();
  const char* statusDisplay = userStatus == SAFE ? "SAFE" : userStatus == CONTACT ? "CONTACT" : "SICK";
  asprintf(&healthStatePublish, "%s=%s", address, statusDisplay);

  // temperatureFeed.publish(myRandomTemp);
  // humidityFeed.publish(myRandomHum);
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

  // if ( !connectAdafruitIO() ) { // Connexion au broker Adafruit
  //   return;
  // }

  health.setCallback(healthCallback);
  MyAdafruitMqtt.subscribe(&health);

  MyAdafruitTicker.attach(FEED_FREQ, getAndSendDataToAdafruit);
}

/**
 * Boucle Adafruit IO
 * - Vérification de l'état de la connexion
 * - Traitement des messages reçus
 * - Maintien de la connexion en vie avec un Ping si on aucun publish télémétrie n'est fait
 */
void loopAdafruitIO() {
  connectAdafruitIO();
  
  MyAdafruitMqtt.processPackets(PROCESS_PACKETS_MS);
  if( !MyAdafruitMqtt.ping() ) {
    MyAdafruitMqtt.disconnect();
  }
}