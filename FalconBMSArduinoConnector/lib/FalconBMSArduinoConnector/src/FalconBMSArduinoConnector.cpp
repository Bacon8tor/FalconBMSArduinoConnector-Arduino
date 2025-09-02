#include "FalconBMSArduinoConnector.h"
#include <Arduino.h>

FalconBMSArduinoConnector::FalconBMSArduinoConnector()
  : lastSerialActivity(0),
    _serial(&Serial),
    lightBits(0),
    lightBits2(0),
    lightBits3(0),
    blinkBits(0),
    instrLight(0),
    idx(0),
    isReading(false),
    connected(false)
{
  memset(_bits, 0, sizeof(_bits));
}

void FalconBMSArduinoConnector::begin(Stream& serial, uint32_t baud) {
  _serial = &serial;
  // Do NOT call begin() on _serial here
  // Do NOT wait for serial connection here
}
/* DEPRECATED FUNCTION KEEPING JUST IN CASE */
// void FalconBMSArduinoConnector::begin(HardwareSerial& serial, uint32_t baud) {
//   _serial = &serial;
//   _serial->begin(baud);
//   while (!_serial);
//  // connected = true;
// }

bool FalconBMSArduinoConnector::isConnected() {
  return connected;
}

void FalconBMSArduinoConnector::update() {
  if (!connected && _serial->available()) {
    byte incoming = _serial->read();
    if (incoming == 0xA5) {
      _serial->write(0x5A);
      connected = true;
      lastSerialActivity = millis();    
    }
  } 

  if (connected && millis() - lastSerialActivity > timeoutMs) {
    connected = false;
    _serial->write(0x5A);
  }
  
}

// Fetch Data
void FalconBMSArduinoConnector::getLightBits(int lb){
 switch(lb){
      case 1:
      sendCommand(0x01);
      break;
      case 2:
      sendCommand(0x02);
      break;
      case 3:
      sendCommand(0x03);
      break;
      // case 4:
      // sendCommand(0x04);
      // break;
      default:
      sendCommand(0x5A);
      break;
 }
}

void FalconBMSArduinoConnector::getblinkBits(){
  sendCommand(0x04);
}

void FalconBMSArduinoConnector::getDED()
{
  sendCommand(0x05);
}

void FalconBMSArduinoConnector::getFuelFlow(){
  sendCommand(0x06);
}

void FalconBMSArduinoConnector::getInstrLight(){
  sendCommand(0x07);
}
void FalconBMSArduinoConnector::getPFL()
{
  sendCommand(0x08);
}

void FalconBMSArduinoConnector::getChaffFlareCount() {
  sendCommand(0x09);
  sendCommand(0x10);
}
void FalconBMSArduinoConnector::getFloodConsole(){
  sendCommand(0x11);
}

void FalconBMSArduinoConnector::getRPM(){
  sendCommand(0x12);
}
void FalconBMSArduinoConnector::getECMBits() {
  sendCommand(0x13);
}

void FalconBMSArduinoConnector::getOilPressure(){
  sendCommand(0x14);
}

void FalconBMSArduinoConnector::getOilPressure2(){
  sendCommand(0x15);
}

void FalconBMSArduinoConnector::getNozzlePos(){
  sendCommand(0x16);
}

void FalconBMSArduinoConnector::getNozzlePos2(){
  sendCommand(0x17);
}

void FalconBMSArduinoConnector::getFTIT(){
  sendCommand(0x18);
}

void FalconBMSArduinoConnector::getFTIT2(){
  sendCommand(0x19);
}

void FalconBMSArduinoConnector::getCabinAlt(){
  sendCommand(0x20);
}

void FalconBMSArduinoConnector::getKIAS(){
  sendCommand(0x21);
}

void FalconBMSArduinoConnector::getinternalFuel(){
  sendCommand(0x22);
}

void FalconBMSArduinoConnector::getexternalFuel(){
  sendCommand(0x23);
}

void FalconBMSArduinoConnector::getEPUFuel(){
  sendCommand(0x24);
}

void FalconBMSArduinoConnector::getHYDPress(){
  sendCommand(0x25);
  sendCommand(0x26);
}

