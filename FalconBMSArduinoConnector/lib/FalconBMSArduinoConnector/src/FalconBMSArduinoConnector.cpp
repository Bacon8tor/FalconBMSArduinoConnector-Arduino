#include "FalconBMSArduinoConnector.h"
#include <Arduino.h>

FalconBMSArduinoConnector::FalconBMSArduinoConnector()
    : lastSerialActivity(0),
      protocolVersion(0),
      _serial(&Serial),
      lightBits(0),
      lightBits2(0),
      lightBits3(0),
      blinkBits(0),
      instrLight(0),
      connected(false),
      consecutiveFailures(0),
      lastPingMs(0),
      requestTimeoutMs(250)
{
  memset(_bits, 0, sizeof(_bits));
  memset(_bits2, 0, sizeof(_bits2));
  memset(_bits3, 0, sizeof(_bits3));
  memset(_blinkBits, 0, sizeof(_blinkBits));
  memset(ecm, 0, sizeof(ecm));
  memset(dedLines, 0, sizeof(dedLines));
  memset(pflLines, 0, sizeof(pflLines));
}

void FalconBMSArduinoConnector::begin(Stream &serial, uint32_t baud)
{
  (void)baud;
  _serial = &serial;
  // Do NOT call begin() on _serial here
  // Do NOT wait for serial connection here
}

bool FalconBMSArduinoConnector::isConnected()
{
  return connected;
}

void FalconBMSArduinoConnector::markDisconnected()
{
  connected = false;
  protocolVersion = 0;
  consecutiveFailures = 0;
  lastPingMs = millis();
}

void FalconBMSArduinoConnector::update()
{
  unsigned long now = millis();

  if (!connected)
  {
    // PC app opened the port and is asking for the handshake
    while (_serial->available())
    {
      if (_serial->read() == FBAC_HANDSHAKE_REQ)
      {
        _serial->write(FBAC_HANDSHAKE_ACK);
        connected = true;
        consecutiveFailures = 0;
        protocolVersion = 0;
        lastSerialActivity = now;
        break;
      }
    }

    // The PC may still be connected even though we timed out (for example the
    // sketch was busy for a while). Ping it to recover without a new handshake.
    if (!connected && now - lastPingMs >= reconnectPingMs)
    {
      lastPingMs = now;
      if (_serial->available() == 0 && request(CMD_PING, true) == REQ_OK)
      {
        connected = true;
        protocolVersion = 0;
      }
    }
  }

  if (!connected)
    return;

  // Find out once per connection which protocol the PC app speaks
  if (protocolVersion == 0)
  {
    RequestResult r = request(CMD_PROTOCOL_VERSION);
    if (r == REQ_UNSUPPORTED)
      protocolVersion = 1;
  }

  // Keep-alive: notice a vanished PC even if the sketch requests nothing
  if (connected && millis() - lastSerialActivity > keepAliveMs)
  {
    request(CMD_PING);
  }
}

// Fetch Data
void FalconBMSArduinoConnector::getLightBits(int lb)
{
  switch (lb)
  {
  case 1:
    request(CMD_LIGHTBITS);
    break;
  case 2:
    request(CMD_LIGHTBITS2);
    break;
  case 3:
    request(CMD_LIGHTBITS3);
    break;
  default:
    break;
  }
}

void FalconBMSArduinoConnector::getblinkBits() { request(CMD_BLINKBITS); }
void FalconBMSArduinoConnector::getDED() { request(CMD_DED); }
void FalconBMSArduinoConnector::getFuelFlow() { request(CMD_FUEL_FLOW); }
void FalconBMSArduinoConnector::getInstrLight() { request(CMD_INSTR_LIGHT); }
void FalconBMSArduinoConnector::getPFL() { request(CMD_PFL); }

void FalconBMSArduinoConnector::getChaffFlareCount()
{
  request(CMD_CHAFF_COUNT);
  request(CMD_FLARE_COUNT);
}

