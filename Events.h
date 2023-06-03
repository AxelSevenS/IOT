#pragma once

#include <vector>
#include <stdio.h>



enum HealthStatus {
  HEALTHY = 2,
  CONTACT = 1,
  SICK = 0
};

typedef void(*CredentialsCallback)(const char* name, const char* password);
typedef void(*MonitoringCallback)(float time, float distance);
typedef void(*HealthCallback)(HealthStatus status);

std::vector<CredentialsCallback> onDeviceCredentialsChangedCallbacks;
void AddDeviceCredentialsChangedCallback(CredentialsCallback callback) {
    onDeviceCredentialsChangedCallbacks.push_back(callback);
}

void InvokeDeviceCredentialsChangedCallbacks(const char* name, const char* password) {
    for (auto callback : onDeviceCredentialsChangedCallbacks) {
        callback(name, password);
    }
}

std::vector<CredentialsCallback> onNetworkCredentialsChangedCallbacks;
void AddNetworkCredentialsChangedCallback(CredentialsCallback callback) {
    onNetworkCredentialsChangedCallbacks.push_back(callback);
}

void InvokeNetworkCredentialsChangedCallbacks(const char* name, const char* password) {
    for (auto callback : onNetworkCredentialsChangedCallbacks) {
        callback(name, password);
    }
}


std::vector<MonitoringCallback> onMonitoringConfigChangedCallbacks;
void AddMonitoringConfigChangedCallback(MonitoringCallback callback) {
    onMonitoringConfigChangedCallbacks.push_back(callback);
}

void InvokeMonitoringConfigChangedCallbacks(float time, float distance) {
    for (auto callback : onMonitoringConfigChangedCallbacks) {
        callback(time, distance);
    }
}

std::vector<HealthCallback> onHealthStatusChangedCallbacks;
void AddHealthStatusChangedCallback(HealthCallback callback) {
    onHealthStatusChangedCallbacks.push_back(callback);
}

void InvokeHealthStatusChangedCallbacks(HealthStatus status) {
    for (auto callback : onHealthStatusChangedCallbacks) {
        callback(status);
    }
}