#ifndef COMBO_H
#define COMBO_H

#include <Arduino.h>
#include <EEPROM.h>
#include "pins.h"

#define buttonB 0
#define buttonY 1
#define buttonSelect 2
#define buttonStart 3
#define buttonUp 4
#define buttonDown 5
#define buttonLeft 6
#define buttonRight 7
#define buttonA 8
#define buttonX 9
#define buttonL 10
#define buttonR 11

struct ButtonCombos {
    int resetCombo[COMBO_SIZE];
    int longResetCombo[COMBO_SIZE];
    bool initialized;
};

extern const int EEPROM_ADDR;
extern ButtonCombos combos;

void saveDefaultCombos();
void loadCombos();
void saveNewResetCombo(int* newCombo, size_t size);
void saveNewLongResetCombo(int* newCombo, size_t size);
void printCombo(const int* combo, size_t size);

#endif