/**
 * @brief CTEvents est un module qui contient les callbacks des événements de l'application.
 * Il est possible d'ajouter des callbacks pour les événements suivants :
 * - Changement des identifiants de l'appareil (@ref onDeviceCredentialsChangedCallbacks)
 * - Changement des identifiants du réseau (@ref onNetworkCredentialsChangedCallbacks)
 * - Changement de la configuration de surveillance (@ref onMonitoringConfigChangedCallbacks)
 * - Changement de l'état de santé (@ref onHealthStatusChangedCallbacks)
 * 
 * @page CTEvents Événements de l'application Contact Tracer
 * @ref CTEvents.h
 * @file CTEvents.h
 */

#pragma once

#include <vector>
#include <stdio.h>


/**
 * @brief Enumération des états de santé possibles.
 * - HEALTHY : L'Utilisateur n'a pas été en contact avec une personne malade.
 * - CONTACT : L'Utilisateur a été en contact avec une personne malade.
 * - SICK : L'Utilisateur est malade.
 */
enum HealthStatus {
  HEALTHY = 2,
  CONTACT = 1,
  SICK = 0
};

typedef void(*CredentialsCallback)(const char* name, const char* password);
typedef void(*MonitoringCallback)(float time, float distance, float temperature);
typedef void(*HealthCallback)(HealthStatus status);

/**
 * @brief Callbacks appelés lors du changement des identifiants de l'appareil. 
 */
std::vector<CredentialsCallback> onDeviceCredentialsChangedCallbacks;
/**
 * @brief Callbacks appelés lors du changement des identifiants du réseau. 
 * 
 * @param callback Callback à ajouter.
 */
void AddDeviceCredentialsChangedCallback(CredentialsCallback callback) {
    onDeviceCredentialsChangedCallbacks.push_back(callback);
}
/**
 * @brief Appelle tous les callbacks enregistrés lors du changement des identifiants de l'appareil.
 * 
 * @param name La valeur mise à jour du nom de l'appareil.
 * @param password La valeur mise à jour du mot de passe de l'appareil.
 */
void InvokeDeviceCredentialsChangedCallbacks(const char* name, const char* password) {
    for (auto callback : onDeviceCredentialsChangedCallbacks) {
        callback(name, password);
    }
}

/**
 * @brief Callbacks appelés lors du changement des identifiants du réseau.
 */
std::vector<CredentialsCallback> onNetworkCredentialsChangedCallbacks;
/**
 * @brief Callbacks appelés lors du changement des identifiants du réseau.
 * 
 * @param callback Callback à ajouter.
 */
void AddNetworkCredentialsChangedCallback(CredentialsCallback callback) {
    onNetworkCredentialsChangedCallbacks.push_back(callback);
}
/**
 * @brief Appelle tous les callbacks enregistrés lors du changement des identifiants du réseau.
 * 
 * @param name La valeur mise à jour du nom du réseau (SSID).
 * @param password La valeur mise à jour du mot de passe du réseau.
 */
void InvokeNetworkCredentialsChangedCallbacks(const char* name, const char* password) {
    for (auto callback : onNetworkCredentialsChangedCallbacks) {
        callback(name, password);
    }
}

/**
 * @brief Callbacks appelés lors du changement de la configuration de surveillance.
 */
std::vector<MonitoringCallback> onMonitoringConfigChangedCallbacks;
/**
 * @brief Callbacks appelés lors du changement de la configuration de surveillance.
 * 
 * @param callback Callback à ajouter.
 */
void AddMonitoringConfigChangedCallback(MonitoringCallback callback) {
    onMonitoringConfigChangedCallbacks.push_back(callback);
}
/**
 * @brief Appelle tous les callbacks enregistrés lors du changement de la configuration de surveillance.
 * 
 * @param time Valeur mise à jour de la durée de surveillance.
 * @param distance Valeur mise à jour de la distance de surveillance.
 * @param temperature Valeur mise à jour de la température Seuil de Maladie.
 */
void InvokeMonitoringConfigChangedCallbacks(float time, float distance, float temperature) {
    for (auto callback : onMonitoringConfigChangedCallbacks) {
        callback(time, distance, temperature);
    }
}

/**
 * @brief Callbacks appelés lors du changement de l'état de santé.
 */
std::vector<HealthCallback> onHealthStatusChangedCallbacks;
/**
 * @brief Callbacks appelés lors du changement de l'état de santé.
 * 
 * @param callback Callback à ajouter.
 */
void AddHealthStatusChangedCallback(HealthCallback callback) {
    onHealthStatusChangedCallbacks.push_back(callback);
}
/**
 * @brief Appelle tous les callbacks enregistrés lors du changement de l'état de santé.
 * 
 * @param status Valeur mise à jour de l'état de santé.
 */
void InvokeHealthStatusChangedCallbacks(HealthStatus status) {
    for (auto callback : onHealthStatusChangedCallbacks) {
        callback(status);
    }
}