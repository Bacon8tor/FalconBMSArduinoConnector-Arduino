#ifndef LIGHTBITS_H
#define LIGHTBITS_H

#include <stdint.h>

enum LightBits : uint32_t {
  MasterCaution = 0x00000001,

  // Brow Lights
  TF            = 0x00000002,
  OXY_BROW      = 0x00000004,
  EQUIP_HOT     = 0x00000008,
  ONGROUND      = 0x00000010,
  ENG_FIRE      = 0x00000020,
  CONFIG        = 0x00000040,
  HYD           = 0x00000080,
  Flcs_ABCD     = 0x00000100,
  FLCS          = 0x00000200,
  CAN           = 0x00000400,
  T_L_CFG       = 0x00000800,

  // AOA Indexers
  AOAAbove      = 0x00001000,
  AOAOn         = 0x00002000,
  AOABelow      = 0x00004000,

  // Refuel/NWS
  RefuelRDY     = 0x00008000,
  RefuelAR      = 0x00010000,
  RefuelDSC     = 0x00020000,

  // Caution Lights
  FltControlSys = 0x00040000,
  LEFlaps       = 0x00080000,
  EngineFault   = 0x00100000,
  Overheat      = 0x00200000,
  FuelLow       = 0x00400000,
  Avionics      = 0x00800000,
  RadarAlt      = 0x01000000,
  IFF           = 0x02000000,
  ECM           = 0x04000000,
  Hook          = 0x08000000,
  NWSFail       = 0x10000000,
  CabinPress    = 0x20000000,

  AutoPilotOn   = 0x40000000,
  TFR_STBY      = 0x80000000,

  AllLampBitsOn = 0xBFFFFFEF
};

#endif // LIGHTBITS_H
