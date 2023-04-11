 /**
 * \file MyWiFi.h
 * \page wifi WiFi ESP32
 * 
 * \subsection howWiFi Comment ça marche ?
 * Il existe plusieurs façons différentes d'initialiser le mode WiFi d'un ESP32.
 * - Les terminaux qui se connectent aux réseaux WiFi proposés par des Access Points sont appelés 
 *   des stations (mode WIFI_STA).
 *   \image html ESP32-Station-Mode.png
 * - Les terminaux peuvent fournir un point d'accès ou Access Point (WIFI-AP) et se comporter comme 
 *   un hub pour une ou plusieurs stations. Classiquement un Access Point est connecté à un réseau 
 *   filaire et intégre un routeur pour donner accès à internet via le WiFi. Chaque Access Point 
 *   fournit un SSID (Service Set IDentifier) visible et accessible pour les stations (autres terminaux 
 *   comme un téléphone mobile ou un ordinateur).
 *   \image  html ESP32-Access-Point-Mode.png
 * - Les terminaux peuvent également activer les 2 modes de fonctionnement en même temps (Station et
 *   Access Point, en mode WIFI_AP_STA).
 * 
 * \subsection foncWiFi Principales fonctions
 * Les principales fonctions de la librairie ESP8266WiFi sont :
 * - WiFi.mode(<MODE>) qui prend en paramètre un des modes suivants :
 *   - WIFI_STA quand la carte veut accéder à une réseau WiFi en mode client
 *   - WIFI_AP quand la carte veut être un point d'accès (être un SSID)
 *   - WIFI_AP_STA pour faire les 2 à la fois
 *   - WIFI_OFF comme son nom l'indique
 * - WiFi.begin("nom-du-SSID", "mot-de-passe")
 * - WiFi.status() qui vous retourne
 *   - 0 : WL_IDLE_STATUS when Wi-Fi is in process of changing between statuses
 *   - 1 : WL_NO_SSID_AVAIL quand le SSID indiqué n'est pas atteingnable
 *   - 3 : WL_CONNECTED quand la connexion a été établi
 *   - 4 : WL_CONNECT_FAILED si le mot de passe du SSID est incorrect
 *   - 6 : WL_DISCONNECTED si le module n'est pas configuration en mode Station
 * - WiFi.localIP() en mode WIFI_STA affiche l'adresse IP de la carte
 * - WiFi.scanNetworks() vous permet de scanner et récupérer les informations sur les
 *   SSID à portée.
 * 
 * Les classes WiFiClient et WiFiServer vous permettent de manipuler les communications
 * en tant que client ou server.
 * 
 * \subsection TPWIfi Prise en main
 * Pour prendre en main le WiFi sur votre carte, je vous invite à découvrir les fichiers exemples suivants :
 * -# <b>Exemples/WiFi/WiFiScan</b> 
 *    scanne les réseaux WiFi (Access Points) visibles par la carte qui est en mode STAtion et de les liste 
 *    sur le moniteur de sortie.
 * -# <b>Exemples/WiFi/WiFiClient</b> 
 *    permet à la carte d'accéder à un réseau WiFi en mode STAtion; la sélection du réseau WiFi se fait en 
 *    remplissant les valeurs de STASSID et STAPSK. Une fois connecté la carte créé un client WiFi TCP et va 
 *    requéter un serveur et afficher les informations qu'elle récupère.
 * -# <b>Exemples/WiFi/WiFiMulti</b> 
 *    permet de lister plusieurs Access Points auquels vous souhaitez/pouvez accéder. Si le premier SSID 
 *    n'est pas disponible alors la carte tente de se connecter au suivant ... Cette fonctionnalité est 
 *    proposée par la classe WiFiMulti.
 *    Vous pouvez ainsi configurer les réseaux WiFi de l'école, de chez vous et celui de votre téléphone, 
 *    par exemple. A noter qu'il faut bien penser à appeler la méthode wifiMulti.run() dans la loop() pour 
 *    surveiller l'état de la connexion à chaque itération et faire le nécessaire si besoin est.
 * -# <b>Exemples/WiFi/WiFiClientBasic</b> 
 *    se connecte en mode STA à un SSID via WiFiMulti et réalise une requête à un serveur HTTP, récupère la 
 *    réponse du serveur et l'affiche sur le moniteur série; et recommence 10 seconds après.
 * -# <b>Exemples/WiFi/WiFiAccessPoint</b> 
 *    permet de créer un point d'accès WiFi. En lançant cet exemple la carte crée un réseau WiFi avec le nom 
 *    renseigné dans ssid; vous pourrez le voir depuis votre ordinateur ou votre téléphone, et y accéder 
 *    en renseignant le mot de passe indiqué dans password.
 *    Une fois connecté, vous pourrez vous rendre sur l'adresse http://192.168.4.1 depuis un navigateur web et
 *    la carte vous renverra le code HTML présent dans la méthode handleRoot() que le serveur appelle pour 
 *    la route "/".
 * -# <b>Exemples/DNSServer/CaptivePortal</b>
 *    démarre la carte en mode Access Point et répond à toutes les requêtes en retournant une page web.
 *    Ainsi dès qu'un terminal (ordinateur, téléphone ...) se connecte au SSID il est routé sur le portail 
 *    captif sur la base de cette page web.
 *
 * Je vous invite à installer la librairie WiFiManager et à tester l'exemple fourni :
 * -# Dans le menu "Outils/Gérer les bibliothèques", renseigner "captive" dans la barre de recherche et installez
 *    la dernière version de WiFiManager dy Tablatronix.
 * -# Dans tester l'exemple qui se trouve sur <b>Exemple/WiFiManager/Basic</b>.
 * 
 * 
 * \subsection exemple Exemple
 * Dans cet exemple, l'objet est utilisé avec les 2 modes Access Point et Station :
 * - Access Point pour que l'utilisateur puisse accéder à un serveur web proposé par la carte
 * - Station pour se connecter à un Access Point pour accéder au réseau externe (internet).
 * 
 * Je peux ajouter un serveur DNS pour réaliser un portail captif ...
*/