void FalconBMSArduinoConnector::getCMDSMode(){
  sendCommand(0x27);
      //  CmdsOFF = 0,
      //   CmdsSTBY = 1,
      //   CmdsMAN = 2,
      //   CmdsSEMI = 3,
      //   CmdsAUTO = 4,
      //   CmdsBYP = 5,
}

void FalconBMSArduinoConnector::getuhfPreset(){
  sendCommand(0x28);
}

void FalconBMSArduinoConnector::getuhfFreq(){
  sendCommand(0x29);
}

void FalconBMSArduinoConnector::getSpeedBrake(){
  sendCommand(0x30);
}

void FalconBMSArduinoConnector::getIFFMode1Digit1(){
  sendCommand(0x31);
}
void FalconBMSArduinoConnector::getIFFMode1Digit2(){
  sendCommand(0x32);
}
void FalconBMSArduinoConnector::getIFFMode3Digit1(){
  sendCommand(0x33);
}
void FalconBMSArduinoConnector::getIFFMode3Digit2(){
  sendCommand(0x34);
}

void FalconBMSArduinoConnector::getfwd(){
  sendCommand(0x35);
}

void FalconBMSArduinoConnector::getaft(){
  sendCommand(0x36);
}

void FalconBMSArduinoConnector::gettotalFuel(){
  sendCommand(0x37);
}

void FalconBMSArduinoConnector::getdesiredCourse(){
  sendCommand(0x38);
}
void FalconBMSArduinoConnector::getcourseDeviation(){
  sendCommand(0x39);
}

void FalconBMSArduinoConnector::getdistanceToBeacon(){
sendCommand(0x40);
}
void FalconBMSArduinoConnector::getbearingToBeacon(){
sendCommand(0x41);
}
//Packet handling
void FalconBMSArduinoConnector::sendCommand(uint8_t commandByte) {
  _serial->write(commandByte);
  waitForPacket();
}

