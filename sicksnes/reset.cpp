#include <Arduino.h>
#include "reset.h"

#define resetPin 4
extern bool debug;

void triggerReset() {
    if (debug) Serial.println("Quick resetting console");
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(100);
    digitalWrite(resetPin, LOW);
    delay(100);
    pinMode(resetPin, INPUT);
}

void triggerMediumReset() {
    if (debug) Serial.println("Medium resetting console");
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(500);
    digitalWrite(resetPin, LOW);
    pinMode(resetPin, INPUT);
}

void triggerLongReset() {
    if (debug) Serial.println("Long resetting console");
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(2000);
    digitalWrite(resetPin, LOW);
    delay(100);
    pinMode(resetPin, INPUT);
}
