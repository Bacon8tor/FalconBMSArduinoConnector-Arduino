#ifndef INSTRLIGHT_H
#define INSTRLIGHT_H

#include <stdint.h>  // Required for 'byte' type

enum InstrLight : byte {
    INSTR_LIGHT_OFF = 0,
    INSTR_LIGHT_DIM = 1,
    INSTR_LIGHT_BRT = 2
};

enum FloodConsole : byte
{
    FLOOD_CONSOLE_OFF,
    FLOOD_CONSOLE_1,
    FLOOD_CONSOLE_2,
    FLOOD_CONSOLE_3,
    FLOOD_CONSOLE_4,
    FLOOD_CONSOLE_5,
    FLOOD_CONSOLE_6
};

#endif // INSTRLIGHT_H
