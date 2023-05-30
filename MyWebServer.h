#include <WebServer.h>


WebServer monWebServeur(80);


void handleRoot() {

  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation

  // htmlPage = readFile("/index.html");

  htmlPage = F(
    "<!DOCTYPE HTML>"
    "<html>"
      "<h1>"
        "Analog input:  {ANALOG}"
      "</h1>"
      "<h2>"
        "Request URL :  {URL}"
      "</h2>"
    "</html>"
    "\r\n"
  );

  htmlPage.replace("{ANALOG}", "analog");
  htmlPage.replace("{URL}", "url");
  
  monWebServeur.send(200, "text/html", htmlPage);
}

void handleConfig() {

  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation

  get_config();

  htmlPage = F(
    "<!DOCTYPE HTML>"
    "<html>"
      "<h2>"
        "Configuration de la carte : "
      "</h2>"
      "<form action='/resetConfigSubmit' method='POST'>"
        "<input type='submit' value='Réinitialiser la Configuration'>"
      "</form>"
      "<form action='/stationConfigSubmit' method='POST'>"
        "<h3>Identifiants de la Carte</h3>"
        "<input type='text' name='ssid' placeholder='SSID de la carte' value='{STATION_SSID}'>"
        "<input type='text' name='password' placeholder='Password de la carte' value='{STATION_PASSWORD}'>"
        "<input type='submit' value='Enregistrer'>"
      "</form>"
      "<br>"
      "<form action='/apConfigSubmit' method='POST'>"
        "<h3>Identifiants du réseau WiFi</h3>"
        "<input type='text' name='ssid' placeholder='SSID du réseau WiFi' value='{AP_SSID}'>"
        "<input type='text' name='password' placeholder='Password du réseau WiFi' value='{AP_PASSWORD}'>"
        "<input type='submit' value='Se Connecter'>"
      "</form>"
    "</html>"
    "\r\n"
  );

  String input_ap_ssid = ap_ssid;
  if ( monWebServeur.hasArg("ap_ssid") ) {
    input_ap_ssid = monWebServeur.arg("ap_ssid");
  }
  String input_ap_password = ap_password;
  if ( monWebServeur.hasArg("ap_password") ) {
    input_ap_password = monWebServeur.arg("ap_password");
  }
  String input_net_ssid = net_ssid;
  if ( monWebServeur.hasArg("net_ssid") ) {
    input_net_ssid = monWebServeur.arg("net_ssid");
  }
  String input_net_password = net_password;
  if ( monWebServeur.hasArg("net_password") ) {
    input_net_password = monWebServeur.arg("net_password");
  }

  htmlPage.replace("{STATION_SSID}", input_ap_ssid);
  htmlPage.replace("{STATION_PASSWORD}", input_ap_password);
  htmlPage.replace("{AP_SSID}", input_net_ssid);
  htmlPage.replace("{AP_PASSWORD}", input_net_password);
  
  monWebServeur.send(200, "text/html", htmlPage);
}

void handleResetConfigSubmit() {
  reset_config();

  monWebServeur.sendHeader("Location", "/config", true);  
  monWebServeur.send(302, "text/plain", "");
}

void handleStationConfigSubmit() {

  // On récupère les paramètres de la requête
  String ssid = monWebServeur.arg("ssid");
  String password = monWebServeur.arg("password");

  launch_access_point(ssid.c_str(), password.c_str());

  monWebServeur.sendHeader("Location", "/config", true);  
  monWebServeur.send(302, "text/plain", "");
}

void handleAPConfigSubmit() {

  // On récupère les paramètres de la requête
  String ssid = monWebServeur.arg("ssid");
  String password = monWebServeur.arg("password");

  try_WiFi_connect(ssid.c_str(), password.c_str());
  
  monWebServeur.sendHeader("Location", "/config", true);  
  monWebServeur.send(302, "text/plain", "");
}

void handleScan() {
  
  MYDEBUG_PRINTLN("-WEBSERVER : requete scan");

  // La carte scanne les réseaux WiFi à proximité
  int n = WiFi.scanNetworks();

  // Construction de la page HTML de réponse
  String out = F(
    "<!DOCTYPE HTML>"
    "<html>"
      "<head>"
        "<meta http-equiv='refresh' content='5'/>"
        "<title>Ynov - Projet IoT B2</title>"
        "<style>"
          "body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }"
        "</style>"
      "</head>"
      "<body>"
        "<h1>"
          "Page de Scan"
        "</h1>"
        "<br>"
        "<ul>"
          "{NETWORKS}"
        "</ul>"
      "</body>"
    "</html>"
    "\r\n"
  );

  // Intégration des réseaux WiFi trouvés dans la page HTML
  String networks = "";
  if (n == 0) {
    networks = "- AUCUN réseau WiFi trouvé -";
  } else {
    for (int i = 0; i < n; ++i) {
      String ssid = WiFi.SSID(i);
      String network = F(
        "<li>"
          "<a href='/config?net_ssid={SSID}&net_password='>{SSID}</a>"
        "</li>"
      );
      network.replace("{SSID}", ssid);

      networks += network;
    }
  }

  out.replace("{NETWORKS}", networks);

  // Envoi de la page HTML
  monWebServeur.send(200, "text/html", out);

}



void setupWebServer(){
  // // On a besoin d'une connexion WiFi !
  // if ( WiFi.status() != WL_CONNECTED ) { 
  //   setupWiFi(); // Connexion WiFi
  // }
  check_AP_state();

  MYDEBUG_PRINTLN("-WEBSERVER : Démarrage");

  // Configuration de mon serveur web
  monWebServeur.on("/", HTTP_GET, handleRoot);
  monWebServeur.on("/scan", HTTP_GET, handleScan);
  monWebServeur.on("/config", HTTP_GET, handleConfig);
  monWebServeur.on("/resetConfigSubmit", HTTP_POST, handleResetConfigSubmit);
  monWebServeur.on("/stationConfigSubmit", HTTP_POST, handleStationConfigSubmit);
  monWebServeur.on("/apConfigSubmit", HTTP_POST, handleAPConfigSubmit);
  // monWebServeur.onNotFound(handle_NotFound);

  monWebServeur.begin();                                  // Démarrage du serveur
}


void loopWebServer(void) {
  monWebServeur.handleClient();
}
