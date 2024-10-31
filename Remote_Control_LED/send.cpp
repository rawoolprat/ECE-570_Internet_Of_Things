// send.cpp: Source file for function definitions
#include "send.h"

const char* ssid = "realme 7 Pro";         // Replace with your WiFi SSID
const char* password = "tashvi28";          // Replace with your WiFi password
const char* resultsUrl = "https://pratiksha.tech/LED/results.txt"; // URL to results.txt

WiFiClientSecure wifiClient;

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi, IP address: ");
    Serial.println(WiFi.localIP());
}

void checkLEDStatus() {
    if (WiFi.status() == WL_CONNECTED) {
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setInsecure();  // This skips certificate validation - not secure, but easy to implement

        HTTPClient https;
        https.begin(*client, resultsUrl);
        int httpCode = https.GET();

        if (httpCode == HTTP_CODE_OK) {
            String payload = https.getString();
            payload.trim(); // Remove any extra spaces or newlines

            // Extract the first line (latest value) from the results.txt content
            String firstLine = payload.substring(0, payload.indexOf('\n'));
            firstLine.trim(); // Clean up the first line value

            Serial.println("Received status: " + firstLine);

            // Control the onboard LED based on the first line value
            if (firstLine == "ON") {
                digitalWrite(LED_BUILTIN, LOW);  // Turn LED ON (active LOW)
                Serial.println("LED is ON");
            } else if (firstLine == "OFF") {
                digitalWrite(LED_BUILTIN, HIGH); // Turn LED OFF (active LOW)
                Serial.println("LED is OFF");
            } else {
                Serial.println("Invalid status in results.txt");
            }
        } else {
            Serial.println("Failed to fetch status, HTTP code: " + String(httpCode));
        }
        https.end();
    } else {
        Serial.println("WiFi disconnected, reconnecting...");
        connectToWiFi(); // Reconnect if disconnected
    }
}
