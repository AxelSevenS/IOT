#define MYDEBUG         1 

// ------------------------------------------------------------------------------------------------
// MODULES
#include "CTDebug.h"        // Debug
#include "CTTicker.h"       // Tickers
#include "CTEvents.h"         // Events

#include "CTSPIFFS.h"       // Flash File System
#include "CTWiFi.h"         // WiFi
#include "CTWebServer.h"    // Serveur Web
#include "CTNTP.h"          // Network Time Protocol
#include "CTAdafruitIO.h"   // Adafruit MQTT
#include "CTBLE.h"          // BLE
#include "CTOTA.h"          // Over the air
#include "CTLED.h"          // LED
#include "CTDHT.h"          // Capteur de température et humidité


void setup() {
  setupDebug();

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
  setupLED();
  setupDhtSensor();
}

// int i=0;
void loop() {
  loopWebServer();
  loopBLEClient();
  loopAdafruitIO();
  loopOTA();
  updateLED();
  delay(20);        // Délai pour que le CPU puisse passer à d'éventuelles autres tâches
}
