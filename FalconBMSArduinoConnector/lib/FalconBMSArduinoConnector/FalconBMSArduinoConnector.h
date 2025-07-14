#ifndef FALCON_BMS_ARDUINO_CONNECTOR_H
#define FALCON_BMS_ARDUINO_CONNECTOR_H

#include <Arduino.h>
#include "LightBits.h"

class FalconBMSArduinoConnector {
public:
  FalconBMSArduinoConnector();

  void begin(HardwareSerial& serial = Serial, uint32_t baud = 115200);
  void update();  // Call in loop()

  // Connection status
  bool isConnected();

  // Light bit getters
  bool isMasterCaution();
  bool isTF();
  bool isOxyBrow();
  bool isEquipHot();
  bool isOnGround();
  bool isEngFire();
  bool isConfig();
  bool isHyd();
  bool isFlcsABCD();
  bool isFlcs();
  bool isCan();
  bool isTLConfig();
  bool isAOAAbove();
  bool isAOAOn();
  bool isAOABelow();
  bool isRefuelRDY();
  bool isRefuelAR();
  bool isRefuelDSC();
  bool isFltControlSys();
  bool isLEFlaps();
  bool isEngineFault();
  bool isOverheat();
  bool isFuelLow();
  bool isAvionics();
  bool isRadarAlt();
  bool isIFF();
  bool isECM();
  bool isHook();
  bool isNWSFail();
  bool isCabinPress();
  bool isAutoPilotOn();
  bool isTFRSTBY();

private:
  HardwareSerial* _serial;
  uint32_t lightBits;
  uint32_t lightBits2;
  uint8_t buffer[10];
  uint8_t idx;
  bool isReading;
  bool connected;
  unsigned long lastSerialActivity;
  const unsigned long timeoutMs = 5000;

  void handlePacket(uint8_t type, uint8_t* data, uint8_t len);
  void checkLightBits();

  // Bit flags
  bool _bits[32];
};

#endif
