#include "FBAC_I2C_Slave.h"

// Static instance pointer for I2C callbacks
FBAC_I2C_Slave* FBAC_I2C_Slave::_instance = nullptr;

FBAC_I2C_Slave::FBAC_I2C_Slave(uint8_t address)
  : _address(address), _lastError(0), _connectionTimeout(5000),
    _dataCallback(nullptr), _lightBitsCallback(nullptr), _floatCallback(nullptr),
    _intCallback(nullptr), _stringCallback(nullptr), _connectionCallback(nullptr),
    _bufferIndex(0), _receivingData(false) {

  // Initialize data structure
  memset(&_data, 0, sizeof(_data));
  _data.masterConnected = false;
  _data.lastHeartbeat = 0;

  // Set static instance for callbacks
  _instance = this;
}

void FBAC_I2C_Slave::begin() {
  Wire.begin(_address); // Initialize as I2C slave with specified address
  Wire.onReceive(onReceiveWrapper);
  Wire.onRequest(onRequestWrapper);
}

void FBAC_I2C_Slave::update() {
  updateConnectionStatus();
}

bool FBAC_I2C_Slave::getLightBit(uint8_t lightBitsType, uint8_t bitPosition) {
  uint32_t bits = 0;

  switch(lightBitsType) {
    case 1: bits = _data.lightBits1; break;
    case 2: bits = _data.lightBits2; break;
    case 3: bits = _data.lightBits3; break;
    default: return false;
  }

  return (bits & (1UL << bitPosition)) != 0;
}

int FBAC_I2C_Slave::getECMStatus(uint8_t panel) {
  if (panel >= 4) return 0;

  uint32_t light = _data.ecm[panel];

  // ECM status mapping (from original library)
  switch(light) {
    case 0x00000000: return 0; // ECM_UNPRESSED_NO_LIT
    case 0x00000001: return 1; // ECM_UNPRESSED_ALL_LIT
    case 0x00000002: return 2; // ECM_PRESSED_NO_LIT
    case 0x00000003: return 3; // ECM_PRESSED_STANDBY
    case 0x00000004: return 4; // ECM_PRESSED_ACTIVE
    case 0x00000005: return 5; // ECM_PRESSED_TRANSMIT
    case 0x00000006: return 6; // ECM_PRESSED_FAIL
    case 0x00000007: return 7; // ECM_PRESSED_ALL_LIT
    default: return 0;
  }
}

bool FBAC_I2C_Slave::requestData(uint8_t command) {
  // Send request to master (this would need to be implemented based on your protocol)
  // For now, this is a placeholder - the master typically sends data automatically
  // or slaves can be configured to request specific data periodically
  return true;
}

bool FBAC_I2C_Slave::requestLightBits(uint8_t lightBitsType) {
  switch(lightBitsType) {
    case 1: return requestData(I2C_CMD_LIGHTBITS_1);
    case 2: return requestData(I2C_CMD_LIGHTBITS_2);
    case 3: return requestData(I2C_CMD_LIGHTBITS_3);
    default: return false;
  }
}

bool FBAC_I2C_Slave::requestFuelData(uint8_t fuelType) {
  return requestData(fuelType);
}

bool FBAC_I2C_Slave::requestEngineData(uint8_t engineParam) {
  return requestData(engineParam);
}

bool FBAC_I2C_Slave::requestAllLights() {
  return requestData(I2C_CMD_ALL_LIGHTS);
}

// Static wrapper functions for Wire library callbacks
void FBAC_I2C_Slave::onReceiveWrapper(int bytes) {
  if (_instance) {
    _instance->onReceive(bytes);
  }
}

void FBAC_I2C_Slave::onRequestWrapper() {
  if (_instance) {
    _instance->onRequest();
  }
}

