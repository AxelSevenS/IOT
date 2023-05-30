#include <WiFi.h>         // WiFi

String ap_ssid;
String ap_password;

String net_ssid;
String net_password;

bool ap_launched = false;



void update_AP_config( const char* new_ap_ssid, const char* new_ap_password ) {
  
  if (!SPIFFS.begin(true))
    return;

  File configFile = SPIFFS.open(strConfigFile, "r");

  if ( !configFile ) { // ------------------------- File can't be read
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode lecture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, configFile);

  if (error){
    MYDEBUG_PRINTLN("-SPIFFS : Impossible de parser le JSON, création d'un nouveau fichier");
  }
  configFile.close();
  configFile = SPIFFS.open(strConfigFile, "w");

  if ( !configFile ) { // ------------------------- File can't be read
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode écriture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  jsonDocument["wifi"]["ap_ssid"] = new_ap_ssid;
  ap_ssid = String(new_ap_ssid);

  jsonDocument["wifi"]["ap_password"] = new_ap_password;
  ap_password = String(new_ap_password);

  if (serializeJson(jsonDocument, configFile) == 0) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
  }

  // Fermeture du fichier
  configFile.close();
  SPIFFS.end();  

}

void update_network_config( const char* new_net_ssid, const char* new_net_password ) {
  
  if (!SPIFFS.begin(true))
    return;

  File configFile = SPIFFS.open(strConfigFile, "r");

  if ( !configFile ) { // ------------------------- File can't be read
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode lecture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  DynamicJsonDocument jsonDocument(512);
  DeserializationError error = deserializeJson(jsonDocument, configFile);

  if (error){
    MYDEBUG_PRINTLN("-SPIFFS : Impossible de parser le JSON, création d'un nouveau fichier");
  }
  configFile.close();
  configFile = SPIFFS.open(strConfigFile, "w");

  if ( !configFile ) { // ------------------------- File can't be read
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en mode écriture");
    configFile.close();
    SPIFFS.end();
    return;
  }

  jsonDocument["wifi"]["net_ssid"] = new_net_ssid;
  net_ssid = String(new_net_ssid);

  jsonDocument["wifi"]["net_password"] = new_net_password;
  net_password = String(new_net_password);

  if (serializeJson(jsonDocument, configFile) == 0) {
    MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
  }

  // Fermeture du fichier
  configFile.close();
  SPIFFS.end();  

}




void wifiScan() {

  int n = WiFi.scanNetworks();
  MYDEBUG_PRINT("Scan done | ");
  if (n == 0) {
      MYDEBUG_PRINTLN("no networks found");
  } else {
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
  
  WiFi.scanDelete();
}



void launch_access_point(const char* ssid, const char* password) {
  WiFi.softAPdisconnect(true);
  WiFi.softAP(ssid, password);

  ap_launched = true;

  update_AP_config(ssid, password);
  
  MYDEBUG_PRINTLN("-WIFI : Access Point Démarré");
  MYDEBUG_PRINTF("-WIFI : Connectez-vous à %s et ouvrez %s dans un navigateur web\n", ap_ssid.c_str(), WiFi.softAPIP().toString().c_str());
  if ( WiFi.status() == WL_CONNECTED ) {
    MYDEBUG_PRINTF("-WIFI : ou connectez-vous à %s et ouvrez %s\n", net_ssid.c_str(), WiFi.localIP().toString().c_str());
  }
}

bool check_AP_state() {
  if (ap_launched) {
    return true;
  }

  get_config();

  launch_access_point(ap_ssid.c_str(), ap_password.c_str());
  return false;
}




bool connect_WiFi_AP(const char* ssid, const char* password) {
  WiFi.disconnect(true);
  WiFi.begin(ssid, password);
  
  MYDEBUG_PRINT("-WIFI : Connexion au réseau : ");
  MYDEBUG_PRINTLN(ssid);

  for (int i = 0; i < 50; i++) {
    if (WiFi.status() == WL_CONNECTED) {

      MYDEBUG_PRINT("-WIFI : Réussite, connecté à ");
      MYDEBUG_PRINTLN(ssid);
      update_network_config(ssid, password);
      ap_launched = false;
      check_AP_state();
      return true;
    }
    MYDEBUG_PRINT(".");
    delay(500);
  }
  MYDEBUG_PRINTLN("");
    
  MYDEBUG_PRINTLN("-WIFI : Connexion échouée");
  return false;
}

bool check_WiFi_connection() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  get_config();

  return connect_WiFi_AP(net_ssid.c_str(), net_password.c_str());
}



bool try_WiFi_connect(const char* ssid, const char* password) {
  
  bool connectionSuccess = connect_WiFi_AP(ssid, password);

  if ( !connectionSuccess ) {

    WiFi.disconnect(true);
    WiFi.begin(net_ssid, net_password);
    MYDEBUG_PRINT("-WIFI : connecté à ");
    MYDEBUG_PRINTLN(net_ssid);

    return false;
  }
  
  return true;
}


void setupWiFi(){
  MYDEBUG_PRINTLN();
  MYDEBUG_PRINTLN("-WIFI : Configuration ");

  // Configuration de la carte en mode Access Point ET Station
  WiFi.mode(WIFI_AP_STA);

  check_AP_state();

  // Connexion à internet via la wifi configurée en mode Station
  check_WiFi_connection();
}

void loopWiFi() {
  check_AP_state();
  check_WiFi_connection();
}