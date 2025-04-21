// reset.cpp
#include <Arduino.h>
#include "reset.h"

#define resetPin 4
extern bool debug;

void triggerReset() {
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(100);
    digitalWrite(resetPin, LOW);
    delay(100);
    pinMode(resetPin, INPUT);
    if (debug) Serial.println("Resetting console");
}

void triggerMediumReset() {
    if (debug) Serial.println("Resetting console");
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(500);
    digitalWrite(resetPin, LOW);
    pinMode(resetPin, INPUT);
}

void triggerLongReset() {
    if (debug) Serial.println("Resetting console");
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(2000);
    digitalWrite(resetPin, LOW);
    delay(100);
    pinMode(resetPin, INPUT);
}
