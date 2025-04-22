#include <Arduino.h>
#include <EEPROM.h>

#include "combo.h"
#include "cic.h"
#include "reset.h"
#include "led.h"
#include "controller.h"

bool debug = true;

// Pinos
#define resetPin 4
#define CicGndPin 5
#define greenPin 6
#define redPin 7
#define cicPin 8
#define ledPin 13
#define latch 19 // A0
#define clock 20 // A1
#define Sdata 21 // A2
constexpr uint8_t LATCH_BIT = 0;
constexpr uint8_t CLOCK_BIT = 1;
constexpr uint8_t SDATA_BIT = 2;

bool alreadyCheckedFactoryReset = false;

void setup() {
    if (debug) Serial.begin(9600);
    pinMode(cicPin, OUTPUT);
    digitalWrite(cicPin, HIGH);
    triggerMediumReset();

    pinMode(ledPin, OUTPUT);
    updateLED();

    pinMode(CicGndPin, INPUT_PULLUP);
    pinMode(clock, INPUT);
    pinMode(latch, INPUT);
    pinMode(Sdata, INPUT);

    delay(1500);

    handleCICUnlockIfNeeded();
    loadCombos();
}

void loop() {
    waitForlatchFalling();
    loadInput();

    if (!alreadyCheckedFactoryReset) {
        const int factoryResetCombo[] = {buttonSelect};
        if (isComboPressed(factoryResetCombo, 1)) {
            if (debug) Serial.println("Factory reset (SELECT)...");
            saveDefaultCombos();
        }
        alreadyCheckedFactoryReset = true;
    }

    if (isComboPressed(combos.resetCombo, sizeof(combos.resetCombo) / sizeof(int))) {
        if (debug) Serial.println("Soft reset");
        blinkActiveLed();
        triggerReset();
    }

    if (isComboPressed(combos.longResetCombo, sizeof(combos.longResetCombo) / sizeof(int))) {
        if (debug) Serial.println("Soft reset longo");
        blinkActiveLed();
        triggerLongReset();
    }

    delay(300);
}
