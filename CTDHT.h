/**
 * \file CTDHT.h 
 * \page debug Debug
 * Ce fichier permet de gérer le capteur de température DHT11.
 *
 *\ref  CTDHT.h
 */






#include <DHT_U.h>                     // https://github.com/adafruit/Adafruit_Sensor

#define DHT_PIN    33
#define DHT_TYPE   DHT11

DHT_Unified myDht(DHT_PIN, DHT_TYPE);
uint32_t delayMS = 5000;




/**
 * 
 * \brief Fonction qui permet de récupérer les données du capteur de température.
 * 
 * \return Retourne un objet de type sensors_event_t qui contient les données du capteur.
 */
sensors_event_t getDhtData(){

  delay(delayMS);

  // Get temperature event and print its value.
  sensors_event_t event;  
  myDht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    MYDEBUG_PRINTLN("-DHT : Erreur de lecture de la température du capteur DHT !");
  }
  else {
    MYDEBUG_PRINT("-DHT : [");
    MYDEBUG_PRINT(event.temperature);
    MYDEBUG_PRINTLN("°C] température !");
  }

  return event;
}

/**
 * \brief Fonction qui permet de print les données du capteur de température.

 */

void setupDhtSensor(){
  myDht.begin();

  MYDEBUG_PRINTLN("DHT Unified Sensor");
  // Information sur le capteur de température
  sensor_t sensor;
  myDht.temperature().getSensor(&sensor);
  MYDEBUG_PRINTLN("------------------------------------");
  MYDEBUG_PRINTLN("Temperature");
  MYDEBUG_PRINT  ("Sensor:       "); MYDEBUG_PRINTLN(sensor.name);
  MYDEBUG_PRINT  ("Driver Ver:   "); MYDEBUG_PRINTLN(sensor.version);
  MYDEBUG_PRINT  ("Unique ID:    "); MYDEBUG_PRINTLN(sensor.sensor_id);
  MYDEBUG_PRINT  ("Max Value:    "); MYDEBUG_PRINT(sensor.max_value); MYDEBUG_PRINTLN(" *C");
  MYDEBUG_PRINT  ("Min Value:    "); MYDEBUG_PRINT(sensor.min_value); MYDEBUG_PRINTLN(" *C");
  MYDEBUG_PRINT  ("Resolution:   "); MYDEBUG_PRINT(sensor.resolution); MYDEBUG_PRINTLN(" *C");  
  MYDEBUG_PRINTLN("------------------------------------");

  delayMS = sensor.min_delay/1000;
}