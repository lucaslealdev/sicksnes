#include <Arduino.h>
#include "led.h"
#include "cic.h"

#define greenPin 6
#define redPin 7
#define greenGNDPin 9
#define redGNDPin 10

void greenLedOn() {
    digitalWrite(redPin, LOW);
    pinMode(redPin, INPUT);
    pinMode(redGNDPin, INPUT);
    delay(200);
    digitalWrite(greenPin, HIGH);
    pinMode(greenPin, OUTPUT);
    digitalWrite(greenGNDPin, LOW);
    pinMode(greenGNDPin, OUTPUT);
}

void redLedOn() {
    digitalWrite(greenPin, LOW);
    pinMode(greenPin, INPUT);
    pinMode(greenGNDPin, INPUT);
    delay(200);
    digitalWrite(redPin, HIGH);
    pinMode(redPin, OUTPUT);
    digitalWrite(redGNDPin, LOW);
    pinMode(redGNDPin, OUTPUT);
}

bool isRedLEDActive() {
    return DDRD & (1 << PD7);
}

void blinkActiveLed() {
    if (isRedLEDActive()) {
        digitalWrite(redPin, LOW);
        pinMode(redGNDPin, INPUT);
        delay(300);
        digitalWrite(redPin, HIGH);
        pinMode(redGNDPin, OUTPUT);
        delay(300);
    } else {
        digitalWrite(greenPin, LOW);
        pinMode(greenGNDPin, INPUT);
        delay(300);
        digitalWrite(greenPin, HIGH);
        pinMode(greenGNDPin, OUTPUT);
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