void FBAC_I2C_Slave::onReceive(int bytes) {
  if (bytes < 3) return; // Minimum: command + length + checksum

  uint8_t command = Wire.read();
  uint8_t length = Wire.read();

  if (length > 32) {
    _lastError = 0xFE; // Invalid length
    // Flush remaining bytes
    while (Wire.available()) Wire.read();
    return;
  }

  uint8_t data[32];
  for (uint8_t i = 0; i < length && Wire.available(); i++) {
    data[i] = Wire.read();
  }

  uint8_t receivedChecksum = Wire.read();

  // Validate checksum
  if (!validateChecksum(data, length, receivedChecksum)) {
    _lastError = 0xFD; // Checksum error
    return;
  }

  // Parse the received data
  parseReceivedData(command, data, length);

  // Update heartbeat timestamp
  _data.lastHeartbeat = millis();

  // Sync public variables with internal data for BMS-style access
  syncPublicVariables();

  // Call generic data callback if registered
  if (_dataCallback) {
    _dataCallback(command, data, length);
  }
}

void FBAC_I2C_Slave::onRequest() {
  // Slave can send status or error codes back to master
  Wire.write(_lastError);
}

void FBAC_I2C_Slave::parseReceivedData(uint8_t command, uint8_t* data, uint8_t length) {
  switch(command) {
    case I2C_CMD_LIGHTBITS_1:
    case I2C_CMD_LIGHTBITS_2:
    case I2C_CMD_LIGHTBITS_3:
    case I2C_CMD_BLINKBITS:
      parseLightBits(command, data, length);
      break;

    case I2C_CMD_DED_LINES:
    case I2C_CMD_PFL_LINES:
      parseStringData(command, data, length);
      break;

    case I2C_CMD_FUEL_FLOW:
    case I2C_CMD_FUEL_INTERNAL:
    case I2C_CMD_FUEL_EXTERNAL:
    case I2C_CMD_FUEL_EPU:
    case I2C_CMD_FUEL_FWD:
    case I2C_CMD_FUEL_AFT:
    case I2C_CMD_FUEL_TOTAL:
    case I2C_CMD_RPM:
    case I2C_CMD_OIL_PRESS_1:
    case I2C_CMD_OIL_PRESS_2:
    case I2C_CMD_NOZZLE_POS_1:
    case I2C_CMD_NOZZLE_POS_2:
    case I2C_CMD_FTIT_1:
    case I2C_CMD_FTIT_2:
    case I2C_CMD_CABIN_ALT:
    case I2C_CMD_KIAS:
    case I2C_CMD_HYD_PRESS_A:
    case I2C_CMD_HYD_PRESS_B:
    case I2C_CMD_SPEED_BRAKE:
    case I2C_CMD_CHAFF_COUNT:
    case I2C_CMD_FLARE_COUNT:
    case I2C_CMD_DESIRED_COURSE:
    case I2C_CMD_COURSE_DEVIATION:
    case I2C_CMD_DISTANCE_BEACON:
    case I2C_CMD_BEARING_BEACON:
      parseFloatData(command, data, length);
      break;

    case I2C_CMD_UHF_PRESET:
    case I2C_CMD_INSTR_LIGHT:
    case I2C_CMD_FLOOD_CONSOLE:
    case I2C_CMD_IFF_MODE1_DIGIT1:
    case I2C_CMD_IFF_MODE1_DIGIT2:
    case I2C_CMD_IFF_MODE3_DIGIT1:
    case I2C_CMD_IFF_MODE3_DIGIT2:
    case I2C_CMD_CMDS_MODE:
      parseIntData(command, data, length);
      break;

    case I2C_CMD_UHF_FREQ:
      // Handle long data type
      if (length >= 4) {
        memcpy(&_data.uhfFreq, data, 4);
      }
      break;

    case I2C_CMD_ECM_BITS:
      parseECMData(data, length);
      break;

    case I2C_CMD_CONNECTION_STATUS:
      if (length >= 1) {
        bool wasConnected = _data.masterConnected;
        _data.masterConnected = (data[0] != 0);

        if (wasConnected != _data.masterConnected && _connectionCallback) {
          _connectionCallback(_data.masterConnected);
        }
      }
      break;

    case I2C_CMD_HEARTBEAT:
      // Just update timestamp - already done in onReceive
      break;

    default:
      _lastError = 0xFC; // Unknown command
      break;
  }
}

