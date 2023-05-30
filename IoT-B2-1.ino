#define MYDEBUG         1 

// ------------------------------------------------------------------------------------------------
// MODULES
#include "MyDebug.h"        // Debug
#include "MyDeepSleep.h"    // Deep Sleep
#include "MyTicker.h"       // Tickers

#include "MySPIFFS.h"       // Flash File System
#include "MyWiFi.h"         // WiFi
#include "MyWebServer.h"    // Serveur Web
//#include "MyNTP.h"          // Network Time Protocol
// #include "MyAdafruitIO.h"   // Adafruit MQTT
#include "MyBLE.h"          // BLE
//#include "MyOTA.h"          // Over the air
//#include "MyLED.h"          // LED
//#include "MyDHT.h"          // Capteur de température et humidité
//#include "MyCore0.h"        // Utilisation du Core 0


void setup() {
  Serial.begin(230400); // Initialisation de la vitesse de communication du port série
  Serial.println("Ouverture du port série");
  setupDebug();
  // setupDeepSleep();
  // setupTicker();      // Initialisation d'un ticker

  MYDEBUG_PRINTLN("------------------- SETUP");
  setupSPIFFS();      // Initialisation du système de fichiers
  setupWiFi();        // Initialisation du WiFi
  setupWebServer();   // Initialisation du Serveur Web
//  setupNTP();         // Initialisation de la connexion avec le serveur NTP (heure)
//  getNTP();           // Récupération de l'heure
//  setupAdafruitIO();  // Initialisation Adafruit MQTT
  setupBLEServer();   // Initialisation du serveur BLE pour publier un ID
  setupBLEClient();   // Initialisation du client BLE pour scanner les ID à proximité
//  setupOTA();         // Initialisation du mode Over The Air
//  setupLED();         // Initialisation de la LED
//  setupDhtSensor();   // Initialisation du capteur DHT
//  setupMyCore0();     // Initialisation du Core 0
}

int i=0;
void loop() {
  loopWiFi();
  loopWebServer();
//  loopAdafruitIO();
  loopBLEClient();
//  loopOTA();
//  playWithLED();
//  getDhtData();
//  delay(20);        // Délai pour que le CPU puisse passer à d'éventuelles autres tâches
}
