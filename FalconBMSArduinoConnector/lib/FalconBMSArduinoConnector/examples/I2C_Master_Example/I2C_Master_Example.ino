/*
  Falcon BMS Arduino Connector - I2C Master Example

  This example demonstrates how to use an Arduino as an I2C master device
  that receives all BMS data via serial and distributes it to slave devices.

  IMPORTANT: This master device uses Serial for BMS communication only.
  No Serial.print() statements are used to avoid interfering with BMS data.
  Use slave devices for debugging output if needed.

  Hardware Setup:
  - Connect to Falcon BMS host via Serial (USB)
  - Connect I2C slaves via SDA/SCL pins
  - Pull-up resistors (4.7kΩ) on SDA/SCL lines

  I2C Addresses:
  - 0x10: Caution Panel Slave
  - 0x11: Engine Display Slave
  - 0x12: Fuel Display Slave
  - 0x13: Navigation Display Slave

  Created by: Bacon8tor
  Date: 2024
*/

#include <FalconBMSArduinoConnector.h>
#include <FBAC_I2C_Master.h>

// Create BMS connector instance
FalconBMSArduinoConnector bms;

// Create I2C master instance
FBAC_I2C_Master i2cMaster(&bms);

// Slave addresses
#define CAUTION_PANEL_SLAVE     0x10
#define ENGINE_DISPLAY_SLAVE    0x11
#define FUEL_DISPLAY_SLAVE      0x12
#define NAV_DISPLAY_SLAVE       0x13

// Update intervals (milliseconds)
unsigned long lastLightUpdate = 0;
unsigned long lastEngineUpdate = 0;
unsigned long lastFuelUpdate = 0;
unsigned long lastNavUpdate = 0;

const unsigned long LIGHT_UPDATE_INTERVAL = 100;    // 10Hz for lights
const unsigned long ENGINE_UPDATE_INTERVAL = 250;   // 4Hz for engine data
const unsigned long FUEL_UPDATE_INTERVAL = 500;     // 2Hz for fuel data
const unsigned long NAV_UPDATE_INTERVAL = 1000;     // 1Hz for navigation data

void setup() {
  Serial.begin(115200);

  // Initialize BMS connector
  bms.begin(Serial);

  // Initialize I2C master
  i2cMaster.begin();

  // Register slave devices
  i2cMaster.registerSlave(CAUTION_PANEL_SLAVE);
  i2cMaster.registerSlave(ENGINE_DISPLAY_SLAVE);
  i2cMaster.registerSlave(FUEL_DISPLAY_SLAVE);
  i2cMaster.registerSlave(NAV_DISPLAY_SLAVE);

  // No serial output - reserved for BMS communication
}

void loop() {
  // Update BMS connection and I2C master
  bms.update();
  i2cMaster.update();

  if (bms.isConnected()) {
    unsigned long now = millis();

    // Update caution panel lights (high frequency)
    if (now - lastLightUpdate >= LIGHT_UPDATE_INTERVAL) {
      lastLightUpdate = now;
      updateCautionPanel();
    }

    // Update engine display (medium frequency)
    if (now - lastEngineUpdate >= ENGINE_UPDATE_INTERVAL) {
      lastEngineUpdate = now;
      updateEngineDisplay();
    }

    // Update fuel display (low frequency)
    if (now - lastFuelUpdate >= FUEL_UPDATE_INTERVAL) {
      lastFuelUpdate = now;
      updateFuelDisplay();
    }

    // Update navigation display (very low frequency)
    if (now - lastNavUpdate >= NAV_UPDATE_INTERVAL) {
      lastNavUpdate = now;
      updateNavigationDisplay();
    }

    // Send connection status to all slaves
    i2cMaster.broadcastData(I2C_CMD_CONNECTION_STATUS);

  } else {
    // Send disconnected status to all slaves
    i2cMaster.broadcastData(I2C_CMD_CONNECTION_STATUS);
    delay(1000);
  }

  // Clear I2C errors silently
  if (i2cMaster.getLastError() != 0) {
    i2cMaster.clearError();
  }
}

