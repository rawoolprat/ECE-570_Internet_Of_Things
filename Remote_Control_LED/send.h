// send.h: Header file for function declarations and variable definitions
#ifndef SEND_H
#define SEND_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

extern const char* ssid;
extern const char* password;
extern const char* resultsUrl;

void connectToWiFi();
void checkLEDStatus();

#endif
