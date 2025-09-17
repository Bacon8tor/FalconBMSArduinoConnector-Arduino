/*
  Falcon BMS Arduino Connector - I2C Slave Caution Panel Example

  This example demonstrates how to use an Arduino as an I2C slave device
  that receives caution light data from the master and controls LEDs.

  Hardware Setup:
  - Connect to I2C master via SDA/SCL pins
  - Connect caution lights to digital pins or use FastLED strip
  - Pull-up resistors (4.7kΩ) on SDA/SCL lines

  I2C Address: 0x10

  FastLED Setup (optional):
  - Data pin: 3
  - 32 LEDs for full caution panel

  Created by: Bacon8tor
  Date: 2024
*/

#include <FBAC_I2C_Slave.h>

// Uncomment to use FastLED for caution panel
#define USE_FASTLED
#ifdef USE_FASTLED
#include <FastLED.h>
#define NUM_LEDS 32
#define DATA_PIN 3
CRGB leds[NUM_LEDS];
#endif

// I2C slave address
#define SLAVE_ADDRESS 0x10

// Create I2C slave instance
FBAC_I2C_Slave slave(SLAVE_ADDRESS);

// Individual LED pins (if not using FastLED)
#ifndef USE_FASTLED
const int ledPins[] = {
  4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5
};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
#endif

// Status tracking
bool masterConnected = false;
unsigned long lastDataReceived = 0;
bool testMode = false;

void setup() {
  Serial.begin(115200);

  // Initialize I2C slave
  slave.begin();

  // Set up callbacks
  slave.setLightBitsCallback(onLightBitsReceived);
  slave.setConnectionStatusCallback(onConnectionStatusChanged);
  slave.setDataReceivedCallback(onDataReceived);

#ifdef USE_FASTLED
  // Initialize FastLED
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  FastLED.clear();
  FastLED.show();
#else
  // Initialize individual LED pins
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
#endif

  Serial.println("Caution Panel Slave Started");
  Serial.print("I2C Address: 0x");
  Serial.println(SLAVE_ADDRESS, HEX);

  // Run startup test sequence
  runStartupTest();
}

void loop() {
  slave.update();

  // Check connection status
  if (slave.isConnected() != masterConnected) {
    masterConnected = slave.isConnected();
    Serial.print("Master connection: ");
    Serial.println(masterConnected ? "CONNECTED" : "DISCONNECTED");

    if (!masterConnected) {
      // Clear all lights when disconnected
      clearAllLights();
    }
  }

  // Update caution panel based on received data
  if (masterConnected) {
    updateCautionPanel();
  } else {
    // Flash red LED to indicate no connection
    static unsigned long lastFlash = 0;
    static bool flashState = false;

    if (millis() - lastFlash > 500) {
      lastFlash = millis();
      flashState = !flashState;

#ifdef USE_FASTLED
      if (flashState) {
        leds[0] = CRGB::Red;
      } else {
        leds[0] = CRGB::Black;
      }
      FastLED.show();
#else
      if (numLeds > 0) {
        digitalWrite(ledPins[0], flashState);
      }
#endif
    }
  }

  // Handle serial commands for testing
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 't':
        runStartupTest();
        break;
      case 'c':
        clearAllLights();
        break;
      case 's':
        printStatus();
        break;
    }
  }

  delay(10);
}

// Callback functions
void onLightBitsReceived(uint8_t lightBitsType, uint32_t bits) {
  lastDataReceived = millis();

  Serial.print("Received LightBits");
  Serial.print(lightBitsType);
  Serial.print(": 0x");
  Serial.println(bits, HEX);
}

void onConnectionStatusChanged(bool connected) {
  Serial.print("Connection status changed: ");
  Serial.println(connected ? "CONNECTED" : "DISCONNECTED");
}

void onDataReceived(uint8_t command, uint8_t* data, uint8_t length) {
  Serial.print("Received command 0x");
  Serial.print(command, HEX);
  Serial.print(" with ");
  Serial.print(length);
  Serial.println(" bytes");
}

