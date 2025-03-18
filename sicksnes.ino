////////////////////////////////////////////////////
//      _      _                                  //
//     (_)    | |                                 //
//  ___ _  ___| | __   ___ ____   ___  ___        //
// / __| |/ __| |/ /  / __| '_ \ / _ \/ __|       //
// \__ \ | (__|   <   \__ \ | | |  __/\__ \       //
// |___/_|\___|_|\_\  \___/_| |_|\___||___/ v1.0  //
////////////////////////////////////////////////////
//
// Made by @lucaslealdev 🙋‍♂️
// 2025 📅
// let's mod these games 🎮
//
// The _sick snes_ allows automatic lockout chip toggle and in-game reset.
// If the cart does not pass the CIC check,
// this mod turns the CIC off and reset the console automatically.
//
// 🔄 Press L + R + START + LEFT to reset.
// 🔄 Press L + R + START + DOWN to long reset*.
// 🔒 Press L + R + START + RIGHT to toggle the CIC off forcefully.
// * Some flashcart only return to menu when long resetting.
//
////////////////////////////////////////////////////////////
//               INSTALLATION GUIDE:                      //
////////////////////////////////////////////////////////////
//            __________________________
//           | Arduino  | SNES          |
//           |‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
//           | 5V       | Board VCC     |
//           | GND      | Board GND     |
//           | D4       | Reset button  |
//           | D5       | CIC pin 1     |
//           | D6       | green led     |
//           | D7       | red led       |
//           | D8       | CIC pin 4**   |
//           | A0       | Controller 10 |
//           | A1       | Controller 8  |
//           | A2       | Controller 6  |
//            ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
//   [**] that CIC pin 4 must be disconnected(lifted)
//
//    Reset button diagram viewed from the PCB bottom:
//                   __________
//                  |  •    •  |
//                  |          |
//                  |          |
//                  | VIN   D4 |
//                   ‾‾‾‾‾‾‾‾‾‾
//
//                CIC F411X diagram:
//        9 ______________________________ 1
//         | •  •  •  •  •  D8   •  •  D5 |
//         |                              |
//         |                              |
//         |                              |
//         |                              |
//         |                              |
//         |                              |
//         |                              |
//         | •  •  •  •  •   •   •  •  •  |
//       10 ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ 18
//
//
//    Snes controller diagram seen from the PCB bottom:
//        10 _____________________________ 2
//          |   A0   A1   A2   •    •    |
//          |                            |
//          | •    •    •    •    •    • |
//        11 ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ 1
//

// PINS
#define debug false
#define resetPin 4
#define CicGndPin 5
#define greenPin 6
#define redPin 7
#define cicPin 8
#define ledPin 13
#define latch 19 // A0
#define clock 20 // A1
#define Sdata 21 // A2

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
const int resetCombo[] = {buttonStart, buttonL, buttonR, buttonLeft};
const int longResetCombo[] = {buttonStart, buttonL, buttonR, buttonDown};
const int toggleCicCombo[] = {buttonStart, buttonL, buttonR, buttonRight};

int buttonsState[12];

void setup()
{
    // init CIC
    pinMode(cicPin, OUTPUT);
    digitalWrite(cicPin, HIGH);
    //reset the console to sync with the arduino
    triggerReset();
    //turn on leds
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
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
    delay(1000);

    // if the CIC does not pass the check, turn the CIC off
    if (digitalRead(CicGndPin) == HIGH)
    {
        if (debug)
        {
            Serial.println("Unlocking console");
        }
        digitalWrite(ledPin, HIGH);
        toggleCIC();
        triggerReset();
        updateLED();
    }
}

void loop()
{
    waitForlatchFalling();

    for (int i = 0; i < 12; i++)
    {
        waitForclockFalling();
        buttonsState[i] = dataRead();
    }

    if (isResetComboPressed())
    {
        if (debug)
        {
            Serial.println("Soft reset");
        }
        triggerReset();
    }

    if (isLongResetComboPressed())
    {
        if (debug)
        {
            Serial.println("Soft reset");
        }
        triggerLongReset();
    }

    if (isToggleCicComboPressed())
    {
        if (debug)
        {
            Serial.println("Toggle CIC");
        }
        toggleCIC();
        delay(300);
        triggerReset();
        updateLED();
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
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
    }
    else
    {
        digitalWrite(redPin, LOW);
        delay(200);
        digitalWrite(6, HIGH);
        delay(200);
        digitalWrite(6, LOW);
        delay(200);
        digitalWrite(6, HIGH);
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

bool isResetComboPressed()
{
    for (int i = 0; i < sizeof(resetCombo) / sizeof(resetCombo[0]); i++)
    {
        if (buttonsState[resetCombo[i]] != 0)
        {
            return false;
        }
    }
    return true;
}

bool isLongResetComboPressed()
{
    for (int i = 0; i < sizeof(longResetCombo) / sizeof(longResetCombo[0]); i++)
    {
        if (buttonsState[longResetCombo[i]] != 0)
        {
            return false;
        }
    }
    return true;
}

bool isToggleCicComboPressed()
{
    for (int i = 0; i < sizeof(toggleCicCombo) / sizeof(toggleCicCombo[0]); i++)
    {
        if (buttonsState[toggleCicCombo[i]] != 0)
        {
            return false;
        }
    }
    return true;
}

int latchState()
{
    int state = (PINC & (1 << (0))); // A0
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
    int state = (PINC & (1 << (1))); // A1
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
    int data = (PINC & (1 << (2))); // A2
    return data;
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