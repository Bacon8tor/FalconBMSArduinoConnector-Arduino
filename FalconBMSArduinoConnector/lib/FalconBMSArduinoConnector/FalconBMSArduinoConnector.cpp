#include "FalconBMSArduinoConnector.h"

FalconBMSArduinoConnector::FalconBMSArduinoConnector()
  : lightBits(0), lightBits2(0), idx(0), isReading(false), connected(false), lastSerialActivity(0), _serial(&Serial) {
  memset(_bits, 0, sizeof(_bits));
}

void FalconBMSArduinoConnector::begin(HardwareSerial& serial, uint32_t baud) {
  _serial = &serial;
  _serial->begin(baud);
  while (!_serial);
  _serial->println("READY");
}

void FalconBMSArduinoConnector::update() {
    _serial->println("READY");
    delay(10);
    _serial->println(0x55);
  while (_serial->available()) {
    uint8_t b = _serial->read();

    if (!isReading) {
      if (b == 0xAA) {
        isReading = true;
        idx = 0;
      }
      continue;
    }

    buffer[idx++] = b;

    if (idx >= 2) {
      uint8_t expectedLen = buffer[1];
      if (idx == 2 + expectedLen + 1) {
        uint8_t type = buffer[0];
        uint8_t len = buffer[1];
        uint8_t* data = &buffer[2];
        uint8_t checksum = buffer[2 + len];

        uint8_t sum = type + len;
        for (int i = 0; i < len; i++) sum += data[i];

        if (checksum == (sum & 0xFF)) {
          handlePacket(type, data, len);
        }

        isReading = false;
      }
    }
  }

  if (connected && (millis() - lastSerialActivity > timeoutMs)) {
    connected = false;
  }
}

bool FalconBMSArduinoConnector::isConnected() {
  return connected;
}

void FalconBMSArduinoConnector::handlePacket(uint8_t type, uint8_t* data, uint8_t len) {
  lastSerialActivity = millis();
  connected = true;

  if (type == 0x01 && len == 4) {
    memcpy(&lightBits, data, 4);
    checkLightBits();
  } else if (type == 0x02 && len == 4) {
    memcpy(&lightBits2, data, 4);
    // Add future handling if needed
  }
}

void FalconBMSArduinoConnector::checkLightBits() {
  _bits[0] = lightBits & MasterCaution;
  _bits[1] = lightBits & TF;
  _bits[2] = lightBits & OXY_BROW;
  _bits[3] = lightBits & EQUIP_HOT;
  _bits[4] = lightBits & ONGROUND;
  _bits[5] = lightBits & ENG_FIRE;
  _bits[6] = lightBits & CONFIG;
  _bits[7] = lightBits & HYD;
  _bits[8] = lightBits & Flcs_ABCD;
  _bits[9] = lightBits & FLCS;
  _bits[10] = lightBits & CAN;
  _bits[11] = lightBits & T_L_CFG;
  _bits[12] = lightBits & AOAAbove;
  _bits[13] = lightBits & AOAOn;
  _bits[14] = lightBits & AOABelow;
  _bits[15] = lightBits & RefuelRDY;
  _bits[16] = lightBits & RefuelAR;
  _bits[17] = lightBits & RefuelDSC;
  _bits[18] = lightBits & FltControlSys;
  _bits[19] = lightBits & LEFlaps;
  _bits[20] = lightBits & EngineFault;
  _bits[21] = lightBits & Overheat;
  _bits[22] = lightBits & FuelLow;
  _bits[23] = lightBits & Avionics;
  _bits[24] = lightBits & RadarAlt;
  _bits[25] = lightBits & IFF;
  _bits[26] = lightBits & ECM;
  _bits[27] = lightBits & Hook;
  _bits[28] = lightBits & NWSFail;
  _bits[29] = lightBits & CabinPress;
  _bits[30] = lightBits & AutoPilotOn;
  _bits[31] = lightBits & TFR_STBY;
}

// Individual accessors
#define DEFINE_GETTER(name, index) bool FalconBMSArduinoConnector::name() { return _bits[index]; }

DEFINE_GETTER(isMasterCaution, 0)
DEFINE_GETTER(isTF, 1)
DEFINE_GETTER(isOxyBrow, 2)
DEFINE_GETTER(isEquipHot, 3)
DEFINE_GETTER(isOnGround, 4)
DEFINE_GETTER(isEngFire, 5)
DEFINE_GETTER(isConfig, 6)
DEFINE_GETTER(isHyd, 7)
DEFINE_GETTER(isFlcsABCD, 8)
DEFINE_GETTER(isFlcs, 9)
DEFINE_GETTER(isCan, 10)
DEFINE_GETTER(isTLConfig, 11)
DEFINE_GETTER(isAOAAbove, 12)
DEFINE_GETTER(isAOAOn, 13)
DEFINE_GETTER(isAOABelow, 14)
DEFINE_GETTER(isRefuelRDY, 15)
DEFINE_GETTER(isRefuelAR, 16)
DEFINE_GETTER(isRefuelDSC, 17)
DEFINE_GETTER(isFltControlSys, 18)
DEFINE_GETTER(isLEFlaps, 19)
DEFINE_GETTER(isEngineFault, 20)
DEFINE_GETTER(isOverheat, 21)
DEFINE_GETTER(isFuelLow, 22)
DEFINE_GETTER(isAvionics, 23)
DEFINE_GETTER(isRadarAlt, 24)
DEFINE_GETTER(isIFF, 25)
DEFINE_GETTER(isECM, 26)
DEFINE_GETTER(isHook, 27)
DEFINE_GETTER(isNWSFail, 28)
DEFINE_GETTER(isCabinPress, 29)
DEFINE_GETTER(isAutoPilotOn, 30)
DEFINE_GETTER(isTFRSTBY, 31)