void updateCautionPanel() {
  // NOW YOU CAN USE EXACT SAME SYNTAX AS ORIGINAL BMS LIBRARY!
  // Just replace "bms" with "slave" and everything else works the same

#ifdef USE_FASTLED
  // Map light bits to LED positions
  // This mapping should match your physical caution panel layout

  // Method 1: Use BMS-style function calls (RECOMMENDED)
  leds[0] = slave.isMasterCaution() ? CRGB::Red : CRGB::Black;
  leds[1] = slave.isFltControlSys() ? CRGB::Orange : CRGB::Black;
  leds[2] = slave.isEquipHot() ? CRGB::Orange : CRGB::Black;
  leds[3] = slave.isConfig() ? CRGB::Orange : CRGB::Black;
  leds[4] = slave.isEngineFault() ? CRGB::Orange : CRGB::Black;
  leds[5] = slave.isOverheat() ? CRGB::Orange : CRGB::Black;
  leds[6] = slave.isFuelLow() ? CRGB::Orange : CRGB::Black;
  leds[7] = slave.isAvionics() ? CRGB::Orange : CRGB::Black;
  leds[8] = slave.isRadarAlt() ? CRGB::Orange : CRGB::Black;
  leds[9] = slave.isIFF() ? CRGB::Orange : CRGB::Black;
  leds[10] = slave.isECM() ? CRGB::Orange : CRGB::Black;
  leds[11] = slave.isHook() ? CRGB::Orange : CRGB::Black;
  leds[12] = slave.isNWSFail() ? CRGB::Orange : CRGB::Black;
  leds[13] = slave.isCabinPress() ? CRGB::Orange : CRGB::Black;

  // LightBits 2 - Use exact same function names as original BMS library
  leds[14] = slave.isFwdFuelLow() ? CRGB::Orange : CRGB::Black;
  leds[15] = slave.isAftFuelLow() ? CRGB::Orange : CRGB::Black;
  leds[16] = slave.isOxyLow() ? CRGB::Orange : CRGB::Black;
  leds[17] = slave.isSeatArm() ? CRGB::Orange : CRGB::Black;
  leds[18] = slave.isBUC() ? CRGB::Orange : CRGB::Black;
  leds[19] = slave.isFuelOilHot() ? CRGB::Orange : CRGB::Black;

  // LightBits 3 - More functions available
  leds[20] = slave.isMainGen() ? CRGB::Orange : CRGB::Black;
  leds[21] = slave.isStbyGen() ? CRGB::Orange : CRGB::Black;
  leds[22] = slave.isEpuGen() ? CRGB::Orange : CRGB::Black;
  leds[23] = slave.isBatFail() ? CRGB::Orange : CRGB::Black;

  // Method 2: You can still use the old way if needed
  // leds[0] = slave.getLightBit(1, 0) ? CRGB::Red : CRGB::Black;

    // Add more mappings for remaining lights...
    // LightBits 3 and BlinkBits can be mapped to remaining LEDs

    // Handle blinking lights
    static unsigned long lastBlink = 0;
    static bool blinkState = false;

    if (millis() - lastBlink > 500) { // 500ms blink cycle
      lastBlink = millis();
      blinkState = !blinkState;

      // Apply blinking to specific lights
      if (data->blinkBits & 0x01) { // Example: OuterMarker blinking
        if (blinkState) {
          leds[20] = CRGB::White;
        } else {
          leds[20] = CRGB::Black;
        }
      }
    }
  }

  FastLED.show();

#else
  // Individual LED control (simplified example)
  for (int i = 0; i < numLeds && i < 16; i++) {
    bool lightOn = false;

    if (i < 14) {
      lightOn = slave.getLightBit(1, i);
    } else {
      lightOn = slave.getLightBit(2, i - 14);
    }

    digitalWrite(ledPins[i], lightOn ? HIGH : LOW);
  }
#endif
}

void clearAllLights() {
#ifdef USE_FASTLED
  FastLED.clear();
  FastLED.show();
#else
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
#endif

  Serial.println("All lights cleared");
}

void runStartupTest() {
  Serial.println("Running startup test...");

#ifdef USE_FASTLED
  // Test sequence: Red -> Green -> Blue -> Orange -> Off
  CRGB testColors[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Orange};

  for (int color = 0; color < 4; color++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = testColors[color];
    }
    FastLED.show();
    delay(250);
  }

  FastLED.clear();
  FastLED.show();

#else
  // Simple on/off test for individual LEDs
  for (int cycle = 0; cycle < 3; cycle++) {
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    delay(250);

    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(250);
  }
#endif

  Serial.println("Startup test complete");
}

void printStatus() {
  Serial.println("=== Caution Panel Status ===");
  Serial.print("I2C Address: 0x");
  Serial.println(SLAVE_ADDRESS, HEX);
  Serial.print("Master Connected: ");
  Serial.println(slave.isConnected() ? "YES" : "NO");
  Serial.print("Last Heartbeat: ");
  Serial.print(slave.getLastHeartbeat());
  Serial.println(" ms");

  SlaveData* data = slave.getData();
  Serial.print("LightBits1: 0x");
  Serial.println(data->lightBits1, HEX);
  Serial.print("LightBits2: 0x");
  Serial.println(data->lightBits2, HEX);
  Serial.print("LightBits3: 0x");
  Serial.println(data->lightBits3, HEX);
  Serial.print("BlinkBits: 0x");
  Serial.println(data->blinkBits, HEX);

  Serial.println("Key Status:");
  Serial.print("  Master Caution: ");
  Serial.println(slave.isMasterCaution() ? "ON" : "OFF");
  Serial.print("  Engine Fire: ");
  Serial.println(slave.getLightBit(1, 5) ? "ON" : "OFF");
  Serial.print("  Config: ");
  Serial.println(slave.getLightBit(1, 6) ? "ON" : "OFF");

  Serial.println("Commands: 't'=test, 'c'=clear, 's'=status");
  Serial.println("========================");
}