void FalconBMSArduinoConnector::getFloodConsole() { request(CMD_FLOOD_CONSOLE); }
void FalconBMSArduinoConnector::getRPM() { request(CMD_RPM); }
void FalconBMSArduinoConnector::getECMBits() { request(CMD_ECM_BITS); }
void FalconBMSArduinoConnector::getOilPressure() { request(CMD_OIL_PRESS); }
void FalconBMSArduinoConnector::getOilPressure2() { request(CMD_OIL_PRESS2); }
void FalconBMSArduinoConnector::getNozzlePos() { request(CMD_NOZZLE_POS); }
void FalconBMSArduinoConnector::getNozzlePos2() { request(CMD_NOZZLE_POS2); }
void FalconBMSArduinoConnector::getFTIT() { request(CMD_FTIT); }
void FalconBMSArduinoConnector::getFTIT2() { request(CMD_FTIT2); }
void FalconBMSArduinoConnector::getCabinAlt() { request(CMD_CABIN_ALT); }
void FalconBMSArduinoConnector::getKIAS() { request(CMD_KIAS); }
void FalconBMSArduinoConnector::getinternalFuel() { request(CMD_INTERNAL_FUEL); }
void FalconBMSArduinoConnector::getexternalFuel() { request(CMD_EXTERNAL_FUEL); }
void FalconBMSArduinoConnector::getEPUFuel() { request(CMD_EPU_FUEL); }

void FalconBMSArduinoConnector::getHYDPress()
{
  request(CMD_HYD_PRESS_A);
  request(CMD_HYD_PRESS_B);
}

// CmdsOFF = 0, CmdsSTBY = 1, CmdsMAN = 2, CmdsSEMI = 3, CmdsAUTO = 4, CmdsBYP = 5
void FalconBMSArduinoConnector::getCMDSMode() { request(CMD_CMDS_MODE); }
void FalconBMSArduinoConnector::getuhfPreset() { request(CMD_UHF_PRESET); }
void FalconBMSArduinoConnector::getuhfFreq() { request(CMD_UHF_FREQ); }
void FalconBMSArduinoConnector::getSpeedBrake() { request(CMD_SPEED_BRAKE); }
void FalconBMSArduinoConnector::getIFFMode1Digit1() { request(CMD_IFF_M1_D1); }
void FalconBMSArduinoConnector::getIFFMode1Digit2() { request(CMD_IFF_M1_D2); }
void FalconBMSArduinoConnector::getIFFMode3Digit1() { request(CMD_IFF_M3_D1); }
void FalconBMSArduinoConnector::getIFFMode3Digit2() { request(CMD_IFF_M3_D2); }
void FalconBMSArduinoConnector::getfwd() { request(CMD_FUEL_FWD); }
void FalconBMSArduinoConnector::getaft() { request(CMD_FUEL_AFT); }
void FalconBMSArduinoConnector::gettotalFuel() { request(CMD_FUEL_TOTAL); }
void FalconBMSArduinoConnector::getdesiredCourse() { request(CMD_DESIRED_COURSE); }
void FalconBMSArduinoConnector::getcourseDeviation() { request(CMD_COURSE_DEVIATION); }
void FalconBMSArduinoConnector::getdistanceToBeacon() { request(CMD_DISTANCE_TO_BEACON); }
void FalconBMSArduinoConnector::getbearingToBeacon() { request(CMD_BEARING_TO_BEACON); }
void FalconBMSArduinoConnector::getSimStatus() { request(CMD_SIM_STATUS); }

// ---------------------------------------------------------------------------
// Packet handling
// ---------------------------------------------------------------------------

void FalconBMSArduinoConnector::drainInput()
{
  // Throw away anything left over (late replies to timed-out requests, noise)
  // so the next packet we read is the answer to the request we are about to send.
  while (_serial->available())
    _serial->read();
}

FalconBMSArduinoConnector::RequestResult FalconBMSArduinoConnector::request(uint8_t cmd, bool force)
{
  if (!connected && !force)
    return REQ_FAIL;

  drainInput();
  _serial->write(cmd);
  RequestResult result = waitForPacket(cmd);

  if (result == REQ_FAIL)
  {
    if (connected && ++consecutiveFailures >= maxFailures)
      markDisconnected();
  }
  else
  {
    consecutiveFailures = 0;
    lastSerialActivity = millis();
  }
  return result;
}

