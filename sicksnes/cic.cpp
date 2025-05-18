#include <Arduino.h>
#include "cic.h"

#define CicGndPin 5
#define cicPin 8
#define ledPin 13
#define greenPin 6
#define redPin 7
#define greenGNDPin 9
#define redGNDPin 10

extern bool debug;

void disableGroundPin() {
    pinMode(CicGndPin, INPUT);
    digitalWrite(CicGndPin, LOW);
}

bool isCicOn() {
    return digitalRead(cicPin) == HIGH;
}

void toggleCIC() {
    digitalWrite(cicPin, !digitalRead(cicPin));
}

void updateLED();
void triggerReset();

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
