//////////////////////////////////////////////////////
//      _      _                                    //
//     (_)    | |                                   //
//  ___ _  ___| | __   ___ ____   ___  ___          //
// / __| |/ __| |/ /  / __| '_ \ / _ \/ __|         //
// \__ \ | (__|   <   \__ \ | | |  __/\__ \         //
// |___/_|\___|_|\_\  \___/_| |_|\___||___/  v1.3.0 //
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

#include <EEPROM.h>

// PINS
constexpr bool debug = false;
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

// Button definitions
// B, Y, SELECT, START, UP, DOWN, LEFT, RIGHT, A, X, L, R
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
    int resetCombo[4];
    int longResetCombo[4];
    bool initialized;
};

const int EEPROM_ADDR = 0;
ButtonCombos combos;

void saveDefaultCombos() {
    combos = {
        {buttonStart, buttonL, buttonR, buttonLeft},
        {buttonStart, buttonL, buttonR, buttonDown},
        true
    };
    EEPROM.put(EEPROM_ADDR, combos);
    if (debug) Serial.println("Reset de fábrica realizado!");
}

void loadCombos() {
    EEPROM.get(EEPROM_ADDR, combos);
    if (!combos.initialized) {
        saveDefaultCombos();
    }
}

int buttonsState[12];

void setup()
{
    // init CIC
    pinMode(cicPin, OUTPUT);
    digitalWrite(cicPin, HIGH);
    //reset the console to sync with the arduino
    triggerMediumReset();
    //turn on leds
    pinMode(ledPin, OUTPUT);
    updateLED();
    if (debug)
    {
        Serial.begin(9600);
    }

    // init watch for the CIC check pin
    pinMode(CicGndPin, INPUT_PULLUP);

    // init buttons
    pinMode(clock, INPUT);
    pinMode(latch, INPUT);
    pinMode(Sdata, INPUT);

    // wait the console to boot
    delay(1500);

    handleCICUnlockIfNeeded();

    loadCombos();
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

void disableGroundPin()
{
    pinMode(CicGndPin, INPUT);
    digitalWrite(CicGndPin, LOW);
}

bool alreadyCheckedFactoryReset = false;
void loop()
{
    waitForlatchFalling();

    for (int i = 0; i < 12; i++)
    {
        waitForclockFalling();
        buttonsState[i] = dataRead();
    }

    if (!alreadyCheckedFactoryReset) {
        const int factoryResetCombo[] = {buttonSelect};
        if (isComboPressed(factoryResetCombo, 1)) {
            if (debug) Serial.println("Reset de fábrica solicitado (SELECT pressionado)...");
            saveDefaultCombos();
        }
        alreadyCheckedFactoryReset = true;
    }

    if (isComboPressed(combos.resetCombo, sizeof(combos.resetCombo) / sizeof(int)))
    {
        if (debug)
        {
            Serial.println("Soft reset");
        }
        blinkActiveLed();
        triggerReset();
    }

    if (isComboPressed(combos.longResetCombo, sizeof(combos.longResetCombo) / sizeof(int)))
    {
        if (debug)
        {
            Serial.println("Soft reset");
        }
        blinkActiveLed();
        triggerLongReset();
    }

    delay(300);
}

//////////////////////////////////////////////
/////// CIC RELATED FUNCTIONS ////////////////
//  88                           88         //
//  88                           88         //
//  88                           88         //
//  88   ,adPPYba,    ,adPPYba,  88   ,d8   //
//  88  a8"     "8a  a8"     ""  88 ,a8"    //
//  88  8b       d8  8b          8888[      //
//  88  "8a,   ,a8"  "8a,   ,aa  88`"Yba,   //
//  88   `"YbbdP"'    `"Ybbd8"'  88   `Y8a  //
//////////////////////////////////////////////
//////////////////////////////////////////////

void toggleCIC()
{
    digitalWrite(cicPin, !digitalRead(cicPin));
}

void updateLED()
{
    if (digitalRead(cicPin))
    {
        digitalWrite(greenPin, LOW);
        pinMode(greenPin, INPUT);
        pinMode(redPin, OUTPUT);
        digitalWrite(redPin, HIGH);
    }
    else
    {
        digitalWrite(redPin, LOW);
        pinMode(redPin, INPUT);
        delay(200);
        pinMode(greenPin, OUTPUT);
        digitalWrite(greenPin, HIGH);
        delay(200);
        digitalWrite(greenPin, LOW);
        delay(200);
        digitalWrite(greenPin, HIGH);
    }
}

//////////////////////////////////////////////////////////////
/////// IGR related functions ////////////////////////////////
//                                                          //
//                                                 ,d       //
//                                                 88       //
//    8b,dPPYba,  ,adPPYba, ,adPPYba,  ,adPPYba, MM88MMM    //
//    88P'   "Y8 a8P_____88 I8[    "" a8P_____88   88       //
//    88         8PP"""""""  `"Y8ba,  8PP"""""""   88       //
//    88         "8b,   ,aa aa    ]8I "8b,   ,aa   88,      //
//    88          `"Ybbd8"' `"YbbdP"'  `"Ybbd8"'   "Y888    //
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

bool isComboPressed(const int combo[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buttonsState[combo[i]] != 0) {
            return false;
        }
    }
    return true;
}

int latchState()
{
    int state = (PINC & (1 << LATCH_BIT)); // A0
    if (state)
    {
        return HIGH;
    }
    else
    {
        return LOW;
    }
}

int clockState()
{
    int state = (PINC & (1 << CLOCK_BIT)); // A1
    if (state)
    {
        return HIGH;
    }
    else
    {
        return LOW;
    }
}

int dataRead()
{
    int state = (PINC & (1 << SDATA_BIT)); // A2
    return state;
}

void waitForclockFalling()
{
    while (clockState() == LOW)
    {
    }
    while (clockState() == HIGH)
    {
    }
}

void waitForlatchFalling()
{
    while (latchState() == LOW)
    {
    }
    while (latchState() == HIGH)
    {
    }
}

bool isRedLEDActive() {
    return DDRD & (1 << PD7);
}

void blinkActiveLed()
{
    if (isRedLEDActive())
    {
        digitalWrite(redPin, LOW);
        delay(200);
        digitalWrite(redPin, HIGH);
    }
    else
    {
        digitalWrite(greenPin, LOW);
        delay(200);
        digitalWrite(greenPin, HIGH);
    }
}

void triggerReset()
{
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(100);
    digitalWrite(resetPin, LOW);
    delay(100);
    pinMode(resetPin, INPUT);
    if (debug)
    {
        Serial.println("Resetting console");
    }
}

void triggerMediumReset()
{
    if (debug)
    {
        Serial.println("Resetting console");
    }
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(500);
    digitalWrite(resetPin, LOW);
    pinMode(resetPin, INPUT);
}

void triggerLongReset()
{
    if (debug)
    {
        Serial.println("Resetting console");
    }
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(2000);
    digitalWrite(resetPin, LOW);
    delay(100);
    pinMode(resetPin, INPUT);
}