FalconBMSArduinoConnector::RequestResult FalconBMSArduinoConnector::waitForPacket(uint8_t expectedType)
{
  const uint8_t maxData = sizeof(buffer) - 3;
  unsigned long start = millis();
  bool reading = false;
  uint8_t idx = 0;

  while (millis() - start < requestTimeoutMs)
  {
    if (!_serial->available())
    {
      yield();
      continue;
    }

    uint8_t b = _serial->read();

    if (!reading)
    {
      if (b == FBAC_PACKET_START)
      {
        reading = true;
        idx = 0;
      }
      continue;
    }

    buffer[idx++] = b;

    if (idx == 2 && buffer[1] > maxData)
    {
      // Impossible length: we locked onto a stray 0xAA. Resync.
      reading = false;
      continue;
    }

    if (idx < 2 || idx != (uint8_t)(3 + buffer[1]))
      continue;

    // Full packet received
    reading = false;
    uint8_t type = buffer[0];
    uint8_t len = buffer[1];
    uint8_t *data = &buffer[2];
    uint8_t checksum = buffer[2 + len];

    uint8_t sum = type + len;
    for (uint8_t i = 0; i < len; i++)
      sum += data[i];

    if (sum != checksum)
      return REQ_FAIL;

    // FBAC PC app <= 0.5 answers 0x41 with type 0x40 by mistake
    if (expectedType == CMD_BEARING_TO_BEACON && type == CMD_DISTANCE_TO_BEACON && protocolVersion < 2)
      type = CMD_BEARING_TO_BEACON;

    if (type == expectedType)
    {
      handlePacket(type, data, len);
      return REQ_OK;
    }

    if (type == CMD_UNKNOWN)
      return REQ_UNSUPPORTED;

    // A packet for some other request: ignore it and keep waiting
  }
  return REQ_FAIL;
}

// Copies a field from the packet. Never writes more than `size` bytes;
// zero-fills when the PC sends fewer bytes than expected.
bool FalconBMSArduinoConnector::copyField(void *dst, size_t size, const uint8_t *data, uint8_t len)
{
  size_t n = len < size ? len : size;
  memcpy(dst, data, n);
  if (n < size)
    memset((uint8_t *)dst + n, 0, size - n);
  return len >= size;
}

