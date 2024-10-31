#include "send.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char* ssid = "realme 7 Pro";
const char* password = "tashvi28";

// IFTTT webhook URL
const char* slack_webhook_url = "https://maker.ifttt.com/trigger/button_pressed/json/with/key/c0wVWMPBAmcq6ncletoyL";

// Create Switch object
Switch mySwitch(D1);

// Create WiFiClientSecure object
WiFiClientSecure wifiClientSecure;

// Switch class constructor
Switch::Switch(uint8_t pin) {
    _pin = pin;
}

// Initializes the switch pin as input
void Switch::begin() {
    pinMode(_pin, INPUT);
}

// Checks if the switch is pressed (LOW state)
bool Switch::isPressed() {
    return digitalRead(_pin) == LOW;
}

// Function to connect to WiFi
void connectToWiFi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
}

// Function to send a notification to Slack via IFTTT
void sendNotification() {
    // WiFiClientSecure object is used for secure HTTPS requests
    wifiClientSecure.setInsecure();

    // Check if the WiFi is connected
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(wifiClientSecure, slack_webhook_url);
        http.addHeader("Content-Type", "application/json");

        // Define the payload to be sent
        String payload = "{\"value1\":\"Welcome to IoT class @ SSU\"}";

        // Debugging: Show the payload being sent
        Serial.print("Payload: ");
        Serial.println(payload);

        // Send HTTP PUT request
        int httpResponseCode = http.PUT(payload);

        // Print response to serial (optional)
        if (httpResponseCode > 0) {
            Serial.print("Message sent, response code: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error sending message: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi not connected.");
    }
}
