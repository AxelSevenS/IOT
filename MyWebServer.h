#pragma once

#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "MySpiffs.h"


AsyncWebServer asyncWebServer(80);

/**
 * @brief Traitement de la page de configuration
 * 
 * @remarks Cette page permet de configurer les identifiants de la carte, du point d'accès réseau, du système de surveillance, de réinitialiser la configuration et de renseigner son état de santé
*/
void handleConfig(AsyncWebServerRequest *request) {

  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation

  get_config();


  htmlPage = F(
    "<!DOCTYPE HTML>"
    "<html lang='fr'>"

      "<head>"
        "<meta charset='UTF-8'>"
      "</head>"

      "<body>"
        "<h2>"
          "Configuration de la carte : "
        "</h2>"
        "<form action='/resetConfigSubmit' method='POST'>"
          "<input type='submit' value='Réinitialiser la Configuration'>"
        "</form>"
        "<form action='/deviceConfigSubmit' method='POST'>"
          "<h3>Identifiants de la Carte</h3>"
          "<input type='text' name='name' placeholder='Nom de la carte' value='{DEVICE_NAME}'>"
          "<input type='text' name='password' placeholder='Password de la carte' value='{DEVICE_PASSWORD}'>"
          "<input type='submit' value='Enregistrer'>"
        "</form>"
        "<form action='/netConfigSubmit' method='POST'>"
          "<h3>Identifiants du réseau WiFi</h3>"
          "<input type='text' name='ssid' placeholder='SSID du réseau WiFi' value='{AP_SSID}'>"
          "<input type='text' name='password' placeholder='Password du réseau WiFi' value='{AP_PASSWORD}'>"
          "<input type='submit' value='Se Connecter'>"
        "</form>"
        "<form action='/monitoringConfigSubmit' method='POST'>"
          "<h3>Paramètres de Surveillance</h3>"
          "<input type='number' step='any' min='0' name='time' placeholder='Période de surveillance' value='{MONITORING_TIME}'>"
          "<label for='time'>Durée de Surveillance Minimum (en minutes)</label>"
          "<br>"
          "<input type='number' step='any' min='0' name='distance' placeholder='Distance de détection' value='{MONITORING_DISTANCE}'>"
          "<label for='distance'>Distance de Surveillance Maximum (en mètres)</label>"
          "<br>"
          "<input type='submit' value='Enregistrer'>"
        "</form>"
        "<form action='/updateHealthStateSubmit' method='POST'>"
          "<h3>État de Santé</h3>"
          "<input type='radio' id='HEALTHY' name='healthState' value='2'>"
          "<label for='2'>Sain</label>"
          "<br>"
          "<input type='radio' id='CONTACT' name='healthState' value='1'>"
          "<label for='1'>Cas Contact</label>"
          "<br>"
          "<input type='radio' id='SICK' name='healthState' value='0'>"
          "<label for='0'>Malade</label>"
          "<br>"
          "<input type='submit' value='Enregistrer'>"
        "</form>"
        "<br>"
        "<div>"
          "<h3>Contact Tracer Contactés</h3>"
          "<table>"
            "{MONITORED_DEVICES}"
          "</table>"
          "<form action='/resetMonitoringSubmit' method='POST'>"
            "<input type='submit' value='Réinitialiser la Surveillance'>"
          "</form>"
          "</div>"
      "</body>"
      
    "</html>"
    "\r\n"
  );

  switch (user_status) {
    default:
    case HEALTHY:
      htmlPage.replace("value='2'", "value='2' checked");
      break;
    case CONTACT:
      htmlPage.replace("value='1'", "value='1' checked");
      break;
    case SICK:
      htmlPage.replace("value='0'", "value='0' checked");
      break;
  }

  String input_net_ssid = request->hasArg("net_ssid") ? request->arg("net_ssid") : net_ssid;

  htmlPage.replace("{DEVICE_NAME}", device_name);
  htmlPage.replace("{DEVICE_PASSWORD}", device_password);
  htmlPage.replace("{AP_SSID}", input_net_ssid);
  htmlPage.replace("{AP_PASSWORD}", net_password);
  htmlPage.replace("{MONITORING_TIME}", String(monitoring_time));
  htmlPage.replace("{MONITORING_DISTANCE}", String(monitoring_distance));


  String devices = F(
    "<tr>"
      "<td>Addresse</td>"
      "<td>Nom</td>"
      "<td>Temps D'exposition</td>"
      "<td>Surveillé de Près</td>"
      "<td>Santé</td>"
    "</tr>"
  );
  
  read_file(MONITORING_FILE_PATH, [&devices](JsonDocument& monitoringDocument, File& monitoringFile) {

    JsonObject monitoredDevices = monitoringDocument.as<JsonObject>();


    for (JsonPair keyValue : monitoredDevices) {
      auto monitoredAddress = keyValue.key();
      auto monitoredDevice = keyValue.value();

      String device = F(
        "<tr>"
          "<td>{ADDRESS}</td>"
          "<td>{NAME}</td>"
          "<td>{TOTAL_EXPOSURE}</td>"
          "<td>{CLOSE_MONITORING}</td>"
          "<td>{HEALTH_STATE}</td>"
        "</tr>"
      );
      device.replace("{ADDRESS}", monitoredAddress.c_str());
      device.replace("{NAME}", monitoredDevice["name"].as<String>().c_str());
      device.replace("{TOTAL_EXPOSURE}", monitoredDevice["total_exposure"].as<String>().c_str());
      device.replace("{CLOSE_MONITORING}", monitoredDevice["close_monitoring"].as<String>().c_str());
      device.replace("{HEALTH_STATE}", monitoredDevice["status"].as<String>().c_str());

      devices += device;
    }
  });
  htmlPage.replace("{MONITORED_DEVICES}", devices);
  

  request->send(200, "text/html", htmlPage);

}

