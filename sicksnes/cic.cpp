// cic.cpp
#include <Arduino.h>
#include "cic.h"

#define CicGndPin 5
#define cicPin 8
#define ledPin 13
#define greenPin 6
#define redPin 7

extern bool debug;

void disableGroundPin() {
    pinMode(CicGndPin, INPUT);
    digitalWrite(CicGndPin, LOW);
}

void toggleCIC() {
    digitalWrite(cicPin, !digitalRead(cicPin));
}

void updateLED(); // declarada em led.h
void triggerReset(); // declarada em reset.h

void handleCICUnlockIfNeeded() {
    bool isGameRunning = digitalRead(CicGndPin) == LOW;
    disableGroundPin();

    if (!isGameRunning) {
        if (debug) Serial.println("Unlocking console");
        digitalWrite(ledPin, HIGH);
        toggleCIC();
        triggerReset();
        updateLED();
    }
}
