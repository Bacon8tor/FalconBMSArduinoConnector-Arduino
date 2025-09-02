#include <FalconBMSArduinoConnector.h>
FalconBMSArduinoConnector bms;



void setup() {
  Serial.begin(115200);
  bms.begin(Serial);

}

void loop() {
  bms.update();
  if (bms.isConnected()) {
    bms.getIFFMode1Digit1();
    bms.getIFFMode1Digit2();
    bms.getIFFMode3Digit1();
    bms.getIFFMode3Digit2();
    //Digits are converted from byte to int for easier use 
    //bms.IFFMode1Digit1;
    //bms.IFFMode1Digit2;
    //bms.IFFMode3Digit1;
    //bms.IFFMode3Digit2;

    
  } else {

  }
 
}
