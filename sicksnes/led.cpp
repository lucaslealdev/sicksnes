// led.cpp
#include <Arduino.h>
#include "led.h"

#define greenPin 6
#define redPin 7

bool isRedLEDActive() {
    return DDRD & (1 << PD7);
}

void blinkActiveLed() {
    if (isRedLEDActive()) {
        digitalWrite(redPin, LOW);
        delay(200);
        digitalWrite(redPin, HIGH);
    } else {
        digitalWrite(greenPin, LOW);
        delay(200);
        digitalWrite(greenPin, HIGH);
    }
}

void updateLED() {
    if (digitalRead(8)) {
        digitalWrite(greenPin, LOW);
        pinMode(greenPin, INPUT);
        pinMode(redPin, OUTPUT);
        digitalWrite(redPin, HIGH);
    } else {
        digitalWrite(redPin, LOW);
        pinMode(redPin, INPUT);
        delay(200);
        pinMode(greenPin, OUTPUT);
        digitalWrite(greenPin, HIGH);
        delay(200);
        digitalWrite(greenPin, LOW);
        delay(200);
        digitalWrite(greenPin, HIGH);
    }
}
