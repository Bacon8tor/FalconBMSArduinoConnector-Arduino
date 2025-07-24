#ifndef INSTRLIGHT_H
#define INSTRLIGHT_H

#include <stdint.h>  // Required for 'byte' type

enum InstrLight : byte {
    INSTR_LIGHT_OFF = 0,
    INSTR_LIGHT_DIM = 1,
    INSTR_LIGHT_BRT = 2
};

#endif // INSTRLIGHT_H
