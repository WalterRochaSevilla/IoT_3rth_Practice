#pragma once

#include <WiFi.h>

class NetworkConfig {
    public:
      const char* ssid;
      const char* password;
  
      NetworkConfig(const char* ssid, const char* password) 
        : ssid(ssid), password(password) {}
  };    
class NetworkController {
    private:
      NetworkConfig& config;
      
    public:
      NetworkController(NetworkConfig& config) : config(config) {}
  
      void connect() {        
        Serial.print("Conectando a ");
        Serial.println(config.ssid);
  
        WiFi.begin(config.ssid, config.password);
        
        while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
        }
        
        Serial.println("\nConectado!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
      }
  };