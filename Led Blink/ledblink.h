
#ifndef LEDBLINK_H
#define LEDBLINK_H

class Blink {
public:
    Blink(int pin);            // Constructor to set the pin
    void blinkRate(int rate);  // Function to set the blink rate in milliseconds

private:
    int _pin;                  // Store the pin number
};

#endif
