/**
 * \file MyWebServer.h
 * \page webserver Serveur Web
 * \brief Un serveur web interne à l'ESP32 pour configurer et voir les données
 * 
 * Comme son nom l'indique, un objet WebServer permet d'écouter des requêtes web, de les analyser et
 * d'y répondre. Le documentation est disponible ici : https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/src/WebServer.h.
 * Ainsi que de nombreux exemples : https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer/examples
 * 
 * Le WebServer est instancié avec le constructeur avec un numéro de port.
 * Les princales fonctions sont :
 * - begin() pour démarrer le serveur
 * - on(uri, fonction) pour appeler une fonction dédiée lorsqu'une route est sollicitée
 * - onNotFound(fonction) pour aller une fonction par défaut quand la route n'est pas connue
 * - handleClient() à appeler dans la loop pour gérer la récupération et le traitemnet de nouveaux messages
 * - send(code, type de contenu, contenu) pour envoyer une réponse avec un code, et un contenu d'un type donné
 * 
 * Dans cet exemple, le serveur web reçoit des requêtes HTTP et y répond sur les routes :
 * - / avec la fonction handleRoot()
 *   Elle donne accès aux fonctionnalités proposées
 * - /scan avec la fonction handleScan()
 *   Affiche la liste des réseaux WiFi disponibles
 * - /config avec la fonction handleConfig()
 *   Affiche un formulaire pour configurer la carte
 * - ...
 * - et avec handleNotFound() si la route n'est pas connue
 * 
 * Fichier \ref MyWebServer.h
 */

// librairies nécessaires 
#include <WebServer.h>

// Variables
WebServer monWebServeur(80);           // Serveur web sur le port 80

void handleScan() {
  
  MYDEBUG_PRINTLN("-WEBSERVER : requete scan");

  // La carte scanne les réseaux WiFi à proximité
  int n = WiFi.scanNetworks();

  // Construction de la page HTML de réponse
  String out = "";
  out += "<html><head><meta http-equiv='refresh' content='5'/>";
  out += "<title>YNOV - Projet IoT B2</title>";
  out += "<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  out += "</head><body>";
  out += "<h1>Page de scan</h1><br>";

  // Intégration des réseaux WiFi trouvés dans la page HTML
  if (n == 0) {
    MYDEBUG_PRINTLN("- AUCUN réseau WiFi trouvé");
  } else {
    out += "<ul>";
    for (int i = 0; i < n; ++i) {
      String ssid = WiFi.SSID(i);
      out += "<li>";
      out += "<a href=\"/config?ssid=";
      out += ssid;
      out += "\">";
      out += ssid;
      out += "</a>";
      out += "</li>";
    }
    out += "</ul>";
  }
  out += "</body></html>";

  // Envoi de la page HTML
  monWebServeur.send(200, "text/html", out);

}

void handleConfig() {

  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation

  htmlPage = F(
    "<!DOCTYPE HTML>"
    "<html>"
      "<h1>"
        "Analog input:  "
    );
    htmlPage += analogRead(A0);
    htmlPage += F(
      "</h1>"
      "<h2>"
        "Request URL :"/* 
    );
    htmlPage += client->url();
    htmlPage += F( */
      "</h2>"
      "<p>"
        "OH WAAW"
      "</p>"
    "</html>"
    "\r\n"
  );
  
  monWebServeur.send(200, "text/html", htmlPage);
}


void handleRoot() {

  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation

  htmlPage = F(
    "<!DOCTYPE HTML>"
    "<html>"
      "<h1>"
        "Analog input:  "
    );
    htmlPage += analogRead(A0);
    htmlPage += F(
      "</h1>"
      "<h2>"
        "Request URL :"/* 
    );
    htmlPage += client->url();
    htmlPage += F( */
      "</h2>"
    "</html>"
    "\r\n"
  );
  
  monWebServeur.send(200, "text/html", htmlPage);
}

/**
 * Initialisation du serveur web
 */
void setupWebServer(){
  // On a besoin d'une connexion WiFi !
  if ( WiFi.status() != WL_CONNECTED ) { 
    setupWiFi(); // Connexion WiFi
  }

  MYDEBUG_PRINTLN("-WEBSERVER : Démarrage");

  // Configuration de mon serveur web
  monWebServeur.on("/", handleRoot);
  monWebServeur.on("/config", handleConfig);
  monWebServeur.on("/scan", handleScan);
  // monWebServeur.onNotFound(handleNotFound);

  monWebServeur.begin();                                  // Démarrage du serveur
  Serial.printf("-WEBSERVER : Serveur Web démarré, ouvrez %s dans un navigateur web\n", WiFi.localIP().toString().c_str());
}

/**
 * Loop pour le serveur web afin qu'il regarde s'il a reçu des requêtes
 */
void loopWebServer(void) {
  monWebServeur.handleClient();
}
