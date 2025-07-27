#define FBAC_DED_1322

#include <Arduino.h>
#include <FalconBMSArduinoConnector.h>
#include <Joystick.h>
#include <Wire.h>
#include <PCF8575.h>

FalconBMSArduinoConnector bms;

// Create Joystick with 32 buttons, no axes
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_GAMEPAD,
                   32, 0,  // Buttons, Hat switches
                   false, false, false, // X, Y, Z Axis
                   false, false, false, // Rx, Ry, Rz
                   false, false,        // Rudder, Throttle
                   false, false, false);// Accelerator, Brake, Steering


// Store previous button states
bool lastState[32];


// PCF8575 I2C addresses (adjust as needed)
PCF8575 pcf_1(0x20);
PCF8575 pcf_2(0x22);

// Detect low-RAM AVR board (Uno/Nano)
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega32U4__)
  #define USE_PAGED_MODE
#endif


void setup() {

  Serial.begin(115200);
 Wire.begin();
Wire.setClock(100000); // Set I2C clock to 100kHz

  // Initialize PCF8575 expanders
  pcf_1.begin();
  pcf_2.begin();

  bms.begin(Serial);


  u8g2_DED.begin();

  u8g2_DED.setFont(SD1322_FONT); 
  // Set all pins to input with pull-ups
  for (uint8_t i = 0; i < 16; i++) {
    pcf_1.write(i, HIGH);
    pcf_2.write(i, HIGH);
    lastState[i] = true;          // Default state = HIGH (not pressed)
    lastState[i + 16] = true;
  }
  // Start the Joystick
  Joystick.begin();

#ifdef USE_PAGED_MODE
  u8g2_DED.firstPage();
  do {
    u8g2_DED.drawStr(0, 10, "STARTING...");
  } while (u8g2_DED.nextPage());
#else
  u8g2_DED.clearBuffer();
  u8g2_DED.drawStr(0, 10, "STARTING...");
  u8g2_DED.sendBuffer();
#endif

  delay(200);
}

void printDED() {
  uint8_t charHeight = u8g2_DED.getMaxCharHeight();
  uint8_t lineSpacing = 1;
  uint8_t lineHeight = charHeight + lineSpacing;

#ifdef USE_PAGED_MODE
  u8g2_DED.firstPage();
  do {
    for (int i = 0; i < 5; i++) {
      const char* line = bms.dedLines[i];
     // int x = (256 - u8g2_DED.getUTF8Width(line)) / 2;
      int y = lineHeight * i + charHeight;
      u8g2_DED.drawStr(DED_H_CONST, y, line);
    }
  } while (u8g2_DED.nextPage());
  
#else
  u8g2_DED.clearBuffer();
  for (int i = 0; i < 5; i++) {
    const char* line = bms.dedLines[i];
   // int x = (256 - u8g2_DED.getUTF8Width(line)) / 2;
    int y = lineHeight * i + charHeight;
    u8g2_DED.drawStr(DED_H_CONST, y, line);
  }
  u8g2_DED.sendBuffer();
#endif
}
void checkButtons() {
  // Check PCF8575 #1 (buttons 0–15)
  for (uint8_t i = 0; i < 16; i++) {
    bool currentState = pcf_1.read(i);
    if (currentState != lastState[i]) {

      Joystick.setButton(i, !currentState); // Invert: LOW = pressed
      lastState[i] = currentState;
    }
  }

  // Check PCF8575 #2 (buttons 16–31)
  for (uint8_t i = 0; i < 16; i++) {
    uint8_t btnIndex = i + 16;
    bool currentState = pcf_2.read(i);
    if (currentState != lastState[btnIndex]) {

      Joystick.setButton(btnIndex, !currentState); // Invert: LOW = pressed
      lastState[btnIndex] = currentState;
    }
  }
}

void loop() {
  bms.update();

  if (bms.isConnected()) {
    bms.getDED();
     printDED();
  } else {
#ifdef USE_PAGED_MODE
    u8g2_DED.firstPage();
    do {
      u8g2_DED.drawStr(0, 15, "NOT CONNECTED....");
    } while (u8g2_DED.nextPage());
#else
    u8g2_DED.clearBuffer();
    u8g2_DED.drawStr(0, 15, "WAITING FOR CONNECTION....");
    u8g2_DED.sendBuffer();
#endif
  }

  // Always check buttons
  checkButtons();
}

