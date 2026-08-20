#ifndef SIMSTATUSBITS_H
#define SIMSTATUSBITS_H

#include <stdint.h>

enum SimStatusBits : uint32_t
{
    STATUS_IN_3D = 0x01,
    // Room for 31 more bits here if needed
};

#endif