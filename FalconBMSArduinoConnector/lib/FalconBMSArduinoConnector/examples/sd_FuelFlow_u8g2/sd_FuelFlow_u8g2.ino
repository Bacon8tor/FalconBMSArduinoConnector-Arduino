#define FBAC_FF_1306

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

  Serial.begin(115200);

  bms.begin(Serial);


  u8g2_FuelFlow.setI2CAddress(0x3C << 1);
  u8g2_FuelFlow.begin();

  u8g2_FuelFlow.setFont(u8g2_font_FBAC_DED_FONT_5x8_tf); 
  u8g2_FuelFlow.clearBuffer();
  u8g2_FuelFlow.drawStr(0, 10, "STARTING...");
  u8g2_FuelFlow.sendBuffer();

  delay(1000);


}

void printFuelFlow() {
    char buffer[16];

    // Truncate fuel flow to the nearest lower hundred
    int flow = ((int)bms.fuelFlow / 100) * 100;

    // Format as 5-digit number with leading zeros
    snprintf(buffer, sizeof(buffer), "%05d", flow);

    u8g2_FuelFlow.clearBuffer();
    u8g2_FuelFlow.setFont(u8g2_font_logisoso32_tf); // Large readable digits

    int width = u8g2_FuelFlow.getStrWidth(buffer);
    u8g2_FuelFlow.drawStr((128 - width) / 2, 50, buffer);

    u8g2_FuelFlow.sendBuffer();
}

void loop() {
  
  bms.update();
  if (bms.isConnected()) {
    digitalWrite(ledPin, HIGH); //is connected and recievign data
    bms.getFuelFlow();
    printFuelFlow();
  }
  else {
    digitalWrite(ledPin, LOW);
    u8g2_FuelFlow.clearBuffer();
    u8g2_FuelFlow.drawStr(0, 15, "NOT CONNECTED....");
    u8g2_FuelFlow.sendBuffer();
  }

 
}
