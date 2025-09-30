# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**FalconBMSArduinoConnector** is an Arduino library that enables microcontrollers to receive real-time cockpit data from Falcon BMS flight simulator via serial communication. The library is designed for home cockpit builders to interface physical hardware (LEDs, displays, gauges) with the simulation.

### Repository Structure

- `FalconBMSArduinoConnector/lib/FalconBMSArduinoConnector/` - Main library code
  - `src/` - Core library implementation
  - `examples/` - Example sketches demonstrating library usage
- `FalconBMSArduinoConnector/platformio.ini` - PlatformIO configuration with multiple board profiles

### PC Companion Application

This library requires a PC-side companion application that reads Falcon BMS shared memory and transmits data over serial. The companion app repository is separate and releases are available at: https://github.com/Bacon8tor/FalconBMSArduinoConnector/releases

## Build System

### PlatformIO (Recommended)

**Build for specific board:**
```bash
cd FalconBMSArduinoConnector
pio run -e esp32dev
```

**Upload to board:**
```bash
pio run -e esp32dev --target upload
```

**Monitor serial output:**
```bash
pio device monitor
```

### Supported Boards

Configured environments in `platformio.ini`:
- `esp32dev` - ESP32 DevKit
- `esp32c3` - ESP32-C3 SuperMini
- `megaatmega2560` - Arduino Mega 2560
- `uno` - Arduino Uno
- `nanoatmega328` / `nanoatmega328new` - Arduino Nano
- `promicro` - SparkFun Pro Micro
- `leonardo` - Arduino Leonardo

**Switch between boards** by changing the `-e` environment flag.

### Arduino IDE

The library can also be used with Arduino IDE by installing the library and opening example sketches.

## Architecture

### Serial Protocol

The library implements a custom binary serial protocol for communication with the PC companion app:

1. **Connection Handshake:**
   - PC sends `0xA5`
   - Arduino responds with `0x5A`
   - Connection established when handshake completes
   - 3-second timeout for connection loss detection

2. **Data Request/Response:**
   - Arduino sends command byte (e.g., `0x01` for LightBits)
   - PC responds with packet: `[0xAA][TYPE][LEN][DATA...][CHECKSUM]`
   - Arduino validates checksum and processes data

3. **Packet Structure:**
   - Start byte: `0xAA`
   - Type byte: Identifies data type (matches command byte)
   - Length byte: Payload size
   - Data: Variable length payload
   - Checksum: `(TYPE + LEN + sum(DATA)) & 0xFF`

### Core Class: FalconBMSArduinoConnector

**Key Methods:**

- `begin(Stream& serial, uint32_t baud)` - Initialize serial connection (call `Serial.begin()` first in your sketch)
- `update()` - **Must be called every loop** - handles connection status and serial data
- `isConnected()` - Returns connection state

**Data Request Methods (call only when connected):**

- `getLightBits(int lb)` - Request LightBits (1), LightBits2 (2), or LightBits3 (3)
- `checkAllLights()` - Request all light states (LightBits 1-3, BlinkBits, ECM)
- `getDED()` / `getPFL()` - Request display lines (DED/PFL)
- `getFuelFlow()` / `getinternalFuel()` / `getexternalFuel()` / `getEPUFuel()`
- `getChaffFlareCount()` - Requests both chaff and flare counts
- `getHYDPress()` - Requests both hydraulic pressure values
- `getOilPressure()` / `getOilPressure2()`
- `getNozzlePos()` / `getNozzlePos2()`
- `getFTIT()` / `getFTIT2()`
- `getCabinAlt()` / `getKIAS()` / `getSpeedBrake()` / `getRPM()` / `getRPM2()`
- `getCMDSMode()` / `getuhfPreset()` / `getuhfFreq()`
- `getInstrLight()` / `getFloodConsole()`
- `getXDot()` / `getYDot()` / `getZDot()` - Motion derivatives
- `getAlpha()` / `getBeta()` / `getGamma()` - Flight angles
- `getMach()` / `getVt()` / `getGs()` - Speed parameters
- `getWindOffset()` - Wind offset angle
- `getNoseGearPos()` / `getLeftGearPos()` / `getRightGearPos()` / `getGearPos()` - Gear positions
- `getFuelFlow2()` - Second engine fuel flow
- `getTrimPitch()` / `getTrimRoll()` / `getTrimYaw()` - Trim positions
- `getCurrentHeading()` / `getDesiredHeading()` - Heading information
- `getAltCalReading()` - Altimeter calibration (AAUZ)
- `getBingoFuel()` - Bingo fuel setting
- `getCaraAlow()` - CARA low altitude warning
- `getBullseyeX()` / `getBullseyeY()` - Bullseye coordinates
- `getTurnRate()` - Turn rate
- `getLefPos()` / `getTefPos()` / `getVtolPos()` - Control surface positions (LEF/TEF/VTOL)
- `getLatitude()` / `getLongitude()` - GPS coordinates
- `getMagDeviationSystem()` / `getMagDeviationReal()` - Magnetic deviation
- `getBumpIntensity()` - Turbulence intensity

