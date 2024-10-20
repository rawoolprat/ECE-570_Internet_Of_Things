#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include "send.h"

void setupWiFi(const char* ssid, const char* password) {
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

//print your wifi details
    Serial.println("");
    Serial.println("Wi-Fi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

// Function to transmit data to the server via HTTPS with formatted time
void transmit(const String& nodeId, float temperature, float humidity, const String& formattedTime) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi Disconnected! Reconnecting...");
        setupWiFi("ssid_name", "password");
    }

    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();  // Disable certificate verification

    String encodedTime = formattedTime;
    encodedTime.replace(" ", "%20");  
    encodedTime.replace(":", "%3A");

    String url = "https://pratiksha.tech/Assignment_PHP_database/db_3.php?"; // Enter your own URL through which you will enter the data into your database server
    url += "nodeId=" + nodeId + "&nodeTemp=" + String(temperature, 1);
    url += "&nodeHumidity=" + String(humidity, 1);
    url += "&time_received=" + encodedTime;

    Serial.println("Sending URL: " + url);

    if (http.begin(client, url)) {
        int httpCode = http.GET();  
        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println("HTTP Response Code: " + String(httpCode));
            Serial.println("Response: " + payload);
        } else {
            Serial.println("Error on sending GET: " + String(httpCode));
        }
        http.end();
    } else {
        Serial.println("Unable to connect to server");
    }
}
