#include "combo.h"

const int EEPROM_ADDR = 0;
extern bool debug;
ButtonCombos combos;

void saveDefaultCombos() {
    combos = {
        {buttonStart, buttonL, buttonR, buttonLeft},
        {buttonStart, buttonL, buttonR, buttonDown},
        true
    };
    if (debug) {
        Serial.println("Default combos saved to EEPROM.");
    }
    EEPROM.put(EEPROM_ADDR, combos);
}

void saveNewResetCombo(int* newCombo, size_t size) {
    if (size != 4) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        combos.resetCombo[i] = newCombo[i];
    }
    EEPROM.put(EEPROM_ADDR, combos);
    if (debug) {
        Serial.print("New reset combo saved: ");
        for (size_t i = 0; i < size; i++) {
            Serial.print(combos.resetCombo[i]);
            if (i < size - 1) {
                Serial.print(", ");
            }
        }
        Serial.println();
    }
}

void saveNewLongResetCombo(int* newCombo, size_t size) {
    if (size != 4) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        combos.longResetCombo[i] = newCombo[i];
    }
    EEPROM.put(EEPROM_ADDR, combos);
    if (debug) {
        Serial.print("New long reset combo saved: ");
        for (size_t i = 0; i < size; i++) {
            Serial.print(combos.longResetCombo[i]);
            if (i < size - 1) {
                Serial.print(", ");
            }
        }
        Serial.println();
    }
}

void loadCombos() {
    EEPROM.get(EEPROM_ADDR, combos);
    if (combos.resetCombo[0] == -1 || !combos.initialized) {
        saveDefaultCombos();
    }
    if (debug) {
        Serial.print("Loaded reset combo: ");
        for (size_t i = 0; i < 4; i++) {
            Serial.print(combos.resetCombo[i]);
            if (i < 3) {
                Serial.print(", ");
            }
        }
        Serial.println();
    }
}