#include "controller.h"

#define LATCH_BIT 0
#define CLOCK_BIT 1
#define SDATA_BIT 2

int buttonsState[12];

void loadInput() {
    for (int i = 0; i < 12; i++) {
        waitForclockFalling();
        buttonsState[i] = dataRead();
    }
}

bool isComboPressed(const int combo[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buttonsState[combo[i]] != 0) {
            return false;
        }
    }
    return true;
}

int getPressedCombo(int* outCombo, int maxSize) {
    int count = 0;
    for (int i = 0; i < 12 && count < maxSize; i++) {
        if (buttonsState[i] == 0) {
            outCombo[count] = i;
            count++;
        }
    }
    return count;
}

bool isAnyComboPressed() {
    int pressed = 0;
    for (int i = 0; i < 12; i++) {
        if (buttonsState[i] == 0) {
            pressed++;
        }
    }
    return pressed == 4;
}

int latchState() {
    int state = (PINC & (1 << LATCH_BIT));
    return state ? HIGH : LOW;
}

int clockState() {
    int state = (PINC & (1 << CLOCK_BIT));
    return state ? HIGH : LOW;
}

int dataRead() {
    int state = (PINC & (1 << SDATA_BIT));
    return state;
}

void waitForclockFalling() {
    while (clockState() == LOW) {}
    while (clockState() == HIGH) {}
}

void waitForlatchFalling() {
    while (latchState() == LOW) {}
    while (latchState() == HIGH) {}
}
