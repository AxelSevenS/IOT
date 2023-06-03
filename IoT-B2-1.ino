#define MYDEBUG         1 

// ------------------------------------------------------------------------------------------------
// MODULES
#include "MyDebug.h"        // Debug
#include "MyDeepSleep.h"    // Deep Sleep
#include "MyTicker.h"       // Tickers
#include "Events.h"         // Events

#include "MySPIFFS.h"       // Flash File System
#include "MyWiFi.h"         // WiFi
#include "MyWebServer.h"    // Serveur Web
#include "MyNTP.h"          // Network Time Protocol
#include "MyAdafruitIO.h"   // Adafruit MQTT
#include "MyBLE.h"          // BLE
#include "MyOTA.h"          // Over the air
//#include "MyLED.h"          // LED
//#include "MyDHT.h"          // Capteur de température et humidité
//#include "MyCore0.h"        // Utilisation du Core 0


void setup() {
  setupDebug();
  // setupDeepSleep();
  // setupTicker();

  MYDEBUG_PRINTLN("------------------- SETUP");
  setupSPIFFS();
  setupWiFi();
  setupWebServer();
  setupNTP();
  getNTP();
  setupBLEServer();
  setupBLEClient();
  setupAdafruitIO();
  setupOTA();
//  setupLED();
//  setupDhtSensor();
//  setupMyCore0();
}

// int i=0;
void loop() {
  loopWiFi();
  loopWebServer();
  loopBLEClient();
  loopAdafruitIO();
  loopOTA();
//  playWithLED();
//  getDhtData();
  delay(20);        // Délai pour que le CPU puisse passer à d'éventuelles autres tâches
}
