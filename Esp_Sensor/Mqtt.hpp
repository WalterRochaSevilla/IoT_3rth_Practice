#include "WiFiClient.h"
#pragma once
#include "Network.hpp"
#include <PubSubClient.h>

class MQTTConfig {
    public:
      const char* server;
      int port;
      const char* publishTopic;
      const char* suscribeTopic;
      const char* clientId;
  
      MQTTConfig(const char* server, int port, const char* publishTopic, 
                const char* suscribeTopic, const char* clientId)
        : server(server), port(port), publishTopic(publishTopic), 
          suscribeTopic(suscribeTopic), clientId(clientId) {}
};

class MQTTClient {
    private:
      PubSubClient client;
      NetworkController& network;
      MQTTConfig& config;
      WiFiClient wifiClient;
      unsigned long lastReconnectAttempt = 0;
      const unsigned long RECONNECT_INTERVAL = 5000;
  
      static void callback(char* topic, byte* payload, unsigned int length) {
        Serial.print("Mensaje recibido [");
        Serial.print(topic);
        Serial.print("]: ");
        for (unsigned int i = 0; i < length; i++) {
          Serial.print((char)payload[i]);
        }
        Serial.println();
      }
  
      bool reconnect() {
        while (!client.connected()) {
          Serial.print("Attempting MQTT connection...");
          // Attempt to connect
          if (client.connect(config.clientId)) {
            Serial.println("connected");
          } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" trying again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
          }
        }
        return client.connected();
      }
  
    public:
      MQTTClient(NetworkController& net, MQTTConfig& config)
        : network(net), config(config), 
          client(config.server, config.port, callback, wifiClient) {
          }
  
      void initialize() {
        network.connect();
        client.setServer(config.server, config.port);
        client.setCallback(callback);
        client.setSocketTimeout(30);
      }
  
      void publish(const char* message) {
        if (!client.connected()) {
          reconnect();
        }
        Serial.print("Publishing message: ");
        Serial.println(message);
        client.publish(config.publishTopic, message);
      }
  
      void loop() {
        if (!client.connected()) {
          unsigned long now = millis();
          if (now - lastReconnectAttempt >= RECONNECT_INTERVAL) {
            lastReconnectAttempt = now;
            if (reconnect()) {
              lastReconnectAttempt = 0;
            }
          }
        } else {
          client.loop();
        }
      }
  };