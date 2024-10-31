#include <Arduino.h>
#include "send.h"

void setup() {
    Serial.begin(9600);
    // Set up switch
    mySwitch.begin();

    // Connect to WiFi
    connectToWiFi();
}

void loop() {
    // Prompt user to press the button
    Serial.println("Please press the button to send a notification...");

    // Wait for button press
    while (!mySwitch.isPressed()) {
        // Loop until the button is pressed
        delay(100);
    }

    // Button pressed, send the notification
    Serial.println("Button pressed! Sending notification...");
    sendNotification();
}
