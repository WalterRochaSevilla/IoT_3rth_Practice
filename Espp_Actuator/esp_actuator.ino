    #include "EspActuator.hpp"

    EspActuator espServo(25, "Matheo", "cg4hts55fh4fyr7", "broker.hivemq.com", 1883, "ucb/grupo12/esp32/test", "ucb/grupo12/esp32/test/door", "ESP32_Actuator_12_Client");

    void setup() {
        espServo.setup();
    }

    void loop() {
        espServo.loop();
    }