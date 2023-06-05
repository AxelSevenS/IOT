/**
 * @file CTNTP.h
 * @brief CTNTP est un module qui permet de récupérer l'heure depuis un serveur NTP.
 * Cette heure est utilisée pour l'horodatage des interactions avec d'autres appareils.
 * 
 * @page CTNTP NTP Contact Tracer
 * @ref CTNTP.h
 */

#pragma once


#include <NTPClient.h>           //https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

/**
 * @brief Récupère l'heure depuis le serveur NTP.
 * 
 * @return String L'heure au format HH:MM:SS.
 */
String getNTP(){
  timeClient.update();
  return timeClient.getFormattedTime();

}

/**
 * @brief Initialise le client NTP.
 */
void setupNTP(){
  // On a besoin d'une connexion à Internet !
  // if (WiFi.status() != WL_CONNECTED){
  //   setupWiFi();
  // }  
  timeClient.begin();
  // Nous avons 1h de décalage avec le méridien de Greenwich (Greenwich Meridian Time : GMT).
  timeClient.setTimeOffset(3600);
}
