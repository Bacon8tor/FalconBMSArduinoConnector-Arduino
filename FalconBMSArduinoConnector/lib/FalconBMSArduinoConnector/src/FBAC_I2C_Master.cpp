#include "FBAC_I2C_Master.h"

FBAC_I2C_Master::FBAC_I2C_Master(FalconBMSArduinoConnector* bms)
  : _bms(bms), _slaveCount(0), _lastError(0), _lastUpdate(0) {
  memset(_registeredSlaves, 0, sizeof(_registeredSlaves));
}

void FBAC_I2C_Master::begin() {
  Wire.begin(); // Initialize as I2C master
  Wire.setClock(400000); // 400kHz for faster communication
}

void FBAC_I2C_Master::update() {
  unsigned long now = millis();
  if (now - _lastUpdate >= _updateInterval) {
    _lastUpdate = now;

    // Ping slaves periodically to check connectivity
    if (now % 5000 < _updateInterval) { // Every 5 seconds
      pingAllSlaves();
    }
  }
}

bool FBAC_I2C_Master::sendToSlave(uint8_t slaveAddress, uint8_t command) {
  return sendToSlave(slaveAddress, command, nullptr, 0);
}

bool FBAC_I2C_Master::sendToSlave(uint8_t slaveAddress, uint8_t command, uint8_t* data, uint8_t length) {
  if (!_bms->isConnected()) {
    _lastError = I2C_CMD_NOT_READY;
    return false;
  }

  I2C_Packet packet;
  packData(&packet, command, data, length);
  return sendPacket(slaveAddress, &packet);
}

void FBAC_I2C_Master::broadcastData(uint8_t command) {
  for (uint8_t i = 0; i < _slaveCount; i++) {
    sendToSlave(_registeredSlaves[i], command);
    delay(5); // Small delay between transmissions
  }
}

bool FBAC_I2C_Master::sendLightBits(uint8_t slaveAddress, uint8_t lightBitsType) {
  if (!_bms->isConnected()) return false;

  I2C_Packet packet;
  uint32_t bits = 0;

  switch(lightBitsType) {
    case 1:
      _bms->getLightBits(1);
      // Pack lightBits as 4 bytes
      bits = _bms->lightBits; // Access private member through friend or getter
      packBooleans(&packet, I2C_CMD_LIGHTBITS_1, bits);
      break;
    case 2:
      _bms->getLightBits(2);
      bits = _bms->lightBits2;
      packBooleans(&packet, I2C_CMD_LIGHTBITS_2, bits);
      break;
    case 3:
      _bms->getLightBits(3);
      bits = _bms->lightBits3;
      packBooleans(&packet, I2C_CMD_LIGHTBITS_3, bits);
      break;
    default:
      _lastError = I2C_CMD_ERROR;
      return false;
  }

  return sendPacket(slaveAddress, &packet);
}

bool FBAC_I2C_Master::sendBlinkBits(uint8_t slaveAddress) {
  if (!_bms->isConnected()) return false;

  _bms->getblinkBits();
  I2C_Packet packet;
  packBooleans(&packet, I2C_CMD_BLINKBITS, _bms->blinkBits);
  return sendPacket(slaveAddress, &packet);
}

bool FBAC_I2C_Master::sendDEDLines(uint8_t slaveAddress) {
  if (!_bms->isConnected()) return false;

  _bms->getDED();
  I2C_Packet packet;

  // Send DED data as concatenated string (5 lines x 26 chars = 130 chars)
  // Split into multiple packets if needed
  for (int line = 0; line < 5; line++) {
    packet.command = I2C_CMD_DED_LINES;
    packet.length = 27; // 26 chars + line number
    packet.data[0] = line; // Line number
    memcpy(&packet.data[1], _bms->dedLines[line], 26);
    packet.checksum = calculateChecksum(packet.data, packet.length);

    if (!sendPacket(slaveAddress, &packet)) return false;
    delay(2); // Small delay between line transmissions
  }
  return true;
}

bool FBAC_I2C_Master::sendPFLLines(uint8_t slaveAddress) {
  if (!_bms->isConnected()) return false;

  _bms->getPFL();
  I2C_Packet packet;

  for (int line = 0; line < 5; line++) {
    packet.command = I2C_CMD_PFL_LINES;
    packet.length = 27;
    packet.data[0] = line;
    memcpy(&packet.data[1], _bms->pflLines[line], 26);
    packet.checksum = calculateChecksum(packet.data, packet.length);

    if (!sendPacket(slaveAddress, &packet)) return false;
    delay(2);
  }
  return true;
}

