#define FBAC_DED_1322 // creates u8g2_DED 
//#define FBAC_PFL_1309 // creates u8g2_PFL
//#define FBAC_FF_1306  // creates u8g2_FuelFlow
#define FBAC_FASTLED // creates FastLED object and related functions


#include <Arduino.h>
#include <FalconBMSArduinoConnector.h>


// Detect low-RAM AVR board (Uno/Nano)
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  #define USE_PAGED_MODE
#endif

#ifdef FBAC_FASTLED
#define NUM_LEDS 32
#define DATA_PIN 3
// Define the array of leds
CRGB leds[NUM_LEDS];
#endif
FalconBMSArduinoConnector bms;

extern uint8_t SmallFont[];
#ifdef FBAC_FASTLED
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
#endif
void setup()
{
  
  Serial.begin(115200);

  bms.begin(Serial);

 
#ifdef FBAC_FASTLED
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
#endif
  
  u8g2_DED.begin();
  #ifdef FBAC_FF_1306
  u8g2_FuelFlow.setI2CAddress(0x3D << 1);
  u8g2_FuelFlow.begin();
  #endif
  #ifdef USE_PAGED_MODE
  u8g2_DED.firstPage();
  do {
    u8g2_DED.drawStr(0, 10, "STARTING...");
  } while (u8g2_DED.nextPage());
  #ifdef FBAC_FF_1306
  u8g2_FuelFlow.firstPage();
  do {
    u8g2_FuelFlow.drawStr(0, 10, "STARTING...");
  } while (u8g2_FuelFlow.nextPage());
  #endif
#else
  u8g2_DED.setFont(u8g2_font_5x7_tr); 
  u8g2_DED.clearBuffer();
  u8g2_DED.drawStr(0, 10, "Starting...");
  u8g2_DED.sendBuffer();
  #ifdef FBAC_FF_1306
  u8g2_FuelFlow.setFont(u8g2_font_5x7_tr); 
  u8g2_FuelFlow.clearBuffer();
  u8g2_FuelFlow.drawStr(0, 10, "Starting...");
  u8g2_FuelFlow.sendBuffer();
  #endif
#endif
#ifdef FBAC_FASTLED
  rainbow();
#endif
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

void printDED() {
  uint8_t charHeight = u8g2_DED.getMaxCharHeight();
  uint8_t lineSpacing = 1;
  uint8_t lineHeight = charHeight + lineSpacing;

#ifdef USE_PAGED_MODE
  u8g2_DED.firstPage();
  do {
    for (int i = 0; i < 5; i++) {
      const char* line = bms.dedLines[i];
      int x = (256 - u8g2_DED.getUTF8Width(line)) / 2;
      int y = lineHeight * i + charHeight;
      u8g2_DED.drawStr(x, y, line);
    }
  } while (u8g2_DED.nextPage());
#else
  u8g2_DED.clearBuffer();
  for (int i = 0; i < 5; i++) {
    const char* line = bms.dedLines[i];
    int x = (256 - u8g2_DED.getUTF8Width(line)) / 2;
    int y = lineHeight * i + charHeight;
    u8g2_DED.drawStr(x, y, line);
  }
  u8g2_DED.sendBuffer();
#endif
}

#ifdef FBAC_FF_1306
void printFuelFlow() {
    char buffer[16];

    // Truncate fuel flow to the nearest lower hundred
    int flow = ((int)bms.fuelFlow / 100) * 100;

    // Format as 5-digit number with leading zeros
    snprintf(buffer, sizeof(buffer), "%05d", flow);

    #ifdef USE_PAGED_MODE
      u8g2_FuelFlow.firstPage();
      do {
        u8g2_FuelFlow.setFont(u8g2_font_logisoso32_tf); // Large readable digits
        int width = u8g2_FuelFlow.getStrWidth(buffer);
        u8g2_FuelFlow.drawStr((128 - width) / 2, 50, buffer);
      } while (u8g2_FuelFlow.nextPage());
    #else 
      u8g2_FuelFlow.clearBuffer();
      u8g2_FuelFlow.setFont(u8g2_font_logisoso32_tf); // Large readable digits

      int width = u8g2_FuelFlow.getStrWidth(buffer);
      u8g2_FuelFlow.drawStr((128 - width) / 2, 50, buffer);

      u8g2_FuelFlow.sendBuffer();
    
    #endif
}
#endif

void loop()
{
  bms.update();
      
  if(bms.isConnected())
  {


    bms.checkAllLights();
    bms.getPFL();
    bms.getDED();
    bms.getFuelFlow();
    bms.getChaffFlareCount();

    printDED();
    #ifdef FBAC_FF_1306
    printFuelFlow();
    #endif
    #ifdef FBAC_FASTLED
    updateCautionPanel();    
    #endif
    
   } 
   else 
   {
      #ifdef FBAC_FASTLED
        FastLED.clear();
        leds[0] = CRGB::Red;
        FastLED.show();
      #endif

      #ifdef USE_PAGED_MODE
        u8g2_DED.firstPage();
        do {
          u8g2_DED.drawStr(0, 15, "NOT CONNECTED....");
        } while (u8g2_DED.nextPage());
      #ifdef FBAC_FF_1306
      u8g2_FuelFlow.firstPage();
      do {
        u8g2_FuelFlow.drawStr(0, 15, "BMS NOT CONNECTED....");
      } while (u8g2_FuelFlow.nextPage());
      #endif
      #else
      u8g2_DED.clearBuffer();
      u8g2_DED.drawStr(0, 15, "Not Connected....");
      u8g2_DED.sendBuffer();
#ifdef FBAC_FF_1306
      u8g2_FuelFlow.setFont(u8g2_font_5x7_tr); 
      u8g2_FuelFlow.clearBuffer();
      u8g2_FuelFlow.drawStr(0, 15, "BMS Not Connected....");
      u8g2_FuelFlow.sendBuffer();
      #endif
      #endif
      
   }
     
}


