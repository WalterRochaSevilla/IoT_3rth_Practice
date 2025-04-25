#include "Mqtt.hpp"
#include "MagneticSensor.h"
const int SENSOR_PIN = 12;  // Pin del sensor magnético
MagneticSensor doorSensor(SENSOR_PIN);

NetworkConfig netConfig("wifi_name", "wifi_password");
NetworkController wifi(netConfig);
MQTTConfig mqttConfig("broker.hivemq.com", 1883, "ucb/grupo12/esp32/test/door", "ucb/grupo12/esp32/test/info", "esp32_testing_client_grupo_12",
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

void setup(){
  Serial.begin(115200);
  doorSensor.begin();
  mqtt.initialize();
}

void loop(){
  mqtt.loop();
  if (doorSensor.hasStateChanged()) {
    if (doorSensor.getLastState()) {
      mqtt.publish("OPEN");
    } else {
      mqtt.publish("CLOSED");
    }
  }
}
