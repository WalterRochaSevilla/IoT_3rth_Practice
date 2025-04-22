#include "Mqtt.hpp"
NetworkConfig netConfig("wifi_name", "wifi_password");
NetworkController wifi(netConfig);
MQTTConfig mqttConfig("broker.hivemq.com", 1883, "ucb/grupo12/esp32/test", "ucb/grupo12/esp32/test", "esp32_testing_client_grupo_12");
MQTTClient  mqtt(wifi, mqttConfig);


void setup() {
  Serial.begin(115200);
  mqtt.initialize();
}

void loop() {
  mqtt.loop();
  unsigned long lastMsg = 0;
  if(millis() - lastMsg > 5000) {
      mqtt.publish("Hello from ESP32!");
      lastMsg = millis();
      Serial.println("Message sent!");
  }
  delay(5000);
}