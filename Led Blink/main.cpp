#include <Arduino.h>
#include "ledblink.h"

Blink LED(5); // Initialize the LED object on pin GPIO5/D1 

void setup() {
    // 
}

void loop() {
    LED.blinkRate(5000); // Blink the LED with a 5s blinkrate
}
