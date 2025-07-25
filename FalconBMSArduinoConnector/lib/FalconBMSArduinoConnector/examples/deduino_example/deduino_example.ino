#define FBAC_DED_1322

#include <Arduino.h>
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;


// Detect low-RAM AVR board (Uno/Nano)
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  #define USE_PAGED_MODE
#endif

void setup() {

  Serial.begin(115200);

  bms.begin(Serial);


  u8g2_DED.begin();

  u8g2_DED.setFont(SD1322_FONT); 


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
  delay(500);
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

void loop() {

    bms.update();
    

    if (bms.isConnected()) {
     
        bms.getDED();
        printDED();

      }
    else {
       
    #ifdef USE_PAGED_MODE
        u8g2_DED.firstPage();
        do {
          u8g2_DED.drawStr(0, 15, "NOT CONNECTED....");
        } while (u8g2_DED.nextPage());
    #else
        u8g2_DED.clearBuffer();
        u8g2_DED.drawStr(0, 15, "NOT CONNECTED....");
        u8g2_DED.sendBuffer();
    #endif
      }

}