void FBAC_I2C_Slave::parseLightBits(uint8_t command, uint8_t* data, uint8_t length) {
  if (length < 4) return;

  uint32_t bits;
  memcpy(&bits, data, 4);

  switch(command) {
    case I2C_CMD_LIGHTBITS_1:
      _data.lightBits1 = bits;
      break;
    case I2C_CMD_LIGHTBITS_2:
      _data.lightBits2 = bits;
      break;
    case I2C_CMD_LIGHTBITS_3:
      _data.lightBits3 = bits;
      break;
    case I2C_CMD_BLINKBITS:
      _data.blinkBits = bits;
      break;
  }

  if (_lightBitsCallback) {
    _lightBitsCallback(command, bits);
  }
}

void FBAC_I2C_Slave::parseFloatData(uint8_t command, uint8_t* data, uint8_t length) {
  if (length < 4) return;

  float value;
  memcpy(&value, data, 4);

  switch(command) {
    case I2C_CMD_FUEL_FLOW: _data.fuelFlow = value; break;
    case I2C_CMD_FUEL_INTERNAL: _data.fuelInternal = value; break;
    case I2C_CMD_FUEL_EXTERNAL: _data.fuelExternal = value; break;
    case I2C_CMD_FUEL_EPU: _data.fuelEPU = value; break;
    case I2C_CMD_FUEL_FWD: _data.fuelFwd = value; break;
    case I2C_CMD_FUEL_AFT: _data.fuelAft = value; break;
    case I2C_CMD_FUEL_TOTAL: _data.fuelTotal = value; break;
    case I2C_CMD_RPM: _data.rpm = value; break;
    case I2C_CMD_OIL_PRESS_1: _data.oilPress1 = value; break;
    case I2C_CMD_OIL_PRESS_2: _data.oilPress2 = value; break;
    case I2C_CMD_NOZZLE_POS_1: _data.nozzlePos1 = value; break;
    case I2C_CMD_NOZZLE_POS_2: _data.nozzlePos2 = value; break;
    case I2C_CMD_FTIT_1: _data.ftit1 = value; break;
    case I2C_CMD_FTIT_2: _data.ftit2 = value; break;
    case I2C_CMD_CABIN_ALT: _data.cabinAlt = value; break;
    case I2C_CMD_KIAS: _data.kias = value; break;
    case I2C_CMD_HYD_PRESS_A: _data.hydPressA = value; break;
    case I2C_CMD_HYD_PRESS_B: _data.hydPressB = value; break;
    case I2C_CMD_SPEED_BRAKE: _data.speedBrake = value; break;
    case I2C_CMD_CHAFF_COUNT: _data.chaffCount = value; break;
    case I2C_CMD_FLARE_COUNT: _data.flareCount = value; break;
    case I2C_CMD_DESIRED_COURSE: _data.desiredCourse = value; break;
    case I2C_CMD_COURSE_DEVIATION: _data.courseDeviation = value; break;
    case I2C_CMD_DISTANCE_BEACON: _data.distanceBeacon = value; break;
    case I2C_CMD_BEARING_BEACON: _data.bearingBeacon = value; break;
  }

  if (_floatCallback) {
    _floatCallback(command, value);
  }
}

void FBAC_I2C_Slave::parseIntData(uint8_t command, uint8_t* data, uint8_t length) {
  if (length < 4) return;

  int value;
  memcpy(&value, data, 4);

  switch(command) {
    case I2C_CMD_UHF_PRESET: _data.uhfPreset = value; break;
    case I2C_CMD_INSTR_LIGHT: _data.instrLight = value; break;
    case I2C_CMD_FLOOD_CONSOLE: _data.floodConsole = value; break;
    case I2C_CMD_IFF_MODE1_DIGIT1: _data.iffMode1Digit1 = value; break;
    case I2C_CMD_IFF_MODE1_DIGIT2: _data.iffMode1Digit2 = value; break;
    case I2C_CMD_IFF_MODE3_DIGIT1: _data.iffMode3Digit1 = value; break;
    case I2C_CMD_IFF_MODE3_DIGIT2: _data.iffMode3Digit2 = value; break;
    case I2C_CMD_CMDS_MODE: _data.cmdsMode = value; break;
  }

  if (_intCallback) {
    _intCallback(command, value);
  }
}

