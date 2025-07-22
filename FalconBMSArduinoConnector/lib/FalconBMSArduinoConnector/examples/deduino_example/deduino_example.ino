#define FBAC_DED_1322

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

  u8g2_DED.setFont(SD1322_FONT); 
  u8g2_DED.clearBuffer();
  u8g2_DED.drawStr(0, 10, "STARTING...");
  u8g2_DED.sendBuffer();

  delay(1000);


}

void printDED() {
    char buffer[32];
    u8g2_DED.clearBuffer();

    uint8_t charHeight = u8g2_DED.getMaxCharHeight();
    uint8_t lineSpacing = 1; // You can tweak this
    uint8_t lineHeight = charHeight + lineSpacing;

    for (int i = 0; i < 5; i++) {
        const char* line = bms.dedLines[i]; // assuming these are null-terminated C-strings

        int x = (256 - u8g2_DED.getUTF8Width(line)) / 2;  // Centered
        int y =  lineHeight * i + charHeight; // baseline y

        u8g2_DED.drawStr(x, y, line);
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
