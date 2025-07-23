#define FBAC_PFL_1309

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

  u8g2_PFL.setI2CAddress(0x3C << 1);
  u8g2_PFL.begin();

  u8g2_PFL.setFont(u8g2_font_5x7_tr); 
  u8g2_PFL.clearBuffer();
  u8g2_PFL.drawStr(0, 10, "Starting...");
  u8g2_PFL.sendBuffer();

  delay(1000);


}

void printPFL(){
    char buffer[32];
    u8g2_PFL.clearBuffer();

      for (int i = 0; i < 5; i++) {
        int x = (128 - u8g2_PFL.getStrWidth(bms.pflLines[i])) / 2;
        u8g2_PFL.drawStr(x, (i + 1) * 11, bms.pflLines[i]);

      }
      u8g2_PFL.sendBuffer();

} 


void loop() {
  bms.update();
  
  if (bms.isConnected()) {
    digitalWrite(ledPin, HIGH); //is connected and recievign data
    bms.getPFL();
    printPFL();
    
  }
  else {
    digitalWrite(ledPin, LOW);
    u8g2_PFL.clearBuffer();
    u8g2_PFL.drawStr(0, 15, "Not Connected....");
    u8g2_PFL.sendBuffer();
  }

 
}
