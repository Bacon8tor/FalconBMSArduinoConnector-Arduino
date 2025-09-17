#ifndef FBAC_I2C_SLAVE_H
#define FBAC_I2C_SLAVE_H

#include <Arduino.h>
#include <Wire.h>
#include "FBAC_I2C_Master.h" // For command definitions

// Callback function types
typedef void (*DataReceivedCallback)(uint8_t command, uint8_t* data, uint8_t length);
typedef void (*LightBitsCallback)(uint8_t lightBitsType, uint32_t bits);
typedef void (*FloatDataCallback)(uint8_t command, float value);
typedef void (*IntDataCallback)(uint8_t command, int value);
typedef void (*StringDataCallback)(uint8_t command, char* str, uint8_t length);
typedef void (*ConnectionStatusCallback)(bool connected);

// Slave data storage structure
struct SlaveData {
  // Connection status
  bool masterConnected;
  unsigned long lastHeartbeat;

  // Light bits
  uint32_t lightBits1;
  uint32_t lightBits2;
  uint32_t lightBits3;
  uint32_t blinkBits;

  // Display data
  char dedLines[5][27];
  char pflLines[5][27];

  // Analog values
  float fuelFlow;
  float fuelInternal;
  float fuelExternal;
  float fuelEPU;
  float fuelFwd;
  float fuelAft;
  float fuelTotal;

  float rpm;
  float oilPress1;
  float oilPress2;
  float nozzlePos1;
  float nozzlePos2;
  float ftit1;
  float ftit2;
  float cabinAlt;
  float kias;
  float hydPressA;
  float hydPressB;
  float speedBrake;

  float chaffCount;
  float flareCount;

  float desiredCourse;
  float courseDeviation;
  float distanceBeacon;
  float bearingBeacon;

  // Communications
  int uhfPreset;
  long uhfFreq;

  // ECM data
  uint32_t ecm[4];

  // IFF data
  int iffMode1Digit1;
  int iffMode1Digit2;
  int iffMode3Digit1;
  int iffMode3Digit2;

  // Lighting
  int instrLight;
  int floodConsole;

  // CMDS
  int cmdsMode;
};

class FBAC_I2C_Slave {
public:
  FBAC_I2C_Slave(uint8_t address);

  void begin();
  void update();

  // Data access functions
  SlaveData* getData() { return &_data; }

  // Individual data getters with convenience functions
  bool isConnected() { return _data.masterConnected; }
  unsigned long getLastHeartbeat() { return _data.lastHeartbeat; }

  // Light bit helper functions
  bool getLightBit(uint8_t lightBitsType, uint8_t bitPosition);

  // LightBits 1 - Exact same interface as FalconBMSArduinoConnector
  bool isMasterCaution() { return getLightBit(1, 0); }
  bool isTF() { return getLightBit(1, 1); }
  bool isOxyBrow() { return getLightBit(1, 2); }
  bool isEquipHot() { return getLightBit(1, 3); }
  bool isOnGround() { return getLightBit(1, 4); }
  bool isEngFire() { return getLightBit(1, 5); }
  bool isConfig() { return getLightBit(1, 6); }
  bool isHyd() { return getLightBit(1, 7); }
  bool isFlcsABCD() { return getLightBit(1, 8); }
  bool isFlcs() { return getLightBit(1, 9); }
  bool isCan() { return getLightBit(1, 10); }
  bool isTLConfig() { return getLightBit(1, 11); }
  bool isAOAAbove() { return getLightBit(1, 12); }
  bool isAOAOn() { return getLightBit(1, 13); }
  bool isAOABelow() { return getLightBit(1, 14); }
  bool isRefuelRDY() { return getLightBit(1, 15); }
  bool isRefuelAR() { return getLightBit(1, 16); }
  bool isRefuelDSC() { return getLightBit(1, 17); }
  bool isFltControlSys() { return getLightBit(1, 18); }
  bool isLEFlaps() { return getLightBit(1, 19); }
  bool isEngineFault() { return getLightBit(1, 20); }
  bool isOverheat() { return getLightBit(1, 21); }
  bool isFuelLow() { return getLightBit(1, 22); }
  bool isAvionics() { return getLightBit(1, 23); }
  bool isRadarAlt() { return getLightBit(1, 24); }
  bool isIFF() { return getLightBit(1, 25); }
  bool isECM() { return getLightBit(1, 26); }
  bool isHook() { return getLightBit(1, 27); }
  bool isNWSFail() { return getLightBit(1, 28); }
  bool isCabinPress() { return getLightBit(1, 29); }
  bool isAutoPilotOn() { return getLightBit(1, 30); }
  bool isTFRSTBY() { return getLightBit(1, 31); }

