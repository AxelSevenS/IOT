/** 
* \file CTNTP.h 
* \page CTNTP.h CTNTP.h
* Ce fichier contient les fonctions permettant de récupérer l'heure sur un serveur NTP.
*
*\ref CTNTP.h
*/
#pragma once


#include <NTPClient.h>           //https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>



WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String getNTP() {
  timeClient.update();
  return timeClient.getFormattedTime();

}

void setupNTP() {
  timeClient.begin();
  timeClient.setTimeOffset(3600);
}
