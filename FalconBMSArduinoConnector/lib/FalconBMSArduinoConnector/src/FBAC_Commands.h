#ifndef FBAC_COMMANDS_H
#define FBAC_COMMANDS_H

#include <stdint.h>

// Serial protocol shared with the FBAC PC app (ArduinoConnector.cs -> FbacCommand).
// Keep both lists in sync. IDs are historical: they read like decimal but are hex.
//
// Request : Arduino -> PC   [cmd:u8]
// Response: PC -> Arduino   [0xAA][type:u8][len:u8][data:len][sum8(type+len+data)]
// The response type always equals the request cmd.

#define FBAC_PROTOCOL_VERSION 2

#define FBAC_PACKET_START 0xAA
#define FBAC_HANDSHAKE_REQ 0xA5 // PC -> Arduino on connect
#define FBAC_HANDSHAKE_ACK 0x5A // Arduino -> PC reply

enum FbacCommand : uint8_t
{
  CMD_LIGHTBITS = 0x01,          // u32
  CMD_LIGHTBITS2 = 0x02,         // u32
  CMD_LIGHTBITS3 = 0x03,         // u32
  CMD_BLINKBITS = 0x04,          // u32
  CMD_DED = 0x05,                // 5 x 24 chars
  CMD_FUEL_FLOW = 0x06,          // float
  CMD_INSTR_LIGHT = 0x07,        // u8
  CMD_PFL = 0x08,                // 5 x 24 chars
  CMD_CHAFF_COUNT = 0x09,        // float
  CMD_FLARE_COUNT = 0x10,        // float
  CMD_FLOOD_CONSOLE = 0x11,      // u8
  CMD_RPM = 0x12,                // float
  CMD_ECM_BITS = 0x13,           // N x u32
  CMD_OIL_PRESS = 0x14,          // float
  CMD_OIL_PRESS2 = 0x15,         // float
  CMD_NOZZLE_POS = 0x16,         // float
  CMD_NOZZLE_POS2 = 0x17,        // float
  CMD_FTIT = 0x18,               // float
  CMD_FTIT2 = 0x19,              // float
  CMD_CABIN_ALT = 0x20,          // float
  CMD_KIAS = 0x21,               // float
  CMD_INTERNAL_FUEL = 0x22,      // float
  CMD_EXTERNAL_FUEL = 0x23,      // float
  CMD_EPU_FUEL = 0x24,           // float
  CMD_HYD_PRESS_A = 0x25,        // float
  CMD_HYD_PRESS_B = 0x26,        // float
  CMD_CMDS_MODE = 0x27,          // i32
  CMD_UHF_PRESET = 0x28,         // i32
  CMD_UHF_FREQ = 0x29,           // i32
  CMD_SPEED_BRAKE = 0x30,        // float
  CMD_IFF_M1_D1 = 0x31,          // u8
  CMD_IFF_M1_D2 = 0x32,          // u8
  CMD_IFF_M3_D1 = 0x33,          // u8
  CMD_IFF_M3_D2 = 0x34,          // u8
  CMD_FUEL_FWD = 0x35,           // float
  CMD_FUEL_AFT = 0x36,           // float
  CMD_FUEL_TOTAL = 0x37,         // float
  CMD_DESIRED_COURSE = 0x38,     // float
  CMD_COURSE_DEVIATION = 0x39,   // float
  CMD_DISTANCE_TO_BEACON = 0x40, // float
  CMD_BEARING_TO_BEACON = 0x41,  // float
  CMD_SIM_STATUS = 0x42,         // u32 (SimStatusBits)
  // ---- protocol v2 ----
  CMD_PROTOCOL_VERSION = 0x43,   // [version:u8][flags:u8]
  CMD_ALL_LIGHTS = 0x44,         // lightBits, lightBits2, lightBits3, blinkBits (4 x u32) + ecm N x u32
  // ---- misc ----
  CMD_PING = 0x0F,               // replies [0xAB]
  CMD_UNKNOWN = 0x00             // PC reply type for unsupported commands
};

#endif
