/**
* \file CTWiFi.h
* \page CTWiFi.h 
* Fichier contenant les fonctions permettant de gérer le wifi de l'esp32 et de le configurer (connexion au point d'accés, création d'un point d'accés, etc...)
*
*\ref CTWiFi.h
*
*/

#pragma once

#include <vector>
#include <WiFi.h>
#include "CTTicker.h"
#include "CTConfig.h"
#include "CTSPIFFS.h"

#define DEFAULT_NET_SSID            "Hamburger"
#define DEFAULT_NET_PASSWORD        "YoLesKids"


bool ap_launched = false;

String net_ssid;
String net_password;


std::vector<CredentialsCallback> onNetworkCredentialsChangedCallbacks;
void AddNetworkCredentialsChangedCallback(CredentialsCallback callback) {
    onNetworkCredentialsChangedCallbacks.push_back(callback);
}

void InvokeNetworkCredentialsChangedCallbacks(const char* name, const char* password) {
    for (auto callback : onNetworkCredentialsChangedCallbacks) {
        callback(name, password);
    }
}


/**
 * @brief Permet de mettre à jour le SSID et le mot de Passe de la Contact Tracer, en tant que point d'accès réseau, dont on se sert pour la configurer.\n
 * Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte.\n\n
 * 
 * Le SSID est alors stocké dans la variable @ref device_name;\n
 * et le mot de Passe dans @ref device_password.
 * 
 * @remark Cette fonction n'ouvre pas de document JSON, elle utilise celui passé en paramètre.
 * 
 * @param new_device_name Le nouveau que la carte va utiliser pour son Access Point et son OTA.
 * @param new_device_password Le nouveau mot de Passe que la carte va utiliser pour son Access Point et son OTA.
 * @param jsonDocument Le document JSON qui contient la configuration Réseau.
 */
void update_network_config( const char* new_net_ssid, const char* new_net_password, JsonDocument& jsonDocument ) {
  bool networkCredentialsChanged = false;

  if ( strcmp(new_net_ssid, net_ssid.c_str()) != 0 ) {
    networkCredentialsChanged = true;
  }
  net_ssid = String(new_net_ssid);
  jsonDocument["wifi"]["net_ssid"] = net_ssid;

  if ( strcmp(new_net_password, net_ssid.c_str()) != 0 ) {
    networkCredentialsChanged = true;
  }
  net_password = String(new_net_password);
  jsonDocument["wifi"]["net_password"] = net_password;

  if ( networkCredentialsChanged ) {
    InvokeNetworkCredentialsChangedCallbacks(net_ssid.c_str(), net_password.c_str());
  }
}

/**
 * @brief Permet de mettre à jour le SSID et le mot de Passe du point d'accès réseau que va utiliser Le Contact Tracer.\n
 * Les nouveaux identifiants sont stockés dans la mémoire (SPIFFS) de la carte.
 * 
 * Le SSID est alors stocké dans la variable @ref net_ssid;\n
 * et le mot de Passe dans @ref net_password.
 * 
 * @remark Cette fonction ouvre un document JSON.
 * @warning NE PAS UTILISER CETTE FONCTION AVEC UN DOCUMENT JSON DEJA OUVERT.
 * 
 * @param new_net_ssid Le nouveau SSID réseau que la carte va utiliser.
 * @param new_net_password Le nouveau mot de Passe réseau que la carte va utiliser
 */
void update_network_config( const char* new_net_ssid, const char* new_net_password ) {

  read_write_file(CONFIG_FILE_PATH, [new_net_ssid, new_net_password](JsonDocument& jsonDocument, File& jsonFile) {

    update_network_config(new_net_ssid, new_net_password, jsonDocument);

  }); 

}


// /**
// * @brief Permet d'obtenir le SSID et le mot de Passe du point d'accès réseau et mettre ces infos dans un json. 
// * @param jsonDocument Le document JSON qui contient la configuration Réseau.
// */
// void get_network_config( JsonDocument& jsonDocument ) {
//   auto json_net_ssid = jsonDocument["wifi"]["net_ssid"];
//   if ( json_net_ssid.isNull() ) {
//     MYDEBUG_PRINTLN("-SPIFFS : Pas de SSID NET dans le fichier de configuration");
//     json_net_ssid = DEFAULT_NET_SSID;
//   }

//   auto json_net_password = jsonDocument["wifi"]["net_password"];
//   if ( json_net_password.isNull() ) {
//     MYDEBUG_PRINTLN("-SPIFFS : Pas de mot de passe NET dans le fichier de configuration");
//     json_net_password = DEFAULT_NET_PASSWORD;
//   }

//   update_network_config(json_net_ssid.as<const char*>(), json_net_password.as<const char*>(), jsonDocument);
// }

// /**
// *@brief appelle la fonction get_network_config avec le document Json passé en paramètre.
// *
// *@param jsonDocument Le document JSON qui contient la configuration Réseau. 
// * 
// *
// */
// void get_network_config() {
//   read_write_file(CONFIG_FILE_PATH, [](JsonDocument& jsonDocument, File& jsonFile) {
//     get_network_config(jsonDocument);
//   });
// }


