#ifndef FBAC_I2C_MASTER_H
#define FBAC_I2C_MASTER_H

#include <Arduino.h>
#include <Wire.h>
#include "FalconBMSArduinoConnector.h"

// I2C Commands for data types
enum I2C_Commands : uint8_t {
  // Light data commands
  I2C_CMD_LIGHTBITS_1 = 0x01,
  I2C_CMD_LIGHTBITS_2 = 0x02,
  I2C_CMD_LIGHTBITS_3 = 0x03,
  I2C_CMD_BLINKBITS = 0x04,

  // Display data commands
  I2C_CMD_DED_LINES = 0x05,
  I2C_CMD_PFL_LINES = 0x08,

  // Analog data commands
  I2C_CMD_FUEL_FLOW = 0x06,
  I2C_CMD_FUEL_INTERNAL = 0x22,
  I2C_CMD_FUEL_EXTERNAL = 0x23,
  I2C_CMD_FUEL_EPU = 0x24,
  I2C_CMD_FUEL_FWD = 0x35,
  I2C_CMD_FUEL_AFT = 0x36,
  I2C_CMD_FUEL_TOTAL = 0x37,

  I2C_CMD_RPM = 0x12,
  I2C_CMD_OIL_PRESS_1 = 0x14,
  I2C_CMD_OIL_PRESS_2 = 0x15,
  I2C_CMD_NOZZLE_POS_1 = 0x16,
  I2C_CMD_NOZZLE_POS_2 = 0x17,
  I2C_CMD_FTIT_1 = 0x18,
  I2C_CMD_FTIT_2 = 0x19,
  I2C_CMD_CABIN_ALT = 0x20,
  I2C_CMD_KIAS = 0x21,
  I2C_CMD_HYD_PRESS_A = 0x25,
  I2C_CMD_HYD_PRESS_B = 0x26,
  I2C_CMD_SPEED_BRAKE = 0x30,

  // Countermeasures
  I2C_CMD_CHAFF_COUNT = 0x09,
  I2C_CMD_FLARE_COUNT = 0x10,

  // Communications
  I2C_CMD_UHF_PRESET = 0x28,
  I2C_CMD_UHF_FREQ = 0x29,

  // Navigation
  I2C_CMD_DESIRED_COURSE = 0x38,
  I2C_CMD_COURSE_DEVIATION = 0x39,
  I2C_CMD_DISTANCE_BEACON = 0x40,
  I2C_CMD_BEARING_BEACON = 0x41,

  // Lighting
  I2C_CMD_INSTR_LIGHT = 0x07,
  I2C_CMD_FLOOD_CONSOLE = 0x11,

  // ECM
  I2C_CMD_ECM_BITS = 0x13,

  // IFF
  I2C_CMD_IFF_MODE1_DIGIT1 = 0x31,
  I2C_CMD_IFF_MODE1_DIGIT2 = 0x32,
  I2C_CMD_IFF_MODE3_DIGIT1 = 0x33,
  I2C_CMD_IFF_MODE3_DIGIT2 = 0x34,

  // CMDS
  I2C_CMD_CMDS_MODE = 0x27,

  // Motion derivatives
  I2C_CMD_X_DOT = 0x42,
  I2C_CMD_Y_DOT = 0x43,
  I2C_CMD_Z_DOT = 0x44,

  // Flight angles
  I2C_CMD_ALPHA = 0x48,
  I2C_CMD_BETA = 0x49,
  I2C_CMD_GAMMA = 0x4A,

  // Speed parameters
  I2C_CMD_MACH = 0x4B,
  I2C_CMD_VT = 0x4C,
  I2C_CMD_GS = 0x4D,
  I2C_CMD_WIND_OFFSET = 0x4E,

  // Gear positions
  I2C_CMD_NOSE_GEAR_POS = 0x4F,
  I2C_CMD_LEFT_GEAR_POS = 0x50,
  I2C_CMD_RIGHT_GEAR_POS = 0x51,
  I2C_CMD_GEAR_POS = 0x52,

  // Engine 2 data
  I2C_CMD_RPM2 = 0x60,
  I2C_CMD_FTIT = 0x61,
  I2C_CMD_OIL_PRESSURE = 0x62,
  I2C_CMD_FUEL_FLOW2 = 0x63,
  I2C_CMD_NOZZLE_POS = 0x66,

