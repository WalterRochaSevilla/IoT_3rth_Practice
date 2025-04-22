#include "Mqtt.hpp"
NetworkConfig netConfig("wifi_name", "wifi_password");
NetworkController wifi(netConfig);
MQTTConfig mqttConfig("broker.hivemq.com", 1883, "ucb/grupo12/esp32/test", "ucb/grupo12/esp32/test/info", "esp32_testing_client_grupo_12",
[](char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
});
MQTTClient  mqtt(wifi, mqttConfig);

void setup() {
  Serial.begin(115200);
  mqtt.initialize();
  mqtt.subscribe();
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