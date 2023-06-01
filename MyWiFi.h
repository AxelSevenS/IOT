#pragma once

#include <WiFi.h>         // WiFi

/** 
 * \file CTWiFi.h
 * \page wifi WiFi
 * \brief WiFi Connection system
 * 
*/

bool ap_launched = false;
Ticker MyWiFiTicker;


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

  update_AP_config(ssid, password);
  
  MYDEBUG_PRINTLN("-WIFI AP : Access Point Démarré");
  MYDEBUG_PRINTF("-WIFI AP : Connectez-vous à %s et ouvrez %s dans un navigateur web\n", ap_ssid.c_str(), WiFi.softAPIP().toString().c_str());
  if ( WiFi.status() == WL_CONNECTED ) {
    MYDEBUG_PRINTF("-WIFI AP : ou connectez-vous à %s et ouvrez %s\n", net_ssid.c_str(), WiFi.localIP().toString().c_str());
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

  get_config();

  launch_access_point(ap_ssid.c_str(), ap_password.c_str());
  return false;
}



/**
* @brief connect_Wifi_network est une fonction qui sert à se connecter au Access point "Hamburger" avec l'esp
* @param ssid id de connexion
* @param password mot de passe de l'ap
* @param cycles nombres de tentatives de check de la connexion 
*
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

      update_network_config(ssid, password);
      launch_access_point(ap_ssid.c_str(), ap_password.c_str());
      
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

  get_config();

  return connect_WiFi_network(net_ssid.c_str(), net_password.c_str());
}



bool try_WiFi_connect(const char* ssid, const char* password) {
  
  bool connectionSuccess = connect_WiFi_network(ssid, password, 20);

  if ( !connectionSuccess ) {

    // MYDEBUG_PRINT("-WIFI : Échec de connexion à ");
    // MYDEBUG_PRINTLN(ssid);
    MYDEBUG_PRINT("-WIFI : connecté à ");
    MYDEBUG_PRINTLN(net_ssid);

    WiFi.disconnect(true);
    WiFi.begin(net_ssid, net_password);

    return false;
  }
  
  return true;
}

void WiFiTicker() {

  // check_AP_state();
  check_WiFi_connection();
}

/**
* @brief Mise en place du wifi (setupwifi())
*
*/
void setupWiFi() {
  MYDEBUG_PRINTLN("-WIFI : Configuration ");

  // Configuration de la carte en mode Access Point ET Station
  WiFi.mode(WIFI_AP_STA);

  launch_access_point(ap_ssid.c_str(), ap_password.c_str());
  connect_WiFi_network(net_ssid.c_str(), net_password.c_str());

  MyWiFiTicker.attach(20, WiFiTicker);

  // check_AP_state();
  // check_WiFi_connection();
}

void loopWiFi() {
  // check_AP_state();
  // check_WiFi_connection();
}