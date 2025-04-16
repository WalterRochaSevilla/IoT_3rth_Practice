#include "WiFiManager.h"
#include "LEDController.h"
#include "MQTTManager.h"

WiFiManager wifiManager;
LEDController ledController;
MQTTManager mqttManager;

unsigned long lastMsg = 0;
int counter = 0;

void setup() {
    Serial.begin(115200);
    
    // Configurar instancia para el callback
    MQTTManager::instancePtr = &mqttManager;
    
    // Inyectar dependencia
    mqttManager.setLedController(&ledController);
    
    // Conectar a redes
    wifiManager.connect();
    mqttManager.connect();
}

void loop() {
    mqttManager.handleClient();
    
    // Publicar contador cada 10 segundos
    unsigned long now = millis();
    if(now - lastMsg > 10000) {
        lastMsg = now;
        String msg = String(counter++);
        mqttManager.publish(TOPIC_COUNTER, msg.c_str());
        Serial.println("Contador publicado: " + msg);
    }
}