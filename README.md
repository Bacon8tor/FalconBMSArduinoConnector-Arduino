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
### In3d

```cpp
bool isIn3D();
```

## How It Works

This library listens for incoming serial data in the Falcon BMS shared memory format. A corresponding PC app should send LightBits/DED/FlightData packets over serial. The Arduino then parses and stores relevant data in memory-accessible variables and methods.

* Call `bms.update()` inside `loop()` to verify connection status.
* In your loop you will wan tto check `bms.isConnected()` this will return `true` or `false`, after this evulates true we start sending commands.
* Data Call: Make a data call like `bms.checkAllLights();` or `bms.getDED()` this is spilt up, to not call unneccessary data, call what you need. 
* Ensure your PC-side application is sending compatible packets

## Serial Protocol (v2)

115200 baud by default, 8N1, no flow control. The Arduino asks and the PC answers.

* **Handshake:** the PC sends `0xA5` every 250 ms (for up to 4 s) until the Arduino replies `0x5A`. The retries cover boards that reset when the port opens (Uno/Nano).
* **Request:** the Arduino sends a single command byte (table below).
* **Response:** the PC sends `[0xAA][type][len][data...][checksum]`. `type` equals the command, multi-byte values are little-endian, and `checksum = (type + len + sum(data)) & 0xFF`.
* **Unknown command:** the PC replies with type `0x00`.
* **No BMS data:** the PC still replies, with the correct type and length and all bytes zero.
* **Version check:** after the handshake the library sends `0x43` once. An older PC app answers `0x00`, so the library falls back to protocol v1 and requests lights one word at a time.
* **Timeouts:** each request waits up to 250 ms (`bms.setRequestTimeout(ms)` to change). After 5 failed requests in a row, `isConnected()` turns false. The library then pings (`0x0F`) once a second to reconnect, and also answers a new handshake.
* Don't `Serial.print()` debug output on the port the PC app uses. The PC reads those bytes as commands.

| Cmd  | Description                       | Arduino field                    | Payload               |
| ---- | --------------------------------- | -------------------------------- | --------------------- |
| 0x01 | LightBits                         | `isMasterCaution()` …            | `uint32_t`            |
| 0x02 | LightBits2                        | `isHandOff()` …                  | `uint32_t`            |
| 0x03 | LightBits3                        | `isFlcsPmg()` …                  | `uint32_t`            |
| 0x04 | BlinkBits                         | `is…Blinking()`                  | `uint32_t`            |
| 0x05 | DED Lines (normalized)            | `dedLines[5]`                    | 5 × 24 chars          |
| 0x06 | Fuel Flow                         | `fuelFlow`                       | `float`               |
| 0x07 | Instrument Light Level            | `getInstrLightStatus()`          | `uint8_t`             |
| 0x08 | PFL Lines (normalized)            | `pflLines[5]`                    | 5 × 24 chars          |
| 0x09 | Chaff Count                       | `chaffCount`                     | `float`               |
| 0x0F | Ping                              | –                                | `0xAB`                |
| 0x10 | Flare Count                       | `flareCount`                     | `float`               |
| 0x11 | Flood Console Brightness          | `getFloodConsoleStatus()`        | `uint8_t` (0-6)       |
| 0x12 | RPM                               | `rpm`                            | `float`               |
| 0x13 | ECM Bits                          | `ecm[5]`, `getECMStatus(i)`      | 5 × `uint32_t`        |
| 0x14 | Oil Pressure 1                    | `oilPress`                       | `float`               |
| 0x15 | Oil Pressure 2                    | `oilPress2`                      | `float`               |
| 0x16 | Nozzle Position 1                 | `nozzlePos`                      | `float`               |
| 0x17 | Nozzle Position 2                 | `nozzlePos2`                     | `float`               |
| 0x18 | FTIT 1                            | `ftit`                           | `float`               |
| 0x19 | FTIT 2                            | `ftit2`                          | `float`               |
| 0x20 | Cabin Altitude                    | `cabinAlt`                       | `float`               |
| 0x21 | KIAS (Speed)                      | `kias`                           | `float`               |
| 0x22 | Internal Fuel                     | `internalFuel`                   | `float`               |
| 0x23 | External Fuel                     | `externalFuel`                   | `float`               |
| 0x24 | EPU Fuel                          | `epuFuel`                        | `float`               |
| 0x25 | Hydraulic Pressure A              | `hydPressA`                      | `float`               |
| 0x26 | Hydraulic Pressure B              | `hydPressB`                      | `float`               |
| 0x27 | CMDS Mode                         | `cmdsMode`                       | `int32_t`             |
| 0x28 | UHF Preset                        | `uhfPreset`                      | `int32_t`             |
| 0x29 | UHF Frequency                     | `uhfFreq`                        | `int32_t`             |
| 0x30 | Speed Brake                       | `speedBrake`                     | `float`               |
| 0x31 | IFF Mode 1 Digit 1                | `IFFMode1Digit1`                 | `uint8_t`             |
| 0x32 | IFF Mode 1 Digit 2                | `IFFMode1Digit2`                 | `uint8_t`             |
| 0x33 | IFF Mode 3A Digit 1               | `IFFMode3Digit1`                 | `uint8_t`             |
| 0x34 | IFF Mode 3A Digit 2               | `IFFMode3Digit2`                 | `uint8_t`             |
| 0x35 | Fuel FWD                          | `fwd`                            | `float`               |
| 0x36 | Fuel AFT                          | `aft`                            | `float`               |
| 0x37 | Fuel Total                        | `totalFuel`                      | `float`               |
| 0x38 | Desired Course                    | `desiredCourse`                  | `float`               |
| 0x39 | Course Deviation                  | `courseDeviation`                | `float`               |
| 0x40 | Distance To Beacon                | `distanceToBearing`              | `float`               |
| 0x41 | Bearing To Beacon                 | `bearingToBearing`               | `float`               |
| 0x42 | Sim Status (bit 0 = in 3D)        | `simStatus`, `isIn3D()`          | `uint32_t`            |
| 0x43 | Protocol Version *(v2)*           | `protocolVersion`                | `[version, flags]`    |
| 0x44 | All Lights *(v2)*                 | used by `checkAllLights()`       | 4 × `uint32_t` (LightBits, LightBits2, LightBits3, BlinkBits) + 5 × `uint32_t` ECM |

The command IDs are defined in `FBAC_Commands.h` (Arduino) and in the `FbacCommand` enum in `ArduinoConnector.cs` (PC). Keep both in sync.

## Credits

Created by [Bacon8tor](https://github.com/Bacon8tor)

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://buymeacoffee.com/bacon8tor)

Contributions welcome! Feel free to fork and submit PRs or issues.
