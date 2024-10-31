// main.cpp: Main file to control the flow of the program
#include <Arduino.h>
#include "send.h"

void setup() {
    Serial.begin(9600);
    connectToWiFi(); // Connect to WiFi

    pinMode(LED_BUILTIN, OUTPUT); // Control onboard LED (D4)
    digitalWrite(LED_BUILTIN, HIGH); // Turn LED off initially (active LOW)
}

void loop() {
    checkLEDStatus(); // Function that makes an HTTP request to read LED status
    delay(5000); // Check the status every 5 seconds
}
