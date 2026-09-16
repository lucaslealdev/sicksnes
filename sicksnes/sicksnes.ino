//////////////////////////////////////////////////////
//      _      _                                    //
//     (_)    | |                                   //
//  ___ _  ___| | __   ___ ____   ___  ___          //
// / __| |/ __| |/ /  / __| '_ \ / _ \/ __|         //
// \__ \ | (__|   <   \__ \ | | |  __/\__ \         //
// |___/_|\___|_|\_\  \___/_| |_|\___||___/  v1.5.0 //
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

#include "pins.h"
#include "combo.h"
#include "cic.h"
#include "reset.h"
#include "led.h"
#include "controller.h"

bool debug = false;

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

static void handleComboConfiguration(void (*saveCombo)(int*, size_t), const char* selectedLabel, int nextStatus) {
    blinkActiveLed();
    if (isAnyComboPressed()) {
        if (debug) Serial.println(selectedLabel);
        int pressedCombo[NUM_BUTTONS];
        int size = getPressedCombo(pressedCombo, sizeof(pressedCombo) / sizeof(pressedCombo[0]));
        if (debug) {
            Serial.print("Pressed combo: ");
            printCombo(pressedCombo, size);
            Serial.println();
        }
        saveCombo(pressedCombo, size);
        status = nextStatus;
        delay(1000);
    }
    delay(300);
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
            handleComboConfiguration(saveNewResetCombo, "Reset combo selected...", 2);
            return;
        case 2:
            handleComboConfiguration(saveNewLongResetCombo, "Long reset combo selected...", 0);
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