bool FBAC_I2C_Master::sendFuelData(uint8_t slaveAddress, uint8_t fuelType) {
  if (!_bms->isConnected()) return false;

  I2C_Packet packet;
  float value = 0;

  switch(fuelType) {
    case I2C_CMD_FUEL_FLOW:
      _bms->getFuelFlow();
      value = _bms->fuelFlow;
      break;
    case I2C_CMD_FUEL_INTERNAL:
      _bms->getinternalFuel();
      value = _bms->internalFuel;
      break;
    case I2C_CMD_FUEL_EXTERNAL:
      _bms->getexternalFuel();
      value = _bms->externalFuel;
      break;
    case I2C_CMD_FUEL_EPU:
      _bms->getEPUFuel();
      value = _bms->epuFuel;
      break;
    case I2C_CMD_FUEL_FWD:
      _bms->getfwd();
      value = _bms->fwd;
      break;
    case I2C_CMD_FUEL_AFT:
      _bms->getaft();
      value = _bms->aft;
      break;
    case I2C_CMD_FUEL_TOTAL:
      _bms->gettotalFuel();
      value = _bms->totalFuel;
      break;
    default:
      return false;
  }

  packFloat(&packet, fuelType, value);
  return sendPacket(slaveAddress, &packet);
}

bool FBAC_I2C_Master::sendEngineData(uint8_t slaveAddress, uint8_t engineParam) {
  if (!_bms->isConnected()) return false;

  I2C_Packet packet;
  float value = 0;

  switch(engineParam) {
    case I2C_CMD_RPM:
      _bms->getRPM();
      value = _bms->rpm;
      break;
    case I2C_CMD_OIL_PRESS_1:
      _bms->getOilPressure();
      value = _bms->oilPress;
      break;
    case I2C_CMD_OIL_PRESS_2:
      _bms->getOilPressure2();
      value = _bms->oilPress2;
      break;
    case I2C_CMD_NOZZLE_POS_1:
      _bms->getNozzlePos();
      value = _bms->nozzlePos;
      break;
    case I2C_CMD_NOZZLE_POS_2:
      _bms->getNozzlePos2();
      value = _bms->nozzlePos2;
      break;
    case I2C_CMD_FTIT_1:
      _bms->getFTIT();
      value = _bms->ftit;
      break;
    case I2C_CMD_FTIT_2:
      _bms->getFTIT2();
      value = _bms->ftit2;
      break;
    default:
      return false;
  }

  packFloat(&packet, engineParam, value);
  return sendPacket(slaveAddress, &packet);
}

bool FBAC_I2C_Master::sendNavigationData(uint8_t slaveAddress, uint8_t navParam) {
  if (!_bms->isConnected()) return false;

  I2C_Packet packet;
  float value = 0;

  switch(navParam) {
    case I2C_CMD_KIAS:
      _bms->getKIAS();
      value = _bms->kias;
      break;
    case I2C_CMD_CABIN_ALT:
      _bms->getCabinAlt();
      value = _bms->cabinAlt;
      break;
    case I2C_CMD_DESIRED_COURSE:
      _bms->getdesiredCourse();
      value = _bms->desiredCourse;
      break;
    case I2C_CMD_COURSE_DEVIATION:
      _bms->getcourseDeviation();
      value = _bms->courseDeviation;
      break;
    case I2C_CMD_DISTANCE_BEACON:
      _bms->getdistanceToBeacon();
      value = _bms->distanceToBearing;
      break;
    case I2C_CMD_BEARING_BEACON:
      _bms->getbearingToBeacon();
      value = _bms->bearingToBearing;
      break;
    default:
      return false;
  }

  packFloat(&packet, navParam, value);
  return sendPacket(slaveAddress, &packet);
}

bool FBAC_I2C_Master::sendCountermeasures(uint8_t slaveAddress) {
  if (!_bms->isConnected()) return false;

  _bms->getChaffFlareCount();

  I2C_Packet packet;

  // Send chaff count
  packFloat(&packet, I2C_CMD_CHAFF_COUNT, _bms->chaffCount);
  if (!sendPacket(slaveAddress, &packet)) return false;

  delay(2);

  // Send flare count
  packFloat(&packet, I2C_CMD_FLARE_COUNT, _bms->flareCount);
  return sendPacket(slaveAddress, &packet);
}

