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
        delay(300);
        digitalWrite(redPin, HIGH);
        delay(300);
    } else {
        digitalWrite(greenPin, LOW);
        delay(300);
        digitalWrite(greenPin, HIGH);
        delay(300);
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
