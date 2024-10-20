#include <Arduino.h>
#include "ledblink.h"

Blink::Blink(int pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);  // Set the pin as an output
}

void Blink::blinkRate(int rate) {
    digitalWrite(_pin, HIGH); // Turn the LED on
    delay(rate);              // Wait for the specified time
    digitalWrite(_pin, LOW);  // Turn the LED off
    delay(rate);              // Wait again for the specified time
}
