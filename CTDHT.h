/**
 * @brief CTDHT est un module qui permet de récupérer les données du capteur DHT11.
 * Lorsque la température excède le seuil de température (@ref temperature_threshold), l'appareil est marqué comme malade.
 * 
 * @page CTDHT DHT11 Contact Tracer
 * @ref CTDHT.h
 * @file CTDHT.h
 */

#include <esp32DHT.h>                     // https://github.com/bertmelis/esp32DHT

#define DHT_PIN    33
#define DHT_TYPE   DHT11

DHT11 CTDHT;
uint32_t delayMS = 5000;


/**
 * @brief Initialise le capteur DHT11
 */
void setupDhtSensor() {
  CTDHT.setup(DHT_PIN);
  CTDHT.onData([](float temperature, float humidity) {
    MYDEBUG_PRINT("-DHT : [");
    MYDEBUG_PRINT(temperature);
    MYDEBUG_PRINTLN("°C] température !");
    MYDEBUG_PRINT("-DHT : [");
    MYDEBUG_PRINT(humidity);
    MYDEBUG_PRINTLN("%] humidité !");
  });
}