**State Accessor Methods:**

After requesting data, use boolean getter methods like:
- `isMasterCaution()`, `isFuelLow()`, `isEngFire()` (LightBits)
- `isEPUOn()`, `isChaffLo()`, `isFlareLo()` (LightBits2)
- `isMainGen()`, `isStbyGen()`, `isNoseGearDown()` (LightBits3)
- `isOuterMarkerBlinking()`, `isEPUOnBlinking()` (BlinkBits)

**Data Members (public access):**

- `char dedLines[5][27]` / `char pflLines[5][27]` - Display text arrays
- `float fuelFlow`, `internalFuel`, `externalFuel`, `epuFuel`, `fuelFlow2`
- `float chaffCount`, `flareCount`
- `float rpm`, `rpm2`, `oilPress`, `oilPress2`, `nozzlePos`, `nozzlePos2`
- `float ftit`, `ftit2`, `cabinAlt`, `kias`, `speedBrake`
- `float hydPressA`, `hydPressB`
- `int cmdsMode`, `uhfPreset`, `bingoFuel`
- `long uhfFreq`
- `uint32_t ecm[4]` - ECM status (use `getECMStatus(0-3)` for decoded values)
- `float xDot`, `yDot`, `zDot` - Motion derivatives
- `float alpha`, `beta`, `gamma` - Flight angles (angle of attack, sideslip, flight path angle)
- `float mach`, `vt`, `gs`, `windOffset` - Speed and wind
- `float noseGearPos`, `leftGearPos`, `rightGearPos`, `gearPos` - Gear positions (0.0-1.0)
- `float oilPressure` - Oil pressure
- `float trimPitch`, `trimRoll`, `trimYaw` - Trim surface positions
- `float currentHeading`, `desiredHeading` - Heading values
- `float altCalReading` - Altimeter calibration (AAUZ)
- `float caraAlow` - CARA low altitude warning setting
- `float bullseyeX`, `bullseyeY` - Bullseye reference point coordinates
- `float turnRate` - Turn rate (degrees per second)
- `float lefPos`, `tefPos`, `vtolPos` - Control surface positions (Leading Edge Flap, Trailing Edge Flap, VTOL)
- `float latitude`, `longitude` - Aircraft GPS position
- `float magDeviationSystem`, `magDeviationReal` - Magnetic deviation values
- `float bumpIntensity` - Turbulence/bump intensity

### LightBits System

The library uses bit masking to decode Falcon BMS light state flags:

- **LightBits.h** - Caution panel, master caution, AOA indexers
- **LightBits2.h** - CMDS, refuel, ECM, fuel low warnings
- **LightBits3.h** - Electrical system, gear position, parking brake
- **BlinkBits.h** - Blinking states for various lights
- **EcmBits.h** - ECM panel light states (7 states per button)

Bit values are defined as enums with hex constants (e.g., `MasterCaution = 0x00000001`).

### Optional FBAC_Config.h Features

The library includes optional display/LED support via preprocessor defines:

**Display Support:**
- `#define FBAC_DED_1322` - SSD1322 256x64 OLED (DED)
- `#define FBAC_DED_1309` - SSD1309 128x64 OLED (DED)
- `#define FBAC_PFL_1309` - SSD1309 128x64 OLED (PFL)
- `#define FBAC_FF_1306` / `FBAC_FF_1107` - Fuel flow displays

**LED Support:**
- `#define FBAC_FASTLED` - Includes FastLED library

These defines automatically configure U8g2 display objects and include necessary fonts.