void FalconBMSArduinoConnector::handlePacket(uint8_t type, uint8_t *data, uint8_t len)
{
  switch (type)
  {
  case CMD_LIGHTBITS:
    copyField(&lightBits, sizeof(lightBits), data, len);
    checkLightBits();
    break;
  case CMD_LIGHTBITS2:
    copyField(&lightBits2, sizeof(lightBits2), data, len);
    checkLightBits2();
    break;
  case CMD_LIGHTBITS3:
    copyField(&lightBits3, sizeof(lightBits3), data, len);
    checkLightBits3();
    break;
  case CMD_BLINKBITS:
    copyField(&blinkBits, sizeof(blinkBits), data, len);
    checkBlinkBits();
    break;
  case CMD_DED:
    decodeDED(data, len);
    break;
  case CMD_FUEL_FLOW:
    copyField(&fuelFlow, sizeof(fuelFlow), data, len);
    break;
  case CMD_INSTR_LIGHT:
    copyField(&instrLight, sizeof(instrLight), data, len);
    setInstrLights();
    break;
  case CMD_PFL:
    decodePFL(data, len);
    break;
  case CMD_CHAFF_COUNT:
    copyField(&chaffCount, sizeof(chaffCount), data, len);
    break;
  case CMD_FLARE_COUNT:
    copyField(&flareCount, sizeof(flareCount), data, len);
    break;
  case CMD_FLOOD_CONSOLE:
    copyField(&floodConsole, sizeof(floodConsole), data, len);
    setFloodConsole();
    break;
  case CMD_RPM:
    copyField(&rpm, sizeof(rpm), data, len);
    break;
  case CMD_ECM_BITS:
    memset(ecm, 0, sizeof(ecm));
    copyField(ecm, sizeof(ecm) < len ? sizeof(ecm) : len, data, len);
    break;
  case CMD_OIL_PRESS:
    copyField(&oilPress, sizeof(oilPress), data, len);
    break;
  case CMD_OIL_PRESS2:
    copyField(&oilPress2, sizeof(oilPress2), data, len);
    break;
  case CMD_NOZZLE_POS:
    copyField(&nozzlePos, sizeof(nozzlePos), data, len);
    break;
  case CMD_NOZZLE_POS2:
    copyField(&nozzlePos2, sizeof(nozzlePos2), data, len);
    break;
  case CMD_FTIT:
    copyField(&ftit, sizeof(ftit), data, len);
    break;
  case CMD_FTIT2:
    copyField(&ftit2, sizeof(ftit2), data, len);
    break;
  case CMD_CABIN_ALT:
    copyField(&cabinAlt, sizeof(cabinAlt), data, len);
    break;
  case CMD_KIAS:
    copyField(&kias, sizeof(kias), data, len);
    break;
  case CMD_INTERNAL_FUEL:
    copyField(&internalFuel, sizeof(internalFuel), data, len);
    break;
  case CMD_EXTERNAL_FUEL:
    copyField(&externalFuel, sizeof(externalFuel), data, len);
    break;
  case CMD_EPU_FUEL:
    copyField(&epuFuel, sizeof(epuFuel), data, len);
    break;
  case CMD_HYD_PRESS_A:
    copyField(&hydPressA, sizeof(hydPressA), data, len);
    break;
  case CMD_HYD_PRESS_B:
    copyField(&hydPressB, sizeof(hydPressB), data, len);
    break;
  case CMD_CMDS_MODE:
    copyField(&cmdsMode, sizeof(cmdsMode), data, len);
    break;
  case CMD_UHF_PRESET:
    copyField(&uhfPreset, sizeof(uhfPreset), data, len);
    break;
  case CMD_UHF_FREQ:
    copyField(&uhfFreq, sizeof(uhfFreq), data, len);
    break;
  case CMD_SPEED_BRAKE:
    copyField(&speedBrake, sizeof(speedBrake), data, len);
    break;
  case CMD_IFF_M1_D1:
    copyField(&IFFMode1Digit1_byte, 1, data, len);
    IFFMode1Digit1 = IFFMode1Digit1_byte;
    break;
  case CMD_IFF_M1_D2:
    copyField(&IFFMode1Digit2_byte, 1, data, len);
    IFFMode1Digit2 = IFFMode1Digit2_byte;
    break;
  case CMD_IFF_M3_D1:
    copyField(&IFFMode3Digit1_byte, 1, data, len);
    IFFMode3Digit1 = IFFMode3Digit1_byte;
    break;
  case CMD_IFF_M3_D2:
    copyField(&IFFMode3Digit2_byte, 1, data, len);
    IFFMode3Digit2 = IFFMode3Digit2_byte;
    break;
  case CMD_FUEL_FWD:
    copyField(&fwd, sizeof(fwd), data, len);
    break;
  case CMD_FUEL_AFT:
    copyField(&aft, sizeof(aft), data, len);
    break;
  case CMD_FUEL_TOTAL:
    copyField(&totalFuel, sizeof(totalFuel), data, len);
    break;
  case CMD_DESIRED_COURSE:
    copyField(&desiredCourse, sizeof(desiredCourse), data, len);
    break;
  case CMD_COURSE_DEVIATION:
    copyField(&courseDeviation, sizeof(courseDeviation), data, len);
    break;
  case CMD_DISTANCE_TO_BEACON:
    copyField(&distanceToBearing, sizeof(distanceToBearing), data, len);
    break;
  case CMD_BEARING_TO_BEACON:
    copyField(&bearingToBearing, sizeof(bearingToBearing), data, len);
    break;
  case CMD_SIM_STATUS:
    copyField(&simStatus, sizeof(simStatus), data, len);
    break;
  case CMD_PROTOCOL_VERSION:
    protocolVersion = (len >= 1 && data[0] >= 1) ? data[0] : 1;
    break;
  case CMD_ALL_LIGHTS:
    if (len >= 16)
    {
      memcpy(&lightBits, &data[0], 4);
      memcpy(&lightBits2, &data[4], 4);
      memcpy(&lightBits3, &data[8], 4);
      memcpy(&blinkBits, &data[12], 4);
      checkLightBits();
      checkLightBits2();
      checkLightBits3();
      checkBlinkBits();
      memset(ecm, 0, sizeof(ecm));
      uint8_t ecmBytes = len - 16;
      copyField(ecm, sizeof(ecm) < ecmBytes ? sizeof(ecm) : ecmBytes, &data[16], ecmBytes);
    }
    break;
  default:
    break;
  }
}

// Assign Data

