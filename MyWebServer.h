#pragma once

#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"


AsyncWebServer monWebServeur(80);


void handleRoot(AsyncWebServerRequest *request) {

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

}

void handleConfig(AsyncWebServerRequest *request) {

  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation

  get_config();

  // SPIFFS.begin();
  // File configFile = SPIFFS.open(strConfigFile, "r");

  // String config = configFile.readString();

  // configFile.close();
  // SPIFFS.end();


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
      "<form action='/monitoringConfigSubmit' method='POST'>"
        "<h3>Paramètres de Surveillance</h3>"
        "<input type='number' name='time' placeholder='Période de surveillance nécessaire (en minutes)' value='{MONITORING_TIME}'>"
        "<input type='number' name='distance' placeholder='Distance de détection de Contact Tracers (en mètres)' value='{MONITORING_DISTANCE}'>"
        "<input type='submit' value='Se Connecter'>"
      "</form>"
      "<br>"
      // "<span>"
      //   "{CONFIG_FILE}"
      // "</span>"
    "</html>"
    "\r\n"
  );

  String input_ap_ssid = request->hasArg("ap_ssid") ? request->arg("ap_ssid") : ap_ssid;

  String input_ap_password = request->hasArg("ap_password") ? request->arg("ap_password") : ap_password;

  String input_net_ssid = request->hasArg("net_ssid") ? request->arg("net_ssid") : net_ssid;

  String input_net_password = request->hasArg("net_password") ? request->arg("net_password") : net_password;

  String input_monitoring_time = request->hasArg("monitoring_time") ? request->arg("monitoring_time") : String(monitoring_time);

  String input_monitoring_distance = request->hasArg("monitoring_distance") ? request->arg("monitoring_distance") : String(monitoring_distance);

  

  htmlPage.replace("{STATION_SSID}", input_ap_ssid);
  htmlPage.replace("{STATION_PASSWORD}", input_ap_password);
  htmlPage.replace("{AP_SSID}", input_net_ssid);
  htmlPage.replace("{AP_PASSWORD}", input_net_password);
  htmlPage.replace("{MONITORING_TIME}", String(input_monitoring_time));
  htmlPage.replace("{MONITORING_DISTANCE}", String(input_monitoring_distance));
  

  request->send(200, "text/html", htmlPage);
}

void handleResetConfigSubmit(AsyncWebServerRequest *request) {
  reset_config();
  request->redirect("config");

  ESP.restart();
}

void handleMonitoringConfigSubmit(AsyncWebServerRequest *request) {

  // On récupère les paramètres de la requête
  float time = atof(request->arg("time").c_str());
  float distance = atof(request->arg("distance").c_str());

  update_monitoring_config(time, distance);

  request->redirect("config");
}

void handleStationConfigSubmit(AsyncWebServerRequest *request) {

  // On récupère les paramètres de la requête
  String ssid = request->arg("ssid");
  String password = request->arg("password");

  launch_access_point(ssid.c_str(), password.c_str());

  request->redirect("config");
}

void handleAPConfigSubmit(AsyncWebServerRequest *request) {

  // On récupère les paramètres de la requête
  String ssid = request->arg("ssid");
  String password = request->arg("password");

  try_WiFi_connect(ssid.c_str(), password.c_str());
  
  request->redirect("config");
}

void handleScan(AsyncWebServerRequest *request) {
  
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
          "<a href='config?net_ssid={SSID}&net_password='>{SSID}</a>"
        "</li>"
      );
      network.replace("{SSID}", ssid);

      networks += network;
    }
  }

  out.replace("{NETWORKS}", networks);

  request->send(200, "text/html", out);
}



void setupWebServer(){

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
  // monWebServeur.handleClient();
}
