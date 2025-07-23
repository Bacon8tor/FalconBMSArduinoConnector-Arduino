# FalconBMSArduinoConnector (Arduino Library)

**FalconBMSArduinoConnector** is an Arduino library that allows you to connect your microcontroller to a PC running Falcon BMS via a serial interface. It provides easy access to cockpit data such as LightBits, LightBits2, LightBits3, DED lines and other Flight Data.

This library is part of a larger ecosystem allowing home cockpit builders and simulation enthusiasts to bring Falcon BMS data into real-world hardware like LEDs, displays, or gauges.

## Features

* Serial communication with Falcon BMS via custom PC tool
* Read status of LightBits, LightBits2, and LightBits3 (e.g., Master Caution, Fuel Low, etc.)
* Retrieve DED lines (Data Entry Display)
* Connection status detection

## Installation


### Platformio (RECOMMENDED)
1. Download or clone this repository [FBAC_Example](https://github.com/Bacon8tor/FBAC_PIO_Example)
2. Copy or Open one of the included example sketches

### Arduino IDE or Arduino Cloud
1. Download the latest release 
2. Upload to IDE 
3. Create Sketch or load example


```cpp
#include <FalconBMSArduinoConnector.h>
```

## Usage

### Initialization

```cpp
FalconBMSArduinoConnector bms;

void setup() {
  bms.begin(); //Starts the Serial Connection
}

void loop() {
  bms.update(); //Updates BMS Data and Checks Serial Connection
  // Your code here
}
```

### Check Connection

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
bms.checkLightbits(1); // checks Lightbits
if (bms.isMasterCaution()) {
  digitalWrite(ledPin, HIGH);
}
```

## Available Getter Functions

### LightBits

* `isMasterCaution()`
* `isTF()`
* `isOxyBrow()`
* `isEquipHot()`
* `isOnGround()`
* `isEngFire()`
* `isConfig()`
* `isHyd()`
* `isFlcsABCD()`
* `isFlcs()`
* `isCan()`
* `isTLConfig()`
* `isAOAAbove()`
* `isAOAOn()`
* `isAOABelow()`
* `isRefuelRDY()`
* `isRefuelAR()`
* `isRefuelDSC()`
* `isFltControlSys()`
* `isLEFlaps()`
* `isEngineFault()`
* `isOverheat()`
* `isFuelLow()`
* `isAvionics()`
* `isRadarAlt()`
* `isIFF()`
* `isECM()`
* `isHook()`
* `isNWSFail()`
* `isCabinPress()`
* `isAutoPilotOn()`
* `isTFRSTBY()`

### LightBits2

* `isHandOff()`
* `isLaunch()`
* `isPriMode()`
* `isNaval()`
* `isUnk()`
* `isTgtSep()`
* `isGo()`
* `isNoGo()`
* `isDegr()`
* `isRdy()`
* `isChaffLo()`
* `isFlareLo()`
* `isAuxSrch()`
* `isAuxAct()`
* `isAuxLow()`
* `isAuxPwr()`
* `isEcmPwr()`
* `isEcmFail()`
* `isFwdFuelLow()`
* `isAftFuelLow()`
* `isEPUOn()`
* `isJFSOn()`
* `isSEC()`
* `isOxyLow()`
* `isProbeHeat()`
* `isSeatArm()`
* `isBUC()`
* `isFuelOilHot()`
* `isAntiSkid()`
* `isTFREngaged()`
* `isGearHandle()`
* `isEngine()`

### LightBits3

* `isFlcsPmg()`
* `isMainGen()`
* `isStbyGen()`
* `isEpuGen()`
* `isEpuPmg()`
* `isToFlcs()`
* `isFlcsRly()`
* `isBatFail()`
* `isHydrazine()`
* `isAir()`
* `isElecFault()`
* `isLefFault()`
* `isOnGround3()`
* `isFlcsBitRun()`
* `isFlcsBitFail()`
* `isDbuWarn()`
* `isNoseGearDown()`
* `isLeftGearDown()`
* `isRightGearDown()`
* `isParkBrakeOn()`
* `isPowerOff()`
* `isCadc()`
* `isSpeedBrake()`
* `isSysTest()`
* `isMCAnnounced()`
* `isMLGWOW()`
* `isNLGWOW()`
* `isATFNotEngaged()`
* `isInletIcing()`

## How It Works

This library listens for incoming serial data in the Falcon BMS shared memory format. A corresponding PC app should send LightBits/DED/FlightData packets over serial. The Arduino then parses and stores relevant data in memory-accessible variables and methods.

## Notes

* Call `update()` inside `loop()` to continuously parse data
* Ensure your PC-side application is sending compatible packets


## Credits

Created by [Bacon8tor](https://github.com/Bacon8tor)

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://buymeacoffee.com/bacon8tor)

Contributions welcome! Feel free to fork and submit PRs or issues.
