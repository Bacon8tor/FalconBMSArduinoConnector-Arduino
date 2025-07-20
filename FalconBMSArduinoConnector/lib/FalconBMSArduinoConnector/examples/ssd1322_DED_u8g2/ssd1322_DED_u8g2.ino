#include <FalconBMSArduinoConnector.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8G2_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8G2_HAVE_HW_I2C
#include <Wire.h>
#endif

// For I2C on ESP32 (SDA = GPIO21, SCL = GPIO22)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_DED(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

FalconBMSArduinoConnector bms;


#if defined(ESP32)
const int ledPin = 2;
#else
const int ledPin = 13;
#endif

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  while (!Serial);
  bms.begin();

  u8g2_DED.setI2CAddress(0x3C << 1);
  u8g2_DED.begin();

  u8g2_DED.setFont(u8g2_font_5x7_tr); 
  u8g2_DED.clearBuffer();
  u8g2_DED.drawStr(0, 10, "Starting...");
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
    u8g2_DED.drawStr(0, 15, "Not Connected....");
    u8g2_DED.sendBuffer();
  }

 
}
