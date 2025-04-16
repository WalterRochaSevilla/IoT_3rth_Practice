#pragma once

#include <WiFi.h>
#include "Config.h"

class WiFiManager {
public:
    WiFiManager() {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_STA);
    }

    void connect() {
        Serial.println("\nConectando a: " + String(WIFI_SSID));
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        
        while(WiFi.status() != WL_CONNECTED) {
            delay(250);
            Serial.print(".");
        }
        
        Serial.println("\nConectado! IP: " + WiFi.localIP().toString());
    }

    bool isConnected() {
        return WiFi.status() == WL_CONNECTED;
    }
};