## Typical Usage Pattern

```cpp
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;

void setup() {
  Serial.begin(115200);
  bms.begin(Serial);
  // Initialize your hardware (LEDs, displays, etc.)
}

void loop() {
  bms.update();  // Always call this first

  if (bms.isConnected()) {
    // Request the data you need
    bms.checkAllLights();
    // or request specific data:
    // bms.getLightBits(1);
    // bms.getFuelFlow();

    // Use the data
    if (bms.isMasterCaution()) {
      // Light up master caution LED
    }

    // Access raw data members
    float fuel = bms.internalFuel;
  }
}
```

## Development Notes

### Serial Timing

- Default baud rate: **115200**
- Connection timeout: **3000ms**
- The `waitForPacket()` method blocks until data arrives or timeout
- Always call `bms.update()` at the start of `loop()` to maintain connection

### Data Request Strategy

**Efficient approach:** Only request data you need each frame to minimize serial latency:
```cpp
bms.getLightBits(1);  // Just caution panel
bms.getFuelFlow();     // Just fuel data
```

**Comprehensive approach:** Use `checkAllLights()` if you need full panel state:
```cpp
bms.checkAllLights();  // Requests LightBits 1-3, BlinkBits, ECM
```

### Command Byte Reference

Complete command byte mappings:
- `0x01-0x04` - LightBits (1-3), BlinkBits
- `0x05-0x08` - DED, Fuel Flow, Instr Light, PFL
- `0x09-0x30` - Chaff/Flare, Flood Console, RPM, ECM, Engine data, CMDS, UHF, Speed Brake
- `0x42-0x44` - Motion derivatives (xDot, yDot, zDot)
- `0x48-0x4A` - Flight angles (alpha, beta, gamma)
- `0x4B-0x4E` - Speed parameters (mach, vt, gs, windOffset)
- `0x4F-0x52` - Gear positions (nose, left, right, general)
- `0x60-0x66` - Engine 2 data (rpm2, ftit, oilPressure, fuelFlow2, nozzlePos)
- `0x70-0x72` - Trim (pitch, roll, yaw)
- `0x93-0x94` - Heading (current, desired)
- `0xB4-0xB9` - Navigation (altimeter, bingo fuel, CARA alow, bullseye)
- `0xC0-0xC3` - Control surfaces (turn rate, LEF, TEF, VTOL positions)
- `0xD1-0xD5` - Position & orientation (lat/lon, mag deviation, bump intensity)
- `0x5A` - Handshake response
- `0x99` - Packet failed (checksum error)

### Board-Specific Considerations

- **ESP32/ESP8266:** Use GPIO pin numbers (ESP32 onboard LED typically GPIO2)
- **AVR boards (Uno/Nano/Mega):** Use `LED_BUILTIN` constant
- **Leonardo/Pro Micro:** Can use HID joystick library (see `lib_deps` in platformio.ini)
- **Memory-constrained boards:** Avoid requesting all data types; request only what you need

### I2C Master/Slave Examples

The library includes I2C examples for distributed cockpit systems:
- Master Arduino requests data from PC and distributes to slave Arduinos
- Slaves control specific panel sections (caution panel, fuel display, etc.)
- See `I2C_Master_Example`, `I2C_Slave_CautionPanel`, `I2C_Slave_FuelDisplay`

## Common Patterns

### Blinking Lights

Combine regular light state with blink state:
```cpp
bms.getLightBits(2);
bms.getblinkBits();

if (bms.isEPUOn() && bms.isEPUOnBlinking()) {
  // Implement blinking behavior
}
```

### ECM Panel

ECM lights have 8 possible states (0-7):
```cpp
bms.getECMBits();
for (int i = 0; i < 4; i++) {
  int state = bms.getECMStatus(i);
  // 0=unpressed_no_lit, 1=unpressed_all_lit, 2=pressed_no_lit
  // 3=pressed_standby, 4=pressed_active, 5=pressed_transmit
  // 6=pressed_fail, 7=pressed_all_lit
}
```

### Display Rendering

DED/PFL lines are null-terminated character arrays ready for display:
```cpp
bms.getDED();
for (int i = 0; i < 5; i++) {
  // dedLines[i] contains 24 chars + null terminator
  display.print(bms.dedLines[i]);
}
```