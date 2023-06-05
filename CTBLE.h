/**
 * @brief CTBLE est un module qui permet de scanner les appareils BLE à proximité et d'interagir avec eux.
 *  Il pourra detecter et calculer la distance entre un esp et un autre.
 *  Il pourra aussi envoyer des informations à un esp à proximité.
 * 
 * @page CTBLE Bluetooth Low Energy
 * @ref CTBLE.h
 * @file CTBLE.h
 */
 
#pragma once


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
// #include <Ticker.h>
#include "CTMonitoring.h"



/************************* Configuration *************************************/
#define SERVICE_UUID     "774320e9-e25a-455c-9230-b1758c4cfa12"

#define BLE_FREQ         10
#define BLE_SCAN_TIME    5


/************************** Variables ****************************************/
BLEScan* pBLEScan;
BLEServer* pServer;
BLEService* pService;
BLEAdvertising* pAdvertising;


/**
 * @brief le temps de processeur du dernier scan
 * 
 * @remark On utilise cette variable pour augmenter le temps d'interaction avec un device dans le fichier de monitoring
*/
long BLE_last_scan;

/**
 * @brief Démarrage du serveur BLE\n
 * On remet à zéro le serveur, le service et l'advertising
 * 
 * @param device_name Nom du device BLE
*/
void start_BLE_server(const char* device_name) {

  MYDEBUG_PRINT("-BLE : Démarrage du serveur sous le nom : ");
  MYDEBUG_PRINTLN(device_name);

  BLEDevice::init(device_name);

  // On remet à zéro le serveur, le service et l'advertising
  if (pServer) {
    pServer->removeService(pService);
  } else {
    pServer = BLEDevice::createServer();
  }

  if (pService) {
    pService->stop();
  }
  pService = pServer->createService(SERVICE_UUID);
  pService->start();

  if (pAdvertising) {
    pAdvertising->stop();
  }
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);

  BLEDevice::startAdvertising();
}

/**
 * Callback quand un device BLE est détecté.
 * Dans notre cas, on vérifie si son service UUID est celui que nous recherchons.
 * Si c'est le cas, on récupère son nom et le niveau de puissance (RSSI)
 * qui nous permet de calculer une distance approximative
 * On éffectue ensuite une interaction avec le device
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {    
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (WiFi.status() != WL_CONNECTED) {
        return;
      }

      if (SERVICE_UUID != advertisedDevice.getServiceUUID().toString()) {
        return;
      }

      std::string address = advertisedDevice.getAddress().toString();
      std::string name = advertisedDevice.getName();
      
      MYDEBUG_PRINT("-BLE client : CONTACT Tracer trouvé : ");
      MYDEBUG_PRINT(address.c_str());
      MYDEBUG_PRINT(" | Nom : ");
      MYDEBUG_PRINTLN(name.c_str());


      float ratio = (float)(-69 -advertisedDevice.getRSSI())/(10 * 2);
      float distance = pow(10,ratio);

      // If the Contact Tracer is close enough, advance the "contact" timer.
      if (distance < monitoring_distance) {
        
        device_interact(address.c_str(), name.c_str(), BLE_last_scan);
        BLE_last_scan = millis();

      }
    }
};

/**
 * Scan BLE
 * - On lance le scan
 * - On récupère les résultats
 * - On vide les résultats
 */
void BLEScan() {
  BLEScanResults foundDevices = pBLEScan->start(BLE_SCAN_TIME, false);
  // MYDEBUG_PRINTLN(foundDevices.getCount());

  pBLEScan->clearResults();
}


/**
 * Configuration du serveur BLE
 * - Initialisation du serveur
 * - Initialisation du service
 * - Initialisation de l'advertising
*/
void setupBLEServer() {
  MYDEBUG_PRINTLN("-BLE Server : Démarrage");
  // get_config();

  start_BLE_server(device_name.c_str());


  // AddDeviceCredentialsChangedCallback([](const char* ssid, const char* password) {
  //   MYDEBUG_PRINTLN("-BLE Client : Nouveau SSID reçu");
  //   start_BLE_server(ssid);
  // });
  MYDEBUG_PRINTLN("-BLE Server : Démarré");
}

/**
 * Configuration du client BLE
 * - Initialisation de l'objet qui va scanner
 * - Association d'un callback
 * - Activation du scan
 */
void setupBLEClient() {
  MYDEBUG_PRINTLN("-BLE Client : Démarrage");

  BLEDevice::init(device_name.c_str());
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  BLE_last_scan = millis();

  MYDEBUG_PRINTLN("-BLE Client : Démarré");
}

/**
 * Boucle du client BLE
 * - On lance le scan
 */
void loopBLEClient() {
  BLEScan();
}