void FBAC_I2C_Slave::parseStringData(uint8_t command, uint8_t* data, uint8_t length) {
  if (length < 2) return; // Need at least line number + 1 char

  uint8_t lineNumber = data[0];
  if (lineNumber >= 5) return; // Invalid line number

  switch(command) {
    case I2C_CMD_DED_LINES:
      if (length >= 27) { // Line number + 26 chars
        memcpy(_data.dedLines[lineNumber], &data[1], 26);
        _data.dedLines[lineNumber][26] = '\0'; // Ensure null termination
      }
      break;
    case I2C_CMD_PFL_LINES:
      if (length >= 27) {
        memcpy(_data.pflLines[lineNumber], &data[1], 26);
        _data.pflLines[lineNumber][26] = '\0';
      }
      break;
  }

  if (_stringCallback) {
    _stringCallback(command, (char*)&data[1], length - 1);
  }
}

void FBAC_I2C_Slave::parseECMData(uint8_t* data, uint8_t length) {
  if (length < 16) return; // Need 4 x uint32_t = 16 bytes

  for (uint8_t i = 0; i < 4; i++) {
    memcpy(&_data.ecm[i], &data[i * 4], 4);
  }
}

uint8_t FBAC_I2C_Slave::calculateChecksum(uint8_t* data, uint8_t length) {
  uint8_t sum = 0;
  for (uint8_t i = 0; i < length; i++) {
    sum += data[i];
  }
  return sum;
}

bool FBAC_I2C_Slave::validateChecksum(uint8_t* data, uint8_t length, uint8_t receivedChecksum) {
  return calculateChecksum(data, length) == receivedChecksum;
}

void FBAC_I2C_Slave::updateConnectionStatus() {
  unsigned long now = millis();
  bool wasConnected = _data.masterConnected;

  // Check if we've exceeded the connection timeout
  if (_data.lastHeartbeat > 0 && (now - _data.lastHeartbeat) > _connectionTimeout) {
    _data.masterConnected = false;
  }

  // Call connection callback if status changed
  if (wasConnected != _data.masterConnected && _connectionCallback) {
    _connectionCallback(_data.masterConnected);
  }
}

void FBAC_I2C_Slave::syncPublicVariables() {
  // Sync all public variables with internal data for BMS-style access
  // This allows code to use slave.fuelFlow instead of slave.getFuelFlow()

  fuelFlow = _data.fuelFlow;
  internalFuel = _data.fuelInternal;
  externalFuel = _data.fuelExternal;
  epuFuel = _data.fuelEPU;
  fwd = _data.fuelFwd;
  aft = _data.fuelAft;
  totalFuel = _data.fuelTotal;
  chaffCount = _data.chaffCount;
  flareCount = _data.flareCount;
  desiredCourse = _data.desiredCourse;
  courseDeviation = _data.courseDeviation;
  distanceToBearing = _data.distanceBeacon;
  bearingToBearing = _data.bearingBeacon;
  rpm = _data.rpm;

  // Copy ECM array
  for (int i = 0; i < 4; i++) {
    ecm[i] = _data.ecm[i];
  }

  oilPress = _data.oilPress1;
  oilPress2 = _data.oilPress2;
  nozzlePos = _data.nozzlePos1;
  nozzlePos2 = _data.nozzlePos2;
  ftit = _data.ftit1;
  ftit2 = _data.ftit2;
  cabinAlt = _data.cabinAlt;
  kias = _data.kias;
  hydPressA = _data.hydPressA;
  hydPressB = _data.hydPressB;
  cmdsMode = _data.cmdsMode;
  uhfPreset = _data.uhfPreset;
  uhfFreq = _data.uhfFreq;
  InstrumentLighting = _data.instrLight;
  FloodConsoleLighting = _data.floodConsole;
  speedBrake = _data.speedBrake;
  IFFMode1Digit1 = _data.iffMode1Digit1;
  IFFMode1Digit2 = _data.iffMode1Digit2;
  IFFMode3Digit1 = _data.iffMode3Digit1;
  IFFMode3Digit2 = _data.iffMode3Digit2;

  // Copy DED and PFL arrays
  for (int i = 0; i < 5; i++) {
    memcpy(dedLines[i], _data.dedLines[i], 27);
    memcpy(pflLines[i], _data.pflLines[i], 27);
  }
}