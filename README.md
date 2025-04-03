# ⛑️ sick snes - Automatic Lockout Chip Toggle and In-Game Reset
![sick snes logo](images/logo.jpg)
## 🗺️ Overview
sick snes is a mod for the Super Nintendo Entertainment System (SNES) that allows automatic lockout chip toggling and in-game reset functionality. If a cartridge fails the CIC (Checking Integrated Chip) check, the mod disables the CIC and resets the console automatically.

### 💎 Features
- **Automatic lockout chip toggle**
- **In-game reset functionality** using controller input
- **Compatibility with all SNES consoles**
- **LED indicators** for CIC status

### 🕹️ Controls
- 🔄 **Soft Reset:** Press `L + R + START + LEFT`
- 🔄 **Long Reset:** Press `L + R + START + DOWN` (Some flashcarts only return to the menu with long reset)
- 🔒 **Forcefully Disable CIC:** Press `L + R + START + RIGHT`

### 🎬 Quick demo
[https://youtu.be/vH6iCG8fFg8](https://youtu.be/vH6iCG8fFg8)

---

## 📋 Installation Guide
### 🔌 Wiring Instructions
Connect the Arduino to the SNES board as follows:

| Arduino Pin | SNES Connection   |
|------------|------------------|
| `5V`       | `Board VCC`       |
| `GND`      | `Board GND`       |
| `D4`       | `Reset Button`    |
| `D5`       | `CIC Pin 1`       |
| `D6`       | `Green LED`       |
| `D7`       | `Red LED`         |
| `D8`       | `CIC Pin 4**`     |
| `A0`       | `Controller Pin 10` |
| `A1`       | `Controller Pin 8`  |
| `A2`       | `Controller Pin 6`  |

**Note:** CIC Pin 4 must be disconnected (lifted) for proper operation.

### 💡 The LEDs
#### 🔴 Stock red LED
First cut the 5v track for the stock red LED, and connect the positive lead to both D6 and D7. The LED will blink as an indicator when the lock-out chip is disabled.
#### 🎨 RGB LED
Connect the positive lead of the red LED to D7 and the positive lead of the green LED (the one that turns on when the lock-out chip is disabled) to D6.
If you are using LEDs rated for less than 5V, make sure to install appropriate resistors to prevent damage.

### ↻ Reset Button Wiring (Viewed from PCB Bottom)
```
         __________
        |  •    •  |
        |          |
        |          |
        | 5V    D4 |
         ‾‾‾‾‾‾‾‾‾‾
```

### 𓇲 CIC F411X Chip Wiring
```
    9 ______________________________ 1
      | •  •  •  •  •  D8   •  •  D5 |
      |                              |
      |                              |
      |                              |
      |                              |
      |                              |
      |                              |
      |                              |
      | •  •  •  •  •   •   •  •  •  |
    10 ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ 18
```

### 🎮 SNES Controller Wiring (Viewed from PCB Bottom)
```
    10 _____________________________ 2
      |   A0   A1   A2   •    •    |
      |                            |
      | •    •    •    •    •    • |
    11 ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ 1
```

### 📈 Diagram

![installation scheme](images/scheme.png)
![sample installation](images/example-1.png)

---

## 💻 Programming Instructions
To flash the Arduino with this mod, follow these steps:

### 💽 Standard Flashing via USB
1. Connect the Arduino to your computer via USB.
2. Use the Arduino IDE or a similar tool to upload the firmware.
3. If using a bootloader, some consoles may require a manual reset workaround for *Street Fighter Alpha 2* (see below).

### 🧰 Flashing Without Bootloader via USBasp + Extreme Burner
For some consoles, *Street Fighter Alpha 2* may require a faster startup. To achieve this:

#### 🧱 1. Install driver with Zadig (one-time setup)
1. Download and open [Zadig](https://zadig.akeo.ie/).
2. Plug in your **USBasp programmer**.
3. In Zadig:
   - Go to `Options > List All Devices` and enable it.
   - Select `USBasp` from the dropdown list.
   - Choose the `libusb-win32` driver (or `libusbK` if needed).
   - Click `Install Driver`.

> This step allows Extreme Burner to recognize your USBasp programmer properly.

#### 💾 2. Flash the firmware using Extreme Burner - AVR
1. Open **Extreme Burner - AVR**.
2. Go to `Settings > Programmer` and select **USBasp**.
3. Under `Chip`, select **ATmega328P** (or the chip you have).
4. Go to `File > Open` and load the firmware `.hex` file [found here](https://github.com/lucaslealdev/sicksnes/releases/download/latest/sicksnes.hex).
5. Click the `Write All` button (chip icon with arrow) to start flashing.

> ⚠️ If you see a "signature mismatch" warning, it's safe to ignore it when using an ATmega328PB.
> The ATmega328PB has a different signature from the 328P, but they are compatible for this purpose.


## 🔗 Compatibility

| SNES Model         | With bootloader (via usb) | No bootloader (USBasp) |
|------------------------|--------------------------------|--------------------------------|
| SNS-CPU-1CHIP-02      | ✅ Full                     | ✅ Full |
| SNS-CPU-1CHIP-03      | ⚠️ Need SFA2 workaround   | ✅ Full |

Every other SNES board should be compatible with this mod, but might need SFA2 workaround when using a bootloader.

### ⚠️ Workaround for *Street Fighter Alpha 2* (only for bootloader usage)
If the Arduino is flashed via USB (with a bootloader), and *Street Fighter Alpha 2* does not boot properly on your console:
- Hold the **reset button** until the Arduino initializes (~1.8s after power-up). This allows the game to start *after* the Arduino executes its code.

**Note:** This issue is rare and only affects *Street Fighter Alpha 2* on certain SNES consoles.

### 🙏 Help Improve Compatibility!  

If you test this mod on a different SNES model, please share your results!  
Let us know which console you used and whether everything worked as expected.  

This will help improve the compatibility list for future users.  

Thank you for your contribution! 🎮🔥

---

## ✨ Credits
- **Project Creator:** @lucaslealdev 🙋‍♂️
- **Year:** 2025 📅
- **Purpose:** Let's mod these games 🎮
- **Youtube channel:** [Pixel que Byte](https://www.youtube.com/@PixelqueByte)

This project is provided as-is, with no guarantees. Modify and distribute at your own risk!

