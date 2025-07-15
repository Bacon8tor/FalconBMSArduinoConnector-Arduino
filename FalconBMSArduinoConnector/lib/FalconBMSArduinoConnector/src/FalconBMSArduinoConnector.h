#ifndef FALCON_BMS_ARDUINO_CONNECTOR_H
#define FALCON_BMS_ARDUINO_CONNECTOR_H

#include <Arduino.h>
#include "LightBits.h"
#include "LightBits2.h"
#include "LightBits3.h"

class FalconBMSArduinoConnector {
public:
  FalconBMSArduinoConnector();

  void begin(HardwareSerial& serial = Serial, uint32_t baud = 115200);
  void update();  // Call in loop()
  void getLightBits(String bits);
  void getDEDLines(int line);

  // Connection status
  bool isConnected();

  //DED
  String dedLines[5];
  
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

  // LightBits2-related getters
  bool isHandOff();
  bool isLaunch();
  bool isPriMode();
  bool isNaval();
  bool isUnk();
  bool isTgtSep();
  bool isGo();
  bool isNoGo();
  bool isDegr();
  bool isRdy();
  bool isChaffLo();
  bool isFlareLo();
  bool isAuxSrch();
  bool isAuxAct();
  bool isAuxLow();
  bool isAuxPwr();
  bool isEcmPwr();
  bool isEcmFail();
  bool isFwdFuelLow();
  bool isAftFuelLow();
  bool isEPUOn();
  bool isJFSOn();
  bool isSEC();
  bool isOxyLow();
  bool isProbeHeat();
  bool isSeatArm();
  bool isBUC();
  bool isFuelOilHot();
  bool isAntiSkid();
  bool isTFREngaged();
  bool isGearHandle();
  bool isEngine();

  // LightBits3-related getters
  bool isFlcsPmg();
  bool isMainGen();
  bool isStbyGen();
  bool isEpuGen();
  bool isEpuPmg();
  bool isToFlcs();
  bool isFlcsRly();
  bool isBatFail();
  bool isHydrazine();
  bool isAir();
  bool isElecFault();
  bool isLefFault();
  bool isOnGround3();
  bool isFlcsBitRun();
  bool isFlcsBitFail();
  bool isDbuWarn();
  bool isNoseGearDown();
  bool isLeftGearDown();
  bool isRightGearDown();
  bool isParkBrakeOn();
  bool isPowerOff();
  bool isCadc();
  bool isSpeedBrake();
  bool isSysTest();
  bool isMCAnnounced();
  bool isMLGWOW();
  bool isNLGWOW();
  bool isATFNotEngaged();
  bool isInletIcing();

private:
  HardwareSerial* _serial;
  uint32_t lightBits;
  uint32_t lightBits2;
  uint32_t lightBits3;
  
  uint8_t buffer[134];
  uint8_t idx;
  bool isReading;
  bool connected;
  unsigned long lastSerialActivity;
  const unsigned long timeoutMs = 5000;

  void handlePacket(uint8_t type, uint8_t* data, uint8_t len);
  void checkLightBits();
  void checkLightBits2();
  void checkLightBits3();

  // Bit flags
  bool _bits[32];
  bool _bits2[32];
  bool _bits3[32];
};

#endif
