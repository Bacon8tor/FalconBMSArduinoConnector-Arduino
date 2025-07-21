#include <FalconBMSArduinoConnector.h>
FalconBMSArduinoConnector bms;

#include <U8g2lib.h>
//U8GLIB_NHD31OLED_2X_BW dedDisp(8, 11); //DED screen SSD1322 based 240*64 (Buydisplay/rising star)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C dedDisp(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#include "falconded_full_u8g.h"

  #define dedFont FalconDED_full
  #define DED_CHAR_W 5//9
  #define DED_CHAR_H 12
// Font offset calc
// width: Screen is 256px wide. font is 9px wide. DED has 24 chars. 24*9=216 (Total text width). 256-216=40 diff. 40/2 = 20 offset in px to center text in screen
// Height: (64-(12*5))/2=2
  #define DED_H_CONST 4//20
  #define DED_V_CONST 2

#if defined(ESP32)
  const int ledPin = 2;  // Most ESP32 boards use GPIO2 for the onboard LED
#else
  const int ledPin = LED_BUILTIN;  // Fallback for Arduino Uno, Mega, Nano etc.
#endif
void printDED(){
  dedDisp.setFont(dedFont);
    char buffer[32];
    dedDisp.clearBuffer();

      for (int i = 0; i < 5; i++) {
        int x = (128 - dedDisp.getStrWidth(bms.dedLines[i])) / 2;
       // dedDisp.drawStr(x, (i + 1) * 11, bms.dedLines[i]);
         dedDisp.drawStr(x, (i + 1) * DED_CHAR_H + DED_V_CONST, bms.dedLines[i]); 
      }
      dedDisp.sendBuffer();

} 
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);


  bms.begin();
  
  dedDisp.setI2CAddress(0x3C << 1);
  dedDisp.begin();

  dedDisp.setFont(u8g2_font_5x7_tr);
  dedDisp.clearBuffer();
  dedDisp.drawStr(0, 10, "Starting...");
  dedDisp.sendBuffer();


}

void loop() {
  bms.update();
  if (bms.isConnected()) {
    digitalWrite(ledPin, HIGH); // CHECKS WHETHER isMasterCaution true or false then sets high or low 
    
    bms.getDED();  
    //Get Any DED Line like 
    //bms.dedLines[0];
    dedDisp.clearBuffer();
    printDED();
    dedDisp.sendBuffer();

  } else {
    digitalWrite(ledPin, LOW);
    dedDisp.setFont(u8g2_font_5x7_tr);
      dedDisp.clearBuffer();
      dedDisp.drawStr(0, 10, "Not Connected...");
      dedDisp.sendBuffer();
    //Serial.println("Not connected");
  }
  //delay(50);  // Optional: adjust for responsiveness
}