/**
* @brief launch_acces_point permet de créer un point d'accés de la carte esp pour permettre de la detecter sur le réseaux (Hamburger) et d'interagir avec cette dernière  
* @param ssid Nom du point d'accés de l'esp
* @param password Mot de passe de ce dernier
*
*/
void launch_access_point(const char* ssid, const char* password) {
  WiFi.softAPdisconnect(true);
  WiFi.softAP(ssid, password);

  ap_launched = true;

  update_device_config(ssid, password);
  
  MYDEBUG_PRINTLN("-WIFI AP : Access Point Démarré");
  MYDEBUG_PRINTF3("-WIFI AP : Connectez-vous à %s et ouvrez %s dans un navigateur web\n", device_name.c_str(), WiFi.softAPIP().toString().c_str());
  if ( WiFi.status() == WL_CONNECTED ) {
    MYDEBUG_PRINTF3("-WIFI AP : ou connectez-vous à %s et ouvrez %s\n", net_ssid.c_str(), WiFi.localIP().toString().c_str());
  }
}


/**
 * @brief Vérifie l'état du point d'accès réseau du Contact Tracer; si il n'est pas démarré, il le démarre.\n 
 * Pour ceci, on utilise @ref ap_launched.
 * En conséquence, on peut changer la valeur de @ref ap_launched pour forcer un redémarrage de l'Access Point.
 * 
 * @return true 
 * lorsque le Point d'accès est déjà démarré et ne redémarre donc pas.
 * @return false 
 * lorsque le Point d'accès n'est pas démarré et par conséquent, démarre.
 */
bool check_AP_state() {
  if (ap_launched) {
    return true;
  }

  get_device_config();

  launch_access_point(device_name.c_str(), device_password.c_str());
  return false;
}



/**
 * @brief connect_Wifi_network est une fonction qui sert à se connecter au Access point "Hamburger" avec l'esp
 * 
 * @param ssid id de connexion
 * @param password mot de passe de l'ap
 * @param cycles nombres de tentatives de check de la connexion 
 *
 * @return true
 * lorsque la connexion est réussie
 * @return false
 * lorsque la connexion est échouée
 */

bool connect_WiFi_network(const char* ssid, const char* password, uint cycles = 50) {
  WiFi.disconnect(true);
  WiFi.begin(ssid, password);
  
  MYDEBUG_PRINT("-WIFI : Connexion au réseau : ");
  MYDEBUG_PRINT(ssid);

  for (int i = 0; i < cycles; i++) {
    if (WiFi.status() == WL_CONNECTED) {

      MYDEBUG_PRINT("\n-WIFI : Réussite, connecté à ");
      MYDEBUG_PRINTLN(ssid);
      
      return true;
    }
    MYDEBUG_PRINT(".");
    delay(100);
  }
  MYDEBUG_PRINTLN("\n-WIFI : Connexion échouée");
  return false;
}

/**
 * @brief Vérifie l'état du Point d'accès réseau du Contact Tracer; si il n'est pas connecté, il tente de se connecter.\n
 * 
 * @return true 
 * lorsque le Contact Tracer est déjà connecté au Point d'accès et ne fait donc rien.
 * @return false 
 * lorsque le Contact Tracer n'a pas réussi à se connecter.
 */
bool check_WiFi_connection() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  // get_network_config();

  return connect_WiFi_network(net_ssid.c_str(), net_password.c_str());
}

/**
 * @brief Tente de se connecter au réseau WiFi avec les identifiants fournis en paramètre.\n
 * Si la connexion échoue, le Contact Tracer se reconnecte au réseau WiFi configuré dans le fichier de configuration.
 * 
 * @param ssid
 * @param password
 * 
 * @return true
 * lorsque la connexion est réussie
 * 
 * @return false
 * lorsque la connexion est échouée
 */
bool try_WiFi_connect(const char* ssid, const char* password) {
  
  bool connectionSuccess = connect_WiFi_network(ssid, password, 20);

  if ( !connectionSuccess ) {

    MYDEBUG_PRINT("-WIFI : connecté à ");
    MYDEBUG_PRINTLN(net_ssid);

    WiFi.disconnect(true);
    WiFi.begin(net_ssid, net_password);

    return false;
  }
  
  return true;
}

void WiFiTicker() {
  check_WiFi_connection();
}

/**
 * @brief Configuration du wifi de l'esp 
 * - Mode AP + Station
 * - Connexion au réseau
 */
void setupWiFi() {
  MYDEBUG_PRINTLN("-WIFI : Configuration ");

  // Configuration de la carte en mode Access Point ET Station
  WiFi.mode(WIFI_AP_STA);

  // launch_access_point(device_name.c_str(), device_password.c_str());
  connect_WiFi_network(net_ssid.c_str(), net_password.c_str());

  AddNetworkCredentialsChangedCallback([](const char* ssid, const char* password) {
    MYDEBUG_PRINTLN("-WIFI NET : Nouveaux identifiants de connexion reçus");
    try_WiFi_connect(net_ssid.c_str(), net_password.c_str());
  });
  AddDeviceCredentialsChangedCallback([](const char* ssid, const char* password) {
    MYDEBUG_PRINTLN("-WIFI AP : Nouveaux identifiants reçus");
    launch_access_point(device_name.c_str(), device_password.c_str());
  });

  myTicker.attach(20, WiFiTicker);

}

void loopWiFi() {
}