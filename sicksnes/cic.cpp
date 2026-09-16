#include <Arduino.h>
#include "cic.h"
#include "pins.h"
#include "led.h"
#include "reset.h"

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
