
#include "send.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "realme 7 Pro";
const char* password = "tashvi28";

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

// Function to check the LED status from the results.txt file
void checkLEDStatus() {
    WiFiClientSecure wifiClientSecure;
    wifiClientSecure.setInsecure(); // Allow insecure connections

    // URL to access the results.txt file
    const char* json_url = "https://pratiksha.tech/LED_json/results.txt";

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(wifiClientSecure, json_url);

        int httpResponseCode = http.GET();

// Print the HTTP response code
       // Serial.print("HTTP Response Code: ");
       // Serial.println(httpResponseCode);
        Serial.println("Fetching the latest status from results.txt file....");


        if (httpResponseCode > 0) {
            // If request is successful
            String payload = http.getString();
            if (payload.length() == 0) {
                Serial.println("Error: Received empty payload.");
                return; // Exit if payload is empty
            }

            // Extract the first line only
            int endOfFirstLine = payload.indexOf('\n');
            String latestStatus = (endOfFirstLine == -1) ? payload : payload.substring(0, endOfFirstLine);

            // Print only the extracted first line for debugging
            Serial.print("Fetched Status: ");
            Serial.println(latestStatus);

            // Format for JSON parsing
            latestStatus.replace(" ", "");
            latestStatus.replace("{Node1{", "{\"Node1\":{");
            latestStatus.replace("LED1:", "\"LED1\":\"");
            latestStatus.replace("}}", "\"}}");

            // Parse the JSON data using StaticJsonDocument
            StaticJsonDocument<200> jsonDoc;  // Properly allocate StaticJsonDocument with 200 bytes
            DeserializationError error = deserializeJson(jsonDoc, latestStatus);

            if (error) {
                Serial.print("Failed to parse JSON: ");
                Serial.println(error.c_str());
                return; // Exit if JSON parsing fails
            }

            // Extract LED status
            const char* ledStatus = jsonDoc["Node1"]["LED1"];
            if (!ledStatus) {
                Serial.println("Error: LED status not found in JSON.");
                return;
            }

            // Control the LED based on the status and print status update
            if (strcmp(ledStatus, "on") == 0 || strcmp(ledStatus, "ON") == 0) {
                digitalWrite(LED_BUILTIN, LOW); // Turn LED on (active low)
                Serial.println("Putting the LED ON");
            } else if (strcmp(ledStatus, "off") == 0 || strcmp(ledStatus, "OFF") == 0) {
                digitalWrite(LED_BUILTIN, HIGH); // Turn LED off (active low)
                Serial.println("Putting the LED OFF");
            }

        } else {
            // Print HTTP error code if response is unsuccessful
            Serial.print("Error getting data, HTTP response code: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi not connected.");
    }
}