bool FBAC_I2C_Master::sendECMData(uint8_t slaveAddress) {
  if (!_bms->isConnected()) return false;

  _bms->getECMBits();
  I2C_Packet packet;

  // Send ECM data as 16 bytes (4 x uint32_t)
  packet.command = I2C_CMD_ECM_BITS;
  packet.length = 16;
  memcpy(packet.data, _bms->ecm, 16);
  packet.checksum = calculateChecksum(packet.data, packet.length);

  return sendPacket(slaveAddress, &packet);
}

bool FBAC_I2C_Master::sendConnectionStatus(uint8_t slaveAddress) {
  I2C_Packet packet;
  packet.command = I2C_CMD_CONNECTION_STATUS;
  packet.length = 1;
  packet.data[0] = _bms->isConnected() ? 1 : 0;
  packet.checksum = calculateChecksum(packet.data, packet.length);

  return sendPacket(slaveAddress, &packet);
}

void FBAC_I2C_Master::registerSlave(uint8_t address) {
  if (_slaveCount < 16 && !isSlaveRegistered(address)) {
    _registeredSlaves[_slaveCount++] = address;
  }
}

void FBAC_I2C_Master::unregisterSlave(uint8_t address) {
  for (uint8_t i = 0; i < _slaveCount; i++) {
    if (_registeredSlaves[i] == address) {
      // Shift remaining slaves down
      for (uint8_t j = i; j < _slaveCount - 1; j++) {
        _registeredSlaves[j] = _registeredSlaves[j + 1];
      }
      _slaveCount--;
      break;
    }
  }
}

bool FBAC_I2C_Master::isSlaveRegistered(uint8_t address) {
  for (uint8_t i = 0; i < _slaveCount; i++) {
    if (_registeredSlaves[i] == address) return true;
  }
  return false;
}

void FBAC_I2C_Master::pingAllSlaves() {
  for (uint8_t i = 0; i < _slaveCount; i++) {
    sendToSlave(_registeredSlaves[i], I2C_CMD_HEARTBEAT);
  }
}

uint8_t FBAC_I2C_Master::getLastError() {
  return _lastError;
}

void FBAC_I2C_Master::clearError() {
  _lastError = 0;
}

// Private helper functions

uint8_t FBAC_I2C_Master::calculateChecksum(uint8_t* data, uint8_t length) {
  uint8_t sum = 0;
  for (uint8_t i = 0; i < length; i++) {
    sum += data[i];
  }
  return sum;
}

bool FBAC_I2C_Master::sendPacket(uint8_t slaveAddress, I2C_Packet* packet) {
  Wire.beginTransmission(slaveAddress);
  Wire.write(packet->command);
  Wire.write(packet->length);
  Wire.write(packet->data, packet->length);
  Wire.write(packet->checksum);

  uint8_t result = Wire.endTransmission();
  if (result != 0) {
    _lastError = result;
    return false;
  }
  return true;
}

void FBAC_I2C_Master::packData(I2C_Packet* packet, uint8_t command, uint8_t* data, uint8_t length) {
  packet->command = command;
  packet->length = length;
  if (data && length > 0) {
    memcpy(packet->data, data, min(length, (uint8_t)32));
  }
  packet->checksum = calculateChecksum(packet->data, packet->length);
}

void FBAC_I2C_Master::packBooleans(I2C_Packet* packet, uint8_t command, uint32_t bits) {
  packet->command = command;
  packet->length = 4;
  memcpy(packet->data, &bits, 4);
  packet->checksum = calculateChecksum(packet->data, packet->length);
}

void FBAC_I2C_Master::packFloat(I2C_Packet* packet, uint8_t command, float value) {
  packet->command = command;
  packet->length = 4;
  memcpy(packet->data, &value, 4);
  packet->checksum = calculateChecksum(packet->data, packet->length);
}

void FBAC_I2C_Master::packInt(I2C_Packet* packet, uint8_t command, int value) {
  packet->command = command;
  packet->length = 4;
  memcpy(packet->data, &value, 4);
  packet->checksum = calculateChecksum(packet->data, packet->length);
}

void FBAC_I2C_Master::packLong(I2C_Packet* packet, uint8_t command, long value) {
  packet->command = command;
  packet->length = 4;
  memcpy(packet->data, &value, 4);
  packet->checksum = calculateChecksum(packet->data, packet->length);
}

void FBAC_I2C_Master::packString(I2C_Packet* packet, uint8_t command, char* str, uint8_t maxLength) {
  packet->command = command;
  packet->length = min(maxLength, (uint8_t)32);
  strncpy((char*)packet->data, str, packet->length);
  packet->checksum = calculateChecksum(packet->data, packet->length);
}