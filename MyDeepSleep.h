#pragma once

#define uS_TO_S_FACTOR 1000000  /* Facteur de conversion de micro secondes en secondes */
#define TIME_TO_SLEEP  3

RTC_DATA_ATTR int bootCount = 0;

void deepSleep() {
  MYDEBUG_PRINT("-DEEPSLEEP : ");
  MYDEBUG_PRINTLN(bootCount++);
  MYDEBUG_PRINTLN("-DEEPSLEEP : DeepSleep pendant 3 secondes");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
