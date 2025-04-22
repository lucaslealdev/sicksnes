#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

extern int buttonsState[12];

void loadInput();
bool isComboPressed(const int combo[], size_t size);

int latchState();
int clockState();
int dataRead();
void waitForclockFalling();
void waitForlatchFalling();
int getPressedCombo(int* outCombo, int maxSize);
bool isAnyComboPressed();

#endif
