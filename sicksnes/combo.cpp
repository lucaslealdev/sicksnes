#include "combo.h"

const int EEPROM_ADDR = 0;
ButtonCombos combos;

void saveDefaultCombos() {
    combos = {
        {buttonStart, buttonL, buttonR, buttonLeft},
        {buttonStart, buttonL, buttonR, buttonDown},
        true
    };
    EEPROM.put(EEPROM_ADDR, combos);
}

void loadCombos() {
    EEPROM.get(EEPROM_ADDR, combos);
    if (!combos.initialized) {
        saveDefaultCombos();
    }
}