/**
 * @brief Traitement de la demande de réinitialisation de la configuration
*/
void handleResetConfigSubmit(AsyncWebServerRequest *request) {
  reset_file(CONFIG_FILE_PATH);
  
  get_config();

  request->redirect("/");
}

/**
 * @brief Traitement de la demande de réinitialisation de la surveillance
*/
void handleResetMonitoringSubmit(AsyncWebServerRequest *request) {
  reset_file(MONITORING_FILE_PATH);
  
  get_config();
  
  request->redirect("/");
}

/**
 * @brief Traitement de la demande de configuration du système de surveillance
*/
void handleMonitoringConfigSubmit(AsyncWebServerRequest *request) {

  // On récupère les paramètres de la requête
  float time = atof(request->arg("time").c_str());
  float distance = atof(request->arg("distance").c_str());

  update_monitoring_config(time, distance);

  request->redirect("/");
}

/**
 * @brief Traitement de la demande de configuration des identifiants de la carte
*/
void handleDeviceConfigSubmit(AsyncWebServerRequest *request) {

  // On récupère les paramètres de la requête
  String name = request->arg("name");
  String password = request->arg("password");

  update_device_config(name.c_str(), password.c_str());
  // launch_access_point(name.c_str(), password.c_str());


  request->redirect("/");
}

/**
 * @brief Traitement de la demande de configuration des identifiants du réseau WiFi
*/
void handleNetConfigSubmit(AsyncWebServerRequest *request) {

  // On récupère les paramètres de la requête
  String ssid = request->arg("ssid");
  String password = request->arg("password");

  update_network_config(ssid.c_str(), password.c_str());
  // try_WiFi_connect(ssid.c_str(), password.c_str());
  
  request->redirect("/");
}

/**
 * @brief Traitement de la demande de configuration de l'état de santé
*/
void handleUpdateHealthStateSubmit(AsyncWebServerRequest *request) {

  // On récupère les paramètres de la requête
  String value = request->arg("healthState");
  int intValue = value.toInt();

  update_health_status((HealthStatus)intValue);
  request->redirect("/");
}

/**
 * @brief Traitement de la page de scan des réseaux WiFi
 * 
 * @remarks Cette fonction est appelée à chaque rafraichissement de la page\
 * lorsqu'on clique sur le SSID du réseau WiFi auquel on souhaite se connecter, on est redirigé vers la page de configuration du réseau WiFi avec le SSID pré-rempli
*/
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
          "<a href='?net_ssid={SSID}&net_password='>{SSID}</a>"
        "</li>"
      );
      network.replace("{SSID}", ssid);

      networks += network;
    }
  }

  out.replace("{NETWORKS}", networks);

  request->send(200, "text/html", out);
}


/**
 * Configuration du serveur web
 * - Définition des routes
 * - Démarrage du serveur
*/
void setupWebServer(){

  MYDEBUG_PRINTLN("-WEBSERVER : Démarrage");

  // Configuration de mon serveur web
  asyncWebServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("config");
  });
  asyncWebServer.on("/config", HTTP_GET, handleConfig);
  asyncWebServer.on("/resetConfigSubmit", HTTP_POST, handleResetConfigSubmit);
  asyncWebServer.on("/resetMonitoringSubmit", HTTP_POST, handleResetMonitoringSubmit);
  asyncWebServer.on("/deviceConfigSubmit", HTTP_POST, handleDeviceConfigSubmit);
  asyncWebServer.on("/monitoringConfigSubmit", HTTP_POST, handleMonitoringConfigSubmit);
  asyncWebServer.on("/netConfigSubmit", HTTP_POST, handleNetConfigSubmit);
  asyncWebServer.on("/updateHealthStateSubmit", HTTP_POST, handleUpdateHealthStateSubmit);
  asyncWebServer.on("/scan", HTTP_GET, handleScan);
  asyncWebServer.onNotFound([](AsyncWebServerRequest *request) {
    request->redirect("/");
  });

  asyncWebServer.begin();
}


void loopWebServer(void) {
}
