
#define FBAC_FASTLED
#include <FalconBMSArduinoConnector.h>

FalconBMSArduinoConnector bms;

#ifdef FBAC_FASTLED
#define NUM_LEDS 4
#define DATA_PIN 2
// Define the array of leds
CRGB ecm[NUM_LEDS];
#endif

void setup() {

  bms.begin();
#ifdef FBAC_FASTLED
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(ecm, NUM_LEDS);
  FastLED.setBrightness(20);
#endif
}

void loop() {
  bms.update();
  if (bms.isConnected()) {
    bms.getECMBits();
        //Iterate over 4 ECM Lights and returns status of lights as an int (0-7)
    for(int i =0; i < 4;i++){
        switch(bms.getECMStatus(i))
        {
            //unpressed no lit
            case 0:
                ecm[i] = CRGB::Black;
            break;
            //unpress all lit 
            case 1:
                ecm[i] = CRGB::White;
            //pressed no lit 
            case 2:
                ecm[i] = CRGB::Black;
            break;
            //pressed standy
            case 3:
                ecm[i] = CRGB::Yellow;
            break;
            // pressed active
            case 4:
                ecm[i] = CRGB::Green;
            break;
            //pressed transmit
            case 5:
                ecm[i] = CRGB::Blue;
            break;
            //pressed fail 
            case 6:
                ecm[i] = CRGB::Red;
            break;
            //pressed all lit
            case 7:
                ecm[i] = CRGB::Magenta;
            break;
            default:
                ecm[i] = CRGB::Black;
            break;
        }
        FastLED.show();
    }
    
  }else{

  }
 delay(100);
}