int FalconBMSArduinoConnector::getECMStatus(int ecmLight)
{

  if (ecmLight < 0 || ecmLight >= FBAC_MAX_ECM)
    return 0;
  uint32_t light = ecm[ecmLight];
  switch (light)
  {
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

void FalconBMSArduinoConnector::setInstrLights()
{
  // check instrlights  0 = off, 1 = dim , 2 = bright
  if (instrLight == INSTR_LIGHT_DIM)
  {
    instrLightStatus = 1;
  }
  else if (instrLight == INSTR_LIGHT_BRT)
  {
    instrLightStatus = 2;
  }
  else
  {
    instrLightStatus = 0;
  }
  InstrumentLighting = instrLightStatus;
}

int FalconBMSArduinoConnector::getInstrLightStatus()
{
  return instrLightStatus;
}

void FalconBMSArduinoConnector::setFloodConsole()
{
  // if(floodConsole == FLOOD_CONSOLE_OFF){
  //   floodConsoleStatus = 0;
  // }
  switch (floodConsole)
  {
  case FLOOD_CONSOLE_OFF:
    floodConsoleStatus = 0;
    break;
  case FLOOD_CONSOLE_1:
    floodConsoleStatus = 1;
    break;
  case FLOOD_CONSOLE_2:
    floodConsoleStatus = 2;
    break;
  case FLOOD_CONSOLE_3:
    floodConsoleStatus = 3;
    break;
  case FLOOD_CONSOLE_4:
    floodConsoleStatus = 4;
    break;
  case FLOOD_CONSOLE_5:
    floodConsoleStatus = 5;
    break;
  case FLOOD_CONSOLE_6:
    floodConsoleStatus = 6;
    break;
  default:
    floodConsoleStatus = 0;
  }
  FloodConsoleLighting = floodConsoleStatus;
}

int FalconBMSArduinoConnector::getFloodConsoleStatus()
{
  return floodConsoleStatus;
}

void FalconBMSArduinoConnector::decodePFL(uint8_t *data, uint8_t len)
{
  if (len < 120)
    return; // Must be 5 lines x 24 chars

  for (int i = 0; i < 5; i++)
  {
    memcpy(pflLines[i], &data[i * 24], 24);
    pflLines[i][24] = '\0'; // Null-terminate
  }
}

void FalconBMSArduinoConnector::decodeDED(uint8_t *data, uint8_t len)
{
  if (len < 120)
    return; // Must be 5 lines x 24 chars

  for (int i = 0; i < 5; i++)
  {
    memcpy(dedLines[i], &data[i * 24], 24);
    dedLines[i][24] = '\0'; // Null-terminate
  }
}

void FalconBMSArduinoConnector::checkAllLights()
{
  // Protocol v2: one round trip for all lamp words + ECM
  if (protocolVersion >= 2 && request(CMD_ALL_LIGHTS) != REQ_UNSUPPORTED)
    return;

  getLightBits(1);
  getLightBits(2);
  getLightBits(3);
  getblinkBits(); // blink
  getECMBits();
}

void FalconBMSArduinoConnector::checkLightBits()
{

  isAllLampBits = (lightBits == AllLampBitsOn);

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

void FalconBMSArduinoConnector::checkLightBits2()
{
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

void FalconBMSArduinoConnector::checkLightBits3()
{
  _bits3[0] = lightBits3 & FlcsPmg;
  _bits3[1] = lightBits3 & MainGen;
  _bits3[2] = lightBits3 & StbyGen;
  _bits3[3] = lightBits3 & EpuGen;
  _bits3[4] = lightBits3 & EpuPmg;
  _bits3[5] = lightBits3 & ToFlcs;
  _bits3[6] = lightBits3 & FlcsRly;
  _bits3[7] = lightBits3 & BatFail;
  _bits3[8] = lightBits3 & Hydrazine;
  _bits3[9] = lightBits3 & Air;
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

void FalconBMSArduinoConnector::checkBlinkBits()
{
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
#define DEFINE_GETTER(name, index) \
  bool FalconBMSArduinoConnector::name() { return _bits[index]; }

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
bool FalconBMSArduinoConnector::isAllLampBitsOn() { return isAllLampBits; }

#define DEFINE_GETTER2(name, index) \
  bool FalconBMSArduinoConnector::name() { return _bits2[index]; }

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

#define DEFINE_GETTER3(name, index) \
  bool FalconBMSArduinoConnector::name() { return _bits3[index]; }

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

#define DEFINE_BLINK_GETTER(name, index) \
  bool FalconBMSArduinoConnector::name() { return _blinkBits[index]; }

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

bool FalconBMSArduinoConnector::isIn3D()
{
  return (simStatus & SimStatusBits::STATUS_IN_3D) != 0;
}