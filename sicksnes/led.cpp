#include <Arduino.h>
#include "led.h"
#include "cic.h"

#define greenPin 6
#define redPin 7

void greenLedOn() {
    digitalWrite(redPin, LOW);
    pinMode(redPin, INPUT);
    delay(200);
    pinMode(greenPin, OUTPUT);
    digitalWrite(greenPin, HIGH);
}

void redLedOn() {
    digitalWrite(greenPin, LOW);
    pinMode(greenPin, INPUT);
    delay(200);
    pinMode(redPin, OUTPUT);
    digitalWrite(redPin, HIGH);
}

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
    if (isCicOn()) {
        redLedOn();
    } else {
        greenLedOn();
        blinkActiveLed();
        blinkActiveLed();
        blinkActiveLed();
    }
}
