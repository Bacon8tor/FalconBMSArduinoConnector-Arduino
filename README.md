````markdown
# FalconBMSArduinoConnector

**FalconBMSArduinoConnector** is an Arduino/ESP32-compatible library designed to interface with Falcon BMS via serial communication. This library parses binary packets sent from a host PC running Falcon BMS and exposes simple functions to check the status of key cockpit indicators like Master Caution, Engine Fire, Fuel Low, and more.

> 💡 Works great with LED indicators, gauges, and custom cockpit panels!

---

## 📦 Features

- Easy-to-use API: query cockpit statuses like `isMasterCaution()`, `isFuelLow()`, etc.
- Compatible with:
  - Arduino Uno, Nano, Mega
  - Leonardo (USB CDC via Serial1)
  - ESP32 / ESP8266
  - Teensy boards
- Lightweight & efficient binary protocol
- Detects connection loss with timeout

---

## 🔧 Setup

### 1. Install

- Copy the `FalconBMSArduinoConnector` folder into your Arduino `libraries/` folder
- Or, in PlatformIO: place it in your `lib/` directory

### 2. Connect Serial

Ensure your PC application sends data over the serial port the Arduino is listening on (default: `Serial` at `115200` baud).

---

## 🚀 Usage

### Basic Example (PlatformIO / Arduino sketch)

```cpp
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;

void setup() {
  Serial.begin(115200);
  bms.begin(Serial); // or Serial1 for Leonardo/Teensy
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  bms.update();  // Call this in your main loop

  if (bms.isConnected()) {
    if (bms.isMasterCaution()) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
````

---

## ✅ Available API

Each function returns `true` or `false` based on the latest Falcon BMS data received.

```cpp
// General alerts
bms.isMasterCaution()
bms.isEngFire()
bms.isFuelLow()
bms.isOverheat()

// Systems
bms.isHyd()
bms.isFlcs()
bms.isFlcsABCD()
bms.isAvionics()
bms.isRadarAlt()
bms.isAutoPilotOn()

// Refueling
bms.isRefuelRDY()
bms.isRefuelAR()
bms.isRefuelDSC()

// Gear/flight
bms.isHook()
bms.isNWSFail()
bms.isCan()
bms.isTLConfig()

// AOA
bms.isAOAAbove()
bms.isAOAOn()
bms.isAOABelow()

// Other
bms.isIFF()
bms.isECM()
bms.isCabinPress()
bms.isFltControlSys()
bms.isLEFlaps()
bms.isOnGround()
```

---

## 📡 Protocol

* Packets sent from PC must begin with `0xAA`
* Format: `[type][len][payload][checksum]`
* Type `0x01`: LightBits (4 bytes)
* Additional types may include `lightBits2`, `blinkBits`, etc.
* Library handles checksum validation and parsing

---

## 📄 License

MIT License

---

## ✈️ Developed For

* Falcon BMS 4.37 / 4.38
* DIY cockpit builders
* USB-connected microcontrollers running physical displays or controls

