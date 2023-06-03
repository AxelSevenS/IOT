#pragma once


#include <NTPClient.h>           //https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String getNTP(){
  timeClient.update();
  return timeClient.getFormattedTime();

}

void setupNTP(){
  // On a besoin d'une connexion à Internet !
  // if (WiFi.status() != WL_CONNECTED){
  //   setupWiFi();
  // }  
  timeClient.begin();
  // Nous avons 1h de décalage avec le méridien de Greenwich (Greenwich Meridian Time : GMT).
  timeClient.setTimeOffset(3600);
}
