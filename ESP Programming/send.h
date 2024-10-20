#ifndef SEND_H
#define SEND_H

#include <Arduino.h>

void setupWiFi(const char* ssid, const char* password);  
void transmit(const String& nodeId, float temperature, float humidity, const String& formattedTime);  

#endif