// Librairies nécessaires
#include <WiFi.h>         // WiFi
// #include "esp_wpa2.h"

// Variables
// pour le mode STATION
const char* station_ssid     = "Hamburger";       // Nom du réseau WiFi pour accéder à internet en mode Station
const char* station_password = "YoLesKids";   // Mot de passe du réseau WiFi pour accéder à internet en mode Station
// pour le mode ACCESS POINT
const char* ap_ssid     = "ESP8266_Axel";      // Nom du réseau WiFi pour accéder à mon serveur web
const char* ap_password = "AdminSex";        // Mot de passe du réseau WiFi pour accéder à mon serveur web

// ------------------------------------------------------------------------------------------------
// SCAN WIFI
// ------------------------------------------------------------------------------------------------
/**
 * Fonction de scan des SSID WiFi qui sont accessibles.
 */
void wifiScan(){
  // WiFi.scanNetworks scanne les SSID accessibles, les stocke en mémoire et retourne le nombre
  // de réseaux trouvés pour qu'on puisse les parcourir.
  int n = WiFi.scanNetworks();
  MYDEBUG_PRINT("Scan done | ");
  if (n == 0) {
      // Aucun SSID n'a été trouvé
      MYDEBUG_PRINTLN("no networks found");
  } else {
      // Boucle d'affichage de tous les SSID trouvés et leurs informations
      MYDEBUG_PRINT(n);
      MYDEBUG_PRINTLN(" networks found");
      MYDEBUG_PRINTLN("Nr | SSID                             | RSSI | CH | Encryption");
      for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          Serial.printf("%2d",i + 1);
          MYDEBUG_PRINT(" | ");
          Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
          MYDEBUG_PRINT(" | ");
          Serial.printf("%4d", WiFi.RSSI(i));
          MYDEBUG_PRINT(" | ");
          Serial.printf("%2d", WiFi.channel(i));
          MYDEBUG_PRINT(" | ");
          switch (WiFi.encryptionType(i))
          {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
          }
          MYDEBUG_PRINTLN();
          delay(10);
      }
  }
  MYDEBUG_PRINTLN("");
  
  // Effacement des résultats du scan de la mémoire, pour optimiser.
  WiFi.scanDelete();
}


bool connect_WiFi_AP(const char* ssid, const char* password) {
  WiFi.disconnect(true);
  WiFi.begin(ssid, password);
  
  MYDEBUG_PRINT("-WIFI : Connexion au réseau : ");
  MYDEBUG_PRINTLN(station_ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    MYDEBUG_PRINT(".");
  }
  MYDEBUG_PRINTLN("");
    
  // J'affiche l'adresse IP de ma carte
  MYDEBUG_PRINT("-WIFI : connecté en mode Station avec l'adresse IP : ");
  MYDEBUG_PRINTLN(WiFi.localIP());
  return true;
}

bool check_WiFi_connection() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  return connect_WiFi_AP(station_ssid, station_password);
}

void launch_access_point(const char* ssid, const char* password) {
  WiFi.softAPdisconnect();
  WiFi.softAP(ssid, password);
  
  // J'affiche l'adresse IP de mon point d'accès
  // Cette adresse IP sera nécessaire pour accéder au portail
  MYDEBUG_PRINT("-WIFI : Access Point mis à disposition : ");
  MYDEBUG_PRINTLN(WiFi.softAPIP());
}


// ------------------------------------------------------------------------------------------------
// CONFIGURATION DU WIFI
// ------------------------------------------------------------------------------------------------
/**
 * Fonction de configuration WiFi de ma carte.
 * 
 * La mode WIFI_AP_STA est utilisé afin de pouvoir à la fois :
 * - Se connecter à un point d'accès réseau en tant que station
 * - Proposer une connexion à l'utilisateur pour accéder à mon serveur web
 */
void setupWiFi(){
  MYDEBUG_PRINTLN();
  MYDEBUG_PRINTLN("-WIFI : Configuration ");

  // Scan de tous les SSID disponibles au démarrage
  // wifiScan();

  // Configuration de la carte en mode Access Point ET Station
  WiFi.mode(WIFI_AP_STA);

  // Connexion à internet via la wifi configurée en mode Station
  check_WiFi_connection();

  // Démarrage du mode Access Point
  // pour que la carte soit visible et accessible en tant que point d'accès WiFi
  launch_access_point(ap_ssid, ap_password);
}

void loopWiFi() {
}