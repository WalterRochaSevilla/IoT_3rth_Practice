#include "EspSensor.hpp"

EspSensor espSensor(12,"Matheo", "cg4hts55fh4fyr7", "broker.hivemq.com", 1883, "ucb/grupo12/esp32/test/door", "ucb/grupo12/esp32/test/info", "esp32_testing_client_grupo_12");
void setup(){
  espSensor.setup();
}

void loop(){
  espSensor.loop();
}