void FalconBMSArduinoConnector::handlePacket(uint8_t type, uint8_t* data, uint8_t len) {

  switch (type) {
    //LightBits
    case 0x01:
      memcpy(&lightBits,data,len);
      checkLightBits();

      break;
    case 0x02:
      memcpy(&lightBits2,data,len);
      checkLightBits2();
      break;
    case 0x03:
      memcpy(&lightBits3,data,len);
      checkLightBits3();
      break;
    case 0x04:
      memcpy(&blinkBits,data,len);
      checkBlinkBits();
      break;
    case 0x05:
      decodeDED(data,len);
      break;
    case 0x06:
      memcpy(&fuelFlow,data,sizeof(float));
      break;
    case 0x07:
      memcpy(&instrLight,data,len);
      setInstrLights();
      break;
    case 0x08:
      decodePFL(data,len);
    break;
    case 0x09:
      memcpy(&chaffCount,data,sizeof(float));
    break;
    case 0x10:
      memcpy(&flareCount,data,sizeof(float));
    break;
    case 0x11:
      //floodconsoleLights
      memcpy(&floodConsole,data,len);
      setFloodConsole();
    break;
    case 0x12:
      memcpy(&rpm,data,sizeof(float));
    break;
    case 0x13:
      for (uint8_t i = 0; i < 4; i++) {
      ecm[i] =
          ((uint32_t)data[i * 4 + 0]) |
          ((uint32_t)data[i * 4 + 1] << 8) |
          ((uint32_t)data[i * 4 + 2] << 16) |
          ((uint32_t)data[i * 4 + 3] << 24);
    }
    break;
    case 0x14:
      memcpy(&oilPress,data,sizeof(float));
    break;
    case 0x15:
      memcpy(&oilPress2,data,sizeof(float));
    break;
    case 0x16:
      memcpy(&nozzlePos,data,sizeof(float));
    break;
    case 0x17:
      memcpy(&nozzlePos2,data,sizeof(float));
    break;
    case 0x18:
      memcpy(&ftit,data,sizeof(float));
    break;
    case 0x19:
      memcpy(&ftit2,data,sizeof(float));
    break;
    case 0x20:
      memcpy(&cabinAlt,data,sizeof(float));
    break;
    case 0x21:
      memcpy(&kias,data,sizeof(float));
    break;
    case 0x22:
      memcpy(&internalFuel,data,sizeof(float));
    break;
    case 0x23:
      memcpy(&externalFuel,data,sizeof(float));
    break;
    case 0x24:
      memcpy(&epuFuel,data,sizeof(float));
    break;
    case 0x25:
      memcpy(&hydPressA,data,sizeof(float));
    break;
    case 0x26:
      memcpy(&hydPressB,data,sizeof(float));
    break;
    case 0x27:
      memcpy(&cmdsMode,data,sizeof(int));
    break;
    case 0x28:
      memcpy(&uhfPreset,data,sizeof(int));
      
    break;
    case 0x29:
      memcpy(&uhfFreq,data,sizeof(long));
    break;
    case 0x30:
      memcpy(&speedBrake,data,sizeof(float));
    break;
    case 0x31:
       memcpy(&IFFMode1Digit1_byte,data,sizeof(byte));
        IFFMode1Digit1 = (int)IFFMode1Digit1_byte;
    break;
    case 0x32:
       memcpy(&IFFMode1Digit2_byte,data,sizeof(byte));
        IFFMode1Digit2 = (int)IFFMode1Digit2_byte;
    break;
    case 0x33:
       memcpy(&IFFMode3Digit1_byte,data,sizeof(byte));
        IFFMode3Digit1 = (int)IFFMode3Digit1_byte;
    break;
    case 0x34:
       memcpy(&IFFMode3Digit2_byte,data,sizeof(byte));
        IFFMode3Digit2 = (int)IFFMode3Digit2_byte;
    break;
    case 0x35:
      memccpy(&fwd,data,sizeof(float),sizeof(float));
    break;
    case 0x36:
      memccpy(&aft,data,sizeof(float),sizeof(float));
    break;
    case 0x37:
      memccpy(&totalFuel,data,sizeof(float),sizeof(float));
    break;
    case 0x38:
      memccpy(&desiredCourse,data,sizeof(float),sizeof(float));
    break;
    case 0x39:
      memccpy(&courseDeviation,data,sizeof(float),sizeof(float));
    break;
    case 0x40:
      memccpy(&distanceToBearing,data,sizeof(float),sizeof(float));
    break;
    case 0x41:
      memccpy(&bearingToBearing,data,sizeof(float),sizeof(float));
    break;
    case 0xA5: // Handshake byte?
      _serial->write(0x5A);
      connected = true;
      lastSerialActivity = millis(); 
      break;
    default: {
      _serial->write(0x5A);
      break;
    }
  }
  lastSerialActivity = millis();
  connected = true;
}

void FalconBMSArduinoConnector::waitForPacket(){
  unsigned long start = millis();
  isReading = false;
  idx = 0;

  while (millis() - start < timeoutMs) {
    if (_serial->available()) {
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
          // Full packet received
          uint8_t type = buffer[0];
          uint8_t len = buffer[1];
          uint8_t* data = &buffer[2];
          uint8_t checksum = buffer[2 + len];

          uint8_t sum = type + len;
          for (int i = 0; i < len; i++) sum += data[i];

          if ((sum & 0xFF) == checksum) {
            handlePacket(type, data, len);
            lastSerialActivity = millis();
            return;
          } else {
              sendCommand(0x99);
              connected = false;
              return;
          }
            sendCommand(0x99);
          return;
        }
      }
    }
  }
}

// Assign Data

int FalconBMSArduinoConnector::getECMStatus(int ecmLight){

uint32_t light = ecm[ecmLight];
switch(light){
  case ECM_UNPRESSED_NO_LIT:
  return 0;
  break;
  case ECM_UNPRESSED_ALL_LIT:
  return 1;
  break;
  case ECM_PRESSED_NO_LIT:
  return 2;
  break;
  case ECM_PRESSED_STANDBY:
  return 3;
  break;
  case ECM_PRESSED_ACTIVE:
  return 4;
  break;
  case ECM_PRESSED_TRANSMIT:
  return 5;
  break;
  case ECM_PRESSED_FAIL:
  return 6;
  break;
  case ECM_PRESSED_ALL_LIT:
  return 7;
  break;
  default:
  return 0;
  break;
}

}

