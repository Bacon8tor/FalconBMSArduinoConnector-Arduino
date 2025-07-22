#ifndef FALCON_BMS_ARDUINO_CONNECTOR_H
#define FALCON_BMS_ARDUINO_CONNECTOR_H

#include <Arduino.h>
#include "LightBits.h"
#include "LightBits2.h"
#include "LightBits3.h"
#include "BlinkBits.h"

//SD1322 OLED 256*64 (deduino compatible?? not confirmed pin changes may be needed)
#ifdef FBAC_DED_1322
#include <U8g2lib.h>
#ifndef FBAC_FONT_U8G2
#define FBAC_FONT_U8G2
#endif
U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2_DED(U8G2_R0, /* cs=*/ 5, /* dc=*/ 16, /* reset=*/ 17);//for ESP32, change pins as needed
#endif

//SD1306 OLED 128*64
#ifdef FBAC_DED_1306
#include <U8g2lib.h>
#ifndef FBAC_FONT_U8G2
#define FBAC_FONT_U8G2
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_DED(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

//PFL OLED 128*64
#ifdef FBAC_PFL_1306
#include <U8g2lib.h>
#ifndef FBAC_FONT_U8G2
#define FBAC_FONT_U8G2
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_PFL(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

//Fuel Flow OLED 128*64 
#ifdef FBAC_FF_1306
#include <U8g2lib.h>
#ifndef FBAC_FONT_U8G2
#define FBAC_FONT_U8G2
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_FuelFlow(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

//FBAC Fonts 
#ifdef FBAC_FONT_U8G2
  #include "falconded_font.h"
  #define SD1322_FONT u8g2_font_FalconDED_18_tf
  #define SD1306_FONT u8g2_font_FBAC_DED_FONT_5x8_tf
#ifdef U8G2_HAVE_HW_SPI
  #include <SPI.h>
#endif
#ifdef U8G2_HAVE_HW_I2C
  #include <Wire.h>
#endif
#endif

//Fastled library 
#ifdef FBAC_FASTLED
#include <FastLED.h>
#endif


class FalconBMSArduinoConnector {
public:
  FalconBMSArduinoConnector();

  // Connection status
  bool isConnected();
  unsigned long lastSerialActivity;
  
  void begin(HardwareSerial& serial = Serial, uint32_t baud = 115200);
  
  void update();  // Call in loop()
  void checkAllLights();
  void getLightBits(int lb);
  void getblinkBits();
  void getDED();
  void getFuelFlow();
  void getPFL();
  void getChaffFlareCount();
  
  //DED
  char dedLines[5][27];
  char pflLines[5][27];
  
  //Fuel
  float fuelFlow;

  //Chaff/Flare
  float chaffCount;
  float flareCount;

  //RPM 
  float rpm;

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
  bool isAllLampBitsOn(); //Not Sure if these are working 
  bool isAllLampBits; //trying to check ALl LampBits

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

private:
  HardwareSerial* _serial;
  uint32_t lightBits;
  uint32_t lightBits2;
  uint32_t lightBits3;
  uint32_t blinkBits;
  uint32_t instrLight;
  
  uint8_t buffer[134];
  uint8_t idx;
  bool isReading;
  bool connected;
  
  const unsigned long timeoutMs = 500;

  void handlePacket(uint8_t type, uint8_t* data, uint8_t len);
  
  void checkLightBits();
  void checkLightBits2();
  void checkLightBits3();
  void checkBlinkBits();
  void setInstrLights();
  void sendCommand(uint8_t commandByte);
  void waitForPacket();
  void decodeDED(uint8_t* data, uint8_t len);
  void decodePFL(uint8_t* data, uint8_t len);


  // Bit flags
  bool _bits[32];
  bool _bits2[32];
  bool _bits3[32];
  bool _blinkBits[13];
};



#endif
