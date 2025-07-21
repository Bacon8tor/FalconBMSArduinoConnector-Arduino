#include <Arduino.h>
#include <FalconBMSArduinoConnector.h>

//#include <OLED_I2C.h>
#include <FastLED.h>

#include <U8g2lib.h>


// For I2C on ESP32 (SDA = GPIO21, SCL = GPIO22)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_DED(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// U8G2 for 128x64 I2C OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_FuelFlow(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#if defined(ESP32)
  const int ledPin = 2;  // Most ESP32 boards use GPIO2 for the onboard LED
#else
  const int ledPin = LED_BUILTIN;  // Fallback for Arduino Uno, Mega, Nano etc.
#endif

#define NUM_LEDS 32

#define DATA_PIN 5

// Define the array of leds
CRGB leds[NUM_LEDS];

FalconBMSArduinoConnector bms;
//OLED  myOLED(21, 22);

extern uint8_t SmallFont[];

void rainbow() {
  // Example: rainbow cycle
  int h = 0;
  while(h<100){
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue + (i * 5), 255, 255);
  }
  FastLED.show();
  hue++;  // slowly cycle through color wheel
  h++;
  delay(20);
  }
  FastLED.clear();
}

void setup()
{
  
  bms.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  //Serial.println("LED pin set.");

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
  //Serial.println("FastLED initialized.");

  bms.begin();
  //Serial.println("Falcon BMS connector initialized.");

 // u8g2_DED.setI2CAddress(0x3C << 1);
  u8g2_DED.begin();
  u8g2_FuelFlow.setI2CAddress(0x3D << 1);
  u8g2_FuelFlow.begin();
  //Serial.println("OLED displays initialized.");

  u8g2_DED.setFont(u8g2_font_5x7_tr); 
  u8g2_DED.clearBuffer();
  u8g2_DED.drawStr(0, 10, "Starting...");
  u8g2_DED.sendBuffer();
  //Serial.println("Display message sent.");

  u8g2_FuelFlow.setFont(u8g2_font_5x7_tr); 
  u8g2_FuelFlow.clearBuffer();
  u8g2_FuelFlow.drawStr(0, 10, "Starting...");
  u8g2_FuelFlow.sendBuffer();


  rainbow();

}

void updateCautionPanel() {
  if(bms.isAllLampBits){
    for(int i = 0; i < NUM_LEDS;i++){
      leds[i] = CRGB::Orange;
    }
  } else 
  {
    leds[0]  = bms.isFltControlSys() ? CRGB::DarkOrange : CRGB::Black;
    leds[1]  = bms.isElecFault() ? CRGB::DarkOrange : CRGB::Black;
    leds[2]  = bms.isProbeHeat() ? CRGB::DarkOrange : CRGB::Black;
    leds[3]  = bms.isCadc() ? CRGB::DarkOrange : CRGB::Black;
    leds[4]  = bms.isConfig() ? CRGB::DarkOrange : CRGB::Black;
    leds[5]  = bms.isATFNotEngaged() ? CRGB::DarkOrange : CRGB::Black;
    leds[6]  = bms.isFwdFuelLow() ? CRGB::DarkOrange : CRGB::Black;
    leds[7]  = bms.isAftFuelLow() ? CRGB::DarkOrange : CRGB::Black;
    leds[8]  = bms.isEngineFault() ? CRGB::DarkOrange : CRGB::Black;
    leds[9]  = bms.isSEC() ? CRGB::DarkOrange : CRGB::Black;
    leds[10] = bms.isFuelOilHot() ? CRGB::DarkOrange : CRGB::Black;
    leds[11] = bms.isInletIcing() ? CRGB::DarkOrange : CRGB::Black;
    leds[12] = bms.isOverheat() ? CRGB::DarkOrange : CRGB::Black;
    leds[13] = CRGB::Black;// leds[13] = bms.isEEC() ? CRGB::DarkOrange : CRGB::Black;
    leds[14] = bms.isBUC() ? CRGB::DarkOrange : CRGB::Black;
    leds[15] = CRGB::Black;//nothing
    leds[16] = bms.isAvionics() ? CRGB::DarkOrange : CRGB::Black;
    leds[17] = bms.isEquipHot() ? CRGB::DarkOrange : CRGB::Black;
    leds[18] = bms.isRadarAlt() ? CRGB::DarkOrange : CRGB::Black;
    leds[19] = bms.isIFF() ? CRGB::DarkOrange : CRGB::Black;
    leds[20] =  CRGB::Black;//leds[20] = bms.isNuclear() ? CRGB::DarkOrange : CRGB::Black;
    leds[21] = CRGB::Black;
    leds[22] = CRGB::Black;
    leds[23] = CRGB::Black;
    leds[24] = bms.isSeatArm() ? CRGB::DarkOrange : CRGB::Black;
    leds[25] = bms.isNWSFail() ? CRGB::DarkOrange : CRGB::Black;
    leds[26] = bms.isAntiSkid() ? CRGB::DarkOrange : CRGB::Black;
    leds[27] = bms.isHook() ? CRGB::DarkOrange : CRGB::Black;
    leds[28] = bms.isOxyLow() ? CRGB::DarkOrange : CRGB::Black;
    leds[29] = bms.isCabinPress() ? CRGB::DarkOrange : CRGB::Black;
    leds[30] = CRGB::Black;
    leds[31] = CRGB::Black;
  }
   FastLED.show();
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

void loop()
{
  bms.update();
      
  if(bms.isConnected())
  {
    digitalWrite(ledPin,HIGH);

    bms.checkAllLights();
    bms.getPFL();
    bms.getDED();
    bms.getFuelFlow();
    bms.getChaffFlareCount();

    printDED();
    printFuelFlow();
    updateCautionPanel();    
    
   } 
   else 
   {
    FastLED.clear();
     digitalWrite(ledPin, LOW);  
      //u8g2_FuelFlow.clearBuffer();
      u8g2_DED.clearBuffer();
      u8g2_DED.drawStr(0, 15, "Not Connected....");
      u8g2_DED.sendBuffer();

      u8g2_FuelFlow.setFont(u8g2_font_5x7_tr); 
      u8g2_FuelFlow.clearBuffer();
      u8g2_FuelFlow.drawStr(0, 15, "BMS Not Connected....");
      u8g2_FuelFlow.sendBuffer();
      leds[0] = CRGB::Red;
       FastLED.show();
   }
     
 // delay(100);
}


