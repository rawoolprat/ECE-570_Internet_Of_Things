#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "send.h"  // Include send.h for setupWiFi and transmit

// Define the pin for the DHT sensor and the tactile push button
#define DHTPIN 14       // GPIO pin D5 for the DHT sensor
#define DHTTYPE DHT11   // DHT11 sensor type
#define BUTTON_PIN 5    // GPIO pin D1 for the tactile push button

// DHT Sensor instance
DHT dht(DHTPIN, DHTTYPE);

String timeZone = "America/Los_Angeles";

// Function declarations
String check_switch();
String fetch_time();

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Setup button pin as input with internal pull-up

    dht.begin();  // Start DHT sensor
    setupWiFi("ssid_name", "password");  // Call setupWiFi from send.cpp
    timeZone = check_switch(); // Select time zone from user input

    Serial.println("Press the button in order to send data to the database");
}

void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) {  // Button is pressed (LOW when pressed)
        delay(200);  // Debouncing

        // Read sensor data
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
        } else {
            // Get formatted time from the time API
            String formattedTime = fetch_time();

            // Transmit the data
            transmit("node_3", temperature, humidity, formattedTime);  // Send data for node_3
        }

        // Wait for button release
        while (digitalRead(BUTTON_PIN) == LOW) {
            yield();  // Prevent watchdog reset
        }

        Serial.println("Press the button again to send new data.");
    }

    delay(500);  // Small delay to avoid bouncing
}

// Function to select time zone based on user input
String check_switch() {
    Serial.println("List of Time Zones (Default is PT): ");
    Serial.println("1) Eastern Time Zone (ET)");
    Serial.println("2) Central Time Zone (CT)");
    Serial.println("3) Mountain Time Zone (MT)");
    Serial.println("4) Pacific Time Zone (PT)");
    Serial.println("5) Alaska Time Zone (AKT)");
    Serial.println("6) Hawaii-Aleutian Time Zone (HAT)");
    Serial.println("7) Atlantic Time Zone (AT)");

    Serial.println("Choose a timezone:" );

    while (Serial.available() == 0) { 
        delay(100); 
    }

    char choice = Serial.read();
    switch (choice) {
        case '1':
            return "America/New_York";
        case '2':
            return "America/Chicago";
        case '3':
            return "America/Denver";
        case '4':
            return "America/Los_Angeles";  
        case '5':
            return "America/Anchorage";
        case '6':
            return "Pacific/Honolulu";
        case '7':
            return "America/Puerto_Rico";
        default:
            Serial.println("Invalid choice. Defaulting to Pacific Time (PT).");
            return "America/Los_Angeles";
    }
}

// Function to fetch and return formatted time from the API
String fetch_time() {
    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();  // Disable certificate validation for HTTPS

    String url = "https://timeapi.io/api/Time/current/zone?timeZone=" + timeZone;
    http.begin(client, url);  // Using WiFiClientSecure for HTTPS connection
    int httpCode = http.GET();

    String formattedTime = "";

    if (httpCode > 0) {
        String payload = http.getString();

        // Parse the JSON response
        DynamicJsonDocument jsonDoc(512);  // Use DynamicJsonDocument instead of deprecated StaticJsonDocument
        DeserializationError error = deserializeJson(jsonDoc, payload);

        if (!error) {
            const char* dateTime = jsonDoc["dateTime"];  // Full date-time in ISO format
            formattedTime = String(dateTime);
            formattedTime.replace("T", " ");  // Replace 'T' with space
        } else {
            Serial.println("Failed to parse the time API response");
        }
    } else {
        Serial.println("Error on HTTP request to Time API: " + String(httpCode));
    }

    http.end();  // End the HTTP connection

    return formattedTime;
}