  // LightBits 2 - Additional warning lights
  bool isHandOff() { return getLightBit(2, 0); }
  bool isLaunch() { return getLightBit(2, 1); }
  bool isPriMode() { return getLightBit(2, 2); }
  bool isNaval() { return getLightBit(2, 3); }
  bool isUnk() { return getLightBit(2, 4); }
  bool isTgtSep() { return getLightBit(2, 5); }
  bool isGo() { return getLightBit(2, 6); }
  bool isNoGo() { return getLightBit(2, 7); }
  bool isDegr() { return getLightBit(2, 8); }
  bool isRdy() { return getLightBit(2, 9); }
  bool isChaffLo() { return getLightBit(2, 10); }
  bool isFlareLo() { return getLightBit(2, 11); }
  bool isAuxSrch() { return getLightBit(2, 12); }
  bool isAuxAct() { return getLightBit(2, 13); }
  bool isAuxLow() { return getLightBit(2, 14); }
  bool isAuxPwr() { return getLightBit(2, 15); }
  bool isEcmPwr() { return getLightBit(2, 16); }
  bool isEcmFail() { return getLightBit(2, 17); }
  bool isFwdFuelLow() { return getLightBit(2, 18); }
  bool isAftFuelLow() { return getLightBit(2, 19); }
  bool isEPUOn() { return getLightBit(2, 20); }
  bool isJFSOn() { return getLightBit(2, 21); }
  bool isSEC() { return getLightBit(2, 22); }
  bool isOxyLow() { return getLightBit(2, 23); }
  bool isProbeHeat() { return getLightBit(2, 24); }
  bool isSeatArm() { return getLightBit(2, 25); }
  bool isBUC() { return getLightBit(2, 26); }
  bool isFuelOilHot() { return getLightBit(2, 27); }
  bool isAntiSkid() { return getLightBit(2, 28); }
  bool isTFREngaged() { return getLightBit(2, 29); }
  bool isGearHandle() { return getLightBit(2, 30); }
  bool isEngine() { return getLightBit(2, 31); }

  // LightBits 3 - Extended warning lights
  bool isFlcsPmg() { return getLightBit(3, 0); }
  bool isMainGen() { return getLightBit(3, 1); }
  bool isStbyGen() { return getLightBit(3, 2); }
  bool isEpuGen() { return getLightBit(3, 3); }
  bool isEpuPmg() { return getLightBit(3, 4); }
  bool isToFlcs() { return getLightBit(3, 5); }
  bool isFlcsRly() { return getLightBit(3, 6); }
  bool isBatFail() { return getLightBit(3, 7); }
  bool isHydrazine() { return getLightBit(3, 8); }
  bool isAir() { return getLightBit(3, 9); }
  bool isElecFault() { return getLightBit(3, 10); }
  bool isLefFault() { return getLightBit(3, 11); }
  bool isOnGround3() { return getLightBit(3, 12); }
  bool isFlcsBitRun() { return getLightBit(3, 13); }
  bool isFlcsBitFail() { return getLightBit(3, 14); }
  bool isDbuWarn() { return getLightBit(3, 15); }
  bool isNoseGearDown() { return getLightBit(3, 16); }
  bool isLeftGearDown() { return getLightBit(3, 17); }
  bool isRightGearDown() { return getLightBit(3, 18); }
  bool isParkBrakeOn() { return getLightBit(3, 19); }
  bool isPowerOff() { return getLightBit(3, 20); }
  bool isCadc() { return getLightBit(3, 21); }
  bool isSpeedBrake() { return getLightBit(3, 22); }
  bool isSysTest() { return getLightBit(3, 23); }
  bool isMCAnnounced() { return getLightBit(3, 24); }
  bool isMLGWOW() { return getLightBit(3, 25); }
  bool isNLGWOW() { return getLightBit(3, 26); }
  bool isATFNotEngaged() { return getLightBit(3, 27); }
  bool isInletIcing() { return getLightBit(3, 28); }

  // BlinkBits - Blinking lights (using blinkBits data)
  bool isOuterMarkerBlinking() { return (_data.blinkBits & 0x01) != 0; }
  bool isMiddleMarkerBlinking() { return (_data.blinkBits & 0x02) != 0; }
  bool isProbeHeatBlinking() { return (_data.blinkBits & 0x04) != 0; }
  bool isAuxSrchBlinking() { return (_data.blinkBits & 0x08) != 0; }
  bool isLaunchBlinking() { return (_data.blinkBits & 0x10) != 0; }
  bool isPriModeBlinking() { return (_data.blinkBits & 0x20) != 0; }
  bool isUnkBlinking() { return (_data.blinkBits & 0x40) != 0; }
  bool isElecFaultBlinking() { return (_data.blinkBits & 0x80) != 0; }
  bool isOxyBrowBlinking() { return (_data.blinkBits & 0x100) != 0; }
  bool isEPUOnBlinking() { return (_data.blinkBits & 0x200) != 0; }
  bool isJFSOnSlowBlinking() { return (_data.blinkBits & 0x400) != 0; }
  bool isJFSOnFastBlinking() { return (_data.blinkBits & 0x800) != 0; }
  bool isECMOperBlinking() { return (_data.blinkBits & 0x1000) != 0; }