void FalconBMSArduinoConnector::setInstrLights(){
  //check instrlights  0 = off, 1 = dim , 2 = bright
  if(instrLight  == INSTR_LIGHT_DIM){
    instrLightStatus = 1;
  } else if(instrLight  == INSTR_LIGHT_BRT){
    instrLightStatus = 2;
  }else {
    instrLightStatus = 0;
  }
  InstrumentLighting = instrLightStatus;
}

int FalconBMSArduinoConnector::getInstrLightStatus(){
  return instrLightStatus;
}

void FalconBMSArduinoConnector::setFloodConsole(){
  // if(floodConsole == FLOOD_CONSOLE_OFF){
  //   floodConsoleStatus = 0;
  // } 
  switch (floodConsole){
        case FLOOD_CONSOLE_OFF:
          floodConsoleStatus =0;
        break;
        case FLOOD_CONSOLE_1:
          floodConsoleStatus =1;
        break;
        case FLOOD_CONSOLE_2:
          floodConsoleStatus =2;
        break;
        case FLOOD_CONSOLE_3:
          floodConsoleStatus =3;
        break;
        case FLOOD_CONSOLE_4:
          floodConsoleStatus =4;
        break;
        case FLOOD_CONSOLE_5:
          floodConsoleStatus =5;
        break;
        case FLOOD_CONSOLE_6:
          floodConsoleStatus =6;
        break;
        default:
        floodConsoleStatus =0;
  }
  FloodConsoleLighting = floodConsoleStatus;
}

int FalconBMSArduinoConnector::getFloodConsoleStatus(){
  return floodConsoleStatus;
}

void FalconBMSArduinoConnector::decodePFL(uint8_t* data, uint8_t len) {
  if (len < 120) return; // Must be 5 lines x 24 chars

  for (int i = 0; i < 5; i++) {
    memcpy(pflLines[i], &data[i * 24], 24);
    pflLines[i][24] = '\0'; // Null-terminate
  }

}

void FalconBMSArduinoConnector::decodeDED(uint8_t* data, uint8_t len) {
  if (len < 120) return; // Must be 5 lines x 24 chars

  for (int i = 0; i < 5; i++) {
    memcpy(dedLines[i], &data[i * 24], 24);
    dedLines[i][24] = '\0'; // Null-terminate
  }

}


void FalconBMSArduinoConnector::checkAllLights(){
    getLightBits(1);
    getLightBits(2);
    getLightBits(3);
    getblinkBits(); //blink
    getECMBits();
}

void FalconBMSArduinoConnector::checkLightBits() {

  if(lightBits == 0xBFFFFFEF){
      isAllLampBits = true;
  } else {
      isAllLampBits = false;
  }

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
  _bits[32] = lightBits & AllLampBitsOn;
}

void FalconBMSArduinoConnector::checkLightBits2() {
  _bits2[0] = lightBits2 & HandOff;
  _bits2[1] = lightBits2 & Launch;
  _bits2[2] = lightBits2 & PriMode;
  _bits2[3] = lightBits2 & Naval;
  _bits2[4] = lightBits2 & Unk;
  _bits2[5] = lightBits2 & TgtSep;
  _bits2[6] = lightBits2 & Go;
  _bits2[7] = lightBits2 & NoGo;
  _bits2[8] = lightBits2 & Degr;
  _bits2[9] = lightBits2 & Rdy;
  _bits2[10] = lightBits2 & ChaffLo;
  _bits2[11] = lightBits2 & FlareLo;
  _bits2[12] = lightBits2 & AuxSrch;
  _bits2[13] = lightBits2 & AuxAct;
  _bits2[14] = lightBits2 & AuxLow;
  _bits2[15] = lightBits2 & AuxPwr;
  _bits2[16] = lightBits2 & EcmPwr;
  _bits2[17] = lightBits2 & EcmFail;
  _bits2[18] = lightBits2 & FwdFuelLow;
  _bits2[19] = lightBits2 & AftFuelLow;
  _bits2[20] = lightBits2 & EPUOn;
  _bits2[21] = lightBits2 & JFSOn;
  _bits2[22] = lightBits2 & SEC;
  _bits2[23] = lightBits2 & OXY_LOW;
  _bits2[24] = lightBits2 & PROBEHEAT;
  _bits2[25] = lightBits2 & SEAT_ARM;
  _bits2[26] = lightBits2 & BUC;
  _bits2[27] = lightBits2 & FUEL_OIL_HOT;
  _bits2[28] = lightBits2 & ANTI_SKID;
  _bits2[29] = lightBits2 & TFR_ENGAGED;
  _bits2[30] = lightBits2 & GEARHANDLE;
  _bits2[31] = lightBits2 & ENGINE;
}

