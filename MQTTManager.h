#pragma once
#include <PubSubClient.h>
#include <WiFiClient.h>
#include "Config.h"

class MQTTManager {
public:
    MQTTManager() : m_client(m_wiFiClient) {
        m_client.setServer(MQTT_BROKER, MQTT_PORT);
        m_client.setCallback(MQTTManager::callbackWrapper);
    }

    void setLedController(LEDController* ledCtrl) {
        m_ledController = ledCtrl;
    }

    void connect() {
        while(!m_client.connected()) {
            Serial.print("Conectando MQTT...");
            
            if(m_client.connect(CLIENT_ID)) {
                Serial.println("Conectado!");
                subscribe(TOPIC_LED);
            } else {
                Serial.println("Falló, reintentando en 5s...");
                delay(5000);
            }
        }
    }

    void subscribe(const char* topic) {
        m_client.subscribe(topic);
    }

    void publish(const char* topic, const char* message) {
        m_client.publish(topic, message);
    }

    void handleClient() {
        if(!m_client.connected()) {
            connect();
        }
        m_client.loop();
    }

    static void callbackWrapper(char* topic, byte* payload, unsigned int length) {
        if(instancePtr) {
            instancePtr->messageReceived(topic, payload, length);
        }
    }

private:
    void messageReceived(char* topic, byte* payload, unsigned int length) {
        // Convertir payload a String
        char message[length + 1];
        memcpy(message, payload, length);
        message[length] = '\0';
        
        Serial.println("Mensaje recibido [" + String(topic) + "]: " + message);
        
        // Controlar LED
        if(String(topic) == TOPIC_LED) {
            handleLedCommand(message);
        }
    }

    void handleLedCommand(const String& command) {
        if(!m_ledController) return;
        
        if(command == "ON") {
            m_ledController->on();
        } else if(command == "OFF") {
            m_ledController->off();
        }
    }

    // Variables miembro
    WiFiClient m_wiFiClient;
    PubSubClient m_client;
    LEDController* m_ledController = nullptr;
    
    // Manejo de instancia para el callback estático
    static MQTTManager* instancePtr;
    friend void setup();
};

// Inicializar puntero estático
MQTTManager* MQTTManager::instancePtr = nullptr;