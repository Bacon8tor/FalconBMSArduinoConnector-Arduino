#ifndef BLINKBITS_H
#define BLINKBITS_H

#include <stdint.h>

// BLINKING LIGHTS - Only indicating if a lamp is blinking
// This enum mimics the bitflags used in Falcon BMS shared memory (BlinkBits)

enum BlinkBits : uint32_t {
    // Currently working
    B_OuterMarker   = 0x00000001, // Slow flashing for outer marker
    B_MiddleMarker  = 0x00000002, // Fast flashing for middle marker
    B_PROBEHEAT     = 0x00000004, // Probe heat system is tested
    B_AuxSrch       = 0x00000008, // Search radar is painting ownship
    B_Launch        = 0x00000010, // Missile is fired at ownship
    B_PriMode       = 0x00000020, // Priority mode with >5 emitters
    B_Unk           = 0x00000040, // Unknown radar detection

    // Not working yet / defined for future use
    B_Elec_Fault    = 0x00000080, // Non-resetting fault
    B_OXY_BROW      = 0x00000100, // Monitor fault during OBOGS
    B_EPUOn         = 0x00000200, // Abnormal EPU operation

    // Working
    B_JFSOn_Slow    = 0x00000400, // Slow blinking: non-critical failure
    B_JFSOn_Fast    = 0x00000800, // Fast blinking: critical failure

    // V19
    B_ECM_Oper      = 0x00001000  // ECM system warming up
};

#endif // BLINKBITS_H
