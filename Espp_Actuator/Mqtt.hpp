#include "WiFiClient.h"
#pragma once
#include "Network.hpp"
#include <PubSubClient.h>

class MQTTConfig {
  public:
    const char* server;
    int port;
    const char* publishTopic;
    const char* subscribeTopic;
    const char* clientId;
    void (*callback)(char*, byte*, unsigned int); 
    MQTTConfig(const char* server, int port, const char* publishTopic, 
              const char* subscribeTopic, const char* clientId,
              void (*callback)(char*, byte*, unsigned int))
        : server(server), port(port), publishTopic(publishTopic), 
        subscribeTopic(subscribeTopic), clientId(clientId), callback(callback) {}
      void setCallback(void (*callback)(char*, byte*, unsigned int)) {
        this->callback = callback;
      }
};

class MQTTClient {
    private:
      PubSubClient client;
      NetworkController& network;
      MQTTConfig& config;
      WiFiClient wifiClient;
      unsigned long lastReconnectAttempt = 0;
      const unsigned long RECONNECT_INTERVAL = 5000;
  
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
          client(config.server, config.port, config.callback, wifiClient) {
          }
  
      void initialize() {
        network.connect();
        client.setServer(config.server, config.port);
        client.setCallback(config.callback);
        client.setSocketTimeout(35);
      }
  
      void publish(const char* message) {
        if (!client.connected()) {
          reconnect();
        }
        Serial.print("Publishing message: ");
        Serial.println(message);
        client.publish(config.publishTopic, message);
      }
      void subscribe(){
        if (!client.connected()) {
          reconnect();
        }
        client.subscribe(config.subscribeTopic);
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