# FalconBMSArduinoConnector (Arduino Library)

**FalconBMSArduinoConnector** is an Arduino library that allows you to connect your microcontroller to a PC running Falcon BMS via a serial interface. It provides easy access to cockpit data such as LightBits, LightBits2, LightBits3, DED lines and other Flight Data.

This library is part of a larger ecosystem allowing home cockpit builders and simulation enthusiasts to bring Falcon BMS data into real-world hardware like LEDs, displays, or gauges.

## PC APP Release 
The PC Companion App's Latest Release can be found here. [Latest Release](https://github.com/Bacon8tor/FalconBMSArduinoConnector/releases)
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
Serial.begin(115200);
  bms.begin(Serial); //Starts the Serial Connection
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
bms.getDED();  // Request DED
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
        
    }
```

## Class API Reference


```cpp
void checkAllLights();
void getLightBits(int lb);
void getblinkBits();
void getDED();
void getFuelFlow();
void getPFL();
void getChaffFlareCount();
void getECMBits();
int getECMStatus(int ecmLight);
void getOilPressure();
void getOilPressure2();
void getNozzlePos();
void getNozzlePos2();
void getFTIT();
void getFTIT2();
void getCabinAlt();
void getKIAS();
void getinternalFuel();
void getexternalFuel();
void getEPUFuel();
void getHYDPress();
```


### DED and PFL

```cpp
char dedLines[5][27];
char pflLines[5][27];
```

### Fuel

```cpp
float fuelFlow;
float internalFuel;
float externalFuel;
float epuFuel;
```

### Chaff / Flare

```cpp
float chaffCount;
float flareCount;
```

### RPM

```cpp
float rpm;
```

### ECM

```cpp
uint32_t ecm[4];
```

### Oil Pressure

```cpp
float oilPress;
float oilPress2;
```

### Nozzle Position

```cpp
float nozzlePos;
float nozzlePos2;
```

### FTIT (Fan Turbine Inlet Temperature)

```cpp
float ftit;
float ftit2;
```

### Cabin Altitude

```cpp
float cabinAlt;
```

### Airspeed

```cpp
float kias;
```

### Hydraulic Pressure

```cpp
float hydPressA;
float hydPressB;
```

## How It Works

This library listens for incoming serial data in the Falcon BMS shared memory format. A corresponding PC app should send LightBits/DED/FlightData packets over serial. The Arduino then parses and stores relevant data in memory-accessible variables and methods.

* Call `bms.update()` inside `loop()` to verify connection status.
* In your loop you will wan tto check `bms.isConnected()` this will return `true` or `false`, after this evulates true we start sending commands.
* Data Call: Make a data call like `bms.checkAllLights();` or `bms.getDED()` this is spilt up, to not call unneccessary data, call what you need. 
* Ensure your PC-side application is sending compatible packets

## All Configured Variables

| Case | Description                       | Field Name              | Type                  |
| ---- | --------------------------------- | ----------------------- | --------------------- |
| 0x01 | LightBits                         | `lightBits`             | `uint32_t`            |
| 0x02 | LightBits2                        | `lightBits2`            | `uint32_t`            |
| 0x03 | LightBits3                        | `lightBits3`            | `uint32_t`            |
| 0x04 | BlinkBits                         | `blinkBits`             | `uint32_t`            |
| 0x05 | DED Lines (normalized)            | `DEDLines`, `Invert`    | `char[5][26]`, `bool` |
| 0x06 | Fuel Flow                         | `fuelFlow`              | `float`               |
| 0x07 | Instrument Light Level            | `instrLight`            | `uint8_t`             |
| 0x08 | PFL Lines (normalized)            | `PFLLines`, `PFLInvert` | `char[5][26]`, `bool` |
| 0x09 | Chaff Count                       | `ChaffCount`            | `float`               |
| 0x10 | Flare Count                       | `FlareCount`            | `float`               |
| 0x11 | Flood Console Brightness (unused) | *(hardcoded to 0x00)*   | `-`                   |
| 0x12 | RPM                               | `rpm`                   | `float`               |
| 0x13 | ECM Bits                          | `ecmBits[4]`            | `uint32_t[4]`         |
| 0x14 | Oil Pressure 1                    | `oilPressure`           | `float`               |
| 0x15 | Oil Pressure 2                    | `oilPressure2`          | `float`               |
| 0x16 | Nozzle Position 1                 | `nozzlePos`             | `float`               |
| 0x17 | Nozzle Position 2                 | `nozzlePos2`            | `float`               |
| 0x18 | FTIT 1                            | `ftit`                  | `float`               |
| 0x19 | FTIT 2                            | `ftit2`                 | `float`               |
| 0x20 | Cabin Altitude                    | `cabinAlt`              | `float`               |
| 0x21 | KIAS (Speed)                      | `kias`                  | `float`               |
| 0x22 | Internal Fuel                     | `internalFuel`          | `float`               |
| 0x23 | External Fuel                     | `externalFuel`          | `float`               |
| 0x24 | EPU Fuel                          | `epuFuel`               | `float`               |
| 0x25 | Hydraulic Pressure A              | `hydPressureA`          | `float`               |
| 0x26 | Hydraulic Pressure B              | `hydPressureB`          | `float`               |
| 0x27 | CMDS Mode                         | `cmdsMode`              | `int`                 |
| 0x28 | UHF Preset                        | `BupUhfPreset`          | `int`                 |
| 0x29 | UHF Frequency                     | `BupUhfFreq`            | `int` or `long`       |
| 0x30 | Speed Brake                     | `speedBrake`            | `float`       |

## Credits

Created by [Bacon8tor](https://github.com/Bacon8tor)

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://buymeacoffee.com/bacon8tor)

Contributions welcome! Feel free to fork and submit PRs or issues.
