/**
 * \file MyBLE.h
 * \page ble Bluetooth Low Energy
 * 
 * Dans le cadre de ce cours de découverte de l'IoT nous utiliserons les fonctionnalités de base du BLE, dans un mode
 * potentiellement dégradé. L'idée est que vous preniez simplement la main sur le module BLE et que vous l'intégriez
 * à l'ensemble du projet.
 * 
 * En mode serveur chaque équipement BLE
 * - Permet d'être vu par les autres équipements BLE,
 * - Propose différents services,
 * - Chaque service propose différentes caractéristiques en mode lecture ou écriture.
 * Il existe une nomenclature des services et caractéristiques, que nous n'utiliserons pas ici.
 * 
*   \image  html ESP32-ble-bulletin-board-model.png

 * En mode client, il est possible de scanner les équipements bluetooth et de potentiellement se connecter à eux
 * pour échanger des données.
 *                                             
 * Les serveurs "publient" ou font la promotion de leurs services pour que les clients puissent savoir ce qu'ils
 * proposent avant de se connecter.
 * Dans notre cas, nous publierons simplement un service et un nom d'équipement. Cela sera suffisant pour savoir
 * quels sont les équipements à proximité qui proposent un service donné "Contact Tracker" et leurs noms.
 */
#pragma once


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "MyNTP.h"

#define SERVICE_UUID        "436f6e74-6163-7420-5472-61636b657273" // "Contact Trackers" d'ascii en hexa, ça ne sert à rien mais bon                             
#define DEVICE_NAME         "ESP32"                          // Nom de votre serveur BLE qui sera détecté par les autres

#define BLE_FREQ         10


int scanTime = 5; //In seconds
BLEScan* pBLEScan;
Ticker MyBLETicker;

int iCompteur = 0;

long BLE_last_scan;

/**
 * Configuration du serveur BLE
 * - Initialisation du serveur avec un nom unique
 *   Ce nom doit être le même que celui qui vous servira pour vous abonner en MQTT !
 * - Démarrage du serveur en mode advertising pour publier ses informations : nom et service
 */
void setupBLEServer() {
  MYDEBUG_PRINT("-BLE : Démarrage du serveur sous le nom : ");
  MYDEBUG_PRINTLN(DEVICE_NAME);

  BLEDevice::init(DEVICE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();
  MYDEBUG_PRINTLN("-BLE : Serveur démarré");
}

/**
 * Callback quand un device BLE est détecté.
 * Dans notre cas, on vérifie si son service UUID est celui que nous recherchons.
 * Si c'est le cas, on récupère son nom et le niveau de puissance (RSSI)
 * qui nous permet de calculer une distance approximative
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
      
      MYDEBUG_PRINT("-BLE client : CONTACT Tracer trouvé : ");
      MYDEBUG_PRINTLN(address.c_str());


      float ratio = (float)(-69 -advertisedDevice.getRSSI())/(10 * 2);
      float distance = pow(10,ratio);

      // If the Contact Tracer is close enough, advance the "contact" timer.
      if (distance < monitoring_distance) {
        
        SPIFFS.begin();

        File configFile = SPIFFS.open(strConfigFile, "r");
        DynamicJsonDocument jsonDocument(512);
        DeserializationError error = deserializeJson(jsonDocument, configFile);
        
        if (error){
          MYDEBUG_PRINT("-SPIFFS : [ERREUR : ");
          MYDEBUG_PRINT(error.c_str());
          MYDEBUG_PRINTLN("] Impossible de parser le JSON, création d'un nouveau fichier");
        }
        


        auto monitored = jsonDocument["monitored"];

        
        auto first_contact = monitored[address]["first_contact"];
        if ( first_contact.isNull() ) {
          first_contact = getNTP();
        }


        auto total_exposure = monitored[address]["total_exposure"];
        float exposure_time = total_exposure.as<float>();
        float time_since_scan_millis = millis() - BLE_last_scan;
        BLE_last_scan = millis();
        float time_since_scan = time_since_scan_millis / (float)1000;

        if ( total_exposure.isNull() ) {
          total_exposure = time_since_scan;
        } else {
          total_exposure = exposure_time + time_since_scan;
        }

        exposure_time = total_exposure.as<float>();
        if (exposure_time > (float)monitoring_time * 60) {
          monitored[address]["contact"] = true;
        }

        if ( monitored[address]["contact"] == true && monitored[address]["status"] == "SICK" /* && userStatus == SAFE */ ) {
          //TODO : envoyer un message à l'utilisateur pour lui dire qu'il est en contact avec un malade
          MYDEBUG_PRINTLN("Envoi d'un message à l'utilisateur pour lui dire qu'il est en contact avec un malade");
          userStatus = CONTACT;
        }

        configFile.close();
        configFile = SPIFFS.open(strConfigFile, "w");

        if ( !configFile ) {
          MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en écriture");
          configFile.close();
          SPIFFS.end();
          return;
        }

        if (serializeJson(jsonDocument, configFile) == 0) {
          MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
        }

        configFile.close();
        SPIFFS.end();
      }
    }
};

void BLEScan() {
  MYDEBUG_PRINTLN("-BLE Client : Scan");
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  MYDEBUG_PRINT("-BLE Client : Devices trouvés ");
  MYDEBUG_PRINTLN(foundDevices.getCount());

  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  MYDEBUG_PRINTLN("-BLE Client : Scan terminé");
}

/**
 * Configuration du client BLE
 * - Initialisation de l'objet qui va scanner
 * - Association d'un callback
 * - Activation du scan
 */
void setupBLEClient() {
  MYDEBUG_PRINTLN("-BLE Client : Démarrage");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  BLE_last_scan = millis();

  MYDEBUG_PRINTLN("-BLE Client : Démarré");

  // MyBLETicker.attach(BLE_FREQ, BLEScan);
}

/**
 * Loop du client BLE qui va scanner les devices à proximité
 * - on limite le temps de scan à scanTime secondes pour ne pas bloquer la carte
 * - une fois terminé, on efface les résultats pour pouvoir recommencer tranquillement
 */
void loopBLEClient() {
  BLEScan();
}
