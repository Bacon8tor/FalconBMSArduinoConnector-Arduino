#ifndef LIGHTBITS3_H
#define LIGHTBITS3_H

#include <stdint.h>

enum LightBits3 : uint32_t {
  // Elec panel
  FlcsPmg        = 0x00000001,
  MainGen        = 0x00000002,
  StbyGen        = 0x00000004,
  EpuGen         = 0x00000008,
  EpuPmg         = 0x00000010,
  ToFlcs         = 0x00000020,
  FlcsRly        = 0x00000040,
  BatFail        = 0x00000080,

  // EPU panel
  Hydrazine      = 0x00000100,
  Air            = 0x00000200,

  // Caution panel
  Elec_Fault     = 0x00000400,
  Lef_Fault      = 0x00000800,

  OnGround3      = 0x00001000,  // renamed to avoid clash
  FlcsBitRun     = 0x00002000,
  FlcsBitFail    = 0x00004000,
  DbuWarn        = 0x00008000,

  NoseGearDown   = 0x00010000,
  LeftGearDown   = 0x00020000,
  RightGearDown  = 0x00040000,
  ParkBrakeOn    = 0x00100000,
  Power_Off      = 0x00200000,

  Cadc           = 0x00400000,
  SpeedBrake     = 0x00800000,

  SysTest        = 0x01000000,
  MCAnnounced    = 0x02000000,
  MLGWOW         = 0x04000000,
  NLGWOW         = 0x08000000,
  ATF_Not_Engaged= 0x10000000,
  Inlet_Icing    = 0x20000000,

  AllLampBits3On = 0x3147EFFF,
  AllLampBits3OnExceptCarapace = 0x3147EFFF ^ SysTest
};

#endif // LIGHTBITS3_H