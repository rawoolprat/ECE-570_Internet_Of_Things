#ifndef SEND_H
#define SEND_H

#include <Arduino.h>

class Switch {
public:
    // Constructor: takes the pin number as an argument
    Switch(uint8_t pin);
    
    // Initializes the switch pin as input
    void begin();
    
    // Checks if the switch is pressed
    bool isPressed();

private:
    uint8_t _pin;  // Stores the pin number where the switch is connected
};

// Declare mySwitch as an external variable
extern Switch mySwitch;

// Function to send a notification to Slack via IFTTT
void sendNotification();

// Function to connect to WiFi
void connectToWiFi();

#endif
