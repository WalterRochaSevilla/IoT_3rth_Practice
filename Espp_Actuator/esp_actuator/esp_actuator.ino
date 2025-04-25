#include "Mqtt.hpp"
#include "ServoController.hpp"


NetworkConfig netConfig("Matheo", "cg4hts55fh4fyr7");
NetworkController wifi(netConfig);
const int SERVO_PIN = 22;   // Pin del servomotor
ServoController servoController(SERVO_PIN);


MQTTConfig mqttConfig("broker.hivemq.com", 1883, "ucb/grupo12/esp32/test", "ucb/grupo12/esp32/test/door", "esp32_actuator_testing_client_grupo_12",
    [](char* topic, byte* payload, unsigned int length) {
        Serial.print("Mensaje recibido [");
        Serial.print(topic);
        Serial.print("]: ");
        byte message;
        Serial.println(payload[0]);
        if(payload[0] == 48){
            servoController.open(); // Open the servo
        } else if(payload[0] == 49){
            Serial.println("Turning OFF the actuator");
            servoController.close();
        } else {
            Serial.println("Invalid command received.");
        }
    });
MQTTClient  mqtt(wifi, mqttConfig);

void setup() {
    Serial.begin(115200);
    mqtt.initialize();
    servoController.begin();
    mqtt.subscribe();
}
void loop(){
    mqtt.loop();;
}