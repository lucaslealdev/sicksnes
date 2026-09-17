#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

#define resetPin 4
#define CicGndPin 5
#define greenPin 6
#define redPin 7
#define greenGNDPin 9
#define redGNDPin 10
#define cicPin 8
#define ledPin 13
#define latch 19 // A0
#define clock 20 // A1
#define Sdata 21 // A2

constexpr uint8_t LATCH_BIT = 0;
constexpr uint8_t CLOCK_BIT = 1;
constexpr uint8_t SDATA_BIT = 2;

constexpr int NUM_BUTTONS = 12;
constexpr size_t COMBO_SIZE = 4;

#endif