void FalconBMSArduinoConnector::checkLightBits3() {
  _bits3[0]  = lightBits3 & FlcsPmg;
  _bits3[1]  = lightBits3 & MainGen;
  _bits3[2]  = lightBits3 & StbyGen;
  _bits3[3]  = lightBits3 & EpuGen;
  _bits3[4]  = lightBits3 & EpuPmg;
  _bits3[5]  = lightBits3 & ToFlcs;
  _bits3[6]  = lightBits3 & FlcsRly;
  _bits3[7]  = lightBits3 & BatFail;
  _bits3[8]  = lightBits3 & Hydrazine;
  _bits3[9]  = lightBits3 & Air;
  _bits3[10] = lightBits3 & Elec_Fault;
  _bits3[11] = lightBits3 & Lef_Fault;
  _bits3[12] = lightBits3 & OnGround3;
  _bits3[13] = lightBits3 & FlcsBitRun;
  _bits3[14] = lightBits3 & FlcsBitFail;
  _bits3[15] = lightBits3 & DbuWarn;
  _bits3[16] = lightBits3 & NoseGearDown;
  _bits3[17] = lightBits3 & LeftGearDown;
  _bits3[18] = lightBits3 & RightGearDown;
  _bits3[19] = lightBits3 & ParkBrakeOn;
  _bits3[20] = lightBits3 & Power_Off;
  _bits3[21] = lightBits3 & Cadc;
  _bits3[22] = lightBits3 & SpeedBrake;
  _bits3[23] = lightBits3 & SysTest;
  _bits3[24] = lightBits3 & MCAnnounced;
  _bits3[25] = lightBits3 & MLGWOW;
  _bits3[26] = lightBits3 & NLGWOW;
  _bits3[27] = lightBits3 & ATF_Not_Engaged;
  _bits3[28] = lightBits3 & Inlet_Icing;
}

