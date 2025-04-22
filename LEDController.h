#pragma once
#include <Arduino.h>

class LEDController {
public:
    LEDController(int pin = LED_BUILTIN) : m_pin(pin) {
        pinMode(m_pin, OUTPUT);
        off();
    }

    void on() { 
        digitalWrite(m_pin, HIGH);
        m_state = true;
    }
    
    void off() {
        digitalWrite(m_pin, LOW);
        m_state = false;
    }
    
    void toggle() {
        m_state ? off() : on();
    }
    
    bool getState() { return m_state; }

private:
    int m_pin;
    bool m_state = false;
};