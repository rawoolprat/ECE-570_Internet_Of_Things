#ifndef SEND_H
#define SEND_H

#include <Arduino.h>

// Function to connect to WiFi
void connectToWiFi();

// Function to check the LED status from the server (JSON format)
void checkLEDStatus();

#endif
