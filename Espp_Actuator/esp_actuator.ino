#include "Mqtt.hpp"
NetworkConfig netConfig("wifi_name", "wifi_password");
NetworkController wifi(netConfig);
MQTTConfig mqttConfig("broker.hivemq.com", 1883, "ucb/grupo12/esp32/test", "ucb/grupo12/esp32/test/info", "esp32_actuator_testing_client_grupo_12",
    [](char* topic, byte* payload, unsigned int length) {
        Serial.print("Mensaje recibido [");
        Serial.print(topic);
        Serial.print("]: ");
        char message[length + 1];
        for (unsigned int i = 0; i < length; i++) {
            Serial.print((char)payload[i]);
            message[i] = (char)payload[i];
        }
        String messageString = String(message);
        Serial.println();
        if(messageString == "ON"){
            servoController.open(); // Open the servo
        } else if(messageString == "OFF"){
            Serial.println("Turning OFF the actuator");
            servoController.close();
        } else {
            Serial.println("Invalid command received.");
        }
    });
MQTTClient  mqtt(wifi, mqttConfig);
const int SERVO_PIN = 15;   // Pin del servomotor
ServoController servoController(SERVO_PIN);

void setup() {
    Serial.begin(115200);
    mqtt.initialize();
    servoController.begin();
    mqtt.subscribe();
}
void loop(){
    mqtt.loop();;
}