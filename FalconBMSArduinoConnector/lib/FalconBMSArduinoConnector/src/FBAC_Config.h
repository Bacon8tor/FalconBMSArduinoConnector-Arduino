#ifndef FBAC_CONFIG_H
#define FBAC_CONFIG_H

#include <Arduino.h>

//SD1322 OLED 256*64 (deduino compatible?? not confirmed pin changes may be needed)
#ifdef FBAC_DED_1322
#include <U8g2lib.h>
#ifndef FBAC_FONT_U8G2
#define FBAC_FONT_U8G2
#endif

// --- Detect board and set appropriate constructor ---
#if defined(ESP32)
U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2_DED(U8G2_R0, /* cs=*/ 5, /* dc=*/ 16, /* reset=*/ 17);
#elif defined(ESP8266)
U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2_DED(U8G2_R0, /* clock=*/14, /* data=*/13, /* cs=*/15, /* dc=*/12, /* reset=*/2);
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) // Uno, Nano
U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI u8g2_DED(U8G2_R0, /* cs=*/10, /* dc=*/9, /* reset=*/8); 
#elif defined(__AVR_ATmega2560__) // Mega 2560
U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2_DED(U8G2_R0, /* cs=*/53, /* dc=*/49, /* reset=*/48); 

#endif

#endif

//SD1309 OLED 128*64
#ifdef FBAC_DED_1309
#include <U8g2lib.h>
#ifndef FBAC_FONT_U8G2
#define FBAC_FONT_U8G2
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_DED(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

//PFL OLED 128*64
#ifdef FBAC_PFL_1309
#include <U8g2lib.h>
#ifndef FBAC_FONT_U8G2
#define FBAC_FONT_U8G2
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_PFL(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

//Fuel Flow OLED 128*64 
#ifdef FBAC_FF_1306
#include <U8g2lib.h>
#ifndef FBAC_FONT_U8G2
#define FBAC_FONT_U8G2
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_FuelFlow(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

//FBAC Fonts 
#ifdef FBAC_FONT_U8G2
  #include "falconded_font.h"
  #define SD1322_FONT u8g2_font_FalconDED_18_tf
  #define SD1306_FONT u8g2_font_FBAC_DED_FONT_5x8_tf
#ifdef U8G2_HAVE_HW_SPI
  #include <SPI.h>
#endif
#ifdef U8G2_HAVE_HW_I2C
  #include <Wire.h>
#endif
#endif

//Fastled library 
#ifdef FBAC_FASTLED
#include <FastLED.h>
#endif


#endif