#include <FalconBMSArduinoConnector.h>
#include <Arduino.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 20, 4);
#if defined(ESP32)
  const int ledPin = 2;  // Most ESP32 boards use GPIO2 for the onboard LED
#else
  const int ledPin = LED_BUILTIN;  // Fallback for Arduino Uno, Mega, Nano etc.
#endif

FalconBMSArduinoConnector bms;


void lcdPrintLine(int line, const char* msg) {
  lcd.setCursor(0, line);
  lcd.print("                    "); // Clear line
  lcd.setCursor(0, line);
  lcd.print(msg);
}

// void printLastSerialTime(unsigned long execTime) {
//   char buffer[21];
//   snprintf(buffer, sizeof(buffer), "Last RX: %lums (%lums)", millis() - bms.lastSerialTime, execTime);
//   lcdPrintLine(3, buffer);
// }

void lcdPrintHexLine(int line, const char* prefix, uint8_t* data, uint8_t len) {
  lcd.setCursor(0, line);
  lcd.print("                    "); // Clear line
  lcd.setCursor(0, line);
  lcd.print(prefix);
  for (int i = 0; i < len && (3 * i + strlen(prefix) < 20); i++) {
    char buf[4];
    sprintf(buf, "%02X ", data[i]);
    lcd.print(buf);
  }
}

void setup()
{
   lcd.init();
   lcd.backlight();
   lcd.clear();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  bms.begin();
   lcdPrintLine(0, "Arduino Ready");
   lcdPrintLine(1, "Waiting handshake");
}

void loop()
{
  bms.update();
  if(bms.isConnected())
  {
    bms.getLightBits(1);
    bms.getLightBits(2);
    bms.getLightBits(3);
    bms.getLightBits(4);
    lcdPrintLine(0, "Receiving Data..");
    lcdPrintLine(1, bms.isMasterCaution() ? "MC: ON" : "MC: OFF");
    lcdPrintLine(2, bms.isUnk() ? "UNK: ON" : "UNK: OFF");
    lcdPrintLine(3, bms.isJFSOnSlowBlinking() ? "Blink: ON" : "Blink: OFF");
    digitalWrite(ledPin, bms.isMasterCaution() ? HIGH : LOW); // CHECKS WHETHER isMasterCaution true or false then sets high or low 
  } else {
    lcdPrintLine(0, "Connection lost");
    lcdPrintLine(1, "");
    lcdPrintLine(2, "");
    lcdPrintLine(3, "");
  }
  delay(100);
}


