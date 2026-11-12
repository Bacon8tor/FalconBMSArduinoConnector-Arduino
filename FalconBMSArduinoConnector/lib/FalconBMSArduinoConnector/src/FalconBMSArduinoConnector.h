#ifndef FALCON_BMS_ARDUINO_CONNECTOR_H
#define FALCON_BMS_ARDUINO_CONNECTOR_H

#include <Arduino.h>
#include "LightBits.h"
#include "LightBits2.h"
#include "LightBits3.h"
#include "BlinkBits.h"
#include "EcmBits.h"
#include "InstrLight.h"
#include "SimStatusBits.h"
#include "FBAC_Commands.h"
// NOTE: FBAC_Config.h defines global display objects. Include this library from
// a single .ino/.cpp file only, or you will get duplicate-symbol link errors.
#include "FBAC_Config.h"

#define FBAC_MAX_ECM 5

class FalconBMSArduinoConnector
{

public:
  FalconBMSArduinoConnector();

  // Connection status
  bool isConnected();
  unsigned long lastSerialActivity;

  // Protocol version reported by the PC app: 0 = not yet known, 1 = legacy (FBAC <= 0.5), 2+ = current
  uint8_t protocolVersion;

  void begin(Stream &serial = Serial, uint32_t baud = 115200);
  // void begin(HardwareSerial& serial = Serial, uint32_t baud = 115200);

  // How long a single request waits for its reply (default 250 ms).
  // Raise it if you run the link at a low baud rate.
  void setRequestTimeout(unsigned long ms) { requestTimeoutMs = ms; }

  void update(); // Call in loop()
  void checkAllLights();
  void getLightBits(int lb);
  void getblinkBits();
  void getDED();
  void getFuelFlow();
  void getPFL();
  void getChaffFlareCount();
  void getECMBits();
  int getECMStatus(int ecmLight);
  void getOilPressure();
  void getOilPressure2();
  void getNozzlePos();
  void getNozzlePos2();
  void getFTIT();
  void getFTIT2();
  void getCabinAlt();
  void getKIAS();
  void getinternalFuel();
  void getexternalFuel();
  void getEPUFuel();
  void getHYDPress();
  void getCMDSMode();
  void getInstrLight(); // Call
  void getuhfPreset();
  void getuhfFreq();
  void getSpeedBrake();
  void getRPM();
  void getFloodConsole();
  void getIFFMode1Digit1();
  void getIFFMode1Digit2();
  void getIFFMode3Digit1();
  void getIFFMode3Digit2();
  void getfwd();
  void getaft();
  void gettotalFuel();
  void getdesiredCourse();
  void getcourseDeviation();
  void getdistanceToBeacon();
  void getbearingToBeacon();
  void getSimStatus();

  // DED
  char dedLines[5][27];
  char pflLines[5][27];

  // Fuel
  float fuelFlow;
  float internalFuel;
  float externalFuel;
  float epuFuel;
  float fwd;
  float aft;
  float totalFuel;
  // Chaff/Flare
  float chaffCount;
  float flareCount;

  // Course/Heading
  float desiredCourse;
  float courseDeviation;
  float distanceToBearing;
  float bearingToBearing;

  // RPM
  float rpm;

  // ECM
  uint32_t ecm[FBAC_MAX_ECM];

  // Oil Press
  float oilPress;
  float oilPress2;

  // Nozzle Pos
  float nozzlePos;
  float nozzlePos2;

  // FTIT
  float ftit;
  float ftit2;

  // Cabin Alt
  float cabinAlt;

  // Airspeed
  float kias;

  // HYD Press
  float hydPressA;
  float hydPressB;

  // CMDS Mode
  int32_t cmdsMode;

  // UHF Comms
  int32_t uhfPreset;
  int32_t uhfFreq;

  // InstrLight Status
  int getInstrLightStatus(); // returns status 0 = off 1 = dim 2 = brt
  int InstrumentLighting;

  // Flood Console Status
  int getFloodConsoleStatus();
  int FloodConsoleLighting;

  // Speed Brake
  float speedBrake;

  // IFF Mode Digits
  int IFFMode1Digit1;
  int IFFMode1Digit2;
  int IFFMode3Digit1;
  int IFFMode3Digit2;

  // Sim status, e.g., whether the simulation is in 3D.
  uint32_t simStatus;

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
  bool isAllLampBitsOn(); // true while the cockpit lamp test is running
  bool isAllLampBits;     // same value, kept for existing sketches

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

  // BlinkBits individual getters
  bool isOuterMarkerBlinking();
  bool isMiddleMarkerBlinking();
  bool isProbeHeatBlinking();
  bool isAuxSrchBlinking();
  bool isLaunchBlinking();
  bool isPriModeBlinking();
  bool isUnkBlinking();
  bool isElecFaultBlinking();
  bool isOxyBrowBlinking();
  bool isEPUOnBlinking();
  bool isJFSOnSlowBlinking();
  bool isJFSOnFastBlinking();
  bool isECMOperBlinking();

  // Sim status getter
  bool isIn3D();

private:
  Stream *_serial;
  //  HardwareSerial* _serial;
  uint32_t lightBits;
  uint32_t lightBits2;
  uint32_t lightBits3;
  uint32_t blinkBits;
  byte instrLight;
  int instrLightStatus;
  byte floodConsole;
  int floodConsoleStatus;

  byte IFFMode1Digit1_byte;
  byte IFFMode1Digit2_byte;
  byte IFFMode3Digit1_byte;
  byte IFFMode3Digit2_byte;

  // [type][len][data up to 128][checksum]
  uint8_t buffer[131];

  bool connected;
  uint8_t consecutiveFailures;
  unsigned long lastPingMs;
  unsigned long requestTimeoutMs;
  static const unsigned long keepAliveMs = 3000;   // ping the PC if idle this long
  static const unsigned long reconnectPingMs = 1000; // ping interval while disconnected
  static const uint8_t maxFailures = 5;            // failed requests before disconnect

  enum RequestResult : uint8_t
  {
    REQ_FAIL = 0,
    REQ_OK = 1,
    REQ_UNSUPPORTED = 2
  };

  RequestResult request(uint8_t cmd, bool force = false);
  RequestResult waitForPacket(uint8_t expectedType);
  void drainInput();
  void markDisconnected();
  static bool copyField(void *dst, size_t size, const uint8_t *data, uint8_t len);

  void handlePacket(uint8_t type, uint8_t *data, uint8_t len);

  void checkLightBits();
  void checkLightBits2();
  void checkLightBits3();
  void checkBlinkBits();
  void setInstrLights();
  void setFloodConsole();
  void decodeDED(uint8_t *data, uint8_t len);
  void decodePFL(uint8_t *data, uint8_t len);

  // Bit flags
  bool _bits[32];
  bool _bits2[32];
  bool _bits3[32];
  bool _blinkBits[13];
};

#endif