void FalconBMSArduinoConnector::checkBlinkBits() {
  _blinkBits[0] = blinkBits & B_OuterMarker;
  _blinkBits[1] = blinkBits & B_MiddleMarker;
  _blinkBits[2] = blinkBits & B_PROBEHEAT;
  _blinkBits[3] = blinkBits & B_AuxSrch;
  _blinkBits[4] = blinkBits & B_Launch;
  _blinkBits[5] = blinkBits & B_PriMode;
  _blinkBits[6] = blinkBits & B_Unk;
  _blinkBits[7] = blinkBits & B_Elec_Fault;
  _blinkBits[8] = blinkBits & B_OXY_BROW;
  _blinkBits[9] = blinkBits & B_EPUOn;
  _blinkBits[10] = blinkBits & B_JFSOn_Slow;
  _blinkBits[11] = blinkBits & B_JFSOn_Fast;
  _blinkBits[12] = blinkBits & B_ECM_Oper;
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
DEFINE_GETTER(isAllLampBitsOn, 32)

#define DEFINE_GETTER2(name, index) bool FalconBMSArduinoConnector::name() { return _bits2[index]; }

DEFINE_GETTER2(isHandOff, 0)
DEFINE_GETTER2(isLaunch, 1)
DEFINE_GETTER2(isPriMode, 2)
DEFINE_GETTER2(isNaval, 3)
DEFINE_GETTER2(isUnk, 4)
DEFINE_GETTER2(isTgtSep, 5)
DEFINE_GETTER2(isGo, 6)
DEFINE_GETTER2(isNoGo, 7)
DEFINE_GETTER2(isDegr, 8)
DEFINE_GETTER2(isRdy, 9)
DEFINE_GETTER2(isChaffLo, 10)
DEFINE_GETTER2(isFlareLo, 11)
DEFINE_GETTER2(isAuxSrch, 12)
DEFINE_GETTER2(isAuxAct, 13)
DEFINE_GETTER2(isAuxLow, 14)
DEFINE_GETTER2(isAuxPwr, 15)
DEFINE_GETTER2(isEcmPwr, 16)
DEFINE_GETTER2(isEcmFail, 17)
DEFINE_GETTER2(isFwdFuelLow, 18)
DEFINE_GETTER2(isAftFuelLow, 19)
DEFINE_GETTER2(isEPUOn, 20)
DEFINE_GETTER2(isJFSOn, 21)
DEFINE_GETTER2(isSEC, 22)
DEFINE_GETTER2(isOxyLow, 23)
DEFINE_GETTER2(isProbeHeat, 24)
DEFINE_GETTER2(isSeatArm, 25)
DEFINE_GETTER2(isBUC, 26)
DEFINE_GETTER2(isFuelOilHot, 27)
DEFINE_GETTER2(isAntiSkid, 28)
DEFINE_GETTER2(isTFREngaged, 29)
DEFINE_GETTER2(isGearHandle, 30)
DEFINE_GETTER2(isEngine, 31)

#define DEFINE_GETTER3(name, index) bool FalconBMSArduinoConnector::name() { return _bits3[index]; }

DEFINE_GETTER3(isFlcsPmg, 0)
DEFINE_GETTER3(isMainGen, 1)
DEFINE_GETTER3(isStbyGen, 2)
DEFINE_GETTER3(isEpuGen, 3)
DEFINE_GETTER3(isEpuPmg, 4)
DEFINE_GETTER3(isToFlcs, 5)
DEFINE_GETTER3(isFlcsRly, 6)
DEFINE_GETTER3(isBatFail, 7)
DEFINE_GETTER3(isHydrazine, 8)
DEFINE_GETTER3(isAir, 9)
DEFINE_GETTER3(isElecFault, 10)
DEFINE_GETTER3(isLefFault, 11)
DEFINE_GETTER3(isOnGround3, 12)
DEFINE_GETTER3(isFlcsBitRun, 13)
DEFINE_GETTER3(isFlcsBitFail, 14)
DEFINE_GETTER3(isDbuWarn, 15)
DEFINE_GETTER3(isNoseGearDown, 16)
DEFINE_GETTER3(isLeftGearDown, 17)
DEFINE_GETTER3(isRightGearDown, 18)
DEFINE_GETTER3(isParkBrakeOn, 19)
DEFINE_GETTER3(isPowerOff, 20)
DEFINE_GETTER3(isCadc, 21)
DEFINE_GETTER3(isSpeedBrake, 22)
DEFINE_GETTER3(isSysTest, 23)
DEFINE_GETTER3(isMCAnnounced, 24)
DEFINE_GETTER3(isMLGWOW, 25)
DEFINE_GETTER3(isNLGWOW, 26)
DEFINE_GETTER3(isATFNotEngaged, 27)
DEFINE_GETTER3(isInletIcing, 28)

#define DEFINE_BLINK_GETTER(name, index) bool FalconBMSArduinoConnector::name() { return _blinkBits[index]; }

DEFINE_BLINK_GETTER(isOuterMarkerBlinking, 0)
DEFINE_BLINK_GETTER(isMiddleMarkerBlinking, 1)
DEFINE_BLINK_GETTER(isProbeHeatBlinking, 2)
DEFINE_BLINK_GETTER(isAuxSrchBlinking, 3)
DEFINE_BLINK_GETTER(isLaunchBlinking, 4)
DEFINE_BLINK_GETTER(isPriModeBlinking, 5)
DEFINE_BLINK_GETTER(isUnkBlinking, 6)
DEFINE_BLINK_GETTER(isElecFaultBlinking, 7)
DEFINE_BLINK_GETTER(isOxyBrowBlinking, 8)
DEFINE_BLINK_GETTER(isEPUOnBlinking, 9)
DEFINE_BLINK_GETTER(isJFSOnSlowBlinking, 10)
DEFINE_BLINK_GETTER(isJFSOnFastBlinking, 11)
DEFINE_BLINK_GETTER(isECMOperBlinking, 12)


