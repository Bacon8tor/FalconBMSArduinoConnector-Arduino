/*
  Falcon BMS Arduino Connector - I2C Slave Fuel Display Example

  This example demonstrates how to use an Arduino as an I2C slave device
  that receives fuel data from the master and displays it on OLED screens.

  Hardware Setup:
  - Connect to I2C master via SDA/SCL pins
  - Connect OLED displays for fuel flow, internal/external fuel
  - Pull-up resistors (4.7kΩ) on SDA/SCL lines

  I2C Address: 0x12

  Display Setup:
  - SSD1306 128x64 OLED for fuel flow
  - Additional displays for fuel quantities

  Created by: Bacon8tor
  Date: 2024
*/

#include <FBAC_I2C_Slave.h>
#include <U8g2lib.h>
#include <Wire.h>

// I2C slave address
#define SLAVE_ADDRESS 0x12

// Create I2C slave instance
FBAC_I2C_Slave slave(SLAVE_ADDRESS);

// OLED display setup (adjust pins for your hardware)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_fuel(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Fuel display variables
float fuelFlow = 0.0;
float fuelInternal = 0.0;
float fuelExternal = 0.0;
float fuelTotal = 0.0;
float fuelFwd = 0.0;
float fuelAft = 0.0;
float chaffCount = 0.0;
float flareCount = 0.0;

// Display update tracking
unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_UPDATE_INTERVAL = 100; // 10Hz display refresh

// Status tracking
bool masterConnected = false;
bool dataValid = false;

void setup() {
  Serial.begin(115200);

  // Initialize I2C slave
  slave.begin();

  // Set up callbacks
  slave.setFloatDataCallback(onFloatDataReceived);
  slave.setConnectionStatusCallback(onConnectionStatusChanged);
  slave.setDataReceivedCallback(onDataReceived);

  // Initialize OLED display
  u8g2_fuel.begin();
  u8g2_fuel.setFont(u8g2_font_ncenB08_tr);

  Serial.println("Fuel Display Slave Started");
  Serial.print("I2C Address: 0x");
  Serial.println(SLAVE_ADDRESS, HEX);

  // Show startup screen
  showStartupScreen();
}

void loop() {
  slave.update();

  // Check connection status
  if (slave.isConnected() != masterConnected) {
    masterConnected = slave.isConnected();
    Serial.print("Master connection: ");
    Serial.println(masterConnected ? "CONNECTED" : "DISCONNECTED");

    if (!masterConnected) {
      dataValid = false;
    }
  }

  // Update display
  unsigned long now = millis();
  if (now - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    lastDisplayUpdate = now;
    updateDisplay();
  }

  // Handle serial commands for testing
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 's':
        printStatus();
        break;
      case 't':
        testDisplay();
        break;
    }
  }

  delay(10);
}

// Callback functions
void onFloatDataReceived(uint8_t command, float value) {
  dataValid = true;

  // Now you can access data in TWO ways:
  // Method 1: Use the callback parameters (old way)
  // Method 2: Access slave variables directly like BMS library (NEW!)

  switch (command) {
    case I2C_CMD_FUEL_FLOW:
      Serial.print("Fuel Flow: ");
      Serial.print(value);
      Serial.print(" (also available as slave.fuelFlow = ");
      Serial.print(slave.fuelFlow);  // BMS-style access!
      Serial.println(")");
      break;

    case I2C_CMD_FUEL_INTERNAL:
      Serial.print("Internal Fuel: ");
      Serial.print(value);
      Serial.print(" (also slave.internalFuel = ");
      Serial.print(slave.internalFuel);  // BMS-style access!
      Serial.println(")");
      break;

    case I2C_CMD_FUEL_EXTERNAL:
      Serial.print("External Fuel: ");
      Serial.println(slave.externalFuel);  // Direct BMS-style access!
      break;

    case I2C_CMD_FUEL_TOTAL:
      Serial.print("Total Fuel: ");
      Serial.println(slave.totalFuel);  // Direct BMS-style access!
      break;

    case I2C_CMD_FUEL_FWD:
      Serial.print("Forward Fuel: ");
      Serial.println(slave.fwd);  // Same variable name as BMS library!
      break;

    case I2C_CMD_FUEL_AFT:
      Serial.print("Aft Fuel: ");
      Serial.println(slave.aft);  // Same variable name as BMS library!
      break;

    case I2C_CMD_CHAFF_COUNT:
      Serial.print("Chaff Count: ");
      Serial.println(slave.chaffCount);  // Direct access!
      break;

    case I2C_CMD_FLARE_COUNT:
      Serial.print("Flare Count: ");
      Serial.println(slave.flareCount);  // Direct access!
      break;
  }
}

void onConnectionStatusChanged(bool connected) {
  Serial.print("Connection status changed: ");
  Serial.println(connected ? "CONNECTED" : "DISCONNECTED");
}

void onDataReceived(uint8_t command, uint8_t* data, uint8_t length) {
  // Debug output for all received data
  /*
  Serial.print("Received command 0x");
  Serial.print(command, HEX);
  Serial.print(" with ");
  Serial.print(length);
  Serial.println(" bytes");
  */
}