  // Display functions - Same as BMS library interface
  char* getDEDLine(uint8_t line) { return (line < 5) ? _data.dedLines[line] : nullptr; }
  char* getPFLLine(uint8_t line) { return (line < 5) ? _data.pflLines[line] : nullptr; }

  // Analog data - Exact same interface as FalconBMSArduinoConnector
  // These mirror the public variables in the original BMS library
  float fuelFlow = 0.0;
  float internalFuel = 0.0;
  float externalFuel = 0.0;
  float epuFuel = 0.0;
  float fwd = 0.0;
  float aft = 0.0;
  float totalFuel = 0.0;
  float chaffCount = 0.0;
  float flareCount = 0.0;
  float desiredCourse = 0.0;
  float courseDeviation = 0.0;
  float distanceToBearing = 0.0;
  float bearingToBearing = 0.0;
  float rpm = 0.0;
  uint32_t ecm[4] = {0, 0, 0, 0};
  float oilPress = 0.0;
  float oilPress2 = 0.0;
  float nozzlePos = 0.0;
  float nozzlePos2 = 0.0;
  float ftit = 0.0;
  float ftit2 = 0.0;
  float cabinAlt = 0.0;
  float kias = 0.0;
  float hydPressA = 0.0;
  float hydPressB = 0.0;
  int cmdsMode = 0;
  int uhfPreset = 0;
  long uhfFreq = 0;
  int InstrumentLighting = 0;
  int FloodConsoleLighting = 0;
  float speedBrake = 0.0;
  int IFFMode1Digit1 = 0;
  int IFFMode1Digit2 = 0;
  int IFFMode3Digit1 = 0;
  int IFFMode3Digit2 = 0;

  // DED and PFL arrays - Same as BMS library
  char dedLines[5][27];
  char pflLines[5][27];

  // Lighting status functions - Same as BMS library
  int getInstrLightStatus() { return _data.instrLight; }
  int getFloodConsoleStatus() { return _data.floodConsole; }

  // ECM functions
  uint32_t getECMPanel(uint8_t panel) { return (panel < 4) ? _data.ecm[panel] : 0; }
  int getECMStatus(uint8_t panel);

  // Request specific data from master
  bool requestData(uint8_t command);
  bool requestLightBits(uint8_t lightBitsType);
  bool requestFuelData(uint8_t fuelType);
  bool requestEngineData(uint8_t engineParam);
  bool requestAllLights();

  // Callback registration
  void setDataReceivedCallback(DataReceivedCallback callback) { _dataCallback = callback; }
  void setLightBitsCallback(LightBitsCallback callback) { _lightBitsCallback = callback; }
  void setFloatDataCallback(FloatDataCallback callback) { _floatCallback = callback; }
  void setIntDataCallback(IntDataCallback callback) { _intCallback = callback; }
  void setStringDataCallback(StringDataCallback callback) { _stringCallback = callback; }
  void setConnectionStatusCallback(ConnectionStatusCallback callback) { _connectionCallback = callback; }

  // Error handling
  uint8_t getLastError() { return _lastError; }
  void clearError() { _lastError = 0; }

  // Connection timeout (default 5 seconds)
  void setConnectionTimeout(unsigned long timeout) { _connectionTimeout = timeout; }

private:
  uint8_t _address;
  SlaveData _data;
  uint8_t _lastError;
  unsigned long _connectionTimeout;

  // Callbacks
  DataReceivedCallback _dataCallback;
  LightBitsCallback _lightBitsCallback;
  FloatDataCallback _floatCallback;
  IntDataCallback _intCallback;
  StringDataCallback _stringCallback;
  ConnectionStatusCallback _connectionCallback;

  // I2C receive buffer
  uint8_t _receiveBuffer[64];
  uint8_t _bufferIndex;
  bool _receivingData;

  // Static callback functions for Wire library
  static void onReceiveWrapper(int bytes);
  static void onRequestWrapper();

  // Instance pointer for static callbacks
  static FBAC_I2C_Slave* _instance;

  // I2C event handlers
  void onReceive(int bytes);
  void onRequest();

  // Data parsing functions
  void parseReceivedData(uint8_t command, uint8_t* data, uint8_t length);
  void parseLightBits(uint8_t command, uint8_t* data, uint8_t length);
  void parseFloatData(uint8_t command, uint8_t* data, uint8_t length);
  void parseIntData(uint8_t command, uint8_t* data, uint8_t length);
  void parseStringData(uint8_t command, uint8_t* data, uint8_t length);
  void parseECMData(uint8_t* data, uint8_t length);

  // Helper functions
  uint8_t calculateChecksum(uint8_t* data, uint8_t length);
  bool validateChecksum(uint8_t* data, uint8_t length, uint8_t receivedChecksum);
  void updateConnectionStatus();
  void syncPublicVariables();
};

#endif // FBAC_I2C_SLAVE_H