  // Trim
  I2C_CMD_TRIM_PITCH = 0x70,
  I2C_CMD_TRIM_ROLL = 0x71,
  I2C_CMD_TRIM_YAW = 0x72,

  // Heading
  I2C_CMD_CURRENT_HEADING = 0x93,
  I2C_CMD_DESIRED_HEADING = 0x94,

  // Altitude & Navigation
  I2C_CMD_ALT_CAL_READING = 0xB4,
  I2C_CMD_BINGO_FUEL = 0xB6,
  I2C_CMD_CARA_ALOW = 0xB7,
  I2C_CMD_BULLSEYE_X = 0xB8,
  I2C_CMD_BULLSEYE_Y = 0xB9,

  // Control surfaces
  I2C_CMD_TURN_RATE = 0xC0,
  I2C_CMD_LEF_POS = 0xC1,
  I2C_CMD_TEF_POS = 0xC2,
  I2C_CMD_VTOL_POS = 0xC3,

  // Position & Orientation
  I2C_CMD_LATITUDE = 0xD1,
  I2C_CMD_LONGITUDE = 0xD2,
  I2C_CMD_MAG_DEVIATION_SYSTEM = 0xD3,
  I2C_CMD_MAG_DEVIATION_REAL = 0xD4,
  I2C_CMD_BUMP_INTENSITY = 0xD5,

  // Status commands
  I2C_CMD_CONNECTION_STATUS = 0x80,
  I2C_CMD_ALL_LIGHTS = 0x81,
  I2C_CMD_HEARTBEAT = 0x82,

  // Error responses
  I2C_CMD_ERROR = 0xFF,
  I2C_CMD_NOT_READY = 0xFE
};

// I2C Data packet structure
struct I2C_Packet {
  uint8_t command;
  uint8_t length;
  uint8_t data[32];  // Max I2C payload
  uint8_t checksum;
};

class FBAC_I2C_Master {
public:
  FBAC_I2C_Master(FalconBMSArduinoConnector* bms);

  void begin();
  void update();

  // Send data to specific slave
  bool sendToSlave(uint8_t slaveAddress, uint8_t command);
  bool sendToSlave(uint8_t slaveAddress, uint8_t command, uint8_t* data, uint8_t length);

  // Broadcast data to all slaves
  void broadcastData(uint8_t command);

  // Send specific data types
  bool sendLightBits(uint8_t slaveAddress, uint8_t lightBitsType);
  bool sendBlinkBits(uint8_t slaveAddress);
  bool sendDEDLines(uint8_t slaveAddress);
  bool sendPFLLines(uint8_t slaveAddress);
  bool sendFuelData(uint8_t slaveAddress, uint8_t fuelType);
  bool sendEngineData(uint8_t slaveAddress, uint8_t engineParam);
  bool sendNavigationData(uint8_t slaveAddress, uint8_t navParam);
  bool sendCountermeasures(uint8_t slaveAddress);
  bool sendECMData(uint8_t slaveAddress);
  bool sendConnectionStatus(uint8_t slaveAddress);

  // Slave management
  void registerSlave(uint8_t address);
  void unregisterSlave(uint8_t address);
  bool isSlaveRegistered(uint8_t address);
  void pingAllSlaves();

  // Error handling
  uint8_t getLastError();
  void clearError();

private:
  FalconBMSArduinoConnector* _bms;
  uint8_t _registeredSlaves[16];  // Support up to 16 slaves
  uint8_t _slaveCount;
  uint8_t _lastError;
  unsigned long _lastUpdate;
  const unsigned long _updateInterval = 50; // 20Hz update rate

  // Helper functions
  uint8_t calculateChecksum(uint8_t* data, uint8_t length);
  bool sendPacket(uint8_t slaveAddress, I2C_Packet* packet);
  void packData(I2C_Packet* packet, uint8_t command, uint8_t* data, uint8_t length);
  void packBooleans(I2C_Packet* packet, uint8_t command, uint32_t bits);
  void packFloat(I2C_Packet* packet, uint8_t command, float value);
  void packInt(I2C_Packet* packet, uint8_t command, int value);
  void packLong(I2C_Packet* packet, uint8_t command, long value);
  void packString(I2C_Packet* packet, uint8_t command, char* str, uint8_t maxLength);
};

#endif // FBAC_I2C_MASTER_H