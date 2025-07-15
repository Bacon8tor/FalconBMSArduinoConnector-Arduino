#ifndef LIGHTBITS2_H
#define LIGHTBITS2_H

#include <stdint.h>

enum LightBits2 : uint32_t {
  // Threat Warning Prime
  HandOff       = 0x00000001,
  Launch        = 0x00000002,
  PriMode       = 0x00000004,
  Naval         = 0x00000008,
  Unk           = 0x00000010,
  TgtSep        = 0x00000020,

  // EWS
  Go            = 0x00000040,
  NoGo          = 0x00000080,
  Degr          = 0x00000100,
  Rdy           = 0x00000200,
  ChaffLo       = 0x00000400,
  FlareLo       = 0x00000800,

  // Aux Threat Warning
  AuxSrch       = 0x00001000,
  AuxAct        = 0x00002000,
  AuxLow        = 0x00004000,
  AuxPwr        = 0x00008000,

  // ECM
  EcmPwr        = 0x00010000,
  EcmFail       = 0x00020000,

  // Caution Lights
  FwdFuelLow    = 0x00040000,
  AftFuelLow    = 0x00080000,

  // Engine status
  EPUOn         = 0x00100000,
  JFSOn         = 0x00200000,

  // Caution panel
  SEC           = 0x00400000,
  OXY_LOW       = 0x00800000,
  PROBEHEAT     = 0x01000000,
  SEAT_ARM      = 0x02000000,
  BUC           = 0x04000000,
  FUEL_OIL_HOT  = 0x08000000,
  ANTI_SKID     = 0x10000000,

  TFR_ENGAGED   = 0x20000000,
  GEARHANDLE    = 0x40000000,
  ENGINE        = 0x80000000,

  // All Lights On
  AllLampBits2On = 0xFFFFF03F,
  AllLampBits2OnExceptCarapace = 0xFFFFF03F 
                                ^ HandOff 
                                ^ Launch 
                                ^ PriMode 
                                ^ Naval 
                                ^ Unk 
                                ^ TgtSep 
                                ^ AuxSrch 
                                ^ AuxAct 
                                ^ AuxLow 
                                ^ AuxPwr
};

#endif // LIGHTBITS2_H
