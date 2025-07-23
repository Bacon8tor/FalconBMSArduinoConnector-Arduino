#define FBAC_DED_1309

#include <Arduino.h>

#include <FalconBMSArduinoConnector.h>


FalconBMSArduinoConnector bms;


#if defined(ESP32)
const int ledPin = 2;
#else
const int ledPin = 13;
#endif

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  bms.begin();

  u8g2_DED.setI2CAddress(0x3C << 1);
  u8g2_DED.begin();

  u8g2_DED.setFont(u8g2_font_FBAC_DED_FONT_5x8_tf); 
  u8g2_DED.clearBuffer();
  u8g2_DED.drawStr(0, 10, "STARTING...");
  u8g2_DED.sendBuffer();

  delay(1000);


}

void printDED(){
    char buffer[32];
    u8g2_DED.clearBuffer();

      for (int i = 0; i < 5; i++) {
        int x = (128 - u8g2_DED.getStrWidth(bms.dedLines[i])) / 2;
        u8g2_DED.drawStr(x, (i + 1) * 11, bms.dedLines[i]);

      }
      u8g2_DED.sendBuffer();

} 


void loop() {
  
  bms.update();
  if (bms.isConnected()) {
    digitalWrite(ledPin, HIGH); //is connected and recievign data
    bms.getDED();
    printDED();
    
  }
  else {
    digitalWrite(ledPin, LOW);
    u8g2_DED.clearBuffer();
    u8g2_DED.drawStr(0, 15, "NOT CONNECTED....");
    u8g2_DED.sendBuffer();
  }

 
}