void updateCautionPanel() {
  // Send all light bits to caution panel slave
  i2cMaster.sendLightBits(CAUTION_PANEL_SLAVE, 1); // LightBits 1
  delay(2);
  i2cMaster.sendLightBits(CAUTION_PANEL_SLAVE, 2); // LightBits 2
  delay(2);
  i2cMaster.sendLightBits(CAUTION_PANEL_SLAVE, 3); // LightBits 3
  delay(2);
  i2cMaster.sendBlinkBits(CAUTION_PANEL_SLAVE);     // BlinkBits
}

void updateEngineDisplay() {
  // Send engine-related data to engine display slave
  i2cMaster.sendEngineData(ENGINE_DISPLAY_SLAVE, I2C_CMD_RPM);
  delay(2);
  i2cMaster.sendEngineData(ENGINE_DISPLAY_SLAVE, I2C_CMD_OIL_PRESS_1);
  delay(2);
  i2cMaster.sendEngineData(ENGINE_DISPLAY_SLAVE, I2C_CMD_OIL_PRESS_2);
  delay(2);
  i2cMaster.sendEngineData(ENGINE_DISPLAY_SLAVE, I2C_CMD_FTIT_1);
  delay(2);
  i2cMaster.sendEngineData(ENGINE_DISPLAY_SLAVE, I2C_CMD_FTIT_2);
  delay(2);
  i2cMaster.sendEngineData(ENGINE_DISPLAY_SLAVE, I2C_CMD_NOZZLE_POS_1);
  delay(2);
  i2cMaster.sendEngineData(ENGINE_DISPLAY_SLAVE, I2C_CMD_NOZZLE_POS_2);
}

void updateFuelDisplay() {
  // Send fuel-related data to fuel display slave
  i2cMaster.sendFuelData(FUEL_DISPLAY_SLAVE, I2C_CMD_FUEL_FLOW);
  delay(2);
  i2cMaster.sendFuelData(FUEL_DISPLAY_SLAVE, I2C_CMD_FUEL_INTERNAL);
  delay(2);
  i2cMaster.sendFuelData(FUEL_DISPLAY_SLAVE, I2C_CMD_FUEL_EXTERNAL);
  delay(2);
  i2cMaster.sendFuelData(FUEL_DISPLAY_SLAVE, I2C_CMD_FUEL_TOTAL);
  delay(2);
  i2cMaster.sendFuelData(FUEL_DISPLAY_SLAVE, I2C_CMD_FUEL_FWD);
  delay(2);
  i2cMaster.sendFuelData(FUEL_DISPLAY_SLAVE, I2C_CMD_FUEL_AFT);

  // Send countermeasures count
  delay(2);
  i2cMaster.sendCountermeasures(FUEL_DISPLAY_SLAVE);
}

void updateNavigationDisplay() {
  // Send navigation data to navigation display slave
  i2cMaster.sendNavigationData(NAV_DISPLAY_SLAVE, I2C_CMD_KIAS);
  delay(2);
  i2cMaster.sendNavigationData(NAV_DISPLAY_SLAVE, I2C_CMD_CABIN_ALT);
  delay(2);
  i2cMaster.sendNavigationData(NAV_DISPLAY_SLAVE, I2C_CMD_DESIRED_COURSE);
  delay(2);
  i2cMaster.sendNavigationData(NAV_DISPLAY_SLAVE, I2C_CMD_COURSE_DEVIATION);
  delay(2);
  i2cMaster.sendNavigationData(NAV_DISPLAY_SLAVE, I2C_CMD_DISTANCE_BEACON);
  delay(2);
  i2cMaster.sendNavigationData(NAV_DISPLAY_SLAVE, I2C_CMD_BEARING_BEACON);

  // Send speed brake position
  delay(2);
  i2cMaster.sendToSlave(NAV_DISPLAY_SLAVE, I2C_CMD_SPEED_BRAKE);
}