void updateDisplay() {
  u8g2_fuel.clearBuffer();

  if (!masterConnected) {
    // Show disconnected status
    u8g2_fuel.setFont(u8g2_font_ncenB10_tr);
    u8g2_fuel.drawStr(5, 20, "NO CONNECTION");
    u8g2_fuel.setFont(u8g2_font_ncenB08_tr);
    u8g2_fuel.drawStr(20, 40, "Waiting for");
    u8g2_fuel.drawStr(30, 55, "Master...");

  } else if (!dataValid) {
    // Show waiting for data
    u8g2_fuel.setFont(u8g2_font_ncenB10_tr);
    u8g2_fuel.drawStr(15, 20, "CONNECTED");
    u8g2_fuel.setFont(u8g2_font_ncenB08_tr);
    u8g2_fuel.drawStr(20, 40, "Waiting for");
    u8g2_fuel.drawStr(35, 55, "Data...");

  } else {
    // Show fuel data
    u8g2_fuel.setFont(u8g2_font_ncenB08_tr);

    // Title
    u8g2_fuel.drawStr(35, 10, "FUEL DATA");

    // NOW USE BMS-STYLE VARIABLE ACCESS - EXACT SAME AS ORIGINAL LIBRARY!

    // Fuel Flow (large display) - Use slave.fuelFlow just like bms.fuelFlow
    u8g2_fuel.setFont(u8g2_font_ncenB12_tr);
    char flowStr[10];
    dtostrf(slave.fuelFlow, 6, 0, flowStr);  // BMS-style access!
    u8g2_fuel.drawStr(25, 28, flowStr);
    u8g2_fuel.setFont(u8g2_font_ncenB08_tr);
    u8g2_fuel.drawStr(85, 28, "PPH");

    // Fuel quantities (smaller text) - Same variable names as BMS library
    char buffer[20];

    // Internal fuel
    sprintf(buffer, "INT: %.0f", slave.internalFuel);  // BMS-style!
    u8g2_fuel.drawStr(5, 42, buffer);

    // External fuel
    sprintf(buffer, "EXT: %.0f", slave.externalFuel);  // BMS-style!
    u8g2_fuel.drawStr(70, 42, buffer);

    // Total fuel
    sprintf(buffer, "TOT: %.0f", slave.totalFuel);  // BMS-style!
    u8g2_fuel.drawStr(5, 55, buffer);

    // Countermeasures
    sprintf(buffer, "C:%02.0f F:%02.0f", slave.chaffCount, slave.flareCount);  // BMS-style!
    u8g2_fuel.drawStr(65, 55, buffer);

    // Connection indicator
    u8g2_fuel.drawPixel(125, 2);
    u8g2_fuel.drawPixel(126, 2);
    u8g2_fuel.drawPixel(127, 2);
  }

  u8g2_fuel.sendBuffer();
}

void showStartupScreen() {
  u8g2_fuel.clearBuffer();
  u8g2_fuel.setFont(u8g2_font_ncenB10_tr);
  u8g2_fuel.drawStr(20, 20, "FUEL DISPLAY");
  u8g2_fuel.setFont(u8g2_font_ncenB08_tr);
  u8g2_fuel.drawStr(30, 35, "Starting...");

  char addrStr[10];
  sprintf(addrStr, "I2C: 0x%02X", SLAVE_ADDRESS);
  u8g2_fuel.drawStr(30, 50, addrStr);

  u8g2_fuel.sendBuffer();
  delay(2000);
}

void testDisplay() {
  Serial.println("Running display test...");

  // Test with fake data
  fuelFlow = 12500.0;
  fuelInternal = 5500.0;
  fuelExternal = 2200.0;
  fuelTotal = 7700.0;
  fuelFwd = 3000.0;
  fuelAft = 2500.0;
  chaffCount = 60.0;
  flareCount = 30.0;

  dataValid = true;
  masterConnected = true;

  updateDisplay();

  delay(3000);

  // Reset to actual values
  SlaveData* data = slave.getData();
  fuelFlow = data->fuelFlow;
  fuelInternal = data->fuelInternal;
  fuelExternal = data->fuelExternal;
  fuelTotal = data->fuelTotal;
  fuelFwd = data->fuelFwd;
  fuelAft = data->fuelAft;
  chaffCount = data->chaffCount;
  flareCount = data->flareCount;

  Serial.println("Display test complete");
}

void printStatus() {
  Serial.println("=== Fuel Display Status ===");
  Serial.print("I2C Address: 0x");
  Serial.println(SLAVE_ADDRESS, HEX);
  Serial.print("Master Connected: ");
  Serial.println(slave.isConnected() ? "YES" : "NO");
  Serial.print("Data Valid: ");
  Serial.println(dataValid ? "YES" : "NO");
  Serial.print("Last Heartbeat: ");
  Serial.print(slave.getLastHeartbeat());
  Serial.println(" ms");

  Serial.println("Current Fuel Data:");
  Serial.print("  Fuel Flow: ");
  Serial.print(fuelFlow);
  Serial.println(" PPH");
  Serial.print("  Internal: ");
  Serial.print(fuelInternal);
  Serial.println(" lbs");
  Serial.print("  External: ");
  Serial.print(fuelExternal);
  Serial.println(" lbs");
  Serial.print("  Total: ");
  Serial.print(fuelTotal);
  Serial.println(" lbs");
  Serial.print("  Forward: ");
  Serial.print(fuelFwd);
  Serial.println(" lbs");
  Serial.print("  Aft: ");
  Serial.print(fuelAft);
  Serial.println(" lbs");
  Serial.print("  Chaff: ");
  Serial.println((int)chaffCount);
  Serial.print("  Flare: ");
  Serial.println((int)flareCount);

  Serial.println("Commands: 's'=status, 't'=test");
  Serial.println("========================");
}