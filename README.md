# FalconBMSArduinoConnector

[![Arduino](https://img.shields.io/badge/Arduino-Compatible-blue.svg)](https://www.arduino.cc/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange.svg)](https://platformio.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

> **Arduino interface library for Falcon BMS flight simulator with I2C master-slave support**

<<<<<<< Updated upstream
## Features
=======
Create realistic F-16 cockpit instruments using Arduino boards that receive real-time flight data from Falcon BMS. Now includes I2C master-slave functionality for distributed cockpit systems.
>>>>>>> Stashed changes

![Falcon BMS Arduino Connector](docs/banner.png)

## 🚀 Features

### Core Functionality
- **Real-time BMS data** - 100+ flight parameters from Falcon BMS
- **Serial communication** - Robust packet-based protocol with checksums
- **Warning lights** - Complete support for all cockpit warning lights
- **Analog gauges** - Engine parameters, fuel quantities, pressures, temperatures
- **Text displays** - DED (Data Entry Display) and PFL (Pilot Fault List) support
- **ECM systems** - Electronic countermeasures panel data
- **Navigation data** - Course, altitude, speed, bearing information

### Hardware Support
- **Multi-board compatibility** - ESP32, Arduino Uno/Nano/Mega, Leonardo
- **Display integration** - OLED (SSD1306, SSD1309, SSD1322) and LED support
- **FastLED integration** - RGB LED strips for warning lights
- **Memory optimization** - Efficient rendering for low-RAM boards

### 🆕 I2C Master-Slave System
- **Distributed cockpits** - One master Arduino distributes data to multiple slave devices
- **Scalable architecture** - Support for up to 16 slave devices
- **Plug-and-play compatibility** - Exact same API as original library
- **Real-time communication** - 400kHz I2C with configurable update rates
- **Error handling** - Connection monitoring and automatic recovery

## 📦 Installation

### Arduino IDE

1. **Download the library:**
   - Go to [Releases](https://github.com/Bacon8tor/FalconBMSArduinoConnector-Arduino/releases)
   - Download the latest `.zip` file

2. **Install the library:**
   - Open Arduino IDE
   - Go to `Sketch` → `Include Library` → `Add .ZIP Library...`
   - Select the downloaded ZIP file

3. **Install dependencies:**
   ```
   Libraries to install via Library Manager:
   - U8g2 (for OLED displays)
   - FastLED (for RGB LED strips)
   - Wire (for I2C communication)
   ```

### PlatformIO

1. **Add to platformio.ini:**
   ```ini
   [env:your_board]
   platform = espressif32  ; or atmelavr, etc.
   board = esp32dev        ; your board type
   framework = arduino

   lib_deps =
       bacon8tor/FalconBMSArduinoConnector @ ^0.4.0
       olikraus/U8g2@^2.34.0
       fastled/FastLED@^3.6.0
   ```

2. **Build and upload:**
   ```bash
   pio run -t upload
   ```

## 🎯 Quick Start

### Basic Usage (Single Arduino)

```cpp
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;

void setup() {
<<<<<<< Updated upstream
  bms.begin(); //Starts the Serial Connection
=======
  Serial.begin(115200);
  bms.begin(Serial);
>>>>>>> Stashed changes
}

void loop() {
  bms.update();

  if (bms.isConnected()) {
    // Request data from BMS
    bms.getLightBits(1);
    bms.getFuelFlow();

<<<<<<< Updated upstream
```cpp
if (bms.isConnected()) {
  // Proceed with data access
}
```

### Access DED Lines

```cpp
bms.getDED();  // Request line 0
Serial.println(bms.dedLines[0]);
```

### LightBits Example

```cpp
bms.getLightbits(1); // checks Lightbits
if (bms.isMasterCaution()) {
  digitalWrite(ledPin, HIGH);
}
```
### ECM Example 

```cpp
bms.getECMBits();
//Go Over Every Button 
for(int i =0; i < 4;i++){
        switch(bms.getECMStatus(i))
        {
            //unpressed no lit
            case 0:
                //turn all leds off
            break;
            //unpress all lit 
            case 1:
                //turn all leds on
            //pressed no lit 
            case 2:
                //turn all leds off
            break;
            //pressed standy
            case 3:
                //turn standby led on 
            break;
            // pressed active
            case 4:
                //turn active led on 
            break;
            //pressed transmit
            case 5:
                //turn on transmit led 
            break;
            //pressed fail 
            case 6:
                //turn on fail led 
            break;
            //Pressed all lit
            case 7:
                //turn all leds on 
            break;
            default:
               //default off
            break;
        }
        
=======
    // Use the data
    if (bms.isMasterCaution()) {
      digitalWrite(13, HIGH);  // Turn on LED
>>>>>>> Stashed changes
    }

    Serial.print("Fuel Flow: ");
    Serial.println(bms.fuelFlow);
  }
}
```

### I2C Master-Slave System

**Master Arduino (connects to PC):**
```cpp
#include <FalconBMSArduinoConnector.h>
#include <FBAC_I2C_Master.h>

FalconBMSArduinoConnector bms;
FBAC_I2C_Master i2cMaster(&bms);

void setup() {
  Serial.begin(115200);
  bms.begin(Serial);
  i2cMaster.begin();

  // Register slave devices
  i2cMaster.registerSlave(0x10);  // Caution panel
  i2cMaster.registerSlave(0x11);  // Engine display
}

void loop() {
  bms.update();
  i2cMaster.update();

  if (bms.isConnected()) {
    // Send data to slaves
    i2cMaster.sendLightBits(0x10, 1);      // Warning lights
    i2cMaster.sendEngineData(0x11, I2C_CMD_RPM);  // Engine data
  }
}
```

**Slave Arduino (receives specific data):**
```cpp
#include <FBAC_I2C_Slave.h>

FBAC_I2C_Slave slave(0x10);  // I2C address

void setup() {
  Serial.begin(115200);
  slave.begin();
}

void loop() {
  slave.update();

  if (slave.isConnected()) {
    // Use EXACT same syntax as original BMS library!
    if (slave.isMasterCaution()) {
      digitalWrite(13, HIGH);
    }

    float fuel = slave.fuelFlow;  // Same variable names
    Serial.println(slave.dedLines[0]);  // Same arrays
  }
}
```

## 📡 Communication Protocol

### Serial Protocol (PC ↔ Master Arduino)

**Request Format:**
```
PC → Arduino: [Command Byte]
```

**Response Format:**
```
Arduino ← PC: [0xAA][Type][Length][Data...][Checksum]
```

### I2C Protocol (Master ↔ Slave Arduinos)

**Packet Format:**
```
Master → Slave: [Command][Length][Data...][Checksum]
```

**Data Types:**
- **Light Bits**: 32-bit warning light states
- **Analog Data**: 32-bit float values (fuel, RPM, etc.)
- **Text Data**: Character arrays for displays
- **Status Data**: Connection and system status

## 🎮 Available Data

### Warning Lights (100+ functions)
```cpp
// LightBits 1 - Primary warning lights
bms.isMasterCaution()    // Master caution light
bms.isEngFire()          // Engine fire warning
bms.isConfig()           // Configuration warning
bms.isFuelLow()          // Low fuel warning
bms.isAvionics()         // Avionics fault
// ... 30+ more functions

// LightBits 2 - Secondary systems
bms.isEPUOn()            // EPU active
bms.isChaffLo()          // Chaff low
bms.isFlareLo()          // Flare low
// ... 30+ more functions

// LightBits 3 - Extended systems
bms.isMainGen()          // Main generator
bms.isBatFail()          // Battery failure
bms.isNoseGearDown()     // Landing gear
// ... 30+ more functions

// BlinkBits - Blinking lights
bms.isEPUOnBlinking()    // EPU blinking
bms.isJFSOnSlowBlinking() // JFS slow blink
// ... 15+ more functions
```

### Analog Data
```cpp
// Fuel system
float fuelFlow;          // Current fuel flow (PPH)
float internalFuel;      // Internal fuel quantity (lbs)
float externalFuel;      // External fuel quantity (lbs)
float totalFuel;         // Total fuel (lbs)

// Engine parameters
float rpm;               // Engine RPM (%)
float oilPress;          // Oil pressure (PSI)
float ftit;              // Fan Turbine Inlet Temperature
float nozzlePos;         // Nozzle position

// Flight data
float kias;              // Knots Indicated Airspeed
float cabinAlt;          // Cabin altitude (ft)
float speedBrake;        // Speed brake position

// Navigation
float desiredCourse;     // Desired course (degrees)
float courseDeviation;   // Course deviation
float distanceToBearing; // Distance to beacon
```

### Text Displays
```cpp
char dedLines[5][27];    // Data Entry Display (5 lines, 26 chars each)
char pflLines[5][27];    // Pilot Fault List (5 lines, 26 chars each)
```

### Digital Data
```cpp
int uhfPreset;           // UHF radio preset
long uhfFreq;            // UHF frequency
int cmdsMode;            // CMDS mode (0-5)
int IFFMode1Digit1;      // IFF transponder digits
uint32_t ecm[4];         // ECM panel states
```

## 🔧 Command Reference

### Serial Commands (Master Arduino)
| Command | Data Type | Description |
|---------|-----------|-------------|
| `0x01` | LightBits | Main warning lights |
| `0x02` | LightBits2 | Additional warnings |
| `0x03` | LightBits3 | Extended warnings |
| `0x04` | BlinkBits | Blinking light states |
| `0x05` | DED | Data Entry Display |
| `0x06` | Float | Fuel flow rate |
| `0x08` | PFL | Pilot Fault List |
| `0x12` | Float | Engine RPM |
| `0x21` | Float | Indicated airspeed |
| `0x22-0x24` | Float | Fuel quantities |
| ... | ... | 40+ total commands |

### I2C Commands (Master → Slave)
| Command | Description | Data Size |
|---------|-------------|-----------|
| `I2C_CMD_LIGHTBITS_1` | Warning lights set 1 | 4 bytes |
| `I2C_CMD_FUEL_FLOW` | Current fuel flow | 4 bytes |
| `I2C_CMD_RPM` | Engine RPM | 4 bytes |
| `I2C_CMD_DED_LINES` | Display text | 27 bytes |
| `I2C_CMD_CONNECTION_STATUS` | Master status | 1 byte |
| `I2C_CMD_HEARTBEAT` | Keep-alive | 0 bytes |

## 🏗️ I2C Master-Slave Architecture

### System Overview
```
PC/BMS App ──Serial──> Master Arduino ──I2C──> Slave Arduino(s)
                           │                        │
                           │                        ├─> LEDs/Displays
                           │                        ├─> Buttons/Controls
                           │                        └─> Local Processing
                           │
                           └─> Manages up to 16 slaves
```

### Network Configuration

**Recommended I2C Addresses:**
- `0x10-0x1F`: Caution/warning panels
- `0x20-0x2F`: Engine displays
- `0x30-0x3F`: Navigation displays
- `0x40-0x4F`: Fuel/systems displays
- `0x50-0x5F`: Communication displays

**Hardware Requirements:**
- Pull-up resistors (4.7kΩ) on SDA/SCL lines
- Short cable runs (< 1m recommended)
- Common ground between all devices
- Adequate power supply for all devices

**Performance:**
- I2C Speed: 400kHz (fast mode)
- Update Rate: 20Hz (configurable)
- Max Slaves: 16 devices
- Response Time: < 5ms

## 🔄 Migration Guide

### Converting Existing Code to I2C System

**Step 1: Identify Your Architecture**

*Single Arduino (current):*
```
PC ──Serial──> Arduino ──> Displays/LEDs
```

*Distributed I2C (new):*
```
PC ──Serial──> Master Arduino ──I2C──> Multiple Slave Arduinos
```

**Step 2: Convert Master Device**

```cpp
// OLD: Direct connection
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;

void setup() {
  Serial.begin(115200);
  bms.begin(Serial);
}

void loop() {
  bms.update();

  if (bms.isConnected()) {
    bms.getLightBits(1);
    // Use data locally
  }
}
```

```cpp
// NEW: Master device
#include <FalconBMSArduinoConnector.h>
#include <FBAC_I2C_Master.h>

FalconBMSArduinoConnector bms;
FBAC_I2C_Master i2cMaster(&bms);

void setup() {
  Serial.begin(115200);
  bms.begin(Serial);
  i2cMaster.begin();
  i2cMaster.registerSlave(0x10);
}

void loop() {
  bms.update();
  i2cMaster.update();

  if (bms.isConnected()) {
    // Send data to slaves instead of using locally
    i2cMaster.sendLightBits(0x10, 1);
  }
}
```

**Step 3: Convert Slave Devices**

```cpp
// OLD: Part of main loop
if (bms.isConnected()) {
  if (bms.isMasterCaution()) {
    digitalWrite(13, HIGH);
  }

  float fuel = bms.fuelFlow;
  updateDisplay(fuel);
}
```

```cpp
// NEW: Slave device (EXACT same syntax!)
#include <FBAC_I2C_Slave.h>

FBAC_I2C_Slave slave(0x10);

void setup() {
  slave.begin();
}

void loop() {
  slave.update();

  if (slave.isConnected()) {    // Same function!
    if (slave.isMasterCaution()) {  // Same function!
      digitalWrite(13, HIGH);
    }

    float fuel = slave.fuelFlow;    // Same variable!
    updateDisplay(fuel);            // Same usage!
  }
}
```

**Key Changes:**
1. Replace `FalconBMSArduinoConnector` with `FBAC_I2C_Slave`
2. Replace `bms` with `slave`
3. Remove data request calls (`getLightBits()`, etc.)
4. Everything else stays exactly the same!

## 📋 Examples

### Complete Examples Included

1. **FBAC_All.ino** - Complete single-Arduino implementation
2. **I2C_Master_Example.ino** - Master device distributing data
3. **I2C_Slave_CautionPanel.ino** - Caution panel with FastLED
4. **I2C_Slave_FuelDisplay.ino** - Fuel display with OLED
5. **I2C_Compatibility_Demo.ino** - Shows API compatibility

### Hardware Examples

**Caution Panel (32 LEDs):**
```cpp
#define USE_FASTLED
#include <FBAC_I2C_Slave.h>
#include <FastLED.h>

FBAC_I2C_Slave slave(0x10);
CRGB leds[32];

void setup() {
  FastLED.addLeds<WS2812, 3, RGB>(leds, 32);
  slave.begin();
}

void loop() {
  slave.update();

  if (slave.isConnected()) {
    leds[0] = slave.isMasterCaution() ? CRGB::Red : CRGB::Black;
    leds[1] = slave.isEngFire() ? CRGB::Orange : CRGB::Black;
    // ... map all 32 lights
    FastLED.show();
  }
}
```

**Engine Display (OLED):**
```cpp
#include <FBAC_I2C_Slave.h>
#include <U8g2lib.h>

FBAC_I2C_Slave slave(0x11);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0);

void setup() {
  display.begin();
  slave.begin();
}

void loop() {
  slave.update();

  if (slave.isConnected()) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB12_tr);

    // Display RPM
    display.setCursor(10, 20);
    display.print("RPM: ");
    display.print(slave.rpm);
    display.print("%");

    // Display oil pressure
    display.setCursor(10, 40);
    display.print("Oil: ");
    display.print(slave.oilPress);
    display.print(" PSI");

    display.sendBuffer();
  }
}
```

## 🛠️ Troubleshooting

### Common Issues

**Connection Problems:**
- Verify baud rate (115200)
- Check serial cable connections
- Ensure BMS host is sending data
- Monitor `lastSerialActivity` for timeouts

**I2C Issues:**
- Check pull-up resistors (4.7kΩ) on SDA/SCL
- Verify unique slave addresses
- Keep cable runs short (< 1m)
- Check common ground connections

**Data Issues:**
- Validate checksums in packet parsing
- Monitor error codes from `getLastError()`
- Check data types match expected formats
- Ensure adequate power supply

### Debug Output

**Enable Debug Mode:**
```cpp
// Only use on slave devices (master serial is reserved for BMS)
void printDebugInfo() {
  Serial.print("Connected: ");
  Serial.println(slave.isConnected());
  Serial.print("Last Heartbeat: ");
  Serial.println(slave.getLastHeartbeat());
  Serial.print("Error Code: ");
  Serial.println(slave.getLastError());
}
```

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Development Setup

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test with both single and I2C configurations
5. Submit a pull request

### Areas for Contribution

- Additional display driver support
- More example implementations
- Performance optimizations
- Documentation improvements
- Bug fixes and testing

## 🙏 Acknowledgments

- **Falcon BMS Team** - For the incredible flight simulator
- **Arduino Community** - For hardware and software support
- **Contributors** - Everyone who has helped improve this library

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/Bacon8tor/FalconBMSArduinoConnector-Arduino/issues)
- **Discussions**: [GitHub Discussions](https://github.com/Bacon8tor/FalconBMSArduinoConnector-Arduino/discussions)
- **Documentation**: See [claude.md](claude.md) for detailed technical reference

---

**Happy Flying!** 🛩️

<<<<<<< Updated upstream
* Call `bms.update()` inside `loop()` to verify connection status.
* In your loop you will wan tto check `bms.isConnected()` this will return `true` or `false`, after this evulates true we start sending commands.
* Data Call: Make a data call like `bms.checkAllLights();` or `bms.getDED()` this is spilt up, to not call unneccessary data, call what you need. 
* Ensure your PC-side application is sending compatible packets


## Credits

Created by [Bacon8tor](https://github.com/Bacon8tor)

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://buymeacoffee.com/bacon8tor)

Contributions welcome! Feel free to fork and submit PRs or issues.
=======
*Build realistic F-16 cockpits with Arduino and Falcon BMS*
>>>>>>> Stashed changes
