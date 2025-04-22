//////////////////////////////////////////////////////
//      _      _                                    //
//     (_)    | |                                   //
//  ___ _  ___| | __   ___ ____   ___  ___          //
// / __| |/ __| |/ /  / __| '_ \ / _ \/ __|         //
// \__ \ | (__|   <   \__ \ | | |  __/\__ \         //
// |___/_|\___|_|\_\  \___/_| |_|\___||___/  v1.4.0 //
//////////////////////////////////////////////////////
//
// Made by @lucaslealdev 🙋‍♂️
// 2025 📅
// let's mod these games 🎮
//
// The _sick snes_ allows automatic lockout chip toggle and in-game reset.
// If the cart does not pass the CIC check,
// this mod turns the CIC off and reset the console automatically.
// https://github.com/lucaslealdev/sicksnes

#include <Arduino.h>
#include <EEPROM.h>

#include "combo.h"
#include "cic.h"
#include "reset.h"
#include "led.h"
#include "controller.h"

bool debug = false;

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
bool alreadyCheckedConfiguration = false;
int status = 0;

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

    if (!alreadyCheckedConfiguration) {
        const int configurationCombo[] = {buttonStart};
        if (isComboPressed(configurationCombo, 1)) {
            if (debug) Serial.println("Configurating buttons");
            status = 1;
        }
        alreadyCheckedConfiguration = true;
    }

    switch (status) {
        case 1:
            blinkActiveLed();
            if (isAnyComboPressed()) {
                if (debug) Serial.println("Reset combo selected...");
                int pressedCombo[12];
                int size = getPressedCombo(pressedCombo, sizeof(pressedCombo) / sizeof(pressedCombo[0]));
                if (debug) Serial.print("Pressed combo: ");
                for (int i = 0; i < size; i++) {
                    if (debug) Serial.print(pressedCombo[i]);
                    if (i < size - 1 && debug) Serial.print(", ");
                }
                if (debug) Serial.println();
                saveNewResetCombo(pressedCombo, size);
                status = 2;
                delay(1000);
            }
            delay(300);
            return;
        case 2:
            blinkActiveLed();
            if (isAnyComboPressed()) {
                if (debug) Serial.println("Long reset combo selected...");
                int pressedCombo[12];
                int size = getPressedCombo(pressedCombo, sizeof(pressedCombo) / sizeof(pressedCombo[0]));
                if (debug) Serial.print("Pressed combo: ");
                for (int i = 0; i < size; i++) {
                    if (debug) Serial.print(pressedCombo[i]);
                    if (i < size - 1 && debug) Serial.print(", ");
                }
                if (debug) Serial.println();
                saveNewLongResetCombo(pressedCombo, size);
                status = 0;
                delay(1000);
            }
            delay(300);
            return;
    }

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
    } else if (isComboPressed(combos.longResetCombo, sizeof(combos.longResetCombo) / sizeof(int))) {
        if (debug) Serial.println("Long soft reset");
        blinkActiveLed();
        triggerLongReset();
    }

    delay(300);
}
